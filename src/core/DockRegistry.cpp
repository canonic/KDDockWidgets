/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "DockRegistry.h"
#include "Config.h"
#include "private/Logging_p.h"
#include "private/Position_p.h"
#include "private/Utils_p.h"
#include "private/Platform_p.h"
#include "private/WidgetResizeHandler_p.h"
#include "private/WindowBeingDragged_p.h"
#include "core/layouting/Item_p.h"

#include "kddockwidgets/core/views/MainWindowViewInterface.h"
#include "kddockwidgets/core/FloatingWindow.h"
#include "kddockwidgets/core/SideBar.h"
#include "kddockwidgets/core/MainWindow.h"
#include "kddockwidgets/core/DockWidget.h"
#include "kddockwidgets/core/DockWidget_p.h"
#include "kddockwidgets/core/DropArea.h"
#include "kddockwidgets/core/Platform.h"
#include "kddockwidgets/core/Window.h"

#include "kdbindings/signal.h"

#include <QPointer>
#include <QDebug>

using namespace KDDockWidgets;
using namespace KDDockWidgets::Core;

class DockRegistry::Private
{
public:
    KDBindings::ConnectionHandle m_connection;
};

DockRegistry::DockRegistry(QObject *parent)
    : QObject(parent)
    , d(new Private())
{
    Platform::instance()->installGlobalEventFilter(this);

    d->m_connection = Platform::instance()->d->focusedViewChanged.connect(
        &DockRegistry::onFocusedViewChanged, this);
}

DockRegistry::~DockRegistry()
{
    Platform::instance()->removeGlobalEventFilter(this);
    d->m_connection.disconnect();
    delete d;
}

void DockRegistry::maybeDelete()
{
    if (isEmpty())
        delete this;
}

void DockRegistry::onFocusedViewChanged(std::shared_ptr<View> view)
{
    auto p = view;
    while (p && !p->isNull()) {
        if (auto group = p->asGroupController()) {
            // Special case: The focused widget is inside the group but not inside the dockwidget.
            // For example, it's a line edit in the QTabBar. We still need to send the signal for
            // the current dw in the tab group
            if (auto dw = group->currentDockWidget()) {
                setFocusedDockWidget(dw);
            }

            return;
        }

        if (auto dw = p->asDockWidgetController()) {
            DockRegistry::self()->setFocusedDockWidget(dw);
            return;
        }
        p = p->parentView();
    }

    setFocusedDockWidget(nullptr);
}

void DockRegistry::setFocusedDockWidget(Core::DockWidget *dw)
{
    if (m_focusedDockWidget.data() == dw)
        return;

    if (m_focusedDockWidget) {
        // Emit DockWidget::isFocusedChanged(). Needs to be delayed,
        // as the FocusScope hasn't been updated yet.
        // It's just for styling purposes, so can be delayed
        auto oldDw = m_focusedDockWidget;
        QMetaObject::invokeMethod(
            oldDw, [oldDw] {
                if (oldDw) // QPointer
                    Q_EMIT oldDw->isFocusedChanged(false);
            },
            Qt::QueuedConnection);
    }

    m_focusedDockWidget = dw;

    if (dw) {
        // Emit DockWidget::isFocusedChanged(). Needs to be delayed,
        // as the FocusScope hasn't been updated yet.
        // It's just for styling purposes, so can be delayed
        QMetaObject::invokeMethod(
            this, [this] {
                if (m_focusedDockWidget) { // QPointer
                    Q_EMIT m_focusedDockWidget->isFocusedChanged(true);
                }
            },
            Qt::QueuedConnection);
    }
}

bool DockRegistry::isEmpty(bool excludeBeingDeleted) const
{
    if (!m_dockWidgets.isEmpty() || !m_mainWindows.isEmpty())
        return false;

    return excludeBeingDeleted ? !hasFloatingWindows() : m_floatingWindows.isEmpty();
}

void DockRegistry::checkSanityAll(bool dumpLayout)
{
    for (auto layout : qAsConst(m_layouts)) {
        layout->checkSanity();
        if (dumpLayout)
            layout->dumpLayout();
    }
}

bool DockRegistry::affinitiesMatch(const QStringList &affinities1,
                                   const QStringList &affinities2) const
{
    if (affinities1.isEmpty() && affinities2.isEmpty())
        return true;

    for (const QString &a1 : affinities1) {
        for (const QString &a2 : affinities2) {
            if (a1 == a2)
                return true;
        }
    }

    return false;
}

