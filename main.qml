import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    visible: true
    id: root
    width: 640
    height: 480
    title: qsTr("Hello World")

    ListModel { id: controlPoints }

    Canvas {
        id: mycanvas
        anchors.fill: parent
        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            ctx.lineWidth = 5;
            ctx.strokeStyle = "red"
            if(controlPoints.count > 1) {
                var p = controlPoints.get(0);
                ctx.moveTo(p.xpos, p.ypos);
                for(var i= 1; i < controlPoints.count; i ++) {
                    p = controlPoints.get(i);
                    ctx.lineTo(p.xpos, p.ypos);
                }
                ctx.stroke()
            }
        }

    }

    MouseArea {
        id: mouseArea
        anchors.fill: mycanvas
        onPressed: {
            controlPoints.append({"xpos": mouse.x - 15/2, "ypos": mouse.y - 15/2, "next": {}, "prev": {}});
        }
    }

    Repeater {
        model: controlPoints
        onItemAdded: {
            if(activeFocusItem instanceof ControlPoint) {
                var prev = activeFocusItem
                item.prevPoint = prev;
                prev.nextPoint = item;
            }
            item.focus = true;
            mycanvas.requestPaint()
        }
        delegate: Item {
            ControlPoint {
                x: xpos; y: ypos;
                prevPoint: prev; nextPoint: next

                MouseArea {
                      anchors.fill: parent
                      scale: 1.8
                      onClicked: parent.focus = true
                      drag.target: parent
                      drag.axis: "XAndYAxis"
                      drag.smoothed: true
                      drag.threshold: 0
                      onPositionChanged: if(drag.active) { xpos += mouse.x - 15/2; ypos += mouse.y - 15/2; mycanvas.requestPaint() }
                  }
            }
        }
    }

    Slider {
        anchors.bottom: mycanvas.bottom
        anchors.horizontalCenter: mycanvas.horizontalCenter
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
