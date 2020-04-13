/*
  This file is part of KDDockWidgets.

  Copyright (C) 2020 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com
  Author: Sérgio Martins <sergio.martins@kdab.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Item_p.h"

#include <QEvent>
#include <QDebug>
#include <QScopedValueRollback>

using namespace Layouting;


ItemContainer *Item::root() const
{
    return m_parent ? m_parent->root()
                    : const_cast<ItemContainer*>(qobject_cast<const ItemContainer*>(this));
}

QRect Item::mapToRoot(QRect r) const
{
    const QPoint topLeft = mapToRoot(r.topLeft());
    r.moveTopLeft(topLeft);
    return r;
}

QPoint Item::mapToRoot(QPoint p) const
{
    if (isRoot())
        return p;

    return p + parentContainer()->mapToRoot(parentContainer()->pos());
}

QPoint Item::mapFromRoot(QPoint p) const
{
    ItemContainer *c = parentContainer();
    while (c) {
        p = p - c->pos();
        c = c->parentContainer();
    }

    return p;
}

QRect Item::mapFromRoot(QRect r) const
{
    const QPoint topLeft = mapFromRoot(r.topLeft());
    r.moveTopLeft(topLeft);
    return r;
}

QPoint Item::mapFromParent(QPoint p) const
{
    if (isRoot())
        return p;

    return p - pos();
}

void Item::setFrame(QWidget *w)
{
    Q_ASSERT(!w || !m_widget);

    if (m_widget) {
        m_widget->removeEventFilter(this);
        disconnect(m_widget, &QObject::destroyed, this, &Item::onWidgetDestroyed);
        disconnect(m_widget, SIGNAL(layoutInvalidated()), this, SLOT(onWidgetLayoutRequested()));
        disconnect(m_widget, &QObject::objectNameChanged, this, &Item::updateObjectName);
    }

    m_widget = w;

    if (m_widget) {
        m_widget->installEventFilter(this);
        m_widget->setParent(m_hostWidget);
        setMinSize(widgetMinSize(m_widget));

        connect(m_widget, &QObject::objectNameChanged, this, &Item::updateObjectName);
        connect(m_widget, &QObject::destroyed, this, &Item::onWidgetDestroyed);
        connect(m_widget, SIGNAL(layoutInvalidated()), this, SLOT(onWidgetLayoutRequested())); // TODO: old-style

        if (m_sizingInfo.geometry.isEmpty()) {
            setGeometry(mapFromRoot(m_widget->geometry()));
        } else {
            updateWidgetGeometries();
        }
    }

    updateObjectName();

}

void Item::updateWidgetGeometries()
{
    if (m_widget)
        m_widget->setGeometry(mapToRoot(m_sizingInfo.geometry));
}

void Item::ref()
{
    m_refCount++;
}

void Item::unref()
{
    Q_ASSERT(m_refCount > 0);
    m_refCount--;
    if (m_refCount == 0) {
        Q_ASSERT(!isRoot());
        parentContainer()->removeItem(this);
    }
}

int Item::refCount() const
{
    return m_refCount;
}

QWidget *Item::hostWidget() const
{
    return m_hostWidget;
}

void Item::restorePlaceholder(QWidget *widget)
{
    Q_ASSERT(isVisible() && !frame());
    setFrame(widget);
    setIsVisible(true);
}

void Item::setHostWidget(QWidget *host)
{
    if (m_hostWidget != host) {
        m_hostWidget = host;
        if (m_widget) {
            m_widget->setParent(host);
            m_widget->setVisible(true);
            updateWidgetGeometries();
        }
    }
}

void Item::resize(QSize newSize)
{
    setSize(newSize);
}

QSize Item::missingSize() const
{
    QSize missing = minSize() - this->size();
    missing.setWidth(qMax(missing.width(), 0));
    missing.setHeight(qMax(missing.height(), 0));

    return missing;
}

int Item::missingLength(Qt::Orientation o) const
{
    return Layouting::length(missingSize(), o);
}

bool Item::isBeingInserted() const
{
    return m_sizingInfo.isBeingInserted;
}

void Item::setBeingInserted(bool is)
{
    m_sizingInfo.isBeingInserted = is;
}

void Item::setParentContainer(ItemContainer *parent)
{
    if (parent != m_parent) {
        if (m_parent) {
            disconnect(this, &Item::minSizeChanged, m_parent, &ItemContainer::onChildMinSizeChanged);
            disconnect(this, &Item::visibleChanged, m_parent, &ItemContainer::onChildVisibleChanged);
            Q_EMIT visibleChanged(this, false);
        }

        m_parent = parent;

        if (parent) {
            connect(this, &Item::minSizeChanged, parent, &ItemContainer::onChildMinSizeChanged);
            connect(this, &Item::visibleChanged, m_parent, &ItemContainer::onChildVisibleChanged);
            setHostWidget(parent->hostWidget());
            updateWidgetGeometries();

            Q_EMIT visibleChanged(this, isVisible());
        }

        QObject::setParent(parent);
    }
}

ItemContainer *Item::parentContainer() const
{
    return m_parent;
}

const ItemContainer *Item::asContainer() const
{
    return qobject_cast<const ItemContainer*>(this);
}

ItemContainer *Item::asContainer()
{
    return qobject_cast<ItemContainer*>(this);
}

void Item::setMinSize(QSize sz)
{
    Q_ASSERT(!isContainer());

    if (sz != m_sizingInfo.minSize) {
        m_sizingInfo.minSize = sz;
        setSize(size().expandedTo(sz));
    }
}

void Item::setMaxSize(QSize sz)
{
    Q_ASSERT(!isContainer());
    m_sizingInfo.maxSize = sz;
}

QSize Item::minSize() const
{
    return m_sizingInfo.minSize;
}

QSize Item::maxSize() const
{
    return m_sizingInfo.maxSize;
}

void Item::setPos(QPoint pos)
{
    QRect geo = m_sizingInfo.geometry;
    geo.moveTopLeft(pos);
    setGeometry(geo);
}

void Item::setPos(int pos, Qt::Orientation o)
{
    if (o == Qt::Vertical) {
        setPos({ x(), pos });
    } else {
        setPos({ pos, y() });
    }
}

int Item::pos(Qt::Orientation o) const
{
    return o == Qt::Vertical ? y() : x();
}

void Item::insertItem(Item *item, Location loc)
{
    Q_ASSERT(item != this);
    if (m_parent->hasOrientationFor(loc)) {
        const bool locIsSide1 = locationIsSide1(loc);
        int indexInParent = m_parent->indexOfVisibleChild(this);
        if (!locIsSide1)
            indexInParent++;

        const Qt::Orientation orientation = orientationForLocation(loc);
        if (orientation != m_parent->orientation()) {
            Q_ASSERT(m_parent->visibleChildren().size() == 1);
            // This is the case where the container only has one item, so it's both vertical and horizontal
            // Now its orientation gets defined
            m_parent->m_orientation = orientation;
        }

        m_parent->insertItem(item, indexInParent);
    } else {
        ItemContainer *container = m_parent->convertChildToContainer(this);
        container->insertItem(item, loc);
    }

    checkSanity();
}

/** static */
QSize Item::hardcodedMinimumSize()
{
    return QSize(KDDOCKWIDGETS_MIN_WIDTH, KDDOCKWIDGETS_MIN_HEIGHT);
}

