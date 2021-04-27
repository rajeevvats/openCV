# ----------------------------------------------------------------------------
#  Detect 3rd-party GUI libraries
# ----------------------------------------------------------------------------

#--- Win32 UI ---
ocv_clear_vars(HAVE_WIN32UI)
if(WITH_WIN32UI)
  try_compile(HAVE_WIN32UI
    "${OpenCV_BINARY_DIR}"
    "${OpenCV_SOURCE_DIR}/cmake/checks/win32uitest.cpp"
    CMAKE_FLAGS "-DLINK_LIBRARIES:STRING=user32;gdi32")
endif()

# --- QT4 ---
ocv_clear_vars(HAVE_QT HAVE_QT5 HAVE_QT6)
if(WITH_QT)
  if(NOT WITH_QT EQUAL 4)
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Gui Widgets Test Concurrent REQUIRED NO_MODULE)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets Test Concurrent REQUIRED NO_MODULE)
    if(QT_FOUND)
      if(${QT_VERSION_MAJOR} EQUAL 6)
        set(HAVE_QT6 ON)
        set(HAVE_QT  ON)
        find_package(Qt6 COMPONENTS OpenGL QUIET)
        if(Qt6OpenGL_FOUND)
          set(QT_QTOPENGL_FOUND ON)
        endif()
      endif()
      
      if(${QT_VERSION_MAJOR} EQUAL 5)
        set(HAVE_QT5 ON)
        set(HAVE_QT  ON)
        find_package(Qt5 COMPONENTS OpenGL QUIET)
        if(Qt5OpenGL_FOUND)
          set(QT_QTOPENGL_FOUND ON)
        endif()
      endif()
    endif()
  endif()

  if(NOT HAVE_QT)
    find_package(Qt4 REQUIRED QtCore QtGui QtTest)
    if(QT4_FOUND)
      set(HAVE_QT TRUE)
    endif()
  endif()
endif()

# --- GTK ---
ocv_clear_vars(HAVE_GTK HAVE_GTK3 HAVE_GTHREAD HAVE_GTKGLEXT)
if(WITH_GTK AND NOT HAVE_QT)
  if(NOT WITH_GTK_2_X)
    ocv_check_modules(GTK3 gtk+-3.0)
    if(HAVE_GTK3)
      ocv_append_build_options(HIGHGUI GTK3)
      set(HAVE_GTK TRUE)
    endif()
  endif()
  if(NOT HAVE_GTK)
    ocv_check_modules(GTK2 gtk+-2.0)
    if(HAVE_GTK2)
      if (GTK2_VERSION VERSION_LESS MIN_VER_GTK)
        message (FATAL_ERROR "GTK support requires a minimum version of ${MIN_VER_GTK} (${GTK2_VERSION} found)")
      else()
        ocv_append_build_options(HIGHGUI GTK2)
        set(HAVE_GTK TRUE)
      endif()
    endif()
  endif()
  ocv_check_modules(GTHREAD gthread-2.0)
  if(HAVE_GTK AND NOT HAVE_GTHREAD)
    message(FATAL_ERROR "gthread not found. This library is required when building with GTK support")
  else()
    ocv_append_build_options(HIGHGUI GTHREAD)
  endif()
  if(WITH_OPENGL AND NOT HAVE_GTK3)
    ocv_check_modules(GTKGLEXT gtkglext-1.0)
    if(HAVE_GTKGLEXT)
      ocv_append_build_options(HIGHGUI GTKGLEXT)
    endif()
  endif()
endif()

# --- OpenGl ---
ocv_clear_vars(HAVE_OPENGL HAVE_QT_OPENGL)
if(WITH_OPENGL)
  if(WITH_WIN32UI OR (HAVE_QT AND QT_QTOPENGL_FOUND) OR HAVE_GTKGLEXT)
    find_package (OpenGL QUIET)
    if(OPENGL_FOUND)
      set(HAVE_OPENGL TRUE)
      list(APPEND OPENCV_LINKER_LIBS ${OPENGL_LIBRARIES})
      if(QT_QTOPENGL_FOUND)
        set(HAVE_QT_OPENGL TRUE)
      else()
        ocv_include_directories(${OPENGL_INCLUDE_DIR})
      endif()
    endif()
  endif()
endif(WITH_OPENGL)

# --- Cocoa ---
if(APPLE)
  if(NOT IOS AND CV_CLANG)
    set(HAVE_COCOA YES)
  endif()
endif()