QStringList DockRegistry::mainWindowsNames() const
{
    QStringList names;
    names.reserve(m_mainWindows.size());
    for (auto mw : m_mainWindows)
        names.push_back(mw->uniqueName());

    return names;
}

QStringList DockRegistry::dockWidgetNames() const
{
    QStringList names;
    names.reserve(m_dockWidgets.size());
    for (auto dw : m_dockWidgets)
        names.push_back(dw->uniqueName());

    return names;
}

bool DockRegistry::isProbablyObscured(Core::Window::Ptr window,
                                      Core::FloatingWindow *exclude) const
{
    if (!window)
        return false;

    const QRect geo = window->geometry();
    for (Core::FloatingWindow *fw : m_floatingWindows) {
        Window::Ptr fwWindow = fw->view()->window();
        if (fw == exclude || fwWindow->equals(window))
            continue;

        if (fwWindow->geometry().intersects(geo)) {
            // fw might be below, but we don't have a way to check. So be conservative and return
            // true.
            return true;
        }
    }

    // Floating windows are Tool (keep above), unless we disabled it in Config
    auto fw = floatingWindowForHandle(window);
    const bool targetIsToolWindow =
        fw && fw->isUtilityWindow();

    for (Core::MainWindow *mw : m_mainWindows) {
        Window::Ptr mwWindow = mw->view()->window();

        if (mwWindow && !mwWindow->equals(window) && !targetIsToolWindow
            && mwWindow->geometry().intersects(geo)) {
            // Two main windows that intersect. Return true. If the target is a tool window it will
            // be above, so we don't care.
            return true;
        }
    }

    return false;
}

bool DockRegistry::isProbablyObscured(Core::Window::Ptr target, WindowBeingDragged *exclude) const
{
    Core::FloatingWindow *fw =
        exclude ? exclude->floatingWindow() : nullptr; // It's null on Wayland. On wayland obscuring
                                                       // never happens anyway, so not a problem.

    return isProbablyObscured(target, fw);
}

SideBarLocation DockRegistry::sideBarLocationForDockWidget(const Core::DockWidget *dw) const
{
    if (Core::SideBar *sb = sideBarForDockWidget(dw))
        return sb->location();

    return SideBarLocation::None;
}

Core::SideBar *DockRegistry::sideBarForDockWidget(const Core::DockWidget *dw) const
{
    for (auto mw : m_mainWindows) {
        if (Core::SideBar *sb = mw->sideBarForDockWidget(dw))
            return sb;
    }

    return nullptr;
}

Core::Group *DockRegistry::groupInMDIResize() const
{
    for (auto mw : m_mainWindows) {
        if (!mw->isMDI())
            continue;

        Layout *layout = mw->layout();
        const QList<Core::Group *> groups = layout->groups();
        for (Core::Group *group : groups) {
            if (WidgetResizeHandler *wrh = group->resizeHandler()) {
                if (wrh->isResizing())
                    return group;
            }
        }
    }

    return nullptr;
}

Core::MainWindow::List
DockRegistry::mainWindowsWithAffinity(const QStringList &affinities) const
{
    Core::MainWindow::List result;
    result.reserve(m_mainWindows.size());

    for (auto mw : m_mainWindows) {
        const QStringList mwAffinities = mw->affinities();
        if (affinitiesMatch(mwAffinities, affinities))
            result << mw;
    }

    return result;
}

Core::Layout *DockRegistry::layoutForItem(const Item *item) const
{
    if (!item->hostView())
        return nullptr;

    return item->hostView()->asLayout();
}

bool DockRegistry::itemIsInMainWindow(const Item *item) const
{
    if (Core::Layout *layout = layoutForItem(item)) {
        return layout->isInMainWindow(/*honoursNesting=*/true);
    }

    return false;
}

DockRegistry *DockRegistry::self()
{
    static QPointer<DockRegistry> s_dockRegistry;

    if (!s_dockRegistry) {
        s_dockRegistry = new DockRegistry();
    }

    return s_dockRegistry;
}

void DockRegistry::registerDockWidget(Core::DockWidget *dock)
{
    if (dock->uniqueName().isEmpty()) {
        qWarning() << Q_FUNC_INFO << "DockWidget" << dock << " doesn't have an ID";
    } else if (auto other = dockByName(dock->uniqueName())) {
        qWarning() << Q_FUNC_INFO << "Another DockWidget" << other << "with name"
                   << dock->uniqueName() << " already exists." << dock;
    }

    m_dockWidgets << dock;
}

