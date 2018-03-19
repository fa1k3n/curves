import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.3
import QtGraphicalEffects 1.0
import QtQml.Models 2.1

import fa1k3n.curves 1.0

Window {
    visible: true
    id: root
    width: 640
    height: 480
    color: "#2E2F30"
    title: qsTr("Curves")

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
        onPressed: {
            var p = Qt.point(mouse.x, mouse.y);
            var index = chaikinsModel.append(p);
        }
    }

    //Component {
    //    id: line

    /*Repeater {
        model: chaikinsModel
        id: connectors

        Line {
            id: theLine
            width: 2
            color: "#F76494"
            visible: showControlPolygon.checked
            start: position ;
            end: prev ? position.prev.position : Qt.point(0, 0);

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
    }*/

    Repeater {
        model: chaikinsModel
        id: test

        onItemAdded: {
            //if(index > 0)
            //    line.createObject(root, {"start": test.itemAt(index), "end": test.itemAt(index-1)});
            item.focus = true;
        }

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
                            chaikinsModel.remove(position);
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

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width * 3/4
        height: 40
        radius: 4
        color: "#414244"
        id: controlBar
        anchors.margins: {
            left: 10
            right: 10
            bottom: 10
            top: 5
        }
        opacity: 1

        Timer {
            id: hideTimer
            interval: 1000
            running: true
            repeat: false
            onTriggered: hideAnim.start()
        }

        PropertyAnimation on opacity { id: hideAnim; from: controlBar.opacity; to: 0; running: false }
        PropertyAnimation on opacity { id: showAnim; from: controlBar.opacity; to: 1; running: false }

        Row {
            anchors.centerIn: controlBar

            CheckBox {
                id: showControlPolygon
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Show control polygon")
                checked: true
                indicator: Rectangle {
                    anchors.verticalCenter: parent.verticalCenter
                    implicitWidth: 16
                    implicitHeight: 16
                    radius: 3
                    color: "#B6DDE7"

                    border.width: 1
                    Rectangle {
                        visible: showControlPolygon.checked
                        color: "#215664"
                        radius: 1
                        anchors.margins: 4
                        anchors.fill: parent
                    }
                }

                contentItem: Text {
                    text: showControlPolygon.text
                    font: showControlPolygon.font
                    color: "#66B7CD"
                    verticalAlignment: Text.AlignVCenter
                    leftPadding: 15
                    rightPadding: showControlPolygon.indicator.width + showControlPolygon.spacing
                }
            }

            Slider {
                id: refinementSlider
                value: 0
                stepSize: 1.0

                from: 0
                to: 10

                background: Rectangle {
                    x: refinementSlider.leftPadding
                    y: refinementSlider.topPadding + refinementSlider.availableHeight / 2 - height / 2
                    implicitWidth: 200
                    implicitHeight: 4
                    width: refinementSlider.availableWidth
                    height: implicitHeight
                    radius: 2
                    color: "#215664"

                    Rectangle {
                        width: refinementSlider.visualPosition * parent.width
                        height: parent.height
                        color: "#B6DDE7"
                        radius: 2
                    }
                }

                handle: Rectangle {
                    x: refinementSlider.leftPadding + refinementSlider.visualPosition * (refinementSlider.availableWidth - width)
                    y: refinementSlider.topPadding + refinementSlider.availableHeight / 2 - height / 2
                    implicitWidth: 8
                    implicitHeight: 18
                    radius: 2
                    color: "#66B7CD"
                }

            }
        }

        MouseArea {
            anchors.fill: controlBar
            hoverEnabled: true

            // Do this as state machine instead
            onEntered: { hideTimer.stop(); hideAnim.stop(); showAnim.start(); }
            onExited: { hideTimer.start() }
            onPressed: mouse.accepted = false
        }

    }
}