int Item::x() const
{
    return m_sizingInfo.geometry.x();
}

int Item::y() const
{
    return m_sizingInfo.geometry.y();
}

int Item::width() const
{
    return m_sizingInfo.geometry.width();
}

int Item::height() const
{
    return m_sizingInfo.geometry.height();
}

QSize Item::size() const
{
    return m_sizingInfo.geometry.size();
}

void Item::setSize(QSize sz)
{
    QRect newGeo = m_sizingInfo.geometry;
    newGeo.setSize(sz);
    setGeometry(newGeo);
}

QPoint Item::pos() const
{
    return m_sizingInfo.geometry.topLeft();
}

int Item::position(Qt::Orientation o) const
{
    return o == Qt::Vertical ? y()
                             : x();
}

QRect Item::geometry() const
{
    return isBeingInserted() ? QRect()
                             : m_sizingInfo.geometry;
}

bool Item::isContainer() const
{
    return m_isContainer;
}

Qt::Orientation Item::orientation() const
{
    return m_orientation;
}

int Item::minLength(Qt::Orientation o) const
{
    return Layouting::length(minSize(), o);
}

void Item::setLength(int length, Qt::Orientation o)
{
    Q_ASSERT(length > 0);
    if (o == Qt::Vertical)
        setSize({ width(), length });
    else
        setSize({ length, height() });
}

void Item::setLength_recursive(int length, Qt::Orientation o)
{
    setLength(length, o);
}

int Item::length(Qt::Orientation o) const
{
    return Layouting::length(size(), o);
}

int Item::availableLength(Qt::Orientation o) const
{
    return length(o) - minLength(o);
}

bool Item::isPlaceholder() const
{
    return !isVisible();
}

bool Item::isVisible() const
{
    return m_isVisible;
}

void Item::setIsVisible(bool is)
{
    if (is != m_isVisible) {

        if (is)
            setBeingInserted(true);

        m_isVisible = is;
        Q_EMIT minSizeChanged(this); // min-size is 0x0 when hidden
        Q_EMIT visibleChanged(this, is);

        if (m_widget) {
            m_widget->setGeometry(mapToRoot(m_sizingInfo.geometry));
            m_widget->setVisible(is);
        }

        setBeingInserted(false);
    }
}

void Item::setGeometry_recursive(QRect rect)
{
    // Recursiveness doesn't apply for for non-container items
    setGeometry(rect);
}

Item *Item::neighbour(Side side) const
{
    return m_parent ? m_parent->neighbourFor(this, side)
                    : nullptr;
}

int Item::separatorThickness()
{
    return 5;
}

bool Item::checkSanity() const
{
    if (minSize().width() > width() || minSize().height() > height()) {
        qWarning() << Q_FUNC_INFO << "Size constraints not honoured" << this
                   << "; min=" << minSize() << "; size=" << size();
        root()->dumpLayout();
        return false;
    }

    if (m_widget && false) { // Uncomment only after honouring layoutInvalidated()
        if (mapFromRoot(m_widget->geometry()) != geometry()) {
            qWarning() << Q_FUNC_INFO << "Guest widget doesn't have correct geometry. has="
                       << mapFromRoot(m_widget->geometry())
                       << m_widget->geometry()
                       << geometry()
                       << this
                       << m_widget;
            return false;
        }
    }

    return true;
}

void Item::setGeometry(QRect rect)
{
    QRect &m_geometry = m_sizingInfo.geometry;

    if (rect != m_geometry) {
        const QRect oldGeo = m_geometry;
        m_geometry = rect;

        if (rect.isEmpty()) {
            root()->dumpLayout();
            Q_ASSERT(false);
        }

        const QSize minSz = minSize();
        if (rect.width() < minSz.width() || rect.height() < minSz.height()) {
            root()->dumpLayout();
            qWarning() << Q_FUNC_INFO << this << "Constraints not honoured."
                       << "sz=" << rect.size() << "; min=" << minSz
                       << ": parent=" << parentContainer();

        }

        Q_EMIT geometryChanged();

        if (oldGeo.x() != x())
            Q_EMIT xChanged();
        if (oldGeo.y() != y())
            Q_EMIT yChanged();
        if (oldGeo.width() != width())
            Q_EMIT widthChanged();
        if (oldGeo.height() != height())
            Q_EMIT heightChanged();

        updateWidgetGeometries();
    }
}

void Item::dumpLayout(int level)
{
    QString indent;
    indent.fill(QLatin1Char(' '), level);
    const QString beingInserted = m_sizingInfo.isBeingInserted ? QStringLiteral(";beingInserted;")
                                                               : QString();
    const QString visible = !isVisible() ? QStringLiteral(";hidden;")
                                         : QString();

    qDebug().noquote() << indent << "- Widget: " << objectName()
                       << m_sizingInfo.geometry// << "r=" << m_geometry.right() << "b=" << m_geometry.bottom()
                       << visible << beingInserted << this;
}

Item::Item(QWidget *hostWidget, ItemContainer *parent)
    : QObject(parent)
    , m_isContainer(false)
    , m_parent(parent)
    , m_hostWidget(hostWidget)
{
}

Item::Item(bool isContainer, QWidget *hostWidget, ItemContainer *parent)
    : QObject(parent)
    , m_isContainer(isContainer)
    , m_parent(parent)
    , m_hostWidget(hostWidget)
{
}

bool Item::eventFilter(QObject *widget, QEvent *e)
{
    if (e->type() != QEvent::ParentChange)
        return false;

    if (widget->parent() != hostWidget()) {
        // Frame was detached into floating window. Turn into placeholder
        Q_ASSERT(isVisible());
        turnIntoPlaceholder();
    }

    return false;
}


void Item::turnIntoPlaceholder()
{
    Q_ASSERT(!isContainer());

    // Turning into placeholder just means hidding it. So we can show it again in its original position.
    // Call removeItem() so we share the code for making the neighbours grow into the space that becomes available
    // after hidding this one
    parentContainer()->removeItem(this, /*hardDelete=*/ false);

    // TODO: Visible widgets changed signal ?
}

void Item::updateObjectName()
{
    if (m_widget) {
        setObjectName(m_widget->objectName().isEmpty() ? QStringLiteral("widget") : m_widget->objectName());
    } else if (!isVisible()) {
        setObjectName(QStringLiteral("hidden"));
    } else if (!m_widget) {
        setObjectName(QStringLiteral("null"));
    } else {
        setObjectName(QStringLiteral("empty"));
    }
}

