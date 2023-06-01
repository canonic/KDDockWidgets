/*
  This file is part of KDDockWidgets.

  SPDX-FileCopyrightText: 2019-2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
  Author: Sérgio Martins <sergio.martins@kdab.com>

  SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
import 'dart:ffi' as ffi;
import 'package:ffi/ffi.dart';
import '../TypeHelpers.dart';
import '../../Bindings.dart';
import '../../Bindings_KDDWBindingsCore.dart' as KDDWBindingsCore;
import '../../Bindings_KDDWBindingsFlutter.dart' as KDDWBindingsFlutter;
import '../../LibraryLoader.dart';
import '../../FinalizerHelpers.dart';

var _dylib = Library.instance().dylib;

class Stack extends KDDWBindingsFlutter.View {
  Stack.fromCppPointer(var cppPointer, [var needsAutoDelete = false])
      : super.fromCppPointer(cppPointer, needsAutoDelete) {}
  Stack.init() : super.init() {}
  factory Stack.fromCache(var cppPointer, [needsAutoDelete = false]) {
    if (KDDWBindingsCore.View.isCached(cppPointer)) {
      var instance =
          KDDWBindingsCore.View.s_dartInstanceByCppPtr[cppPointer.address];
      if (instance != null) return instance as Stack;
    }
    return Stack.fromCppPointer(cppPointer, needsAutoDelete);
  } //Stack(KDDockWidgets::Core::Stack * controller, KDDockWidgets::Core::View * parent)
  Stack(KDDWBindingsCore.Stack? controller,
      {required KDDWBindingsCore.View? parent})
      : super.init() {
    final voidstar_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<voidstar_Func_voidstar_voidstar_FFI>>(
            'c_KDDockWidgets__flutter__Stack__constructor_Stack_View')
        .asFunction();
    thisCpp = func(controller == null ? ffi.nullptr : controller.thisCpp,
        parent == null ? ffi.nullptr : parent.thisCpp);
    KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] = this;
    registerCallbacks();
  }
  static void activateWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::activateWindow()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.activateWindow();
  }

  static int close_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print("Dart instance not found for Stack::close()! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.close();
    return result ? 1 : 0;
  }

  static void createPlatformWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::createPlatformWindow()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.createPlatformWindow();
  }

  static int flags_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::flags() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.flags();
    return result;
  }

  static ffi.Pointer<void> geometry_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::geometry() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.geometry();
    return result.thisCpp;
  }

  static void grabMouse_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::grabMouse()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.grabMouse();
  }

  static int hasFocus_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::hasFocus() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.hasFocus();
    return result ? 1 : 0;
  }

  static void hide_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print("Dart instance not found for Stack::hide()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.hide();
  }

  static void init_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print("Dart instance not found for Stack::init()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.init();
  }

  static int isActiveWindow_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isActiveWindow() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isActiveWindow();
    return result ? 1 : 0;
  }

  static int isExplicitlyHidden_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isExplicitlyHidden() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isExplicitlyHidden();
    return result ? 1 : 0;
  }

  static int isMaximized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isMaximized() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isMaximized();
    return result ? 1 : 0;
  }

  static int isMinimized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isMinimized() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isMinimized();
    return result ? 1 : 0;
  }

  static int isMounted_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isMounted() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isMounted();
    return result ? 1 : 0;
  }

  static int isNull_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isNull() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isNull();
    return result ? 1 : 0;
  } // isPositionDraggable(QPoint p) const

  bool isPositionDraggable(QPoint p) {
    final bool_Func_voidstar_voidstar func = _dylib
        .lookup<ffi.NativeFunction<bool_Func_voidstar_voidstar_FFI>>(
            cFunctionSymbolName(1095))
        .asFunction();
    return func(thisCpp, p == null ? ffi.nullptr : p.thisCpp) != 0;
  }

  static int isPositionDraggable_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> p) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isPositionDraggable(QPoint p) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isPositionDraggable(QPoint.fromCppPointer(p));
    return result ? 1 : 0;
  }

  static int isRootView_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isRootView() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isRootView();
    return result ? 1 : 0;
  }

  static int isVisible_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::isVisible() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.isVisible();
    return result ? 1 : 0;
  }

  static ffi.Pointer<void> mapFromGlobal_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> globalPt) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::mapFromGlobal(QPoint globalPt) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.mapFromGlobal(QPoint.fromCppPointer(globalPt));
    return result.thisCpp;
  }

  static ffi.Pointer<void> mapTo_calledFromC(ffi.Pointer<void> thisCpp,
      ffi.Pointer<void>? parent, ffi.Pointer<void> pos) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::mapTo(KDDockWidgets::Core::View * parent, QPoint pos) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.mapTo(
        (parent == null || parent.address == 0)
            ? null
            : KDDWBindingsCore.View.fromCppPointer(parent),
        QPoint.fromCppPointer(pos));
    return result.thisCpp;
  }

  static ffi.Pointer<void> mapToGlobal_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> localPt) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::mapToGlobal(QPoint localPt) const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.mapToGlobal(QPoint.fromCppPointer(localPt));
    return result.thisCpp;
  }

  static ffi.Pointer<void> maxSizeHint_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::maxSizeHint() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.maxSizeHint();
    return result.thisCpp;
  }

  static ffi.Pointer<void> minSize_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::minSize() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.minSize();
    return result.thisCpp;
  }

  static int minimumHeight_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::minimumHeight() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.minimumHeight();
    return result;
  }

  static int minimumWidth_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::minimumWidth() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.minimumWidth();
    return result;
  }

  static void move_2_calledFromC(ffi.Pointer<void> thisCpp, int x, int y) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::move(int x, int y)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.move_2(x, y);
  }

  static ffi.Pointer<void> normalGeometry_calledFromC(
      ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::normalGeometry() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.normalGeometry();
    return result.thisCpp;
  }

  static ffi.Pointer<void> objectName_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::objectName() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.objectName();
    return result.thisCpp;
  }

  static void onChildAdded_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? childView) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::onChildAdded(KDDockWidgets::Core::View * childView)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.onChildAdded((childView == null || childView.address == 0)
        ? null
        : KDDWBindingsCore.View.fromCppPointer(childView));
  }

  static void onChildRemoved_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? childView) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::onChildRemoved(KDDockWidgets::Core::View * childView)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.onChildRemoved((childView == null || childView.address == 0)
        ? null
        : KDDWBindingsCore.View.fromCppPointer(childView));
  }

  static void onChildVisibilityChanged_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? childView) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::onChildVisibilityChanged(KDDockWidgets::Core::View * childView)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.onChildVisibilityChanged(
        (childView == null || childView.address == 0)
            ? null
            : KDDWBindingsCore.View.fromCppPointer(childView));
  }

  static void onGeometryChanged_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::onGeometryChanged()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.onGeometryChanged();
  }

  static int onResize_2_calledFromC(ffi.Pointer<void> thisCpp, int h, int w) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::onResize(int h, int w)! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.onResize_2(h, w);
    return result ? 1 : 0;
  }

  static void raise_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print("Dart instance not found for Stack::raise()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.raise();
  }

  static void raiseAndActivate_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::raiseAndActivate()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.raiseAndActivate();
  }

  static void releaseKeyboard_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::releaseKeyboard()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.releaseKeyboard();
  }

  static void releaseMouse_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::releaseMouse()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.releaseMouse();
  }

  static void setCursor_calledFromC(ffi.Pointer<void> thisCpp, int shape) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setCursor(Qt::CursorShape shape)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setCursor(shape);
  } // setDocumentMode(bool arg__1)

  setDocumentMode(bool arg__1) {
    final void_Func_voidstar_bool func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_ffi_Int8_FFI>>(
            cFunctionSymbolName(1098))
        .asFunction();
    func(thisCpp, arg__1 ? 1 : 0);
  }

  static void setDocumentMode_calledFromC(
      ffi.Pointer<void> thisCpp, int arg__1) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setDocumentMode(bool arg__1)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setDocumentMode(arg__1 != 0);
  }

  static void setFixedHeight_calledFromC(ffi.Pointer<void> thisCpp, int h) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setFixedHeight(int h)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setFixedHeight(h);
  }

  static void setFixedWidth_calledFromC(ffi.Pointer<void> thisCpp, int w) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setFixedWidth(int w)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setFixedWidth(w);
  }

  static void setGeometry_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> geometry) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setGeometry(QRect geometry)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setGeometry(QRect.fromCppPointer(geometry));
  }

  static void setHeight_calledFromC(ffi.Pointer<void> thisCpp, int h) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setHeight(int h)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setHeight(h);
  }

  static void setMaximumSize_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> sz) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setMaximumSize(QSize sz)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setMaximumSize(QSize.fromCppPointer(sz));
  }

  static void setMinimumSize_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void> sz) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setMinimumSize(QSize sz)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setMinimumSize(QSize.fromCppPointer(sz));
  }

  static void setMouseTracking_calledFromC(
      ffi.Pointer<void> thisCpp, int enable) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setMouseTracking(bool enable)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setMouseTracking(enable != 0);
  }

  static void setObjectName_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? name) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setObjectName(const QString & name)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setObjectName(QString.fromCppPointer(name).toDartString());
  }

  static void setParent_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? parent) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setParent(KDDockWidgets::Core::View * parent)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setParent((parent == null || parent.address == 0)
        ? null
        : KDDWBindingsCore.View.fromCppPointer(parent));
  }

  static void setSize_2_calledFromC(ffi.Pointer<void> thisCpp, int w, int h) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setSize(int w, int h)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setSize_2(w, h);
  }

  static void setVisible_calledFromC(ffi.Pointer<void> thisCpp, int visible) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setVisible(bool visible)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setVisible(visible != 0);
  }

  static void setWidth_calledFromC(ffi.Pointer<void> thisCpp, int w) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setWidth(int w)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setWidth(w);
  }

  static void setWindowOpacity_calledFromC(
      ffi.Pointer<void> thisCpp, double v) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setWindowOpacity(double v)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setWindowOpacity(v);
  }

  static void setWindowTitle_calledFromC(
      ffi.Pointer<void> thisCpp, ffi.Pointer<void>? title) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setWindowTitle(const QString & title)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setWindowTitle(QString.fromCppPointer(title).toDartString());
  }

  static void setZOrder_calledFromC(ffi.Pointer<void> thisCpp, int z) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::setZOrder(int z)! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.setZOrder(z);
  }

  static void show_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print("Dart instance not found for Stack::show()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.show();
  }

  static void showMaximized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::showMaximized()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.showMaximized();
  }

  static void showMinimized_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::showMinimized()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.showMinimized();
  }

  static void showNormal_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::showNormal()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.showNormal();
  }

  static ffi.Pointer<void> sizeHint_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::sizeHint() const! (${thisCpp.address})");
      throw Error();
    }
    final result = dartInstance.sizeHint();
    return result.thisCpp;
  }

  static void update_calledFromC(ffi.Pointer<void> thisCpp) {
    var dartInstance =
        KDDWBindingsCore.View.s_dartInstanceByCppPtr[thisCpp.address] as Stack;
    if (dartInstance == null) {
      print(
          "Dart instance not found for Stack::update()! (${thisCpp.address})");
      throw Error();
    }
    dartInstance.update();
  }

  void release() {
    final void_Func_voidstar func = _dylib
        .lookup<ffi.NativeFunction<void_Func_voidstar_FFI>>(
            'c_KDDockWidgets__flutter__Stack__destructor')
        .asFunction();
    func(thisCpp);
  }

  String cFunctionSymbolName(int methodId) {
    switch (methodId) {
      case 925:
        return "c_KDDockWidgets__flutter__Stack__activateWindow";
      case 936:
        return "c_KDDockWidgets__flutter__Stack__close";
      case 939:
        return "c_KDDockWidgets__flutter__Stack__createPlatformWindow";
      case 946:
        return "c_KDDockWidgets__flutter__Stack__flags";
      case 949:
        return "c_KDDockWidgets__flutter__Stack__geometry";
      case 951:
        return "c_KDDockWidgets__flutter__Stack__grabMouse";
      case 954:
        return "c_KDDockWidgets__flutter__Stack__hasFocus";
      case 956:
        return "c_KDDockWidgets__flutter__Stack__hide";
      case 959:
        return "c_KDDockWidgets__flutter__Stack__init";
      case 961:
        return "c_KDDockWidgets__flutter__Stack__isActiveWindow";
      case 962:
        return "c_KDDockWidgets__flutter__Stack__isExplicitlyHidden";
      case 963:
        return "c_KDDockWidgets__flutter__Stack__isMaximized";
      case 964:
        return "c_KDDockWidgets__flutter__Stack__isMinimized";
      case 1047:
        return "c_KDDockWidgets__flutter__Stack__isMounted";
      case 965:
        return "c_KDDockWidgets__flutter__Stack__isNull";
      case 1095:
        return "c_KDDockWidgets__flutter__Stack__isPositionDraggable_QPoint";
      case 966:
        return "c_KDDockWidgets__flutter__Stack__isRootView";
      case 967:
        return "c_KDDockWidgets__flutter__Stack__isVisible";
      case 968:
        return "c_KDDockWidgets__flutter__Stack__mapFromGlobal_QPoint";
      case 969:
        return "c_KDDockWidgets__flutter__Stack__mapTo_View_QPoint";
      case 970:
        return "c_KDDockWidgets__flutter__Stack__mapToGlobal_QPoint";
      case 971:
        return "c_KDDockWidgets__flutter__Stack__maxSizeHint";
      case 972:
        return "c_KDDockWidgets__flutter__Stack__minSize";
      case 973:
        return "c_KDDockWidgets__flutter__Stack__minimumHeight";
      case 974:
        return "c_KDDockWidgets__flutter__Stack__minimumWidth";
      case 976:
        return "c_KDDockWidgets__flutter__Stack__move_int_int";
      case 977:
        return "c_KDDockWidgets__flutter__Stack__normalGeometry";
      case 978:
        return "c_KDDockWidgets__flutter__Stack__objectName";
      case 1048:
        return "c_KDDockWidgets__flutter__Stack__onChildAdded_View";
      case 1049:
        return "c_KDDockWidgets__flutter__Stack__onChildRemoved_View";
      case 1050:
        return "c_KDDockWidgets__flutter__Stack__onChildVisibilityChanged_View";
      case 1052:
        return "c_KDDockWidgets__flutter__Stack__onGeometryChanged";
      case 980:
        return "c_KDDockWidgets__flutter__Stack__onResize_int_int";
      case 983:
        return "c_KDDockWidgets__flutter__Stack__raise";
      case 984:
        return "c_KDDockWidgets__flutter__Stack__raiseAndActivate";
      case 986:
        return "c_KDDockWidgets__flutter__Stack__releaseKeyboard";
      case 987:
        return "c_KDDockWidgets__flutter__Stack__releaseMouse";
      case 991:
        return "c_KDDockWidgets__flutter__Stack__setCursor_CursorShape";
      case 1098:
        return "c_KDDockWidgets__flutter__Stack__setDocumentMode_bool";
      case 992:
        return "c_KDDockWidgets__flutter__Stack__setFixedHeight_int";
      case 993:
        return "c_KDDockWidgets__flutter__Stack__setFixedWidth_int";
      case 994:
        return "c_KDDockWidgets__flutter__Stack__setGeometry_QRect";
      case 995:
        return "c_KDDockWidgets__flutter__Stack__setHeight_int";
      case 996:
        return "c_KDDockWidgets__flutter__Stack__setMaximumSize_QSize";
      case 997:
        return "c_KDDockWidgets__flutter__Stack__setMinimumSize_QSize";
      case 998:
        return "c_KDDockWidgets__flutter__Stack__setMouseTracking_bool";
      case 999:
        return "c_KDDockWidgets__flutter__Stack__setObjectName_QString";
      case 1000:
        return "c_KDDockWidgets__flutter__Stack__setParent_View";
      case 1002:
        return "c_KDDockWidgets__flutter__Stack__setSize_int_int";
      case 1003:
        return "c_KDDockWidgets__flutter__Stack__setVisible_bool";
      case 1004:
        return "c_KDDockWidgets__flutter__Stack__setWidth_int";
      case 1005:
        return "c_KDDockWidgets__flutter__Stack__setWindowOpacity_double";
      case 1006:
        return "c_KDDockWidgets__flutter__Stack__setWindowTitle_QString";
      case 1007:
        return "c_KDDockWidgets__flutter__Stack__setZOrder_int";
      case 1008:
        return "c_KDDockWidgets__flutter__Stack__show";
      case 1009:
        return "c_KDDockWidgets__flutter__Stack__showMaximized";
      case 1010:
        return "c_KDDockWidgets__flutter__Stack__showMinimized";
      case 1011:
        return "c_KDDockWidgets__flutter__Stack__showNormal";
      case 1013:
        return "c_KDDockWidgets__flutter__Stack__sizeHint";
      case 1015:
        return "c_KDDockWidgets__flutter__Stack__update";
    }
    return super.cFunctionSymbolName(methodId);
  }

  static String methodNameFromId(int methodId) {
    switch (methodId) {
      case 925:
        return "activateWindow";
      case 936:
        return "close";
      case 939:
        return "createPlatformWindow";
      case 946:
        return "flags";
      case 949:
        return "geometry";
      case 951:
        return "grabMouse";
      case 954:
        return "hasFocus";
      case 956:
        return "hide";
      case 959:
        return "init";
      case 961:
        return "isActiveWindow";
      case 962:
        return "isExplicitlyHidden";
      case 963:
        return "isMaximized";
      case 964:
        return "isMinimized";
      case 1047:
        return "isMounted";
      case 965:
        return "isNull";
      case 1095:
        return "isPositionDraggable";
      case 966:
        return "isRootView";
      case 967:
        return "isVisible";
      case 968:
        return "mapFromGlobal";
      case 969:
        return "mapTo";
      case 970:
        return "mapToGlobal";
      case 971:
        return "maxSizeHint";
      case 972:
        return "minSize";
      case 973:
        return "minimumHeight";
      case 974:
        return "minimumWidth";
      case 976:
        return "move_2";
      case 977:
        return "normalGeometry";
      case 978:
        return "objectName";
      case 1048:
        return "onChildAdded";
      case 1049:
        return "onChildRemoved";
      case 1050:
        return "onChildVisibilityChanged";
      case 1052:
        return "onGeometryChanged";
      case 980:
        return "onResize_2";
      case 983:
        return "raise";
      case 984:
        return "raiseAndActivate";
      case 986:
        return "releaseKeyboard";
      case 987:
        return "releaseMouse";
      case 991:
        return "setCursor";
      case 1098:
        return "setDocumentMode";
      case 992:
        return "setFixedHeight";
      case 993:
        return "setFixedWidth";
      case 994:
        return "setGeometry";
      case 995:
        return "setHeight";
      case 996:
        return "setMaximumSize";
      case 997:
        return "setMinimumSize";
      case 998:
        return "setMouseTracking";
      case 999:
        return "setObjectName";
      case 1000:
        return "setParent";
      case 1002:
        return "setSize_2";
      case 1003:
        return "setVisible";
      case 1004:
        return "setWidth";
      case 1005:
        return "setWindowOpacity";
      case 1006:
        return "setWindowTitle";
      case 1007:
        return "setZOrder";
      case 1008:
        return "show";
      case 1009:
        return "showMaximized";
      case 1010:
        return "showMinimized";
      case 1011:
        return "showNormal";
      case 1013:
        return "sizeHint";
      case 1015:
        return "update";
    }
    throw Error();
  }

  void registerCallbacks() {
    assert(thisCpp != null);
    final RegisterMethodIsReimplementedCallback registerCallback = _dylib
        .lookup<ffi.NativeFunction<RegisterMethodIsReimplementedCallback_FFI>>(
            'c_KDDockWidgets__flutter__Stack__registerVirtualMethodCallback')
        .asFunction();
    final callback925 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.activateWindow_calledFromC);
    registerCallback(thisCpp, callback925, 925);
    const callbackExcept936 = 0;
    final callback936 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.close_calledFromC, callbackExcept936);
    registerCallback(thisCpp, callback936, 936);
    final callback939 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsCore.View.createPlatformWindow_calledFromC);
    registerCallback(thisCpp, callback939, 939);
    const callbackExcept946 = 0;
    final callback946 = ffi.Pointer.fromFunction<int_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.flags_calledFromC, callbackExcept946);
    registerCallback(thisCpp, callback946, 946);
    final callback949 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.geometry_calledFromC);
    registerCallback(thisCpp, callback949, 949);
    final callback951 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.grabMouse_calledFromC);
    registerCallback(thisCpp, callback951, 951);
    const callbackExcept954 = 0;
    final callback954 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.hasFocus_calledFromC, callbackExcept954);
    registerCallback(thisCpp, callback954, 954);
    final callback956 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.hide_calledFromC);
    registerCallback(thisCpp, callback956, 956);
    final callback959 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.Stack.init_calledFromC);
    registerCallback(thisCpp, callback959, 959);
    const callbackExcept961 = 0;
    final callback961 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isActiveWindow_calledFromC, callbackExcept961);
    registerCallback(thisCpp, callback961, 961);
    const callbackExcept962 = 0;
    final callback962 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isExplicitlyHidden_calledFromC,
        callbackExcept962);
    registerCallback(thisCpp, callback962, 962);
    const callbackExcept963 = 0;
    final callback963 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isMaximized_calledFromC, callbackExcept963);
    registerCallback(thisCpp, callback963, 963);
    const callbackExcept964 = 0;
    final callback964 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isMinimized_calledFromC, callbackExcept964);
    registerCallback(thisCpp, callback964, 964);
    const callbackExcept1047 = 0;
    final callback1047 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isMounted_calledFromC, callbackExcept1047);
    registerCallback(thisCpp, callback1047, 1047);
    const callbackExcept965 = 0;
    final callback965 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsCore.View.isNull_calledFromC, callbackExcept965);
    registerCallback(thisCpp, callback965, 965);
    const callbackExcept1095 = 0;
    final callback1095 =
        ffi.Pointer.fromFunction<bool_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.Stack.isPositionDraggable_calledFromC,
            callbackExcept1095);
    registerCallback(thisCpp, callback1095, 1095);
    const callbackExcept966 = 0;
    final callback966 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isRootView_calledFromC, callbackExcept966);
    registerCallback(thisCpp, callback966, 966);
    const callbackExcept967 = 0;
    final callback967 = ffi.Pointer.fromFunction<bool_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.isVisible_calledFromC, callbackExcept967);
    registerCallback(thisCpp, callback967, 967);
    final callback968 =
        ffi.Pointer.fromFunction<voidstar_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.mapFromGlobal_calledFromC);
    registerCallback(thisCpp, callback968, 968);
    final callback969 =
        ffi.Pointer.fromFunction<voidstar_Func_voidstar_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.mapTo_calledFromC);
    registerCallback(thisCpp, callback969, 969);
    final callback970 =
        ffi.Pointer.fromFunction<voidstar_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.mapToGlobal_calledFromC);
    registerCallback(thisCpp, callback970, 970);
    final callback971 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.maxSizeHint_calledFromC);
    registerCallback(thisCpp, callback971, 971);
    final callback972 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.minSize_calledFromC);
    registerCallback(thisCpp, callback972, 972);
    const callbackExcept973 = 0;
    final callback973 = ffi.Pointer.fromFunction<int_Func_voidstar_FFI>(
        KDDWBindingsCore.View.minimumHeight_calledFromC, callbackExcept973);
    registerCallback(thisCpp, callback973, 973);
    const callbackExcept974 = 0;
    final callback974 = ffi.Pointer.fromFunction<int_Func_voidstar_FFI>(
        KDDWBindingsCore.View.minimumWidth_calledFromC, callbackExcept974);
    registerCallback(thisCpp, callback974, 974);
    final callback976 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.move_2_calledFromC);
    registerCallback(thisCpp, callback976, 976);
    final callback977 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.normalGeometry_calledFromC);
    registerCallback(thisCpp, callback977, 977);
    final callback978 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.objectName_calledFromC);
    registerCallback(thisCpp, callback978, 978);
    final callback1048 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.onChildAdded_calledFromC);
    registerCallback(thisCpp, callback1048, 1048);
    final callback1049 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.onChildRemoved_calledFromC);
    registerCallback(thisCpp, callback1049, 1049);
    final callback1050 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.onChildVisibilityChanged_calledFromC);
    registerCallback(thisCpp, callback1050, 1050);
    final callback1052 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.onGeometryChanged_calledFromC);
    registerCallback(thisCpp, callback1052, 1052);
    const callbackExcept980 = 0;
    final callback980 =
        ffi.Pointer.fromFunction<bool_Func_voidstar_ffi_Int32_ffi_Int32_FFI>(
            KDDWBindingsCore.View.onResize_2_calledFromC, callbackExcept980);
    registerCallback(thisCpp, callback980, 980);
    final callback983 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.raise_calledFromC);
    registerCallback(thisCpp, callback983, 983);
    final callback984 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.raiseAndActivate_calledFromC);
    registerCallback(thisCpp, callback984, 984);
    final callback986 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.releaseKeyboard_calledFromC);
    registerCallback(thisCpp, callback986, 986);
    final callback987 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.releaseMouse_calledFromC);
    registerCallback(thisCpp, callback987, 987);
    final callback991 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setCursor_calledFromC);
    registerCallback(thisCpp, callback991, 991);
    final callback1098 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int8_FFI>(
            KDDWBindingsFlutter.Stack.setDocumentMode_calledFromC);
    registerCallback(thisCpp, callback1098, 1098);
    final callback992 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setFixedHeight_calledFromC);
    registerCallback(thisCpp, callback992, 992);
    final callback993 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setFixedWidth_calledFromC);
    registerCallback(thisCpp, callback993, 993);
    final callback994 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.setGeometry_calledFromC);
    registerCallback(thisCpp, callback994, 994);
    final callback995 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setHeight_calledFromC);
    registerCallback(thisCpp, callback995, 995);
    final callback996 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.setMaximumSize_calledFromC);
    registerCallback(thisCpp, callback996, 996);
    final callback997 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.setMinimumSize_calledFromC);
    registerCallback(thisCpp, callback997, 997);
    final callback998 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int8_FFI>(
            KDDWBindingsFlutter.View.setMouseTracking_calledFromC);
    registerCallback(thisCpp, callback998, 998);
    final callback999 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.setObjectName_calledFromC);
    registerCallback(thisCpp, callback999, 999);
    final callback1000 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.setParent_calledFromC);
    registerCallback(thisCpp, callback1000, 1000);
    final callback1002 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setSize_2_calledFromC);
    registerCallback(thisCpp, callback1002, 1002);
    final callback1003 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int8_FFI>(
            KDDWBindingsFlutter.View.setVisible_calledFromC);
    registerCallback(thisCpp, callback1003, 1003);
    final callback1004 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setWidth_calledFromC);
    registerCallback(thisCpp, callback1004, 1004);
    final callback1005 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Double_FFI>(
            KDDWBindingsFlutter.View.setWindowOpacity_calledFromC);
    registerCallback(thisCpp, callback1005, 1005);
    final callback1006 =
        ffi.Pointer.fromFunction<void_Func_voidstar_voidstar_FFI>(
            KDDWBindingsFlutter.View.setWindowTitle_calledFromC);
    registerCallback(thisCpp, callback1006, 1006);
    final callback1007 =
        ffi.Pointer.fromFunction<void_Func_voidstar_ffi_Int32_FFI>(
            KDDWBindingsFlutter.View.setZOrder_calledFromC);
    registerCallback(thisCpp, callback1007, 1007);
    final callback1008 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.show_calledFromC);
    registerCallback(thisCpp, callback1008, 1008);
    final callback1009 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.showMaximized_calledFromC);
    registerCallback(thisCpp, callback1009, 1009);
    final callback1010 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.showMinimized_calledFromC);
    registerCallback(thisCpp, callback1010, 1010);
    final callback1011 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.showNormal_calledFromC);
    registerCallback(thisCpp, callback1011, 1011);
    final callback1013 = ffi.Pointer.fromFunction<voidstar_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.sizeHint_calledFromC);
    registerCallback(thisCpp, callback1013, 1013);
    final callback1015 = ffi.Pointer.fromFunction<void_Func_voidstar_FFI>(
        KDDWBindingsFlutter.View.update_calledFromC);
    registerCallback(thisCpp, callback1015, 1015);
  }
}
