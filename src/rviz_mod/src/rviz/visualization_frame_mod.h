/*
 * Copyright (c) 2008, Willow Garage, Inc.
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

#ifndef RVIZ_VISUALIZATION_FRAME_MOD_H
#define RVIZ_VISUALIZATION_FRAME_MOD_H

#include <boost/shared_ptr.hpp>

#include <QMainWindow>
#include <QList>

#include <string>
#include <deque>

#include "rviz/config.h"
#include "rviz/window_manager_interface.h"
#include "rviz/panel.h"
#include "rviz/rviz_export.h"

#include <ros/time.h>

#include "rviz/visualization_frame.h"
#include "rviz/render_panel.h"
#include "rviz/ogre_helpers/qt_quick_ogre_render_window.h"


class QSplashScreen;
class QAction;
class QActionGroup;
class QTimer;
class QDockWidget;
class QLabel;
class QToolButton;

namespace rviz
{
class PanelFactory;
struct Preferences;
class RenderPanel;
class VisualizationManager;
class Tool;
class WidgetGeometryChangeDetector;

/** @brief The main rviz window.
 *
 * VisualizationFrameMod is a QMainWindow, which means it has a center
 * area and a bunch of dock areas around it.  The central widget here
 * is a RenderPanel, and around it (by default) are a DisplaysPanel,
 * ViewsPanel, TimePanel, SelectionPanel, and ToolPropertiesPanel.  At
 * the top is a toolbar with "Move Camera", "Select", etc.  There is
 * also a menu bar with file/open, etc.
 */
class RVIZ_EXPORT VisualizationFrameMod : public VisualizationFrame
{
  Q_OBJECT
public:
  VisualizationFrameMod();
  ~VisualizationFrameMod();
  //void initialize(const QString& display_config_file = "");
  void initialize(const QString& display_config_file = "",QtQuickOgreRenderWindow* renderWindow=nullptr);
  void closeEvent(QCloseEvent* event) override {
      frameCloseSignal(false);
      event->accept();
  }
  void initMenus() override;
  //void initToolbars() override {};

  protected:
  bool event(QEvent* event) override
  {
	  if(event->type() == QEvent::WindowActivate) {;
		  frameActiveSignal(true);
	  }
	  else if(event->type() == QEvent::WindowDeactivate) {;
		  frameActiveSignal(false);
	  }
	  return QWidget::event(event);
  }

Q_SIGNALS:
  void frameCloseSignal(bool value);
  void frameActiveSignal(bool value);
};

} // namespace rviz

#endif // RVIZ_VISUALIZATION_FRAME_MOD_H
