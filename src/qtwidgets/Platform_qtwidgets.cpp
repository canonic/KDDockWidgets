/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "Platform_qtwidgets.h"
#include "KDDockWidgets.h"

#include "Window_qtwidgets.h"
#include "DebugWindow_p.h"
#include "views/ViewWrapper_qtwidgets.h"
#include "FrameworkWidgetFactory_qtwidgets.h"

#include <QScreen>
#include <QStyleFactory>
#include <QApplication>

#include <memory.h>


using namespace KDDockWidgets;

Platform_qtwidgets::Platform_qtwidgets()
{
    init();
}

void Platform_qtwidgets::init()
{
#ifdef DOCKS_DEVELOPER_MODE
    if (qEnvironmentVariableIntValue("KDDOCKWIDGETS_SHOW_DEBUG_WINDOW") == 1) {
        auto dv = new Debug::DebugWindow();
        dv->show();
    }
#endif

    qApp->connect(qApp, &QGuiApplication::focusObjectChanged, qApp, [this](QObject *obj) {
        ViewWrapper *wrapper = obj ? new Views::ViewWrapper_qtwidgets(obj) : nullptr;
        focusedViewChanged.emit(std::shared_ptr<ViewWrapper>(wrapper));
    });
}

Platform_qtwidgets::~Platform_qtwidgets()
{
}

const char *Platform_qtwidgets::name() const
{
    return "qtwidgets";
}

bool Platform_qtwidgets::hasActivePopup() const
{
    return qApp->activePopupWidget() != nullptr;
}

std::shared_ptr<ViewWrapper> Platform_qtwidgets::qobjectAsView(QObject *obj) const
{
    if (auto w = qobject_cast<QWidget *>(obj)) {
        ViewWrapper *wrapper = new Views::ViewWrapper_qtwidgets(w);
        return std::shared_ptr<ViewWrapper>(wrapper);
    }

    return nullptr;
}

std::shared_ptr<Window> Platform_qtwidgets::windowFromQWindow(QWindow *qwindow) const
{
    return std::shared_ptr<Window>(new Window_qtwidgets(qwindow));
}

FrameworkWidgetFactory *Platform_qtwidgets::createDefaultFrameworkWidgetFactory()
{
    return new DefaultWidgetFactory_qtwidgets();
}

Window::Ptr Platform_qtwidgets::windowAt(QPoint globalPos) const
{
    if (auto qwindow = qApp->QGuiApplication::topLevelAt(globalPos)) {
        auto window = new Window_qtwidgets(qwindow);
        return Window::Ptr(window);
    }

    return {};
}

int Platform_qtwidgets::screenNumberFor(View *view) const
{
    if (auto widget = qobject_cast<QWidget *>(view->asQObject())) {
        if (QWindow *qtwindow = widget->window()->windowHandle())
            return screenNumberForQWindow(qtwindow);
    }

    return -1;
}

QSize Platform_qtwidgets::screenSizeFor(View *view) const
{
    if (auto widget = qobject_cast<QWidget *>(view->asQObject())) {
        if (QScreen *screen = widget->screen()) {
            return screen->size();
        }
    }

    return {};
}

#ifdef DOCKS_DEVELOPER_MODE

Platform_qtwidgets::Platform_qtwidgets(int argc, char *argv[])
{
    qputenv("KDDOCKWIDGETS_SHOW_DEBUG_WINDOW", "");
    new QApplication(argc, argv);
    qApp->setStyle(QStyleFactory::create(QStringLiteral("fusion")));
    init();
}

void Platform_qtwidgets::tests_initPlatform_impl()
{
    Platform_qt::tests_initPlatform_impl();
}

void Platform_qtwidgets::tests_deinitPlatform_impl()
{
    Platform_qt::tests_deinitPlatform_impl();
}

#endif
