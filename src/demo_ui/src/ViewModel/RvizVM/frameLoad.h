#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QObject>
#include <QDebug>

// #include "widgetitem2.h"
#include <QQuickWindow>
#include <QWidgetItem>
#include <QQuickItem>
#include <QWidget>
#include <QMainWindow>

#include "rviz/visualization_manager.h"
#include "rviz/displays_panel.h"

#include "rviz/visualizer_app_mod.h"
#include <QApplication>
#include "rviz/quick_visualization_frame.h"
#include "rviz/ogre_helpers/qt_quick_ogre_render_window.h"
#include "rviz/visualization_frame_mod.h"
#include <rviz/window_manager_interface.h>



class FrameLoad : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool configVisible READ getConfigVisible WRITE setConfigVisible NOTIFY configVisibleChanged)
public:
    rviz::VisualizationManager* m_manager;
    FrameLoad(QApplication* qapp, int argc, char** argv, QObject* parent = nullptr) : QObject(parent) {
        m_qapp=qapp;
        m_argc=argc;
        m_argv=argv;
        m_configVisible=false;
        
    }
    ~FrameLoad(){
        delete widgetRviz;
    }
    bool getConfigVisible(){
        return m_configVisible;
    }

    // Q_INVOKABLE void viewDisplay(WidgetItem2* widgetPanel){
    //     widgetPanel->setWidget(m_frame);
    // }

    Q_INVOKABLE void initRvizApp(rviz::QtQuickOgreRenderWindow* renderWindow,QQuickWindow *qmlWindow){
        rviz::VisualizerAppMod* widgetRviz = new rviz::VisualizerAppMod();
        widgetRviz->setApp(m_qapp);
        widgetRviz->init(m_argc, m_argv, renderWindow,source_);
        m_frame=widgetRviz->frame_;
        m_vman=m_frame->getManager();
        connect(m_frame, SIGNAL(frameCloseSignal(bool)), this, SLOT(setConfigVisible(bool)));
        m_mainWindow=qmlWindow;

        QObject::connect(m_mainWindow, &QQuickWindow::xChanged, [=]() {
            m_frame->move(m_mainWindow->x()+m_mainWindow->width()-rvizConfigWid, m_mainWindow->y());
        });
        QObject::connect(m_mainWindow, &QQuickWindow::yChanged, [=]() {
            m_frame->move(m_mainWindow->x()+m_mainWindow->width()-rvizConfigWid, m_mainWindow->y());
        });
    }
    Q_INVOKABLE bool isFocus(){
        return m_frame->hasFocus();
    }
    
public Q_SLOTS:
    void setConfigVisible(bool value){
        {
        if (m_configVisible == value) {
            return;
        }
        m_configVisible = value;
        Q_EMIT configVisibleChanged();
        }
    }
    void openFrame(rviz::QuickVisualizationFrame* frame_) {
        // rviz::VisualizationManager* vman = frame_->getManager();
        // rviz::DisplaysPanel* displays_panel = new rviz::DisplaysPanel;
        // displays_panel->initialize(vman);
        // displays_panel->show();

        // moveit_panel = new moveit_rviz_plugin::MotionPlanningDisplay;
        // moveit_panel->frameWidget_=widgetWindow;
        // moveit_panel->initialize(m_vman);
        // m_moveit_frame_=moveit_panel->getFrame();
        // m_moveit_frame_->setParent(nullptr);
        // widgetWindow->show();
    }

    void showRvizBtn(){
        if(m_frame->isVisible()){
            m_frame->close();
            setConfigVisible(false);
        }else{
            m_frame->setGeometry(m_mainWindow->x()+m_mainWindow->width()-rvizConfigWid
                ,m_mainWindow->y()
                ,rvizConfigWid
                ,rvizConfigHei);
            m_frame->setFixedSize(m_frame->size());
            m_frame->show();
            m_frame->setFocus();
            setConfigVisible(true);
        }
    }
    void hideRviz(){
        m_frame->close();
        setConfigVisible(false);
    }
Q_SIGNALS:
    void configVisibleChanged();

private:
    QWidget* widgetWindow;
    QQuickWindow *m_mainWindow;
    rviz::DisplaysPanel* displays_panel;

    rviz::VisualizationManager* m_vman;
    rviz::VisualizationFrameMod* m_frame;
    rviz::VisualizerAppMod* widgetRviz;
    QApplication* m_qapp;
    int m_argc;
    char** m_argv;
    bool m_configVisible;

    int rvizConfigWid=1100;
    int rvizConfigHei=410;

    QString source_= QString::fromStdString(getenv("HOME") + std::string("/tomo_config/moveit_ui.rviz"));
};