void Item::onWidgetDestroyed()
{
    if (m_refCount) {
        turnIntoPlaceholder();
    } else {
        Q_ASSERT(!isRoot());
        parentContainer()->removeItem(this);
    }
}

void Item::onWidgetLayoutRequested()
{
    if (m_widget && m_widget->size() != size()) {
        // TODO
        qDebug() << Q_FUNC_INFO << "TODO: Not implemented yet"
                   << m_widget->size()
                   << m_sizingInfo.geometry
                   << m_sizingInfo.isBeingInserted;
    }
}

bool Item::isRoot() const
{
    return m_parent == nullptr;
}

bool Item::isVertical() const
{
    return m_orientation == Qt::Vertical;
}

bool Item::isHorizontal() const
{
    return m_orientation == Qt::Horizontal;
}

int Item::visibleCount_recursive() const
{
    return isVisible() ? 1 : 0;
}

ItemContainer::ItemContainer(QWidget *hostWidget, ItemContainer *parent)
    : Item(true, hostWidget, parent)
{
    Q_ASSERT(parent);
    connect(this, &Item::xChanged, this, [this] {
        for (Item *item : qAsConst(m_children)) {
            Q_EMIT item->xChanged();
        }
    });

    connect(this, &Item::yChanged, this, [this] {
        for (Item *item : qAsConst(m_children)) {
            Q_EMIT item->yChanged();
        }
    });
}

ItemContainer::ItemContainer(QWidget *hostWidget)
    : Item(true, hostWidget, /*parentContainer=*/ nullptr)
{
    // CTOR for root item
    Q_ASSERT(hostWidget);
}

bool ItemContainer::checkSanity() const
{
    if (!Item::checkSanity())
        return false;

    if (numChildren() == 0 && !isRoot()) {
        qWarning() << Q_FUNC_INFO << "Container is empty. Should be deleted";
        return false;
    }

    // Check that the geometries don't overlap
    int expectedPos = 0;
    for (Item *item : m_children) {
        if (!item->isVisible())
            continue;
        const int pos = Layouting::pos(item->pos(), m_orientation);
        if (expectedPos != pos) {
            root()->dumpLayout();
            qWarning() << Q_FUNC_INFO << "Unexpected pos" << pos << "; expected=" << expectedPos
                       << "; for item=" << item
                       << "; isContainer=" << item->isContainer();
            return false;
        }

        expectedPos = pos + Layouting::length(item->size(), m_orientation) + separatorThickness();
    }

    const int h1 = Layouting::length(size(), oppositeOrientation(m_orientation));
    for (Item *item : m_children) { 
        if (item->parentContainer() != this) {
            qWarning() << "Invalid parent container for" << item
                       << "; is=" << item->parentContainer() << "; expected=" << this;
            return false;
        }

        if (item->parent() != this) {
            qWarning() << "Invalid QObject parent for" << item
                       << "; is=" << item->parent() << "; expected=" << this;
            return false;
        }

        // Check the children height (if horizontal, and vice-versa)
        const int h2 = Layouting::length(item->size(), oppositeOrientation(m_orientation));
        if (h1 != h2) {
            qWarning() << Q_FUNC_INFO << "Invalid size for item." << item
                       << "Container.length=" << h1 << "; item.length=" << h2;
            root()->dumpLayout();
            return false;
        }

        if (!rect().contains(item->geometry())) {
            qWarning() << Q_FUNC_INFO << "Item geo is out of bounds. item=" << item << "; geo="
                       << item->geometry() << "; container.rect=" << rect();
            root()->dumpLayout();
            return false;
        }

        if (!item->checkSanity())
            return false;
    }

    const Item::List visibleChildren = this->visibleChildren();
    if (!visibleChildren.isEmpty()) {
        int occupied = qMax(0, Item::separatorThickness() * (visibleChildren.size() - 1));
        for (Item *item : visibleChildren) {
            occupied += item->length(m_orientation);
        }

        if (occupied != length()) {
            root()->dumpLayout();
            qWarning() << Q_FUNC_INFO << "Unexpected length. Expected=" << length()
                       << "; got=" << occupied;
            return false;
        }

        const QVector<double> percentages = childPercentages();
        const double totalPercentage = std::accumulate(percentages.begin(), percentages.end(), 0.0);
        if (!qFuzzyCompare(totalPercentage, 1.0)) {
            qWarning() << Q_FUNC_INFO << "Percentages don't add up"
                       << totalPercentage << percentages;
            const_cast<ItemContainer*>(this)->updateChildPercentages();
            qWarning() << Q_FUNC_INFO << childPercentages();
        }
    }

    return true;
}

bool ItemContainer::hasOrientation() const
{
    return isVertical() || isHorizontal();
}

int ItemContainer::numChildren() const
{
    return m_children.size();
}

int ItemContainer::numVisibleChildren() const
{
    int num = 0;
    for (Item *child : m_children) {
        if (child->isVisible())
            num++;
    }
    return num;
}

int ItemContainer::indexOfChild(const Item *item) const
{
    return m_children.indexOf(const_cast<Item *>(item));
}

int ItemContainer::indexOfVisibleChild(const Item *item) const
{
    const Item::List items = visibleChildren();
    return items.indexOf(const_cast<Item*>(item));
}

void ItemContainer::removeItem(Item *item, bool hardRemove)
{
    Q_ASSERT(!item->isRoot());
    if (contains(item)) {
        Item *side1Item = visibleNeighbourFor(item, Side1);
        Item *side2Item = visibleNeighbourFor(item, Side2);
        const bool isContainer = item->isContainer();
        const bool wasVisible = !isContainer && item->isVisible();

        if (hardRemove) {
            m_children.removeOne(item);
            item->setParentContainer(nullptr);
            delete item;
            if (!isContainer)
                Q_EMIT root()->numItemsChanged();
        } else {
            if (wasVisible) {
                item->setIsVisible(false);
                item->setFrame(nullptr);
            } else {
                // Nothing to do, item was already a placeholder.
                return;
            }
        }

        if (wasVisible) {
            Q_EMIT root()->numVisibleItemsChanged(root()->numVisibleChildren());
        }

        const bool containerShouldBeRemoved = !isRoot() && ((hardRemove && isEmpty()) ||
                                                            (!hardRemove && !hasVisibleChildren()));

        if (containerShouldBeRemoved) {
            parentContainer()->removeItem(this, hardRemove);
        } else {
            // Neighbours will occupy the space of the deleted item
            growNeighbours(side1Item, side2Item);
            Q_EMIT itemsChanged();
            updateChildPercentages();
        }
    } else {
        // Not ours, ask parent
        item->parentContainer()->removeItem(item, hardRemove);
    }
}

bool ItemContainer::isEmpty() const
{
    return m_children.isEmpty();
}

void ItemContainer::setGeometry_recursive(QRect rect)
{
    setPos(rect.topLeft());

    // Call resize, which is recursive and will resize the children too
    resize(rect.size());
}

