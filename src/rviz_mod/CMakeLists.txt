cmake_minimum_required(VERSION 2.8.11.2)
project(rviz)

if (POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()
if (POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

# Specify the path to your custom Qt installation
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm" OR CMAKE_SYSTEM_PROCESSOR STREQUAL "aarch64")
  set(Qt5_DIR "/usr/local/Qt-5.9.5/lib/cmake/Qt5" CACHE PATH "Path to Qt5 cmake files" FORCE)
  set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()
#set(OpenGL_GL_PREFERENCE LEGACY)
#unset : unset Qt5_DIR

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

if(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

# Define global caktin_lint suppressions
#catkin_lint: ignore uninstalled_script missing_install_target

find_package(Boost REQUIRED
  COMPONENTS
  filesystem
  program_options
  signals
  system
  thread
)

find_package(urdfdom_headers REQUIRED)

find_package(PkgConfig REQUIRED)

find_package(ASSIMP QUIET)
if (NOT ASSIMP_FOUND)
  pkg_check_modules(ASSIMP assimp)
endif()
if (ASSIMP_FOUND)
  if( NOT ${ASSIMP_VERSION} VERSION_LESS "2.0.1150" )
    add_definitions(-DASSIMP_UNIFIED_HEADER_NAMES)
    message(STATUS "Assimp version has unified headers")
  else()
    message(STATUS "Assimp version does not have unified headers")
  endif()
  include_directories(${ASSIMP_INCLUDE_DIRS})
  #catkin_lint: ignore_once link_directory
  link_directories(${ASSIMP_LIBRARY_DIRS})
else()
  message(STATUS "could not find assimp (perhaps available thorugh ROS package?), so assimping assimp v2")
  set(ASSIMP_LIBRARIES assimp)
  set(ASSIMP_LIBRARY_DIRS)
  set(ASSIMP_CXX_FLAGS)
  set(ASSIMP_CFLAGS_OTHER)
  set(ASSIMP_LINK_FLAGS)
  set(ASSIMP_INCLUDE_DIRS)
endif()

pkg_check_modules(OGRE_OV OGRE OGRE-Overlay)

# Old versions of OGRE (pre 1.9) included OGRE-Overlay in the main package
# (i.e. there was no OGRE-Overlay component).  So if the above
# pkg_check_modules() failed, try looking for just OGRE.
if(NOT OGRE_OV_FOUND)
  pkg_check_modules(OGRE_OV REQUIRED OGRE)
endif(NOT OGRE_OV_FOUND)

## Find OGRE Plugin path (not necessarily platform-independent, I guess)
if(NOT DEFINED OGRE_PLUGIN_PATH)
  execute_process(COMMAND
    ${PKG_CONFIG_EXECUTABLE} --variable=plugindir OGRE
    OUTPUT_VARIABLE OGRE_PLUGIN_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif(NOT DEFINED OGRE_PLUGIN_PATH)
message(STATUS OGRE_PLUGIN_PATH=${OGRE_PLUGIN_PATH})

# find absolute path of ogre libraries.
# This is stored in the cache to save time on cmake re-run.
# Using absolute paths is necessary if pkg-config finds Ogre in a different
# location than the default.  This can happen when Ogre is built from source,
# or when 2 versions of Ogre are installed.  Using absolute paths ensures that
# components that link against Ogre (rviz and any packages depending on rviz)
# all find the same Ogre shared library.
if(NOT DEFINED OGRE_OV_LIBRARIES_ABS)
  unset(OGRE_OV_LIBRARIES_ABS_TMP)
  foreach(_lib ${OGRE_OV_LIBRARIES})
    set(OGRE_OV_LIB_TAG "OGRE_OV_RVIZ_LIB_${_lib}")

    find_library(${OGRE_OV_LIB_TAG}
            NAMES ${_lib}
            HINTS ${OGRE_OV_LIBRARY_DIRS}
            PATHS ${OGRE_OV_LIBRARY_DIRS}
    )

    set(OGRE_OV_LIBRARIES_ABS_TMP ${OGRE_OV_LIBRARIES_ABS_TMP} ${${OGRE_OV_LIB_TAG}})
  endforeach(_lib)

  set(OGRE_OV_LIBRARIES_ABS ${OGRE_OV_LIBRARIES_ABS_TMP}
        CACHE FILEPATH "Pathname of library ${_lib}")
endif(NOT DEFINED OGRE_OV_LIBRARIES_ABS)

set(STATIC_OGRE_PLUGINS
    RenderSystem_GL)

foreach(_lib ${STATIC_OGRE_PLUGINS})
    set(OGRE_PLUGIN_LIB_TAG "OGRE_RVIZ_PLUGIN_${_lib}")

    set(RESTORE_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES})
    set(CMAKE_FIND_LIBRARY_PREFIXES "")
    find_library(${OGRE_PLUGIN_LIB_TAG}
        NAMES ${_lib}
        HINTS ${OGRE_PLUGIN_PATH}
        PATHS ${OGRE_PLUGIN_PATH}
        NO_DEFAULT_PATH
        )
    set(CMAKE_FIND_LIBRARY_PREFIXES ${RESTORE_LIBRARY_PREFIXES})

    set(OGRE_PLUGIN_LIBRARIES_ABS_TMP ${OGRE_PLUGIN_LIBRARIES_ABS_TMP} ${${OGRE_PLUGIN_LIB_TAG}})
endforeach(_lib)
set(OGRE_PLUGIN_LIBRARIES_ABS ${OGRE_PLUGIN_LIBRARIES_ABS_TMP}
    CACHE FILEPATH "Pathname of library ${_lib}")


if(APPLE)
  FIND_LIBRARY(Cocoa_LIBRARIES Cocoa)
  set(rviz_ADDITIONAL_LIBRARIES ${Cocoa_LIBRARIES})
  
  # This definition prevents an Apple header from defining a macro called "check", which if
  # present, collides with another function in the rviz code. See:
  # https://opensource.apple.com/source/CarbonHeaders/CarbonHeaders-18.1/AssertMacros.h
  add_definitions(-D__ASSERT_MACROS_DEFINE_VERSIONS_WITHOUT_UNDERSCORES=0)
endif()

find_package(OpenGL REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Добавить папку с доп cmake файлами
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# include(Qt5Configuration)

option(UseQt5 "UseQt5" ON)
if (UseQt5)
  find_package(Qt5 ${QT_VERSION} REQUIRED COMPONENTS Core Widgets Quick OpenGL)
  # set variable names already used with Qt4
  set(QT_LIBRARIES Qt5::Widgets Qt5::Quick)
  set(QTVERSION ${Qt5Widgets_VERSION})
else()
  find_package(Qt5 REQUIRED COMPONENTS QtCore QtGui QtOpenGL)
  include(${QT_USE_FILE})
endif()
add_definitions(-DQT_NO_KEYWORDS)

find_package(catkin REQUIRED
  COMPONENTS
  angles
  cmake_modules
  geometry_msgs
  image_transport
  interactive_markers
  laser_geometry
  map_msgs
  message_filters
  message_generation
  nav_msgs
  pluginlib
  python_qt_binding
  resource_retriever
  rosbag
  rosconsole
  roscpp
  roslib
  rospy
  sensor_msgs
  std_msgs
  std_srvs
  tf
  urdf
  visualization_msgs
  urdfdom_headers
)

if(${tf_VERSION} VERSION_LESS "1.11.3")
  add_definitions("-DRVIZ_USE_BOOST_SIGNAL_1")
endif()

find_package(PythonLibs "${PYTHON_VERSION_MAJOR}.${PYTHON_VERSION_MINOR}" REQUIRED)

find_package(Eigen3 QUIET)
if(NOT EIGEN3_FOUND)
  # Fallback to cmake_modules
  find_package(cmake_modules REQUIRED)
  find_package(Eigen REQUIRED)
  set(EIGEN3_INCLUDE_DIRS ${EIGEN_INCLUDE_DIRS})
  set(EIGEN3_LIBRARIES ${EIGEN_LIBRARIES})  # Not strictly necessary as Eigen is head only
  # Possibly map additional variables to the EIGEN3_ prefix.
else()
  set(EIGEN3_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIR})
endif()

find_package(TinyXML2 REQUIRED)

catkin_python_setup()

if(NOT OGRE_OV_LIBRARIES_ABS)
  set(OGRE_OV_LIBRARIES_ABS ${OGRE_OV_LIBRARIES})
endif()

# Set the Qt version for use in the extras file.
set(rviz_QT_VERSION ${QTVERSION})

# This variable controls the target name of the default plugin library.
# It is used in the extras file (processed in caktin_package(...)) and the
# cmake code for the default plugin.
# There is a matching instance of this in the plugin_description.xml.
set(rviz_DEFAULT_PLUGIN_LIBRARY_TARGET_NAME rviz_default_plugin)

add_service_files(
  FILES
  SendFilePath.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

set(EXPORT_HEADER_DIR "${CATKIN_DEVEL_PREFIX}/include")
file(MAKE_DIRECTORY "${EXPORT_HEADER_DIR}")

#catkin_lint: ignore_once external_include_path
catkin_package(
  CFG_EXTRAS
    rviz-extras.cmake
  INCLUDE_DIRS
    src
    ${EXPORT_HEADER_DIR}
    ${EIGEN3_INCLUDE_DIRS}
    ${OGRE_OV_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
  LIBRARIES
    rviz
    ${OGRE_OV_LIBRARIES_ABS}
    ${OGRE_PLUGIN_LIBRARIES_ABS}
    ${rviz_ADDITIONAL_LIBRARIES}
    ${OPENGL_LIBRARIES}
  CATKIN_DEPENDS
    geometry_msgs
    image_transport
    interactive_markers
    laser_geometry
    map_msgs
    message_filters
    message_runtime
    nav_msgs
    pluginlib
    resource_retriever
    roscpp
    roslib
    sensor_msgs
    std_msgs
    std_srvs
    tf
    urdf
    visualization_msgs
)

include_directories(SYSTEM
  ${EIGEN3_INCLUDE_DIRS}
  ${OGRE_OV_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${PYTHON_INCLUDE_PATH}
  ${TinyXML2_INCLUDE_DIRS}
  ${urdfdom_headers_INCLUDE_DIRS}
)
include_directories(src ${catkin_INCLUDE_DIRS})
#catkin_lint: ignore_once external_directory
include_directories(src ${EXPORT_HEADER_DIR})
include_directories(SYSTEM ${catkin_INCLUDE_DIRS})


#### If gtk ends up being the best way to get the correct window
#### position under X11, this is how to compile it in.
#
# find_package(GTK2)
# include_directories(${GTK2_INCLUDE_DIRS})
# include_directories(/usr/include/gdk-pixbuf-2.0)

add_subdirectory(src)

install(DIRECTORY ogre_media
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY icons
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(DIRECTORY images
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(FILES default.rviz
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(FILES plugin_description.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)
install(FILES help/help.html
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/help
)
#catkin_lint: ignore uninstalled_target
