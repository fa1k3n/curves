import QtQuick 2.0

Rectangle {
   id: marker
   objectName: "marker"
   color: "transparent"
   property var nextPoint
   property var prevPoint

   x: xpos; y: ypos;
   border.color: focus ? "green" : "blue"
   border.width: 3
   width: 15; height: width
   radius: width/2

 onFocusChanged: {
     if(focus) {
         console.log("prev " + prevPoint + " next " + nextPoint);
     }
 }
/*
 Rectangle {
     x: marker.x; y: marker.y
     width: modelItem.prev.xpos; height: modelItem.prev.ypos
     visible: modelItem.prev !== null
     color: "red"
 }*/
}
