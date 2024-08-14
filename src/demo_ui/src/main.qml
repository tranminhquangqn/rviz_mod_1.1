import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import ros.rviz 1.0

import "View/TomoView"

ApplicationWindow {
  id: mainWindow
  width: 1500
  height: 900
  visible: true
   property var panel_view: null
  onClosing: {
    rvizLoader.closeRviz()
    close.accepted = true
  }
  // onActiveChanged: {
  //     if (!active && !rvizLoader.isFocus()) {
  //         rvizLoader.closeRviz()
  //     }
  // }

  Component.onCompleted:{
    createRviz()
  }
  function createRviz() {
      var component;
      component = Qt.createComponent("View/TomoView/RvizPanel.qml");
      if (component.status === Component.Ready){
          panel_view = component.createObject(mainWindow);
          if (panel_view === null) {
              console.log("Error creating object RvizPanel.qml");
          }
      }
      else if (component.status === Component.Error) {
          console.log("Error loading component:", component.errorString());
      }
  }
}


