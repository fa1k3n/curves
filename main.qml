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

    ChaikinsCurve {
        id: curve
        anchors.fill: parent
        refinement: refinementSlider.value
        controlPolygon: controlPoints
    }

    ListModel {
        id: controlPoints
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: {
             controlPoints.append({pos: Qt.point(mouse.x, mouse.y), xpos: mouse.x, ypos: mouse.y, next: null, prev: null});
        }
    }

    Component {
        id: line

        Line {
            id: theLine
            width: 2
            color: "#F76494"

            onStartChanged: {
                if(!start) destroyAnimation.start()
            }
            onEndChanged: {
                if(!end) destroyAnimation.start();
            }

            PropertyAnimation on opacity {from: 0; to: 1; duration: 400}

            SequentialAnimation {
                id: destroyAnimation
                PropertyAnimation {target: theLine; property: "opacity"; from: 1; to: 0; duration: 400}
                ScriptAction { script: destroy() }
            }

        }
    }

    function connectPoints(left, right) {
        right.next = left
        left.prev = right
        line.createObject(root, { "start": left, "end": right});
    }

    Repeater {
        model: controlPoints
        id: test

        onItemAdded: {
            if(activeFocusItem instanceof ControlPoint) {
                // Connect these two objects up
               connectPoints(item, activeFocusItem)
            }
            item.focus = true;
        }

        delegate:
            ControlPoint {
                id: thePoint
                radius: 8
                x: xpos - width/2
                y: ypos - height/2

                MouseArea {
                      anchors.fill: parent
                      scale: 1.8
                      onClicked: {
                          if(activeFocusItem instanceof ControlPoint) {
                              connectPoints(activeFocusItem, thePoint);
                          }
                          parent.focus = true
                      }
                      drag.target: parent
                      drag.axis: "XAndYAxis"
                      drag.smoothed: true
                      drag.threshold: 0
                      onPositionChanged: if(drag.active) { parent.focus = true; xpos += mouse.x - thePoint.width/2; ypos += mouse.y - thePoint.height/2 }
                }

                Keys.onDeletePressed: {
                    destroyAnimation.start()
                }

                // Destroy animation
                SequentialAnimation {
                    id: destroyAnimation
                    running: false; loops: 1
                    ParallelAnimation {
                        PropertyAnimation {target: thePoint; property: "opacity"; from: 1; to: 0; duration: 400}
                        PropertyAnimation {target: thePoint; property: "radius"; from: 8; to: 40; duration: 400}
                    }
                    ScriptAction {
                        script: {
                            var left  = test.itemAt(index-1)
                            var right = test.itemAt(index+1)
                            console.log("L " + left + " R " + right);
                            // Setup new connections
                            if(left && right) connectPoints(left, right);
                            controlPoints.remove(index);
                        }
                    }
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
        id: refinementSlider
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        value: 0
        stepSize: 1.0

        from: 0
        to: 10
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
