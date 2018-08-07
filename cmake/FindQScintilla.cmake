# FindQScintilla
# --------
#
# Find the native QScintilla includes and library.
#
# IMPORTED Targets
# ^^^^^^^^^^^^^^^^
#
# This module defines :prop_tgt:`IMPORTED` target ``QScintilla::QScintilla``, if
# QScintilla has been found.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
# ::
#
#   QScintilla_INCLUDE_DIRS   - where to find qsciscintilla.h, etc.
#   QScintilla_LIBRARIES      - List of libraries when using qsciscintilla.
#   QScintilla_FOUND          - True if qsciscintilla found.
#
# ::
#
#   QScintilla_VERSION_STRING - The version of qsciscintilla found (x.y.z)
#   QScintilla_VERSION_MAJOR  - The major version of qsciscintilla
#   QScintilla_VERSION_MINOR  - The minor version of qsciscintilla
#   QScintilla_VERSION_PATCH  - The patch version of qsciscintilla
#
# Hints
# ^^^^^
#
# A user may set ``QScintilla_ROOT`` to a qsciscintilla installation root to tell this
# module where to look.

set(_QScintilla_SEARCHES)

# Search QScintilla_ROOT first if it is set.
if(QScintilla_ROOT)
  set(_QScintilla_SEARCH_ROOT PATHS ${QScintilla_ROOT} NO_DEFAULT_PATH)
  list(APPEND _QScintilla_SEARCHES _QScintilla_SEARCH_ROOT)
else()
  list(APPEND _QScintilla_SEARCHES "/")
endif()

# Try each search configuration.
foreach(search ${_QScintilla_SEARCHES})
  find_path(QScintilla_INCLUDE_DIR NAMES qsciglobal.h ${${search}} PATH_SUFFIXES include/qt/Qsci include/qt5/Qsci include/x86_64-linux-gnu/qt/Qsci include/x86_64-linux-gnu/qt5/Qsci)
  if (QScintilla_INCLUDE_DIR)
    message(STATUS "QScintilla include dir: ${QScintilla_INCLUDE_DIR}")
    set(QScintilla_FOUND "True")
  else()
    message(WARNING "QScintilla include dir: NOT FOUND!")
  endif()
endforeach()

# Allow QScintilla_LIBRARY to be set manually, as the location of the qscintilla library
if(NOT QScintilla_LIBRARY)
  foreach(search ${_QScintilla_SEARCHES})
    find_library(QScintilla_LIBRARY NAMES qscintilla2_qt5 NAMES_PER_DIR ${${search}} PATH_SUFFIXES lib)
    if (QScintilla_LIBRARY)
      message(STATUS "QScintilla library: ${QScintilla_LIBRARY}")
    else()
      message(WARNING "QScintilla library: NOT FOUND!")
    endif()
  endforeach()
endif()

mark_as_advanced(QScintilla_INCLUDE_DIR)

if(QScintilla_INCLUDE_DIR AND EXISTS "${QScintilla_INCLUDE_DIR}/qsciglobal.h")
    file(STRINGS "${QScintilla_INCLUDE_DIR}/qsciglobal.h" QScintilla_H REGEX "^#define[ \t]+QSCINTILLA_VERSION_STR[ \t]+\"[^\"]*\"$")
    #message(STATUS "[DEBUG] QScintilla_H: ${QScintilla_H}")
    
    string(REGEX REPLACE "^#define[ \t]+QSCINTILLA_VERSION_STR[ \t]+\"([^\"]*)\"$" "\\1" QScintilla_VERSION "${QScintilla_H}")
    #message(STATUS "[DEBUG] QScintilla_VERSION: ${QScintilla_VERSION}")

    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+$" "\\1" QScintilla_VERSION_MAJOR "${QScintilla_VERSION}")
    #message(STATUS "[DEBUG] QScintilla_VERSION_MAJOR: ${QScintilla_VERSION_MAJOR}")
    
    string(REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+$" "\\1" QScintilla_VERSION_MINOR  "${QScintilla_VERSION}")
    #message(STATUS "[DEBUG] QScintilla_VERSION_MINOR: ${QScintilla_VERSION_MINOR}")
    
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)$" "\\1" QScintilla_VERSION_PATCH "${QScintilla_VERSION}")
    #message(STATUS "[DEBUG] QScintilla_VERSION_PATCH: ${QScintilla_VERSION_PATCH}")
    
    set(QScintilla_VERSION_STRING "${QScintilla_VERSION_MAJOR}.${QScintilla_VERSION_MINOR}.${QScintilla_VERSION_PATCH}")
    #message(STATUS "[DEBUG] QScintilla_VERSION_STRING: ${QScintilla_VERSION_STRING}")

    set(QScintilla_MAJOR_VERSION "${QScintilla_VERSION_MAJOR}")
    set(QScintilla_MINOR_VERSION "${QScintilla_VERSION_MINOR}")
    set(QScintilla_PATCH_VERSION "${QScintilla_VERSION_PATCH}")
    message(STATUS "QScintilla version: ${QScintilla_VERSION_STRING}")
else()
    message(WARNING "QScintilla version: NOT FOUND!")
endif()

if(QScintilla_FOUND)
    set(QScintilla_INCLUDE_DIRS ${QScintilla_INCLUDE_DIR})

    if(NOT QScintilla_LIBRARIES)
      set(QScintilla_LIBRARIES ${QScintilla_LIBRARY})
    endif()

    if(NOT TARGET QScintilla::QScintilla)
      add_library(QScintilla::QScintilla UNKNOWN IMPORTED)
      set_target_properties(QScintilla::QScintilla PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${QScintilla_INCLUDE_DIRS}")

      if(QScintilla_LIBRARY)
        set_property(TARGET QScintilla::QScintilla APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(QScintilla::QScintilla PROPERTIES IMPORTED_LOCATION_RELEASE "${QScintilla_LIBRARY}")
      endif()

      if(NOT QScintilla_LIBRARY)
        set_property(TARGET QScintilla::QScintilla APPEND PROPERTY
          IMPORTED_LOCATION "${QScintilla_LIBRARY}")
      endif()
    endif()
endif()