ItemContainer *ItemContainer::convertChildToContainer(Item *leaf)
{
    const int index = indexOfChild(leaf);
    Q_ASSERT(index != -1);
    auto container = new ItemContainer(hostWidget(), this);
    insertItem(container, index, /*growItem=*/false);
    m_children.removeOne(leaf);
    container->setGeometry(leaf->geometry());
    container->insertItem(leaf, Location_OnTop);
    Q_EMIT itemsChanged();
    updateChildPercentages();

    return container;
}

void ItemContainer::insertItem(Item *item, Location loc)
{
    item->setIsVisible(false);

    Q_ASSERT(item != this);
    if (contains(item)) {
        qWarning() << Q_FUNC_INFO << "Item already exists";
        return;
    }

    const Qt::Orientation locOrientation = orientationForLocation(loc);

    if (hasOrientationFor(loc)) {
        if (m_children.size() == 1) {
            // 2 items is the minimum to know which orientation we're layedout
            m_orientation = locOrientation;
        }

        const int index = locationIsSide1(loc) ? 0 : m_children.size();
        insertItem(item, index);
    } else {
        // Inserting directly in a container ? Only if it's root.
        Q_ASSERT(isRoot());
        auto container = new ItemContainer(hostWidget(), this);
        container->setChildren(m_children);
        container->m_orientation = m_orientation;
        m_children.clear();
        m_orientation = oppositeOrientation(m_orientation);
        insertItem(container, 0, /*grow=*/ false);
        container->setGeometry(rect());
        container->setIsVisible(container->numVisibleChildren() > 0);

        // Now we have the correct orientation, we can insert
        insertItem(item, loc);
    }

    updateChildPercentages();
    checkSanity();
}

void ItemContainer::onChildMinSizeChanged(Item *child)
{
    const QSize missingSize = this->missingSize();
    if (!missingSize.isNull()) {
        QScopedValueRollback<bool> resizing(m_isResizing, true);

        if (isRoot()) {
            // Resize the whole layout
            resize(size() + missingSize);
            Item::List childs = visibleChildren();
            Item *lastChild = nullptr;
            for (int i = childs.size() - 1; i >= 0; i--) {
                if (!childs[i]->isBeingInserted()) {
                    lastChild = childs[i];
                    break;
                }
            }

            if (lastChild) {
                QRect r = lastChild->geometry();
                r.adjust(0, 0, missingSize.width(), missingSize.height());
                lastChild->setGeometry(r);
            }
        }

        // Our min-size changed, notify our parent, and so on until it reaches root()
        Q_EMIT minSizeChanged(this);
    }

    if (numVisibleChildren() == 1) {
        // The easy case. Child is alone in the layout, occupies everything.
        child->setGeometry(rect());
        return;
    }

    if (child->isBeingInserted())
        return;

    const QSize missingForChild = child->missingSize();
    if (missingForChild.isNull()) {
        // The child changed its minSize. Thanks for letting us know, but there's nothing needed doing.
        // Item is bigger than its minimum.
        //Q_ASSERT(false); // Never happens I think. Remove this if!
        return;
    }
    // Child has some growing to do. It will grow left and right equally, (and top-bottom), as needed.
    growItem(child, Layouting::length(missingForChild, m_orientation), GrowthStrategy::BothSidesEqually);
}

void ItemContainer::onChildVisibleChanged(Item */*child*/, bool visible)
{
    const int numVisible = numVisibleChildren();
    if (visible && numVisible == 1) {
        // Child became visible and there's only 1 visible child. Meaning there were 0 visible before.
        Q_EMIT visibleChanged(this, true);
    } else if (!visible && numVisible == 0) {
        Q_EMIT visibleChanged(this, false);
    }
}

QRect ItemContainer::suggestedDropRect(QSize minSize, const Item *relativeTo, Location loc) const
{
    if (relativeTo && !relativeTo->parentContainer()) {
        qWarning() << Q_FUNC_INFO << "No parent container";
        return {};
    }

    if (relativeTo && relativeTo->parentContainer() != this) {
        qWarning() << Q_FUNC_INFO << "Called on the wrong container";
        return {};
    }

    if (relativeTo && !relativeTo->isVisible()) {
        qWarning() << Q_FUNC_INFO << "relative to isn't visible";
        return {};
    }

    if (loc == Location_None) {
        qWarning() << Q_FUNC_INFO << "Invalid location";
        return {};
    }

    const int itemMin = Layouting::length(minSize, m_orientation);
    const int available = availableLength() - Item::separatorThickness();
    const SizingInfo::List sizes = this->sizes();
    const int count = sizes.count();

    if (relativeTo && count == 1) {
        // If it's the only item then the result is that it's relative to the whole layout
        // So simplify our code
        relativeTo = nullptr;
    }

    if (relativeTo) {
        const int equitativeLength = usableLength() / (m_children.size() + 1);
        const int suggestedLength = qMax(qMin(available, equitativeLength), itemMin);
        const int indexOfRelativeTo = indexOfVisibleChild(relativeTo);

        int suggestedPos = 0;

        //const int availableSide2 = availableOnSide(m_children.at(indexOfRelativeTo), Side2);
        const int relativeToPos = relativeTo->position(m_orientation);
        const QRect relativeToGeo = relativeTo->geometry();
        const Qt::Orientation orientation = orientationForLocation(loc);

        if (orientation == m_orientation) {
            if (sideForLocation(loc) == Side1) {
                if (indexOfRelativeTo == 0) {
                    suggestedPos = 0;
                } else {
                    const LengthOnSide side1Length = lengthOnSide(sizes, indexOfRelativeTo - 1, Side1, m_orientation);
                    const LengthOnSide side2Length = lengthOnSide(sizes, indexOfRelativeTo, Side2, m_orientation);
                    const int min1 = relativeToPos - side1Length.available();
                    const int max2 = relativeToPos + side2Length.available() - suggestedLength;
                    suggestedPos = relativeToPos - suggestedLength / 2;
                    suggestedPos = qBound(min1, suggestedPos, max2);
                }
            } else { // Side2
                 if (indexOfRelativeTo == count - 1) { // is last
                     suggestedPos = length() - suggestedLength;
                 } else {
                     const LengthOnSide side1Length = lengthOnSide(sizes, indexOfRelativeTo, Side1, m_orientation);
                     const LengthOnSide side2Length  = lengthOnSide(sizes, indexOfRelativeTo + 1, Side2, m_orientation);
                     const int min1 = relativeToPos + relativeTo->length(m_orientation) - side1Length.available();
                     const int max2 = relativeToPos + relativeTo->length(m_orientation) + side2Length.available() - suggestedLength;
                     suggestedPos = relativeToPos + relativeTo->length(m_orientation) - (suggestedLength / 2);
                     suggestedPos = qBound(min1, suggestedPos, max2);
                 }
            }

        } else {
            // Incompatible orientations, takes half then.
            switch (loc) {
            case Location_OnLeft:
                suggestedPos = relativeToGeo.x();
                break;
            case Location_OnTop:
                suggestedPos = relativeToGeo.y();
                break;
            case Location_OnRight:
                suggestedPos = relativeToGeo.right() - suggestedLength + 1;
                break;
            case Location_OnBottom:
                suggestedPos = relativeToGeo.bottom() - suggestedLength + 1;
                break;
            default:
                Q_ASSERT(false);
            }
        }


        QRect rect;

        if (orientationForLocation(loc) == Qt::Vertical) {
            rect.setTopLeft(QPoint(relativeTo->x(), suggestedPos));
            rect.setSize(QSize(relativeTo->width(), suggestedLength));
        } else {
            rect.setTopLeft(QPoint(suggestedPos, relativeTo->y()));
            rect.setSize(QSize(suggestedLength, relativeTo->height()));
        }

        /*qDebug() << "; min1=" << min1
                 << "; max2=" << max2
                 << "; a1=" << side1Length.available()
                 << "; a2=" << side2Length.available()
                 << "; indexOfRelativeTo=" << indexOfRelativeTo
                 << "; available=" << available;*/

        return rect;

    } else if (isRoot()) {
        // Relative to the window itself
        QRect rect = this->rect();
        const int oneThird = length() / 3;
        const int suggestedLength = qMax(qMin(available, oneThird), itemMin);

        switch (loc) {
        case Location_OnLeft:
            rect.setWidth(suggestedLength);
            break;
        case Location_OnTop:
            rect.setHeight(suggestedLength);
            break;
        case Location_OnRight:
            rect.adjust(rect.width() - suggestedLength, 0, 0, 0);
            break;
        case Location_OnBottom:
            rect.adjust(0, rect.bottom() - suggestedLength, 0, 0);
            break;
        case Location_None:
            return {};
        }

        return rect;

    } else {
        qWarning() << Q_FUNC_INFO << "Shouldn't happen";
    }

    return {};
}

