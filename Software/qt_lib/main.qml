import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import com.yinhao.myquickpainteditem
import com.yinhao.mygameserver
//import QtQuick.Extras

Window {
	id:window_mainWindow
	property bool controllable : true
	flags: Qt.Dialog
	height: 470
	width: 340
	maximumHeight: 470
	minimumHeight: 470
	maximumWidth: 340
	minimumWidth: 340
	visible: true
	title: qsTr("VDM22")
	Rectangle{
		id: rectangle_display
		width: 256
		height: 256
		anchors.top: parent.top
		anchors.left: text_stickXAxis.right
		anchors.margins: 15
		border.color: "#252525"
		border.width: 2
		radius: width*1
		clip: true
		focus: true
		//		focus: true
		//		Keys.onPressed: myGameServer.onKeyboardKeysPressed(event)
		Keys.onPressed: {
			switch(event.key){
			case Qt.Key_R:
				myGameServer.onButtonValueChanged("SPACE",1)
				break
			case Qt.Key_W:
				myGameServer.onSliderValueChanged("slider_stickY",1)
				break
			case Qt.Key_S:
				myGameServer.onSliderValueChanged("slider_stickY",0)
				break
			case Qt.Key_A:
				myGameServer.onSliderValueChanged("slider_stickX",1)
				break
			case Qt.Key_D:
				myGameServer.onSliderValueChanged("slider_stickX",0)
				break
			case Qt.Key_J:
				myGameServer.onButtonValueChanged("button_launchButton",true)
				break
			case Qt.Key_K:
				myGameServer.onButtonValueChanged("button_counterMeasureButton",true)
				break
			case Qt.Key_U:
				myGameServer.onButtonValueChanged("button_piperUpButton",true)
				break
			case Qt.Key_M:
				myGameServer.onButtonValueChanged("button_piperDownButton",true)
				break
			}

		}
		Keys.onReleased: {
			switch(event.key){
			case Qt.Key_W:
				myGameServer.onSliderValueChanged("slider_stickY",0.5)
				break
			case Qt.Key_S:
				myGameServer.onSliderValueChanged("slider_stickY",0.5)
				break
			case Qt.Key_A:
				myGameServer.onSliderValueChanged("slider_stickX",0.5)
				break
			case Qt.Key_D:
				myGameServer.onSliderValueChanged("slider_stickX",0.5)
				break
			case Qt.Key_J:
				myGameServer.onButtonValueChanged("button_launchButton",false)
				break
			case Qt.Key_K:
				myGameServer.onButtonValueChanged("button_counterMeasureButton",false)
				break
			case Qt.Key_U:
				myGameServer.onButtonValueChanged("button_piperUpButton",false)
				break
			case Qt.Key_M:
				myGameServer.onButtonValueChanged("button_piperDownButton",false)
				break
			}
		}
	}
	MyQuickPaintedItem{
		id: myQuickPaintedItem_radarScreen
		objectName: "myQuickPaintedItem_radarScreen"
		anchors{
			top : rectangle_display.top
			bottom : rectangle_display.bottom
			left : rectangle_display.left
			right : rectangle_display.right
		}
		layer.enabled: true
		layer.effect: OpacityMask{
			maskSource: Rectangle{
				id: rectangle_mask
				width: 256
				height: 256
				anchors.fill: parent
				radius: 256
			}
		}
	}
	Text {
		id: text_stickXAxis
		anchors.left: parent.left
		anchors.leftMargin: 20
		anchors.top: parent.top
		anchors.topMargin: 5
		text: qsTr("X")
		font.family: "Microsoft YaHei UI"
	}
	Text {
		id: text_stickYAxis
		text: qsTr("Y")
		anchors.left: rectangle_display.right
		anchors.leftMargin: 20
		anchors.top: parent.top
		anchors.topMargin: 5
		font.family: "Microsoft YaHei UI"
	}
	Slider{
		id: slider_stickX
		objectName: "slider_stickX"
		palette.highlight: "#252525"
		anchors.horizontalCenter: text_stickXAxis.horizontalCenter
		anchors.top: text_stickXAxis.bottom
		anchors.topMargin: 5
		anchors.bottom: button_launchButton.top
		anchors.bottomMargin: 10
		orientation: Qt.Vertical
		value: 0.5
		onValueChanged: myGameServer.onSliderValueChanged("slider_stickX",value)
		onPressedChanged: {
			if(!pressed)
				value = 0.5
		}
		opacity: controllable? 1.0 : 0.3
		enabled: controllable
	}
	Slider{
		id: slider_stickY
		objectName: "slider_stickY"
		palette.highlight: "#252525"
		anchors.horizontalCenter: text_stickYAxis.horizontalCenter
		anchors.top: text_stickYAxis.bottom
		anchors.topMargin: 5
		anchors.bottom: button_launchButton.top
		anchors.bottomMargin: 10
		orientation: Qt.Vertical
		value: 0.5
		onValueChanged: myGameServer.onSliderValueChanged("slider_stickY",value)
		onPressedChanged: {
			if(!pressed)
				value = 0.5
		}
		opacity: controllable? 1.0 : 0.3
		enabled: controllable
	}
	Text{
		id: text_rwr1Led
		anchors.bottom: rectangle_rwr1Led.top
		font.pointSize: 5
		anchors.bottomMargin: 1
		anchors.horizontalCenter: rectangle_rwr1Led.horizontalCenter
		text: qsTr("指示灯1/RWR1")
		font.family: "Microsoft YaHei UI"
	}
	Text{
		id: text_rwr2Led
		anchors.bottom: rectangle_rwr2Led.top
		font.pointSize: 5
		anchors.bottomMargin: 1
		anchors.horizontalCenter: rectangle_rwr2Led.horizontalCenter
		text: qsTr("指示灯2/RWR2")
		font.family: "Microsoft YaHei UI"
	}
	Text{
		id: text_buzzer
		anchors.bottom: rectangle_buzzer.top
		font.pointSize: 5
		anchors.bottomMargin: 1
		anchors.horizontalCenter: rectangle_buzzer.horizontalCenter
		text: qsTr("蜂鸣器/BUZZ")
		font.family: "Microsoft YaHei UI"
	}
	Rectangle{
		id: rectangle_rwr1Led
		objectName: "rectangle_rwr1Led"
		color: "black"
		height: 10
		width: 50
		anchors.right: rectangle_rwr2Led.left
		anchors.rightMargin: 40
		anchors.bottom: button_launchButton.top
		anchors.bottomMargin: 10
		border.color: "#949494"
		border.width: 1
	}
	Rectangle{
		id: rectangle_rwr2Led
		objectName: "rectangle_rwr2Led"
		color: "black"
		height: 10
		width: 50
		anchors.bottom: button_launchButton.top
		anchors.bottomMargin: 10
		anchors.horizontalCenter: button_launchButton.horizontalCenter
		border.color: "#949494"
		border.width: 1
	}
	Rectangle{
		id: rectangle_buzzer
		objectName: "rectangle_buzzer"
		color: "black"
		height: 10
		width: 50
		anchors.left: rectangle_rwr2Led.right
		anchors.leftMargin: 40
		anchors.bottom: button_launchButton.top
		anchors.bottomMargin: 10
		border.color: "#949494"
		border.width: 1
	}
	Button{
		id: button_launchButton
		height: 30
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.margins: 10
		anchors.bottom: button_counterMeasureButton.top
		text:"发射/Launch (J)"
		font.family: "Microsoft YaHei UI"
		palette.button: "#252525"
		onPressed: myGameServer.onButtonValueChanged("button_launchButton",true)
		onReleased: myGameServer.onButtonValueChanged("button_launchButton",false)
		enabled: controllable
	}
	Button{
		id: button_counterMeasureButton
		height: 30
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: button_piperUpButton.top
		anchors.margins: 10
		text: "反制/Counter Measure (K)"
		font.family: "Microsoft YaHei UI"
		palette.button: "#252525"
		onPressed: myGameServer.onButtonValueChanged("button_counterMeasureButton",true)
		onReleased: myGameServer.onButtonValueChanged("button_counterMeasureButton",false)
		enabled: controllable
	}
	Button{
		id: button_piperUpButton
		height: 30
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.bottom: button_piperDownButton.top
		anchors.margins:10
		text: "锁定器上移/Piper Up (U)"
		font.family: "Microsoft YaHei UI"
		palette.button: "#252525"
		onPressed: myGameServer.onButtonValueChanged("button_piperUpButton",true)
		onReleased: myGameServer.onButtonValueChanged("button_piperUpButton",false)
		enabled: controllable
	}
	Button{
		id: button_piperDownButton
		height: 30
		anchors.bottom: parent.bottom
		anchors.left: parent.left
		anchors.right: parent.right
		anchors.margins: 10
		text: "锁定器下移/Piper Down (M)"
		font.family: "Microsoft YaHei UI"
		palette.button: "#252525"
		onPressed: myGameServer.onButtonValueChanged("button_piperDownButton",true)
		onReleased: myGameServer.onButtonValueChanged("button_piperDownButton",false)
		enabled: controllable
	}
}

/*##^##
Designer {
	D{i:0;formeditorZoom:0.75}
}
##^##*/