void DockRegistry::unregisterDockWidget(Core::DockWidget *dock)
{
    if (m_focusedDockWidget == dock)
        m_focusedDockWidget = nullptr;

    m_dockWidgets.removeOne(dock);
    maybeDelete();
}

void DockRegistry::registerMainWindow(Core::MainWindow *mainWindow)
{
    if (mainWindow->uniqueName().isEmpty()) {
        qWarning() << Q_FUNC_INFO << "MainWindow" << mainWindow << " doesn't have an ID";
    } else if (auto other = mainWindowByName(mainWindow->uniqueName())) {
        qWarning() << Q_FUNC_INFO << "Another MainWindow" << other << "with name"
                   << mainWindow->uniqueName() << " already exists." << mainWindow;
    }

    m_mainWindows << mainWindow;
}

void DockRegistry::unregisterMainWindow(Core::MainWindow *mainWindow)
{
    m_mainWindows.removeOne(mainWindow);
    maybeDelete();
}

void DockRegistry::registerFloatingWindow(Core::FloatingWindow *fw)
{
    m_floatingWindows << fw;
    Platform::instance()->onFloatingWindowCreated(fw);
}

void DockRegistry::unregisterFloatingWindow(Core::FloatingWindow *fw)
{
    m_floatingWindows.removeOne(fw);
    Platform::instance()->onFloatingWindowDestroyed(fw);
    maybeDelete();
}

void DockRegistry::registerLayout(Core::Layout *layout)
{
    m_layouts << layout;
}

void DockRegistry::unregisterLayout(Core::Layout *layout)
{
    m_layouts.removeOne(layout);
}

void DockRegistry::registerGroup(Core::Group *group)
{
    m_groups << group;
}

void DockRegistry::unregisterGroup(Core::Group *group)
{
    m_groups.removeOne(group);
}

Core::DockWidget *DockRegistry::focusedDockWidget() const
{
    return m_focusedDockWidget;
}

bool DockRegistry::containsDockWidget(const QString &uniqueName) const
{
    return dockByName(uniqueName) != nullptr;
}

bool DockRegistry::containsMainWindow(const QString &uniqueName) const
{
    return mainWindowByName(uniqueName) != nullptr;
}

Core::DockWidget *DockRegistry::dockByName(const QString &name, DockByNameFlags flags) const
{
    for (auto dock : qAsConst(m_dockWidgets)) {
        if (dock->uniqueName() == name)
            return dock;
    }

    if (flags.testFlag(DockByNameFlag::ConsultRemapping)) {
        // Name doesn't exist, let's check if it was remapped during a layout restore.
        const QString newName = m_dockWidgetIdRemapping.value(name);
        if (!newName.isEmpty())
            return dockByName(newName);
    }

    if (flags.testFlag(DockByNameFlag::CreateIfNotFound)) {
        // DockWidget doesn't exist, ask to create it
        if (auto factoryFunc = Config::self().dockWidgetFactoryFunc()) {
            auto dw = factoryFunc(name);
            if (dw && dw->uniqueName() != name) {
                // Very special case
                // The user's factory function returned a dock widget with a different ID.
                // We support it. Save the mapping though.
                m_dockWidgetIdRemapping.insert(name, dw->uniqueName());
            }
            return dw;
        } else {
            qWarning() << Q_FUNC_INFO << "Couldn't find dock widget" << name;
        }
    }

    return nullptr;
}

Core::MainWindow *DockRegistry::mainWindowByName(const QString &name) const
{
    for (auto mainWindow : qAsConst(m_mainWindows)) {
        if (mainWindow->uniqueName() == name)
            return mainWindow;
    }

    return nullptr;
}

bool DockRegistry::isSane() const
{
    QSet<QString> names;
    for (auto dock : qAsConst(m_dockWidgets)) {
        const QString name = dock->uniqueName();
        if (name.isEmpty()) {
            qWarning() << "DockRegistry::isSane: DockWidget" << dock << "is missing a name";
            return false;
        } else if (names.contains(name)) {
            qWarning() << "DockRegistry::isSane: dockWidgets with duplicate names:" << name;
            return false;
        } else {
            names.insert(name);
        }
    }

    names.clear();
    for (auto mainwindow : qAsConst(m_mainWindows)) {
        const QString name = mainwindow->uniqueName();
        if (name.isEmpty()) {
            qWarning() << "DockRegistry::isSane: MainWindow" << mainwindow << "is missing a name";
            return false;
        } else if (names.contains(name)) {
            qWarning() << "DockRegistry::isSane: mainWindow with duplicate names:" << name;
            return false;
        } else {
            names.insert(name);
        }

        if (!mainwindow->layout()->checkSanity())
            return false;
    }

    return true;
}

