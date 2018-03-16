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

    ChaikinsModel {
        id: chaikinsModel
    }

    ChaikinsCurve {
        id: curve
        anchors.fill: parent
        refinement: refinementSlider.value
        model: chaikinsModel
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onPressed: chaikinsModel.append(Qt.point(mouse.x, mouse.y))
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

    Repeater {
        model: chaikinsModel
        id: test

        onItemAdded: item.focus = true;

        delegate:
            ControlPoint {
                id: thePoint
                radius: 8
                x: position.x - width/2
                y: position.y - height/2

                MouseArea {
                      anchors.fill: parent
                      scale: 1.8
                      onClicked: parent.focus = true  //if(activeFocusItem instanceof ControlPoint)
                      drag.target: parent
                      drag.axis: "XAndYAxis"
                      drag.smoothed: true
                      drag.threshold: 0
                      onPositionChanged: if(drag.active) {
                        parent.focus = true;
                        position = Qt.point(thePoint.x + mouse.x, thePoint.y + mouse.y)
                     }
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
                            chaikinsModel.remove(index);
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
