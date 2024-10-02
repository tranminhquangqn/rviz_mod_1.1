import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2

import ros.rviz 1.0
// import WidgetItem 1.0 //For testing


Rectangle {
  id: rvizPanel
  anchors.fill: parent
  color: "black"
  ColumnLayout{
      anchors.fill: parent
      Rectangle{
          id:configFrame
          // visible: rvizLoader.configVisible
          Layout.preferredHeight: 410
          Layout.fillWidth: true
          color: "black"
          onVisibleChanged: {
              if(!visible){
                  rvizLoader.hideRviz()
              }
          }
          onWidthChanged: {
              if(visible){
                  rvizLoader.setRvizGeometry()
              }
          }
          onHeightChanged: {
              if(visible){
                  rvizLoader.setRvizGeometry()
              }
          }
      }
      RenderWindow {
        id: renderWindow
        Layout.fillWidth: true
        Layout.fillHeight: true
        Component.onCompleted:{
          rvizLoader.initRvizApp(renderWindow,configFrame,rvizPanel,qmlEngine)
        }
      }
  }
}


