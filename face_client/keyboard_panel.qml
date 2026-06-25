import QtQuick 2.12
import QtQuick.VirtualKeyboard 2.4
import QtQuick.VirtualKeyboard.Styles 2.4
import QtQuick.VirtualKeyboard.Settings 2.4

Item {
    id: root
    width: 621
    height: 540

    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    InputPanel {
        id: inputPanel
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        // 根据当前输入框类型调整输入法（姓名/地址使用中文，手机号使用数字）
        function updateLayout() {
            var item = Qt.inputMethod.inputItem
            if (!item)
                return

            if (item.objectName === "phoneLineEdit") {
                // 手机号：数字输入
                VirtualKeyboardSettings.locale = "zh_CN"
                // 数字模式更多依赖 inputMethodHints=ImhFormattedNumbersOnly
            } else {
                // 姓名、地址：中文输入
                VirtualKeyboardSettings.locale = "zh_CN"
            }
        }

        Component.onCompleted: updateLayout()

        Connections {
            target: Qt.inputMethod
            onInputItemChanged: inputPanel.updateLayout()
        }

        // 配置支持的语言列表
        Component.onCompleted: {
            VirtualKeyboardSettings.locale = "zh_CN"  // 默认中文
            VirtualKeyboardSettings.availableLocales = ["zh_CN", "en_US"]
        }

        // 键盘样式配置
        keyboard.style: KeyboardStyle {
            // 键盘设计尺寸
            keyboardDesignWidth: 621
            keyboardDesignHeight: 540

            // 边距配置（相对值）
            keyboardRelativeLeftMargin: 20 / keyboardDesignWidth
            keyboardRelativeRightMargin: 20 / keyboardDesignWidth
            keyboardRelativeTopMargin: 10 / keyboardDesignHeight
            keyboardRelativeBottomMargin: 10 / keyboardDesignHeight

            // 按键高度（适配触摸屏）
            keyboardHeight: 45

            // 候选词栏配置
            selectionListHeight: 60
            selectionListFontSize: 16
        }
    }
}
