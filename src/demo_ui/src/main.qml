import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtQuick.Window 2.3
import ros.rviz 1.0

import "View/TomoView"
import QtQuick.VirtualKeyboard 2.2
import QtQuick.VirtualKeyboard.Settings 2.2
import QtQuick.VirtualKeyboard.Styles 2.2
ApplicationWindow {
  id: mainWindow
  width: 1500
  height: 900
  visible: true
   property var panel_view: null
    Window{
      id:keyboardWindow
      width:960
      height:360
      flags:Qt.ToolTip
      x:mainWindow.x+mainWindow.width/2-keyboardWindow.width/2
      y:mainWindow.y+mainWindow.height-keyboardWindow.height
      color: "black"
      visible:Qt.inputMethod.visible
      InputPanel {
          id: inputPanel
          anchors.fill: parent
          Component.onCompleted: {
            VirtualKeyboardSettings.activeLocales= ["en"]
        }
      }
    }
  // onActiveChanged: {
  //     if (!active && !rvizLoader.isFocus()) {
  //         rvizLoader.closeRviz()
  //     }
  // }

  // Component.onCompleted:{
  //   createRviz()
  // }

  // function createRviz() {
  //     var component;
  //     component = Qt.createComponent("View/TomoView/RvizPanel.qml");
  //     if (component.status === Component.Ready){
  //         panel_view = component.createObject(mainWindow);
  //         if (panel_view === null) {
  //             console.log("StackLayout >>>> Error creating object RvizPanel.qml");
  //         }
  //     }
  //     else if (component.status === Component.Error) {
  //         console.log("StackLayout >>>> Error loading component:", component.errorString());
  //     }
  // }
  RvizPanel{
    anchors.fill: parent
  }
  Row{
    anchors.bottom:parent.bottom
    Button {
      id:btnshow
      z:10
      width:100
      height:50
      text: rvizLoader.configVisible?"Hide Config":"Show Config"
      onClicked: {
        if(rvizLoader.configVisible){
            rvizLoader.hideRviz()
        }else{
            rvizLoader.showRviz()
        }
      }
    }
    TextField{
      width:100
      height:50
    }
  }
}


