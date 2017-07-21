import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Show coverInfo for a MP3 song")


    Image{
        id:songPic
        anchors.centerIn: parent
        source: "image://myprovider/50/50/"
    }

    Button {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        text: "Get File"
        onClicked: {
            console.log("it is clicked")
            var path = localSong.openFile();
            console.log("path: " + path)
            var source = "image://myprovider/" + path
            songPic.source = source

//            var temp=localSong.getSong(curIndex);
//            var path = "file:///home/liuxg/Music/test/gohome.mp3"
//            var lastLrc = readWrite.readLrc(path);
//            localSong.setInfo(path)
//            console.log("读取歌词封面...");
//            var upurl = localSong.getCovPicture(path);
//            songPic.source = upurl;
        }
    }
}
