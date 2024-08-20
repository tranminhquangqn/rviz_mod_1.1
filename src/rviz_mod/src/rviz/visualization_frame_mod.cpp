/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <fstream>
#include <memory>

#include <QAction>
#include <QShortcut>
#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDockWidget>
#include <QDir>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QUrl>
#include <QStatusBar>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QTabBar>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>

#include <ros/console.h>
#include <ros/package.h>
#include <ros/init.h>

#include <OgreRenderWindow.h>
#include <OgreMeshManager.h>

#include <ogre_helpers/initialization.h>
#include <ogre_helpers/qt_widget_ogre_render_window.h>

#include "rviz/displays_panel.h"
#include "rviz/env_config.h"
#include "rviz/failed_panel.h"
#include "rviz/help_panel.h"
#include "rviz/loading_dialog.h"
#include "rviz/new_object_dialog.h"
#include "rviz/preferences.h"
#include "rviz/preferences_dialog.h"
#include "rviz/panel_dock_widget.h"
#include "rviz/panel_factory.h"
#include "rviz/render_panel.h"
#include "rviz/screenshot_dialog.h"
#include "rviz/selection/selection_manager.h"
#include "rviz/selection_panel.h"
#include "rviz/splash_screen.h"
#include "rviz/time_panel.h"
#include "rviz/tool.h"
#include "rviz/tool_manager.h"
#include "rviz/tool_properties_panel.h"
#include "rviz/views_panel.h"
#include "rviz/visualization_manager.h"
#include "rviz/widget_geometry_change_detector.h"
#include "rviz/load_resource.h"
#include "rviz/yaml_config_reader.h"
#include "rviz/yaml_config_writer.h"

#include "rviz/visualization_frame_mod.h"

#include <QDebug>

namespace fs = boost::filesystem;

#define CONFIG_EXTENSION "rviz"
#define CONFIG_EXTENSION_WILDCARD "*." CONFIG_EXTENSION
#define RECENT_CONFIG_COUNT 10

#if BOOST_FILESYSTEM_VERSION == 3
#define BOOST_FILENAME_STRING filename().string
#define BOOST_FILE_STRING string
#else
#define BOOST_FILENAME_STRING filename
#define BOOST_FILE_STRING file_string
#endif

namespace rviz
{
VisualizationFrameMod::VisualizationFrameMod()
{
  setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
  //setFocusPolicy(Qt::StrongFocus);
}

VisualizationFrameMod::~VisualizationFrameMod()
{
}
void VisualizationFrameMod::initialize(const QString& display_config_file,QtQuickOgreRenderWindow* renderWindow)
{
  initConfigs();

  loadPersistentSettings();

  QIcon app_icon(
      QString::fromStdString((fs::path(package_path_) / "icons/package.png").BOOST_FILE_STRING()));
  setWindowIcon(app_icon);

  // if (splash_path_ != "")
  // {
  //   QPixmap splash_image(splash_path_);
  //   splash_ = new SplashScreen(splash_image);
  //   splash_->show();
  //   connect(this, SIGNAL(statusUpdate(const QString&)), splash_, SLOT(showMessage(const QString&)));
  // }
  Q_EMIT statusUpdate("Initializing");

  // Periodically process events for the splash screen.
  // See: http://doc.qt.io/qt-5/qsplashscreen.html#details
  if (app_)
    app_->processEvents();

  if (!ros::isInitialized())
  {
    int argc = 0;
    ros::init(argc, nullptr, "rviz", ros::init_options::AnonymousName);
  }

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  // QWidget* central_widget = new QWidget(this);
  // QHBoxLayout* central_layout = new QHBoxLayout;
  // central_layout->setSpacing(0);
  // central_layout->setMargin(0);


  auto render_window = renderWindow;

  //auto render_window = new QtQuickOgreRenderWindowWidget( central_widget );
//auto render_window = new QtWidgetOgreRenderWindow( central_widget );
  render_panel_ = new RenderPanel( render_window, 0 );

  hide_left_dock_button_ = new QToolButton();
  hide_left_dock_button_->setContentsMargins(0, 0, 0, 0);
  hide_left_dock_button_->setArrowType(Qt::LeftArrow);
  hide_left_dock_button_->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));
  hide_left_dock_button_->setFixedWidth(16);
  hide_left_dock_button_->setAutoRaise(true);
  hide_left_dock_button_->setCheckable(true);

  connect(hide_left_dock_button_, SIGNAL(toggled(bool)), this, SLOT(hideLeftDock(bool)));

  hide_right_dock_button_ = new QToolButton();
  hide_right_dock_button_->setContentsMargins(0, 0, 0, 0);
  hide_right_dock_button_->setArrowType(Qt::RightArrow);
  hide_right_dock_button_->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));
  hide_right_dock_button_->setFixedWidth(16);
  hide_right_dock_button_->setAutoRaise(true);
  hide_right_dock_button_->setCheckable(true);

  connect(hide_right_dock_button_, SIGNAL(toggled(bool)), this, SLOT(hideRightDock(bool)));

  // central_layout->addWidget(hide_left_dock_button_, 0);
  // central_layout->addWidget(render_window_, 1 );
  // central_layout->addWidget(hide_right_dock_button_, 0);

  // central_widget->setLayout(central_layout);

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  initMenus();

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  initToolbars();

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  // setCentralWidget(central_widget);

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();
  // this->show();

  manager_ = new VisualizationManager(render_panel_, this);
  manager_->setHelpPath(help_path_);
  connect(manager_, SIGNAL(escapePressed()), this, SLOT(exitFullScreen()));

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  manager_->setRenderFromRenderPanel( true );//i add
  render_panel_->initialize(manager_->getSceneManager(), manager_);

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  ToolManager* tool_man = manager_->getToolManager();

  connect(manager_, SIGNAL(configChanged()), this, SLOT(setDisplayConfigModified()));
  connect(tool_man, SIGNAL(toolAdded(Tool*)), this, SLOT(addTool(Tool*)));
  connect(tool_man, SIGNAL(toolRemoved(Tool*)), this, SLOT(removeTool(Tool*)));
  connect(tool_man, SIGNAL(toolRefreshed(Tool*)), this, SLOT(refreshTool(Tool*)));
  connect(tool_man, SIGNAL(toolChanged(Tool*)), this, SLOT(indicateToolIsCurrent(Tool*)));

  manager_->initialize();

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  if (display_config_file != "")
  {
    loadDisplayConfig(display_config_file);
  }
  else
  {
    loadDisplayConfig(QString::fromStdString(default_display_config_file_));
  }

  // Periodically process events for the splash screen.
  if (app_)
    app_->processEvents();

  // delete splash_;
  // splash_ = nullptr;

  manager_->startUpdate();
  initialized_ = true;
  Q_EMIT statusUpdate("RViz is ready.");

  connect(manager_, SIGNAL(preUpdate()), this, SLOT(updateFps()));
  connect(manager_, SIGNAL(statusUpdate(const QString&)), this, SIGNAL(statusUpdate(const QString&)));
}

