import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: root
   property real radius: 8
   x: xpos - width/2
   y: ypos - height/2
   width: radius*2; height: width

   Rectangle {
       id: marker
       color: "transparent"
       anchors.fill: parent
       border.color: "#c18c45"
       border.width: 3
       radius: parent.width/2
    }

   Glow {
       id: selGlow
       visible: parent.focus
       anchors.fill: marker
       radius: 2
       samples: 17
       color: "white"
       source: marker

       SequentialAnimation {
           loops: Animation.Infinite
           running: selGlow.visible
           NumberAnimation {target: selGlow; property: "radius"; from: 2; to: 8; duration: 1000}
           NumberAnimation {target: selGlow; property: "radius"; from: 8; to: 2; duration: 1000}
       }
   }
}
