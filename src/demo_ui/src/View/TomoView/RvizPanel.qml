import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import ros.rviz 1.0

// import WidgetItem 1.0 //For testing
// WidgetItem {
//     id:widgetItem1
//     Component.onCompleted:{
//       rvizLoader.viewDisplay(widgetItem1)
//     }
// }
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
  Row {
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    Button {
      text: "open plug"
      onClicked: {
        rvizLoader.openFrame(visualizationFrame)
      }
    }
    Button {
      text: rvizLoader.configVisible?"Hide Config":"Show Config"
      onClicked: {
        rvizLoader.showRvizBtn()
      }
    }
    Button {
      text: "Test delay"
      onClicked: {
        popupDelay.restart()
      }
      Timer{
        id: popupDelay
        interval:3000
        onTriggered:{
          rvizLoader.closeRviz()
        }
      }
    }
  }
}