void VisualizationFrameMod::initMenus()
{
  file_menu_ = menuBar()->addMenu("&File");

//   QAction* file_menu_open_action =
//       file_menu_->addAction("&Open Config", this, SLOT(onOpen()), QKeySequence("Ctrl+O"));
//   this->addAction(file_menu_open_action);
//   QAction* file_menu_save_action =
//       file_menu_->addAction("&Save Config", this, SLOT(onSave()), QKeySequence("Ctrl+S"));
//   this->addAction(file_menu_save_action);
//   QAction* file_menu_save_as_action =
//       file_menu_->addAction("Save Config &As", this, SLOT(onSaveAs()), QKeySequence("Ctrl+Shift+S"));
//   this->addAction(file_menu_save_as_action);

//   recent_configs_menu_ = file_menu_->addMenu("&Recent Configs");
//   file_menu_->addAction("Save &Image", this, SLOT(onSaveImage()));
//   if (show_choose_new_master_option_)
//   {
//     file_menu_->addSeparator();
//     file_menu_->addAction("Change &Master", this, SLOT(changeMaster()));
//   }
  file_menu_->addAction("&Reset", this, SLOT(reset()));
  QAction* file_menu_quit_action =
      file_menu_->addAction("&Quit", this, SLOT(close()), QKeySequence("Ctrl+Q"));
  file_menu_quit_action->setObjectName("actQuit");
  this->addAction(file_menu_quit_action);

  view_menu_ = menuBar()->addMenu("&Panels");
  view_menu_->addAction("Add &New Panel", this, SLOT(openNewPanelDialog()));
  delete_view_menu_ = view_menu_->addMenu("&Delete Panel");
  delete_view_menu_->setEnabled(false);

  // QWidget *widget = new QWidget(this);
  // QHBoxLayout *layout = new QHBoxLayout(widget);
  // QToolButton* close_button = new QToolButton();
  // close_button->setText("X");
  // close_button->setToolButtonStyle(Qt::ToolButtonTextOnly);
  // close_button->setFixedSize(40, 20);
  // close_button->setStyleSheet("font-size: 14px;");
  // connect(close_button, SIGNAL(clicked()), this, SLOT(close()));
  // status_label_->setMinimumWidth(750);
  // status_label_->setMaximumWidth(800);
  // fps_label_->setMaximumWidth(50);
  // layout->addWidget(status_label_);
  // layout->addWidget(fps_label_);
  // layout->addWidget(close_button);
  // layout->setContentsMargins(2, 2, 2, 2); // Remove margins
  // layout->setAlignment(Qt::AlignRight); // Align button to the right
  // widget->setLayout(layout);
  // menuBar()->setCornerWidget(widget, Qt::TopRightCorner);

}

} // end namespace rviz
