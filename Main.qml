import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import dev.unstablewoofer

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Disk Space Widget")

    Component.onCompleted: {
        getVolumes()
    }

    function bytesToGb(value) {
        return String(Number(Number(value) / 1073741824).toFixed(2));
    }

    function getColour(percentage) {
        if (percentage > 90)
            return "red"
        else if (percentage > 75)
            return "yellow"
        else
            return "green"
    }

    function getVolumes() {
        var volumeNames = volumes.getVolumeNames()

        volumeNames.forEach(function(item, index) {
            var volumeInfo = volumes.getVolumeInfo(item)

            if (Object.keys(volumeInfo).length != 0) {
                var found=false;

                for (var i=0; i < volumeList.model.count; i++) {
                    if (volumeList.model.get(i).volumeName === item) {
                        found = true

                        for (var key in volumeInfo) {
                            volumeList.model.setProperty(i, key, volumeInfo[key])
                        }

                        break
                    }
                }

                if (!found)
                    volumeList.model.append(volumeInfo)
            }
        })

        volumeRefreshTimer.start()
    }

    ListView {
        id: volumeList
        anchors.fill: parent
        delegate: Component {
            Rectangle {
                property int percentageFree: (Number(freespace) / Number(capacity)) * 100
                property int percentageUsed: 100 - percentageFree

                color: "lightblue"
                height: 95
                width: root.width

                ColumnLayout {
                    anchors.fill: parent

                    Row {
                        Layout.fillWidth: true

                        Text {
                            text: "Drive Name: " + name
                        }
                    }

                    Row {
                        Layout.fillWidth: true

                        Text {
                            text: "Mount Point: " + path
                        }
                    }

                    Row {
                        Layout.fillWidth: true
                        ProgressBar {
                            id: freespaceStatus
                            background: Rectangle {
                                implicitHeight: 25
                                implicitWidth: parent.width
                            }
                            width: parent.width
                            contentItem: Item {
                                Text {
                                    anchors.centerIn: parent
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                    text: "Freespace: " + bytesToGb(freespace) + " / " + bytesToGb(capacity) + " GB (" + percentageFree + "%)"
                                    z: 1000
                                }

                                Rectangle {
                                    height: 25
                                    width: freespaceStatus.visualPosition * parent.width
                                    color: getColour(percentageUsed)
                                    radius: 2
                                }
                            }
                            to: 100
                            value: percentageUsed
                        }
                    }
                }
            }
        }
        model: ListModel {}
    }

    Volumes {
        id: volumes
    }

    Timer {
        id: volumeRefreshTimer
        interval: 30000
        repeat: false
        running: true
        onTriggered: {
            getVolumes()
        }
    }
}