void ItemContainer::positionItems()
{
    SizingInfo::List sizes = this->sizes();
    positionItems(/*by-ref=*/sizes);
    applyPositions(sizes);

    updateChildPercentages();
}

void ItemContainer::applyPositions(const SizingInfo::List &sizes)
{
    const Item::List items = visibleChildren();
    const int count = items.size();
    Q_ASSERT(count == sizes.size());
    for (int i = 0; i < count; ++i) {
        Item *item = items.at(i);
        const SizingInfo &sizing = sizes[i];
        if (sizing.isBeingInserted) {
            continue;
        }

        const Qt::Orientation oppositeOrientation = Layouting::oppositeOrientation(m_orientation);
        // If the layout is horizontal, the item will have the height of the container. And vice-versa
        item->setLength_recursive(sizing.length(oppositeOrientation), oppositeOrientation);

        item->setPos(sizing.geometry.topLeft());
    }
}

void ItemContainer::positionItems(SizingInfo::List &sizes)
{
    int nextPos = 0;
    const int count = sizes.count();
    const Qt::Orientation oppositeOrientation = Layouting::oppositeOrientation(m_orientation);
    for (int i = 0; i < count; ++i) {
        SizingInfo &sizing = sizes[i];
        if (sizing.isBeingInserted) {
            nextPos += Item::separatorThickness();
            continue;
        }

        // If the layout is horizontal, the item will have the height of the container. And vice-versa
        const int oppositeLength = Layouting::length(size(), oppositeOrientation);
        sizing.setLength(oppositeLength, oppositeOrientation);

        sizing.setPos(nextPos, m_orientation);
        nextPos += sizing.length(m_orientation) + Item::separatorThickness();
    }
}

void ItemContainer::clear()
{
    for (Item *item : qAsConst(m_children)) {
        if (ItemContainer *container = item->asContainer())
            container->clear();

        delete item;
    }
    m_children.clear();
}

Item *ItemContainer::itemForFrame(const QWidget *w) const
{
    for (Item *item : m_children) {
        if (item->isContainer()) {
            if (Item *result = item->asContainer()->itemForFrame(w))
                return result;
        } else if (item->frame() == w) {
            return item;
        }
    }

    return nullptr;
}

int ItemContainer::visibleCount_recursive() const
{
    int count = 0;
    for (Item *item : m_children) {
        count += item->visibleCount_recursive();
    }

    return count;
}

int ItemContainer::count_recursive() const
{
    int count = 0;
    for (Item *item : m_children) {
        if (auto c = item->asContainer()) {
            count += c->count_recursive();
        } else {
            count++;
        }
    }

    return count;
}

Item *ItemContainer::itemAt(QPoint p) const
{
    for (Item *item : m_children) {
        if (item->isVisible() && item->geometry().contains(p))
            return item;
    }

    return nullptr;
}

Item *ItemContainer::itemAt_recursive(QPoint p) const
{
    if (Item *item = itemAt(p)) {
        if (auto c = item->asContainer()) {
            return c->itemAt_recursive(c->mapFromParent(p));
        } else {
            return item;
        }
    }

    return nullptr;
}

Item::List ItemContainer::items_recursive() const
{
   Item::List items;
   items.reserve(30); // sounds like a good upper number to minimize allocations
   for (Item *item : m_children) {
       if (auto c  = item->asContainer()) {
           items << c->items_recursive();
       } else {
           items << item;
       }
   }

   return items;
}

void ItemContainer::setHostWidget(QWidget *host)
{
    Item::setHostWidget(host);
    for (Item *item : qAsConst(m_children)) {
        item->setHostWidget(host);
    }
}

void ItemContainer::setIsVisible(bool is)
{
    for (Item *item : qAsConst(m_children)) {
        item->setIsVisible(is);
    }
}

bool ItemContainer::isVisible() const
{
    return hasVisibleChildren();
}

void ItemContainer::setLength_recursive(int length, Qt::Orientation o)
{
    QSize sz = size();
    if (o == Qt::Vertical) {
        sz.setHeight(length);
    } else {
        sz.setWidth(length);
    }

    resize(sz);
}

void ItemContainer::insertItem(Item *item, int index, bool growItem)
{
    m_children.insert(index, item);
    item->setParentContainer(this);
    Q_EMIT itemsChanged();

    if (growItem)
        restorePlaceholder(item);

    if (!item->isContainer()) {
        if (item->isVisible())
            Q_EMIT root()->numVisibleItemsChanged(root()->numVisibleChildren());
        Q_EMIT root()->numItemsChanged();
    }
}

bool ItemContainer::hasChildren() const
{
    return !m_children.isEmpty();
}

bool ItemContainer::hasVisibleChildren() const
{
    for (Item *item : m_children)
        if (item->isVisible())
            return true;

    return false;
}

bool ItemContainer::hasOrientationFor(Location loc) const
{
    if (m_children.size() <= 1)
        return true;

    return m_orientation == orientationForLocation(loc);
}

Item::List ItemContainer::children() const
{
    return m_children;
}

Item::List ItemContainer::visibleChildren() const
{
    Item::List items;
    items.reserve(m_children.size());
    for (Item *item : m_children) {
        if (item->isVisible())
            items << item;
    }

    return items;
}

