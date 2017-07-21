#include "myimageprovider.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QFileInfo>
#include <tag.h>
#include <fileref.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mpegfile.h>
#include <mp4file.h>

MyImageProvider::MyImageProvider(ImageType type, Flags flags) :
    QQuickImageProvider(type,flags)
{
    manager = new QNetworkAccessManager;
}

MyImageProvider::~MyImageProvider()
{
    delete manager;
}

QImage MyImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "id: " << id;
    qDebug() << "reequestedSize: " << requestedSize.width() + " " + requestedSize.height();
    QUrl url("http://lorempixel.com/" + id);
    qDebug() << "url: " << url.toString();

    QString path("/home/liuxg/Music/test/Lay Low.mp3");
    QImage image = getCoverPicture(id);

    return image;
}

QImage MyImageProvider::getCoverPicture(QString filePath) {
    QFileInfo fileInfo(filePath);
    fileExtension = fileInfo.completeSuffix();
    QString singer="";
    QString sname="";
    QString album="";
    QImage covpic;
    if(fileExtension == "m4a")
    {

        TagLib::MP4::File *mp4File = new TagLib::MP4::File(
                    QFile::encodeName(filePath).constData());
        if(false==mp4File->isOpen())
            return QImage();
        sname=QString(mp4File->tag()->title().toCString(true));
        singer=QString(mp4File->tag()->artist().toCString(true));
        album=QString(mp4File->tag()->album().toCString(true));
        TagLib::MP4::ItemListMap itemListMap = mp4File->tag()->itemListMap();
        TagLib::MP4::Item albumArtItem = itemListMap["covr"];
        TagLib::MP4::CoverArtList albumArtList = albumArtItem.toCoverArtList();
        TagLib::MP4::CoverArt albumArt = albumArtList.front();
        QImage cover;
        cover.loadFromData((const uchar *)
                           albumArt.data().data(),
                           albumArt.data().size());
        covpic = cover.scaled(
                    200,
                    200,
                    Qt::IgnoreAspectRatio,
                    Qt::SmoothTransformation);
        if(!covpic.isNull())
        {
            qDebug()<<"读取M4A封面信息成功";
        }else
        {
            covpic.load(":/image/image/playIcon.png");
            qDebug()<<"读取音乐封面信息失败";
        }
    }
    else if(fileExtension == "mp3")
    {
        TagLib::MPEG::File *mpegFile = new TagLib::MPEG::File(
                    QFile::encodeName(filePath).constData());
        if(false==mpegFile->isOpen())
            return QImage();
        else
        {
            sname=QString(mpegFile->tag()->title().toCString(true));
            singer=QString(mpegFile->tag()->artist().toCString(true));
            album=QString(mpegFile->tag()->album().toCString(true));
            auto tag = mpegFile->ID3v2Tag(false);
            auto list = tag->frameListMap()["APIC"];
            if(!list.isEmpty()) {
                auto frame = list.front();
                auto pic = reinterpret_cast<TagLib::ID3v2::AttachedPictureFrame *>(frame);
                if(pic && !pic->picture().isNull())
                {
                    QImage cover;
                    if(cover.loadFromData(QByteArray::fromRawData(pic->picture().data(), pic->picture().size()))) {
                        covpic = cover.scaled(
                                    200,
                                    200,
                                    Qt::IgnoreAspectRatio,
                                    Qt::SmoothTransformation);
                        qDebug()<<"读取MP3封面信息成功";
                    }
                }
            }
            else
            {
                covpic.load(":/image/image/playIcon.png");
                qDebug()<<"读取音乐封面信息失败";
            }
        }
    }
//    singerlist.append(singer);
//    snamelist.append(sname);
//    albumlist.append(album);
//    covPiclist.append(covpic);
//    int covCount=covPiclist.length();
//    QString covpicpath=QDir::tempPath()+QString("/%1.png").arg(covCount);
//    covpic.save(covpicpath);
//    QUrl covpicUrl=QUrl::fromLocalFile(covpicpath);
    qDebug()<<"歌名为"<<sname;
    qDebug()<<"歌手为"<<singer;
    qDebug()<<"专辑为"<<album;
//    qDebug()<<"URL为"<<covpicUrl;
    // emit setSongInfo(singer,sname,album,covpicUrl);
    return covpic;
}
