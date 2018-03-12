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
            console.log(controlPoints.count);
            if(controlPoints.count > 0) {
                var ctx = getContext("2d");
                ctx.lineWidth = 5;
                ctx.strokeStyle = "red"
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
        anchors.fill: mycanvas
        onPressed: {
            controlPoints.append({"xpos": mouse.x + 10, "ypos": mouse.y - 10});
            mycanvas.requestPaint();
        }
    }

    Repeater {
        model: controlPoints
        delegate: Rectangle {
            id: marker
            x: xpos; y: ypos
            color: focus ? "green" : "blue"
            width: 10; height: width
            radius: width/2

            MouseArea {
                anchors.fill: parent
                onClicked: parent.focus = true
                drag.target: parent
                drag.axis: "XAndYAxis"
                onPositionChanged: if(drag.active) { xpos = mouse.x; mycanvas.requestPaint(); }
            }
        }
    }

    Slider {
        anchors.bottom: mycanvas.bottom
        anchors.horizontalCenter: mycanvas
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