int ItemContainer::usableLength() const
{
    const int numVisibleChildren = this->numVisibleChildren();
    if (numVisibleChildren <= 1)
        return Layouting::length(size(), m_orientation);

    const int separatorWaste = separatorThickness() * (numVisibleChildren - 1);
    return length() - separatorWaste;
}

bool ItemContainer::hasSingleVisibleItem() const
{
    return numVisibleChildren() == 1;
}

bool ItemContainer::contains(const Item *item) const
{
    return m_children.contains(const_cast<Item *>(item));
}

bool ItemContainer::contains_recursive(const Item *item) const
{
    for (Item *it : m_children) {
        if (it == item) {
            return true;
        } else if (it->isContainer()) {
            if (it->asContainer()->contains_recursive(item))
                return true;
        }
    }

    return false;
}

void ItemContainer::setChildren(const Item::List children)
{
    m_children = children;
    for (Item *item : children)
        item->setParentContainer(this);
}

QSize ItemContainer::minSize() const
{
    int minW = 0;
    int minH = 0;

    if (!isEmpty()) {
        const Item::List visibleChildren = this->visibleChildren();
        for (Item *item : visibleChildren) {
            if (isVertical()) {
                minW = qMax(minW, item->minSize().width());
                minH += item->minSize().height();
            } else {
                minH = qMax(minH, item->minSize().height());
                minW += item->minSize().width();
            }
        }

        const int separatorWaste = (visibleChildren.size() - 1) * separatorThickness();
        if (isVertical())
            minH += separatorWaste;
        else
            minW += separatorWaste;
    }

    return { minW, minH };
}

QSize ItemContainer::maxSize() const
{
    int maxW = 0;
    int maxH = 0;

    if (!isEmpty()) {
        const Item::List visibleChildren = this->visibleChildren();
        for (Item *item : visibleChildren) {
            if (isVertical()) {
                maxW = qMin(maxW, item->maxSize().width());
                maxH += item->maxSize().height();
            } else {
                maxH = qMin(maxH, item->maxSize().height());
                maxW += item->maxSize().width();
            }
        }

        const int separatorWaste = (visibleChildren.size() - 1) * separatorThickness();
        if (isVertical())
            maxH += separatorWaste;
        else
            maxW += separatorWaste;
    }

    return { maxW, maxH };
}

void ItemContainer::resize(QSize newSize) // Rename to setSize_recursive
{
    QScopedValueRollback<bool> block(m_blockUpdatePercentages, true);

    const QSize minSize = this->minSize();
    if (newSize.width() < minSize.width() || newSize.height() < minSize.height()) {
        qWarning() << Q_FUNC_INFO << "New size doesn't respect size constraints"
                   << "; new=" << newSize
                   << "; min=" << minSize
                   << this;
        return;
    }

    const bool widthChanged = width() != newSize.width();
    const bool heightChanged = height() != newSize.height();
    if (!widthChanged && !heightChanged)
        return;

    const bool lengthChanged = (isVertical() && heightChanged) || (isHorizontal() && widthChanged);

    setSize(newSize);

    if (m_isResizing) {
        // We're already under a resize, nothing to do
        return;
    }

    const int totalNewLength = usableLength();
    int remaining = totalNewLength;

    const QVector<double> childPercentages = this->childPercentages();
    const Item::List children = visibleChildren();
    const int count = children.size();
    SizingInfo::List childSizes = sizes();

    // #1 Apply the new sizes, based on the % they occupied previously.
    // But apply them to our SizingInfo::List first before setting actual Item/QWidget geometries
    // Because we need step #2 where we ensure min sizes for each item are respected. We could
    // calculate and do everything in a single-step, but we already have the code for #2 in growItem()
    // so doing it in 2 steps will reuse much logic.
    for (int i = 0; i < count; ++i) {
        const bool isLast = i == count - 1;

        SizingInfo &itemSize = childSizes[i];

        const qreal childPercentage = childPercentages.at(i);
        const int newItemLength = lengthChanged ? (isLast ? remaining
                                                          : int(childPercentage * totalNewLength))
                                                : itemSize.length(m_orientation);

        if (newItemLength <= 0) {
            qWarning() << Q_FUNC_INFO << "Invalid resize. Dumping layout";
            root()->dumpLayout();
            Q_ASSERT(false);
            return;
        }

        remaining = remaining - newItemLength;

        if (isVertical()) {
            itemSize.geometry.setSize({ width(), newItemLength });
        } else {
            itemSize.geometry.setSize({ newItemLength, height() });
        }
    }

    positionItems(childSizes);

    // #2 Adjust sizes so that each item has at least Item::minSize.
    for (int i = 0; i < count; ++i) {
        SizingInfo &size = childSizes[i];
        const int missing = size.missingLength(m_orientation);
        if (missing == 0)
            continue;

        growItem(i, childSizes, missing, GrowthStrategy::BothSidesEqually);
        size.setLength(size.minLength(m_orientation), m_orientation); // TODO: growItem should do it instead ?
    }

    // #3 Sizes are now correct and honour min/max sizes. So apply them to our Items
    applySizes(childSizes);

    // #4. All sizes are correct. Just layed them out at the correct position. Spaced with 5px in between each other
    positionItems(); // TODO: Just a single applyGeometries ?
}

int ItemContainer::length() const
{
    return isVertical() ? height() : width();
}

QRect ItemContainer::rect() const
{
    QRect rect = m_sizingInfo.geometry;
    rect.moveTo(QPoint(0, 0));
    return rect;
}

void ItemContainer::dumpLayout(int level)
{
    QString indent;
    indent.fill(QLatin1Char(' '), level);
    const QString beingInserted = m_sizingInfo.isBeingInserted ? QStringLiteral("; beingInserted;")
                                                               : QString();
    const QString visible = !isVisible() ? QStringLiteral(";hidden;")
                                         : QString();

    const QString typeStr = isRoot() ? QStringLiteral("* Root: ")
                                     : "* Layout: ";

    qDebug().noquote() << indent << typeStr << m_orientation
                       << m_sizingInfo.geometry /*<< "r=" << m_geometry.right() << "b=" << m_geometry.bottom()*/
                       << "; this=" << this << beingInserted << visible
                       << "; %=" << childPercentages();
    for (Item *item : qAsConst(m_children)) {
        item->dumpLayout(level + 1);
    }
}

void ItemContainer::updateChildPercentages()
{
    if (m_blockUpdatePercentages)
        return;

    const int usable = usableLength();
    for (Item *item : m_children) {
        if (item->isVisible()) {
            item->m_sizingInfo.percentageWithinParent = (1.0 * item->length(m_orientation)) / usable;
            Q_ASSERT(!qFuzzyIsNull(item->m_sizingInfo.percentageWithinParent));
        } else {
            item->m_sizingInfo.percentageWithinParent = 0.0;
        }
    }
}

