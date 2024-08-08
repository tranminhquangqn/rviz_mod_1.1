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
  //     if (!active && !frameLoad.isFocus()) {
  //         frameLoad.hideRviz()
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
      //       frameLoad.viewDisplay(widgetItem1)
      //       frameLoad.forceActiveFocus()
      //       //console.log("widgetItem1 active focus "+widgetItem1.hasActiveFocus() )
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
            frameLoad.initRvizApp(renderWindow,mainWindow)
          }
        }
      }
      Row {
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        Button {
          text: "open plug"
          onClicked: {
            frameLoad.openFrame(visualizationFrame)
          }
        }
        Button {
          text: frameLoad.configVisible?"Hide Config":"Show Config"
          onClicked: {
            frameLoad.showRvizBtn()
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
          // frameLoad.hideRviz()
          console.log("ifocus"+frameLoad.isFocus())
        }
      }

    }
  // }
  Button {
    text: "visible"
    anchors.horizontalCenter: parent.horizontalCenter
    onClicked: {
      configFrame.visible=!configFrame.visible
    }
  }
}