const Core::DockWidget::List DockRegistry::dockwidgets() const
{
    return m_dockWidgets;
}

const Core::DockWidget::List DockRegistry::dockWidgets(const QStringList &names)
{
    Core::DockWidget::List result;
    result.reserve(names.size());

    for (auto dw : qAsConst(m_dockWidgets)) {
        if (names.contains(dw->uniqueName()))
            result.push_back(dw);
    }

    return result;
}

const Core::MainWindow::List DockRegistry::mainWindows(const QStringList &names)
{
    Core::MainWindow::List result;
    result.reserve(names.size());

    for (auto mw : qAsConst(m_mainWindows)) {
        if (names.contains(mw->uniqueName()))
            result.push_back(mw);
    }

    return result;
}

const Core::DockWidget::List DockRegistry::closedDockwidgets() const
{
    Core::DockWidget::List result;
    result.reserve(m_dockWidgets.size());

    for (Core::DockWidget *dw : m_dockWidgets) {
        if (dw->parent() == nullptr && !dw->isVisible())
            result.push_back(dw);
    }

    return result;
}

const Core::MainWindow::List DockRegistry::mainwindows() const
{
    return m_mainWindows;
}

const QList<Core::MainWindowViewInterface *> DockRegistry::mainDockingAreas() const
{
    QList<Core::MainWindowViewInterface *> areas;

    for (auto mw : m_mainWindows) {
        if (View *view = mw->view()) {
            auto viewInterface = dynamic_cast<Core::MainWindowViewInterface *>(view);
            areas << viewInterface;
        }
    }

    return areas;
}

const QVector<Core::Layout *> DockRegistry::layouts() const
{
    return m_layouts;
}

const Core::Group::List DockRegistry::groups() const
{
    return m_groups;
}

const QVector<Core::FloatingWindow *>
DockRegistry::floatingWindows(bool includeBeingDeleted) const
{
    // Returns all the FloatingWindow which aren't being deleted
    QVector<Core::FloatingWindow *> result;
    result.reserve(m_floatingWindows.size());
    for (Core::FloatingWindow *fw : m_floatingWindows) {
        if (includeBeingDeleted || !fw->beingDeleted())
            result.push_back(fw);
    }

    return result;
}

const Window::List DockRegistry::floatingQWindows() const
{
    Window::List windows;
    windows.reserve(m_floatingWindows.size());
    for (Core::FloatingWindow *fw : m_floatingWindows) {
        if (!fw->beingDeleted()) {
            if (Core::Window::Ptr window = fw->view()->window()) {
                windows.push_back(window);
            } else {
                qWarning() << Q_FUNC_INFO << "FloatingWindow doesn't have QWindow";
            }
        }
    }

    return windows;
}

bool DockRegistry::hasFloatingWindows() const
{
    return std::any_of(m_floatingWindows.begin(), m_floatingWindows.end(),
                       [](Core::FloatingWindow *fw) { return !fw->beingDeleted(); });
}

Core::FloatingWindow *DockRegistry::floatingWindowForHandle(Core::Window::Ptr windowHandle) const
{
    for (Core::FloatingWindow *fw : m_floatingWindows) {
        if (fw->view()->window()->equals(windowHandle))
            return fw;
    }

    return nullptr;
}

Core::FloatingWindow *DockRegistry::floatingWindowForHandle(WId hwnd) const
{
    for (Core::FloatingWindow *fw : m_floatingWindows) {
        Window::Ptr window = fw->view()->window();
        if (window && window->handle() == hwnd)
            return fw;
    }

    return nullptr;
}

Core::MainWindow *DockRegistry::mainWindowForHandle(Core::Window::Ptr window) const
{
    if (!window)
        return nullptr;

    for (Core::MainWindow *mw : m_mainWindows) {
        if (mw->view()->isInWindow(window))
            return mw;
    }

    return nullptr;
}