QVector<double> ItemContainer::childPercentages() const
{
    QVector<double> percentages;
    percentages.reserve(m_children.size());

    for (Item *item : m_children) {
        if (item->isVisible())
            percentages << item->m_sizingInfo.percentageWithinParent;
    }

    return percentages;
}

void ItemContainer::restorePlaceholder(Item *item)
{
    Q_ASSERT(contains(item));

    item->setIsVisible(true);
    if (numVisibleChildren() == 1)
        return;

    const int available = availableLength(); // Already deduced separator thickness, as it's visible now.

    const int max = item->length(m_orientation) + available;
    const int min = item->minLength(m_orientation);
    const int proposed = item->length(m_orientation);
    const int newLength = qBound(min, proposed, max);

    item->setLength_recursive(newLength, m_orientation);
    Q_ASSERT(item->isVisible());
    growItem(item, newLength + Item::separatorThickness(), GrowthStrategy::BothSidesEqually);
}

void ItemContainer::updateWidgetGeometries()
{
    for (Item *item : qAsConst(m_children))
        item->updateWidgetGeometries();
}

Item *ItemContainer::visibleNeighbourFor(const Item *item, Side side) const
{
    const Item::List children = visibleChildren();
    const int index = children.indexOf(const_cast<Item *>(item));
    const int neighbourIndex = side == Side1 ? index - 1
                                             : index + 1;

    return (neighbourIndex >= 0 && neighbourIndex < children.size()) ? children.at(neighbourIndex)
                                                                     : nullptr;
}

Item *ItemContainer::neighbourFor(const Item *item, Side side) const
{
    const int index = indexOfChild(item);
    const int neighbourIndex = side == Side1 ? index - 1
                                             : index + 1;

    return (neighbourIndex >= 0 && neighbourIndex < m_children.size()) ? m_children.at(neighbourIndex)
                                                                       : nullptr;
}

QSize ItemContainer::availableSize() const
{
    return size() - this->minSize();
}

int ItemContainer::availableLength() const
{
    return isVertical() ? availableSize().height()
                        : availableSize().width();
}

ItemContainer::LengthOnSide ItemContainer::lengthOnSide(const SizingInfo::List &sizes, int fromIndex,
                                                        Side side, Qt::Orientation o) const
{
    if (fromIndex < 0)
        return {};

    const int count = sizes.count();
    if (fromIndex >= count)
        return {};

    int start = 0;
    int end = -1;
    if (side == Side1) {
        start = 0;
        end = fromIndex;
    } else {
        start = fromIndex;
        end = count - 1;

    }

    LengthOnSide result;
    for (int i = start; i <= end; ++i) {
        const SizingInfo &size = sizes.at(i);
        result.length += size.length(o);
        result.minLength += size.minLength(o);
    }

    return result;
}

int ItemContainer::neighboursLengthFor(const Item *item, Side side, Qt::Orientation o) const
{
    const int index = indexOfChild(item);
    if (index == -1) {
        qWarning() << Q_FUNC_INFO << "Couldn't find item" << item;
        return 0;
    }

    if (o == m_orientation) {
        int neighbourLength = 0;
        int start = 0;
        int end = -1;
        if (side == Side1) {
            start = 0;
            end = index - 1;
        } else {
            start = index + 1;
            end = m_children.size() - 1;
        }

        for (int i = start; i <= end; ++i)
            neighbourLength += m_children.at(i)->length(m_orientation);

        return neighbourLength;
    } else {
        // No neighbours in the other orientation. Each container is bidimensional.
        return 0;
    }
}

int ItemContainer::neighboursLengthFor_recursive(const Item *item, Side side, Qt::Orientation o) const
{
    return neighboursLengthFor(item, side, o) + (isRoot() ? 0
                                                          : parentContainer()->neighboursLengthFor_recursive(this, side, o));

}

int ItemContainer::neighboursMinLengthFor(const Item *item, Side side, Qt::Orientation o) const
{
    const int index = indexOfChild(item);
    if (index == -1) {
        qWarning() << Q_FUNC_INFO << "Couldn't find item" << item;
        return 0;
    }

    if (o == m_orientation) {
        int neighbourMinLength = 0;
        int start = 0;
        int end = -1;
        if (side == Side1) {
            start = 0;
            end = index - 1;
        } else {
            start = index + 1;
            end = m_children.size() - 1;
        }

        for (int i = start; i <= end; ++i)
            neighbourMinLength += m_children.at(i)->minLength(m_orientation);

        return neighbourMinLength;
    } else {
        // No neighbours here
        return 0;
    }
}

int ItemContainer::neighboursMinLengthFor_recursive(const Item *item, Side side, Qt::Orientation o) const
{
    return neighboursMinLengthFor(item, side, o) + (isRoot() ? 0
                                                             : parentContainer()->neighboursMinLengthFor(this, side, o));
}

int ItemContainer::neighbourSeparatorWaste(const Item *item, Side side, Qt::Orientation o) const
{
    const int index = indexOfChild(item);
    if (index == -1) {
        qWarning() << Q_FUNC_INFO << "Couldn't find item" << item;
        return 0;
    }

    if (o == m_orientation) {
        if (side == Side1) {
            return index * Item::separatorThickness();
        } else {
            return (m_children.size() - 1 - index) * Item::separatorThickness();
        }
    } else {
        return 0;
    }
}

int ItemContainer::neighbourSeparatorWaste_recursive(const Item *item, Side side, Qt::Orientation orientation) const
{
    return neighbourSeparatorWaste(item, side, orientation) + (isRoot() ? 0
                                                                        : parentContainer()->neighbourSeparatorWaste(item, side, orientation));
}

int ItemContainer::availableOnSide(Item *child, Side side) const
{
    const int length = neighboursLengthFor(child, side, m_orientation);
    const int min = neighboursMinLengthFor(child, side, m_orientation);

    const int available = length - min;
    if (available < 0) {
        root()->dumpLayout();
        Q_ASSERT(false);
    }
    return available;
}

QSize ItemContainer::missingSizeFor(Item *item, Qt::Orientation o) const
{
    QSize missing = {0, 0};
    const QSize available = availableSize();
    const int anchorWasteW = (o == Qt::Vertical || !hasVisibleChildren()) ? 0 : Item::separatorThickness();
    const int anchorWasteH = (o == Qt::Vertical && hasVisibleChildren()) ? Item::separatorThickness() : 0;
    missing.setWidth(qMax(item->minSize().width() - available.width() + anchorWasteW, 0));
    missing.setHeight(qMax(item->minSize().height() - available.height() + anchorWasteH, 0));

    return missing;
}

QVariantList ItemContainer::items() const
{
    QVariantList items;
    items.reserve(m_children.size());

    for (auto item : m_children)
        items << QVariant::fromValue(item);

    return items;
}

