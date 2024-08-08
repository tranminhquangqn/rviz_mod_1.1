import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import ros.rviz 1.0

// import WidgetItem 1.0

ApplicationWindow {
  id: mainWindow
  width: 1500
  height: 900
  visible: true
  property int rvizConfigHeight: 410
  // onActiveChanged: {
  //     if (!active && !rvizLoader.isFocus()) {
  //         rvizLoader.hideRviz()
  //     }
  // }
  
  // Loader {
  //   id: loader
  //   anchors.fill: parent
  //   sourceComponent: rvizComp
  //   asynchronous: true
  // }
  // Component {
  //   id:rvizComp
      // WidgetItem {
      //     id:widgetItem1
      //     width: parent.width
      //     height: parent.height/2
      //     Component.onCompleted:{
      //       rvizLoader.viewDisplay(widgetItem1)
      //     }
      // }
    Item {
      id:compView
      anchors.bottom:parent.bottom
      width:parent.width
      height:parent.height
      VisualizationFrame {
        id: visualizationFrame
        anchors.fill: parent
        renderWindow: renderWindow
      }
      Rectangle {
        anchors.fill: parent
        color: "lightblue"
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
        }
        
      }
      Timer{
        id: popupDelay
        interval:3000
        onTriggered:{
          // rvizLoader.hideRviz()
          console.log("ifocus"+rvizLoader.isFocus())
        }
      }

    }
  // }
}


