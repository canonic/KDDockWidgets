/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "Controller_c.h"


#include <QDebug>


namespace Dartagnan {

typedef int (*CleanupCallback)(void *thisPtr);
static CleanupCallback s_cleanupCallback = nullptr;

template<typename T>
struct ValueWrapper
{
    T value;
};

}
namespace KDDockWidgetsBindings_wrappersNS {
namespace KDDWBindingsCore {
Controller_wrapper::Controller_wrapper(KDDockWidgets::Core::ViewType type, KDDockWidgets::Core::View *arg__2)
    : ::KDDockWidgets::Core::Controller(type, arg__2)
{
}
bool Controller_wrapper::close()
{
    return ::KDDockWidgets::Core::Controller::close();
}
QRect Controller_wrapper::geometry() const
{
    return ::KDDockWidgets::Core::Controller::geometry();
}
int Controller_wrapper::height() const
{
    return ::KDDockWidgets::Core::Controller::height();
}
bool Controller_wrapper::inDtor() const
{
    return ::KDDockWidgets::Core::Controller::inDtor();
}
bool Controller_wrapper::isVisible() const
{
    return ::KDDockWidgets::Core::Controller::isVisible();
}
QPoint Controller_wrapper::mapToGlobal(QPoint arg__1) const
{
    return ::KDDockWidgets::Core::Controller::mapToGlobal(arg__1);
}
void Controller_wrapper::parentViewChanged(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::Core::Controller::parentViewChanged(parent);
}
QPoint Controller_wrapper::pos() const
{
    return ::KDDockWidgets::Core::Controller::pos();
}
QRect Controller_wrapper::rect() const
{
    return ::KDDockWidgets::Core::Controller::rect();
}
void Controller_wrapper::setParentView(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::Core::Controller::setParentView(parent);
}
void Controller_wrapper::setParentView_impl(KDDockWidgets::Core::View *parent)
{
    if (m_setParentView_implCallback) {
        const void *thisPtr = this;
        m_setParentView_implCallback(const_cast<void *>(thisPtr), parent);
    } else {
        ::KDDockWidgets::Core::Controller::setParentView_impl(parent);
    }
}
void Controller_wrapper::setParentView_impl_nocallback(KDDockWidgets::Core::View *parent)
{
    ::KDDockWidgets::Core::Controller::setParentView_impl(parent);
}
void Controller_wrapper::setVisible(bool arg__1)
{
    ::KDDockWidgets::Core::Controller::setVisible(arg__1);
}
void Controller_wrapper::show() const
{
    ::KDDockWidgets::Core::Controller::show();
}
QSize Controller_wrapper::size() const
{
    return ::KDDockWidgets::Core::Controller::size();
}
QString Controller_wrapper::tr(const char *s, const char *c, int n)
{
    return ::KDDockWidgets::Core::Controller::tr(s, c, n);
}
KDDockWidgets::Core::ViewType Controller_wrapper::type() const
{
    return ::KDDockWidgets::Core::Controller::type();
}
KDDockWidgets::Core::View *Controller_wrapper::view() const
{
    return ::KDDockWidgets::Core::Controller::view();
}
void Controller_wrapper::visibleChanged(bool arg__1)
{
    ::KDDockWidgets::Core::Controller::visibleChanged(arg__1);
}
int Controller_wrapper::width() const
{
    return ::KDDockWidgets::Core::Controller::width();
}
int Controller_wrapper::x() const
{
    return ::KDDockWidgets::Core::Controller::x();
}
int Controller_wrapper::y() const
{
    return ::KDDockWidgets::Core::Controller::y();
}
Controller_wrapper::~Controller_wrapper()
{
}

}
}
static KDDockWidgets::Core::Controller *fromPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgets::Core::Controller *>(ptr);
}
static KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Controller_wrapper *fromWrapperPtr(void *ptr)
{
    return reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Controller_wrapper *>(ptr);
}
extern "C" {
void c_KDDockWidgets__Core__Controller_Finalizer(void *, void *cppObj, void *)
{
    delete reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Controller_wrapper *>(cppObj);
}
void *c_KDDockWidgets__Core__Controller__constructor_ViewType_View(int type, void *arg__2_)
{
    auto arg__2 = reinterpret_cast<KDDockWidgets::Core::View *>(arg__2_);
    auto ptr = new KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Controller_wrapper(static_cast<KDDockWidgets::Core::ViewType>(type), arg__2);
    return reinterpret_cast<void *>(ptr);
}
// close()
bool c_KDDockWidgets__Core__Controller__close(void *thisObj)
{
    return fromPtr(thisObj)->close();
}
// geometry() const
void *c_KDDockWidgets__Core__Controller__geometry(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QRect> { fromPtr(thisObj)->geometry() };
}
// height() const
int c_KDDockWidgets__Core__Controller__height(void *thisObj)
{
    return fromPtr(thisObj)->height();
}
// inDtor() const
bool c_KDDockWidgets__Core__Controller__inDtor(void *thisObj)
{
    return fromPtr(thisObj)->inDtor();
}
// isVisible() const
bool c_KDDockWidgets__Core__Controller__isVisible(void *thisObj)
{
    return fromPtr(thisObj)->isVisible();
}
// mapToGlobal(QPoint arg__1) const
void *c_KDDockWidgets__Core__Controller__mapToGlobal_QPoint(void *thisObj, void *arg__1_)
{
    assert(arg__1_);
    auto &arg__1 = *reinterpret_cast<QPoint *>(arg__1_);
    return new Dartagnan::ValueWrapper<QPoint> { fromPtr(thisObj)->mapToGlobal(arg__1) };
}
// parentViewChanged(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__Core__Controller__parentViewChanged_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    fromPtr(thisObj)->parentViewChanged(parent);
}
// pos() const
void *c_KDDockWidgets__Core__Controller__pos(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QPoint> { fromPtr(thisObj)->pos() };
}
// rect() const
void *c_KDDockWidgets__Core__Controller__rect(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QRect> { fromPtr(thisObj)->rect() };
}
// setParentView(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__Core__Controller__setParentView_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    fromPtr(thisObj)->setParentView(parent);
}
// setParentView_impl(KDDockWidgets::Core::View * parent)
void c_KDDockWidgets__Core__Controller__setParentView_impl_View(void *thisObj, void *parent_)
{
    auto parent = reinterpret_cast<KDDockWidgets::Core::View *>(parent_);
    fromWrapperPtr(thisObj)->setParentView_impl_nocallback(parent);
}
// setVisible(bool arg__1)
void c_KDDockWidgets__Core__Controller__setVisible_bool(void *thisObj, bool arg__1)
{
    fromPtr(thisObj)->setVisible(arg__1);
}
// show() const
void c_KDDockWidgets__Core__Controller__show(void *thisObj)
{
    fromPtr(thisObj)->show();
}
// size() const
void *c_KDDockWidgets__Core__Controller__size(void *thisObj)
{
    return new Dartagnan::ValueWrapper<QSize> { fromPtr(thisObj)->size() };
}
// tr(const char * s, const char * c, int n)
void *c_static_KDDockWidgets__Core__Controller__tr_char_char_int(const char *s, const char *c, int n)
{
    return new Dartagnan::ValueWrapper<QString> { KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Controller_wrapper::tr(s, c, n) };
}
// type() const
int c_KDDockWidgets__Core__Controller__type(void *thisObj)
{
    return int(fromPtr(thisObj)->type());
}
// view() const
void *c_KDDockWidgets__Core__Controller__view(void *thisObj)
{
    return fromPtr(thisObj)->view();
}
// visibleChanged(bool arg__1)
void c_KDDockWidgets__Core__Controller__visibleChanged_bool(void *thisObj, bool arg__1)
{
    fromPtr(thisObj)->visibleChanged(arg__1);
}
// width() const
int c_KDDockWidgets__Core__Controller__width(void *thisObj)
{
    return fromPtr(thisObj)->width();
}
// x() const
int c_KDDockWidgets__Core__Controller__x(void *thisObj)
{
    return fromPtr(thisObj)->x();
}
// y() const
int c_KDDockWidgets__Core__Controller__y(void *thisObj)
{
    return fromPtr(thisObj)->y();
}
void c_KDDockWidgets__Core__Controller__destructor(void *thisObj)
{
    delete fromPtr(thisObj);
}
void c_KDDockWidgets__Core__Controller__registerVirtualMethodCallback(void *ptr, void *callback, int methodId)
{
    auto wrapper = fromWrapperPtr(ptr);
    switch (methodId) {
    case 893:
        wrapper->m_setParentView_implCallback = reinterpret_cast<KDDockWidgetsBindings_wrappersNS::KDDWBindingsCore::Controller_wrapper::Callback_setParentView_impl>(callback);
        break;
    }
}
}
