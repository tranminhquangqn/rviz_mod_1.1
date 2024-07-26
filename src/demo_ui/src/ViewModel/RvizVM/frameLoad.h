#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QDebug>
#include "displayconfig.h"

#include "rviz/visualization_manager.h"
#include "rviz/displays_panel.h"
#include "moveit/motion_planning_rviz_plugin/motion_planning_display.h"


class FrameLoad : public QObject {
    Q_OBJECT
public:
    // FrameLoad(QObject* parent = nullptr) : QObject(parent) {}
public Q_SLOTS:
    void openFrame(rviz::QuickVisualizationFrame* frame_) {
        // rviz::VisualizationManager* vman = frame_->getManager();
        // rviz::DisplaysPanel* displays_panel = new rviz::DisplaysPanel;
        // displays_panel->initialize(vman);
        
        // moveit_rviz_plugin::MotionPlanningDisplay* moveit_panel = new moveit_rviz_plugin::MotionPlanningDisplay;
        // // moveit_panel->initialize(vman);

        // displays_panel->show();
        // // moveit_panel->show();
    }
};

