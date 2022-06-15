/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2020-2022 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/

#pragma once

#include "kddockwidgets/KDDockWidgets.h"

namespace KDDockWidgets {

namespace Controllers {
class DockWidget;
class Frame;
class TitleBar;
}

namespace Views {

/// @brief The interface that DockWidget views should implement
class DOCKS_EXPORT DockWidgetViewInterface
{
public:
    explicit DockWidgetViewInterface(Controllers::DockWidget *);
    virtual ~DockWidgetViewInterface();

    Controllers::Frame *frame() const;
    Controllers::DockWidget *dockWidget() const;
    Controllers::TitleBar *actualTitleBar() const;

protected:
    Controllers::DockWidget *const m_dockWidget;
};

}

}