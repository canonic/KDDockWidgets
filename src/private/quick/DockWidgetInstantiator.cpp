/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2021 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#include "DockWidgetInstantiator_p.h"
#include "DockWidgetQuick.h"
#include "DockRegistry_p.h"

using namespace KDDockWidgets;

QString DockWidgetInstantiator::uniqueName() const
{
    return m_uniqueName;
}

void DockWidgetInstantiator::setUniqueName(const QString &name)
{
    m_uniqueName = name;
    Q_EMIT uniqueNameChanged();
}

QString DockWidgetInstantiator::source() const
{
    return m_sourceFilename;
}

void DockWidgetInstantiator::setSource(const QString &source)
{
    m_sourceFilename = source;
    Q_EMIT sourceChanged();
}

DockWidgetQuick *DockWidgetInstantiator::dockWidget() const
{
    return m_dockWidget;
}

void DockWidgetInstantiator::componentComplete()
{
    if (m_uniqueName.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "Each DockWidget need an unique name. Set the uniqueName property.";
        return;
    }

    if (DockRegistry::self()->containsDockWidget(m_uniqueName)) {
        // Dock widget already exists. all good.
        return;
    }

    if (m_dockWidget) {
        qWarning() << Q_FUNC_INFO << "Unexpected bug.";
        return;
    }

    if (m_sourceFilename.isEmpty()) {
        qWarning() << Q_FUNC_INFO << "the 'source' property must be set.";
        return;
    }

    m_dockWidget = new DockWidgetQuick(m_uniqueName);
    m_dockWidget->setWidget(m_sourceFilename);

    Q_EMIT dockWidgetChanged();
}