void ItemContainer::growNeighbours(Item *side1Neighbour, Item *side2Neighbour)
{
    if (!side1Neighbour && !side2Neighbour)
        return;

    if (side1Neighbour && side2Neighbour) {
        // Give half/half to each neighbour
        QRect geo1 = side1Neighbour->geometry();
        QRect geo2 = side2Neighbour->geometry();

        if (isVertical()) {
            const int available = geo2.y() - geo1.bottom() - separatorThickness();
            geo1.setHeight(geo1.height() + available / 2);
            geo2.setTop(geo1.bottom() + separatorThickness() + 1);
        } else {
            const int available = geo2.x() - geo1.right() - separatorThickness();
            geo1.setWidth(geo1.width() + available / 2);
            geo2.setLeft(geo1.right() + separatorThickness() + 1);
        }

        side1Neighbour->setGeometry_recursive(geo1);
        side2Neighbour->setGeometry_recursive(geo2);

    } else if (side1Neighbour) {
        // Grow all the way to the right (or bottom if vertical)
        QRect geo = side1Neighbour->geometry();

        if (isVertical()) {
            geo.setBottom(rect().bottom());
        } else {
            geo.setRight(rect().right());
        }

        side1Neighbour->setGeometry_recursive(geo);

    } else if (side2Neighbour) {
        // Grow all the way to the left (or top if vertical)
        QRect geo = side2Neighbour->geometry();

        if (isVertical()) {
            geo.setTop(0);
        } else {
            geo.setLeft(0);
        }

        side2Neighbour->setGeometry_recursive(geo);
    }
}

void ItemContainer::growItem(int index, SizingInfo::List &sizes, int amount, GrowthStrategy growthStrategy)
{
    Q_ASSERT(index != -1);
    if (amount == 0)
        return;

    SizingInfo &sizingInfo = sizes[index];

    Q_ASSERT(growthStrategy == GrowthStrategy::BothSidesEqually);
    const int count = sizes.count();
    if (count == 1) {
        //There's no neighbours to push, we're alone. Occupy the full container
        sizingInfo.setLength(sizingInfo.length(m_orientation) + amount, m_orientation);
        return;
    }

    const LengthOnSide side1Length = lengthOnSide(sizes, index - 1, Side1, m_orientation);
    const LengthOnSide side2Length = lengthOnSide(sizes, index + 1, Side2, m_orientation);

    int available1 = side1Length.available();
    int available2 = side2Length.available();
    int side1Growth = 0;
    int side2Growth = 0;

    Q_ASSERT(amount <= available1 + available2);
    int missing = amount;
    while (missing > 0) {

        if (available1 == 0) {
            Q_ASSERT(available2 >= missing);
            side2Growth += missing;
            break;
        } else if (available2 == 0) {
            Q_ASSERT(available1 >= missing);
            side1Growth += missing;
            break;
        }

        const int toTake = qMax(1, missing / 2);
        const int took1 = qMin(toTake, available1);
        missing -= took1;
        side1Growth += took1;
        if (missing == 0)
            break;

        const int took2 = qMin(toTake, available2);
        missing -= took2;
        side2Growth += took2;
    }

    growItem(index, sizes, side1Growth, side2Growth);
}

void ItemContainer::growItem(Item *item, int amount, GrowthStrategy growthStrategy)
{
    const Item::List items = visibleChildren();
    const int index = items.indexOf(item);
    SizingInfo::List sizes = this->sizes();

    growItem(index, /*by-ref=*/sizes, amount, growthStrategy);
    applySizes(sizes);
}

void ItemContainer::applySizes(const SizingInfo::List &sizes)
{
    const Item::List items = visibleChildren();
    const int count = items.size();
    Q_ASSERT(count == sizes.size());

    for (int i = 0; i < count; ++i) {
        Item *item = items.at(i);
        item->resize(sizes[i].geometry.size());
    }

    positionItems();
}

SizingInfo::List ItemContainer::sizingInfosPerNeighbour(Item *item, Side side) const
{
    Item::List children = visibleChildren();
    const int indexOfChild = children.indexOf(item);
    int start = 0;
    int end = 0;
    if (side == Side1) {
        start = 0;
        end = indexOfChild - 1;
    } else {
        start = indexOfChild + 1;
        end = children.size() - 1;
    }

    SizingInfo::List result;
    result.reserve(end - start + 1);
    for (int i = start; i <= end; ++i) {
        Item *neighbour = children.at(i);
        result << neighbour->m_sizingInfo;
    }

    return result;
}

SizingInfo::List ItemContainer::sizes() const
{
    const Item::List children = visibleChildren();
    SizingInfo::List result;
    result.reserve(children.count());
    for (Item *item : children) {
        if (item->isContainer()) {
            item->m_sizingInfo.minSize = item->minSize();
        }
        result << item->m_sizingInfo;
    }

    return result;
}

QVector<int> ItemContainer::calculateSqueezes(SizingInfo::List::ConstIterator begin,
                                              SizingInfo::List::ConstIterator end, int needed) const
{
    QVector<int> availabilities;
    for (auto it = begin; it < end; ++it) {
        availabilities << it->availableLength(m_orientation);
    }

    const int count = availabilities.count();

    QVector<int> squeezes(count, 0);
    int missing = needed;
    while (missing > 0) {
        const int numDonors = std::count_if(availabilities.cbegin(), availabilities.cend(), [] (int num) {
            return num > 0;
        });

        if (numDonors == 0) {
            root()->dumpLayout();
            Q_ASSERT(false);
            return {};
        }

        int toTake = missing / numDonors;
        if (toTake == 0)
            toTake = missing;

        for (int i = 0; i < count; ++i) {
            const int available = availabilities.at(i);
            if (available == 0)
                continue;
            const int took = qMin(toTake, available);
            availabilities[i] -= took;
            missing -= took;
            squeezes[i] += took;
            if (missing == 0)
                break;
        }
    }

    return squeezes;
}

void ItemContainer::growItem(int index, SizingInfo::List &sizes, int side1Growth, int side2Growth)
{
    Q_ASSERT(side1Growth > 0 || side2Growth > 0);
    //Q_ASSERT(side1Growth >= 0 && side2Growth >= 0); // never negative

    if (side1Growth > 0) {
        auto begin = sizes.cbegin();
        auto end = sizes.cbegin() + index;

        const QVector<int> squeezes = calculateSqueezes(begin, end, side1Growth);
        for (int i = 0; i < squeezes.size(); ++i) {
            const int squeeze = squeezes.at(i);
            SizingInfo &sizing = sizes[i];
            sizing.setGeometry(adjustedRect(sizing.geometry, m_orientation, 0, -squeeze));
        }
    }

    if (side2Growth > 0) {
        auto begin = sizes.cbegin() + index + 1;
        auto end = sizes.cend();

        const QVector<int> squeezes = calculateSqueezes(begin, end, side2Growth);
        for (int i = 0; i < squeezes.size(); ++i) {
            const int squeeze = squeezes.at(i);
            SizingInfo &sizing = sizes[i + index + 1];
            sizing.setGeometry(adjustedRect(sizing.geometry, m_orientation, squeeze, 0));
        }
    }
}
