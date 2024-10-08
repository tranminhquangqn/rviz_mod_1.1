/*
 * Copyright (c) 2011, Willow Garage, Inc.
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

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QString>
#include <QDebug>

#include <QInputMethod>

#include "rviz/ogre_helpers/qt_quick_ogre_render_window.h"

//panel
#include "ViewModel/TomoVM/rviz_loader.h"
//#include "ViewModel/TomoVM/widgetitem2.h"

using namespace rviz;

int main(int argc, char **argv)
{
  qputenv("QSG_RENDER_LOOP","basic");
      qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
      QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QApplication qapp( argc, argv );
  // qapp.setAutoSipEnabled(true);

  qmlRegisterType<QtQuickOgreRenderWindow>("ros.rviz", 1, 0, "RenderWindow");
  // qmlRegisterType<WidgetItem2>("WidgetItem", 1, 0, "WidgetItem");

  QQmlApplicationEngine engine;
  
  const QUrl url(QStringLiteral("qrc:/main.qml"));


  RvizVM rviz_loader(argc, argv, &qapp,nullptr);
  engine.rootContext()->setContextProperty(QStringLiteral("rvizLoader"), &rviz_loader);

  engine.load(url);
  return qapp.exec();
}
