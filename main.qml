import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0


import fa1k3n.curves 1.0

Window {
    visible: true
    id: root
    width: 640
    height: 480
    color: "#2E2F30"
    title: qsTr("Hello World")

    /*Canvas {
        id: mycanvas
        anchors.fill: parent
    }*/

    ListModel {
        id: controlPoints        
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: {
             controlPoints.append({xpos: mouse.x, ypos: mouse.y, next: null, prev: null});
        }
    }

    Component {
        id: line

        Line {
            id: theLine
            width: 2
            color: "#F76494"

            function doDestroy() {
                //destroyAnimation.start()
                destroy()
            }

            PropertyAnimation on opacity {from: 0; to: 1; duration: 400}

            SequentialAnimation {
                id: destroyAnimation
                PropertyAnimation {target: theLine; property: "opacity"; from: 1; to: 0; duration: 400}
                ScriptAction { script: destroy() }
            }

        }
    }


    function getTarget(item, conn) {
        if (!conn) return null
        return conn.start === item ? conn.start : conn.end
    }

    function isConnected(left, right) {
        console.log("isConnected: " + getTarget(left, left.model.prev) + " " + right)
        console.log("isConnected: " + getTarget(left, left.model.next) + " " + right)

        return (getTarget(left, left.model.prev) === right || getTarget(left, left.model.next) === right)
    }

    function connectPoints(left, right) {
        console.log("Connecting " + left + " with " + right)

        if(isConnected(left, right)) {
            console.log("Items already connected");
            return;
        }

        var conn = line.createObject(root, { "start": left, "end": right});
        right.model.next = conn
        left.model.prev = conn
    }

    Repeater {
        model: controlPoints

        onItemAdded: {
            if(activeFocusItem instanceof ControlPoint) {
                // Connect these two objects up
                console.log("Item " + item)
               connectPoints(item, activeFocusItem)
            }
            console.log("Item " + item + " left " + item.model.left + " right " + item.model.right)
            item.focus = true;
        }

        onItemRemoved: {

        }

        delegate:
            ControlPoint {
                model: controlPoints.get(index)
                id: thePoint
                radius: 8

                MouseArea {
                      anchors.fill: parent
                      scale: 1.8
                      onClicked: {
                          //if(activeFocusItem instanceof ControlPoint) {
                          //    console.log("CONNECTING");
                          //    connectPoints(activeFocusItem, thePoint);
                          //}
                          console.log("Item " + thePoint + " left " + thePoint.model.prev + " right " + thePoint.next)

                          parent.focus = true
                      }
                      drag.target: parent
                      drag.axis: "XAndYAxis"
                      drag.smoothed: true
                      drag.threshold: 0
                      onPositionChanged: if(drag.active) { parent.focus = true; model.xpos += mouse.x - thePoint.width/2; model.ypos += mouse.y - thePoint.height/2 }
                }

                Keys.onDeletePressed: {
                    destroyAnimation.start()
                    var left, right;
                    // Remove the line connectors
                   if(thePoint.model.prev) {
                        left = thePoint.model.prev.end;
                        thePoint.model.prev.doDestroy();
                    }

                    if(thePoint.model.next) {
                        right = thePoint.model.next.start;
                        thePoint.model.next.doDestroy();
                    }

                    // Setup new connections
                    if(left && right) {
                        var conn = line.createObject(root, { "start": left, "end": right});
                        right.model.next = conn
                        left.model.prev = conn
                    }

                    for(var i = 0; i < controlPoints.count; i++) {
                        if(thePoint.model.prev && thePoint.model.prev.start === thePoint)
                            console.log("HUSTON WE HAVE AN PROBLEM " +  + " " + controlPoints.get(i).next + " " + thePoint.model.prev.start + " " + thePoint)
                    }
                }

                // Destroy animation
                SequentialAnimation {
                    id: destroyAnimation
                    running: false; loops: 1
                    ParallelAnimation {
                        PropertyAnimation {target: thePoint; property: "opacity"; from: 1; to: 0; duration: 400}
                        PropertyAnimation {target: thePoint; property: "radius"; from: 8; to: 40; duration: 400}
                    }
                    //ScriptAction {
                    //    script: controlPoints.remove(index);
                    //}
                }

                // Creation animation
                ParallelAnimation {
                    id: createAnimation
                    running: true; loops: 1
                    PropertyAnimation {target: thePoint; property: "opacity"; from: 0; to: 1; duration: 400}
                    PropertyAnimation {target: thePoint; property: "radius"; from: 40; to: 8; duration: 400}
                }
            }
    }

    Slider {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        value: 0.5
    }

    Drawer {
        id: drawer
        width: 0.25 * root.width
        height: root.height

        Label {
            Column {
                CheckBox {
                    text: qsTr("Blue")
                    checked: true
                }
                CheckBox {
                    text: qsTr("Red")
                }
                CheckBox {
                    text: qsTr("Green")
                    checked: true
                }
            }
        }
    }
}
