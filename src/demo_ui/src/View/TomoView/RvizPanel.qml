import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import ros.rviz 1.0

Item {
  id: rvizPanel
  anchors.fill: parent
  Rectangle {
    anchors.fill: parent
    color: "black"
    RenderWindow {
      id: renderWindow
      anchors.fill: parent
      Component.onCompleted:{
        rvizLoader.initRvizApp(renderWindow,mainWindow)
      }
    }
  }
}


