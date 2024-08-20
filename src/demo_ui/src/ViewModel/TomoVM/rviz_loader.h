#ifndef RVIZVM_H
#define RVIZVM_H

#pragma once
#include <QDebug>
#include <QGuiApplication>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QMainWindow>
#include <QQuickItem>
#include <QQuickWindow>
#include <QWidget>
#include <QWidgetItem>

#include "rviz/displays_panel.h"
#include "rviz/visualization_manager.h"

#include "rviz/ogre_helpers/qt_quick_ogre_render_window.h"
#include "rviz/quick_visualization_frame.h"
#include "rviz/visualization_frame_mod.h"
#include "rviz/visualizer_app_mod.h"
#include <QApplication>
#include <rviz/window_manager_interface.h>

#include <QTimer>

class RvizVM : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool configVisible READ getConfigVisible WRITE setConfigVisible NOTIFY configVisibleChanged)
	Q_PROPERTY(bool isInit READ getIsInit NOTIFY isInitChanged)
public:
	rviz::VisualizationManager* m_manager;
	RvizVM(int& argc, char** argv, QApplication* qapp, QObject* parent)
		: QObject(parent), m_qapp(qapp), m_argc(argc), m_argv(argv), m_configVisible(false), m_isInit(false)
	{
	}
	~RvizVM()
	{
		if(widgetRviz != nullptr) {
			delete widgetRviz;
			widgetRviz = nullptr;
		}
	}
	bool getConfigVisible()
	{
		return m_configVisible;
	}
	bool getIsInit()
	{
		return m_isInit;
	}

	// Q_INVOKABLE void viewDisplay(WidgetItem2* widgetPanel){
	//     widgetPanel->setWidget(m_frame);
	// }

	Q_INVOKABLE void initRvizApp(rviz::QtQuickOgreRenderWindow* renderWindow, QQuickWindow* qmlWindow)
	{
		rviz::VisualizerAppMod* widgetRviz = new rviz::VisualizerAppMod();
		widgetRviz->setApp(m_qapp);
		widgetRviz->init(m_argc, m_argv, renderWindow, source_);
		m_frame = widgetRviz->frame_;
		m_vman	= m_frame->getManager();
		connect(m_frame, SIGNAL(frameCloseSignal(bool)), this, SLOT(setConfigVisible(bool)));
		connect(m_frame, SIGNAL(frameActiveSignal(bool)), this, SLOT(setIsFrameActive(bool)));
		m_mainWindow = qmlWindow;

		// QObject::connect(m_mainWindow, &QQuickWindow::xChanged, [=]() {
		//     m_frame->move(m_mainWindow->x()+m_mainWindow->width()-rvizConfigWid, m_mainWindow->y());
		// });
		// QObject::connect(m_mainWindow, &QQuickWindow::yChanged, [=]() {
		//     m_frame->move(m_mainWindow->x()+m_mainWindow->width()-rvizConfigWid, m_mainWindow->y());
		// });
		m_isInit = true;
		Q_EMIT isInitChanged();
	}
	Q_INVOKABLE bool isFocus()
	{
		return m_frame->hasFocus();
	}

public Q_SLOTS:
	void setConfigVisible(bool value)
	{
		{
			if(m_configVisible == value) {
				return;
			}
			m_configVisible = value;
			Q_EMIT configVisibleChanged();
		}
	}
	void setIsFrameActive(bool value)
	{
		if(isFrameActive == value) {
			return;
		}
		isFrameActive = value;
		startQmlCheckHideRviz();
	}
	void setQmlActive(bool value)
	{
		if(isQmlActive == value) {
			return;
		}
		isQmlActive = value;
		startQmlCheckHideRviz();
	}
	void startQmlCheckHideRviz()
	{
		// QTimer::singleShot(300, this, [this]() {
		// 	 	if(m_isInit && !isQmlActive && !isFrameActive) {
		// 			closeRviz();
		// 		}
		// 	 });
	}
	void openFrame(rviz::QuickVisualizationFrame* frame_)
	{
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

	void showRvizBtn()
	{
		if(m_isInit) {
			if(m_frame->isVisible()) {
				m_frame->close();
				setConfigVisible(false);
			}
			else {
				m_frame->setGeometry(m_mainWindow->x() + (m_mainWindow->width() / 2) - (rvizConfigWid / 2),
									 m_mainWindow->y() + (rvizConfigHei / 2) - 30, rvizConfigWid, rvizConfigHei);
				// m_frame->setFixedSize(m_frame->size());
				m_frame->show();
				setConfigVisible(true);
			}
		}
	}
	void closeRviz()
	{
		if(m_isInit) {
			m_frame->close();
			setConfigVisible(false);
		}
	}
Q_SIGNALS:
	void configVisibleChanged();
	void isInitChanged();

private:
	QWidget* widgetWindow				= nullptr;
	QQuickWindow* m_mainWindow			= nullptr;
	rviz::DisplaysPanel* displays_panel = nullptr;

	rviz::VisualizationManager* m_vman	 = nullptr;
	rviz::VisualizationFrameMod* m_frame = nullptr;
	rviz::VisualizerAppMod* widgetRviz	 = nullptr;
	QApplication* m_qapp;
	int m_argc;
	char** m_argv;
	bool m_configVisible;
	bool m_isInit;

	int rvizConfigWid = 1100;
	int rvizConfigHei = 410;

	bool isQmlActive = false;
	bool isFrameActive = false;

	QString source_ = QString::fromStdString(getenv("HOME") + std::string("/tomo_config/moveit_ui.rviz"));
};
#endif