Window::List DockRegistry::topLevels(bool excludeFloatingDocks) const
{
    Window::List windows;
    windows.reserve(m_floatingWindows.size() + m_mainWindows.size());

    if (!excludeFloatingDocks) {
        for (Core::FloatingWindow *fw : m_floatingWindows) {
            if (fw->isVisible()) {
                if (Core::Window::Ptr window = fw->view()->window()) {
                    windows << window;
                } else {
                    qWarning() << Q_FUNC_INFO << "FloatingWindow doesn't have QWindow";
                }
            }
        }
    }

    for (Core::MainWindow *m : m_mainWindows) {
        if (m->isVisible()) {
            if (Core::Window::Ptr window = m->view()->window()) {
                windows << window;
            } else {
                qWarning() << Q_FUNC_INFO << "MainWindow doesn't have QWindow";
            }
        }
    }

    return windows;
}

void DockRegistry::clear(const QStringList &affinities)
{
    // Clears everything
    clear(m_dockWidgets, m_mainWindows, affinities);
}

void DockRegistry::clear(const Core::DockWidget::List &dockWidgets,
                         const Core::MainWindow::List &mainWindows,
                         const QStringList &affinities)
{
    for (auto dw : qAsConst(dockWidgets)) {
        if (affinities.isEmpty() || affinitiesMatch(affinities, dw->affinities())) {
            dw->forceClose();
            dw->d->lastPosition()->removePlaceholders();
        }
    }

    for (auto mw : qAsConst(mainWindows)) {
        if (affinities.isEmpty() || affinitiesMatch(affinities, mw->affinities())) {
            mw->layout()->clearLayout();
        }
    }
}

void DockRegistry::ensureAllFloatingWidgetsAreMorphed()
{
    for (Core::DockWidget *dw : qAsConst(m_dockWidgets)) {
        if (dw->view()->rootView()->equals(dw->view()) && dw->isVisible())
            dw->d->morphIntoFloatingWindow();
    }
}

bool DockRegistry::onMouseButtonPress(View *view, MouseEvent *event)
{
    if (!view)
        return false;

    // When clicking on a MDI Frame we raise the window
    if (Controller *c = view->firstParentOfType(ViewType::Frame)) {
        auto group = static_cast<Group *>(c);
        if (group->isMDI())
            group->view()->raise();
    }

    // The following code is for hididng the overlay
    if (!(Config::self().flags() & Config::Flag_AutoHideSupport))
        return false;

    if (view->is(ViewType::Frame)) {
        // break recursion
        return false;
    }

    auto p = view->asWrapper();
    while (p) {
        if (auto dw = p->asDockWidgetController())
            return onDockWidgetPressed(dw, event);

        if (auto layout = p->asLayout()) {
            if (auto mw = layout->mainWindow()) {
                // The user clicked somewhere in the main window's drop area, but outside of the
                // overlayed dock widget
                mw->clearSideBarOverlay();
                return false;
            }
        }

        p = p->parentView();
    }


    return false;
}

bool DockRegistry::onDockWidgetPressed(Core::DockWidget *dw, MouseEvent *ev)
{
    // Here we implement "auto-hide". If there's a overlayed dock widget, we hide it if some other
    // dock widget is clicked.

    // Don't be sending mouse events around if a popup is open, they are sensitive
    if (Platform::instance()->hasActivePopup())
        return false;

    Core::MainWindow *mainWindow = dw->mainWindow();
    if (!mainWindow) // Only docked widgets are interesting
        return false;

    if (Core::DockWidget *overlayedDockWidget = mainWindow->overlayedDockWidget()) {
        ev->ignore();
        Platform::instance()->sendEvent(overlayedDockWidget->d->group()->view(), ev);

        if (ev->isAccepted()) {
            // The Frame accepted it. It means the user is resizing it. We allow for 4px outside for
            // better resize.
            return true; // don't propagate the event further
        }
        if (dw != overlayedDockWidget) {
            // User clicked outside if the overlay, then we close the overlay.
            mainWindow->clearSideBarOverlay();
            return false;
        }
    }

    return false;
}

bool DockRegistry::onExposeEvent(Core::Window::Ptr window)
{
    if (Core::FloatingWindow *fw = floatingWindowForHandle(window)) {
        // This floating window was exposed
        m_floatingWindows.removeOne(fw);
        m_floatingWindows.append(fw);
    }

    return false;
}