#include "c++/LocalSong.h"
#include <QUrl>
#include <QMediaContent>
#include <QFileDialog>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDir>
#include <QDebug>
#pragma execution_character_set("utf-8")

LocalSong::LocalSong(QWidget *parent) : QLabel(parent)
{
}

LocalSong::~LocalSong()
{
    saveList("locallist");
    reset();
}

QString LocalSong::openFile()
{



    filePath = QFileDialog::getOpenFileName(
                this, tr("添加本地音乐"),
                musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),
                tr("Music files (*.mp3 *.m4a);;All files (*.*)"));
    if(filePath.isEmpty())
    {
        return "";
    } else {
        return filePath;
    }

//    addSongList(filePath);
//    setInfo(filePath);
//    qDebug()<<"文件路径为"<<filePath;
//    unsigned long sinLength=songlist.length();
//    qDebug()<<"歌曲总数为"<<sinLength;
//    emit dirCount(sinLength);
}
void LocalSong::addDir()
{
    locDir= QFileDialog::getExistingDirectory(this,tr("扫描本地目录"),musicPaths.isEmpty() ? QDir::homePath() : musicPaths.first(),QFileDialog::ShowDirsOnly
                                              | QFileDialog::DontResolveSymlinks);
    QDir dir(locDir);
    if(false==dir.exists())
    {
        return;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();

    int file_count = list.count();
    if(file_count <= 0)
    {
        return;
    }
    for(int i=0; i<file_count;++i)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();
        if(QString::compare(suffix, QString("mp3"), Qt::CaseInsensitive) == 0)
        {
            QString absolute_file_path = file_info.absoluteFilePath();
            addSongList(absolute_file_path);
            setInfo(absolute_file_path);
        }
    }
    qDebug()<<"已添加整个目录"<<locDir;
    unsigned long dirLength=songlist.length();
    qDebug()<<"歌曲总数为"<<dirLength;
    emit dirCount(dirLength);
}
QString LocalSong::getSong(int SongIndex)
{
    if(SongIndex<=0)
    {
        return "";
    }
    else
    {
        QString localPath=songlist.at(SongIndex-1).toUtf8();
        return localPath;
    }
}
QString LocalSong::getSinger(int SongIndex)
{
    if(SongIndex<=0)
    {
        return "未知";
    }
    else
    {
        QString lsinger=singerlist.at(SongIndex-1).toUtf8();
        qDebug()<<"歌手为"<<lsinger;
        return lsinger;
    }
}
QString LocalSong::getSname(int SongIndex)
{
    if(SongIndex<=0)
    {
        return "未知";
    }
    else
    {
        QString lsname=snamelist.at(SongIndex-1).toUtf8();
        qDebug()<<"第"<<SongIndex<<"首歌名为"<<lsname;
        return lsname;
    }
}
QString LocalSong::getAlbum(int SongIndex)
{
    if(SongIndex<=0)
    {
        return "未知";
    }
    else
    {
        QString lalbum=albumlist.at(SongIndex-1).toUtf8();
        qDebug()<<"专辑为"<<lalbum;
        return lalbum;
    }
}
QUrl LocalSong::getCovPic(int SongIndex)
{
    if(SongIndex<=0)
    {
        QUrl covUrl=QUrl::fromLocalFile(":/image/image/playIcon.png");
        return covUrl;
    }
    else
    {
        QImage covpic=covPiclist.at(SongIndex-1);
        QString covpicpath=QDir::tempPath()+QString("/%1.png").arg(SongIndex);
        covpic.save(covpicpath);
        QUrl covUrl=QUrl::fromLocalFile(covpicpath);
        return covUrl;
    }
}
void LocalSong::addSongList(const QString &filePath)
{
    songlist.append(filePath);
    int listLength=songlist.length();
    emit setSongPath(filePath,listLength);
}
void LocalSong::setInfo(const QString &filePath)
{
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
            return;
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
                    50,
                    50,
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
            return;
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
                                    50,
                                    50,
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
    singerlist.append(singer);
    snamelist.append(sname);
    albumlist.append(album);
    covPiclist.append(covpic);
    int covCount=covPiclist.length();
    QString covpicpath=QDir::tempPath()+QString("/%1.png").arg(covCount);
    covpic.save(covpicpath);
    QUrl covpicUrl=QUrl::fromLocalFile(covpicpath);
    qDebug()<<"歌名为"<<sname;
    qDebug()<<"歌手为"<<singer;
    qDebug()<<"专辑为"<<album;
    qDebug()<<"URL为"<<covpicUrl;
    emit setSongInfo(singer,sname,album,covpicUrl);
    return;
}

QImage LocalSong::getCoverPicture(QString path) {
    QFileInfo fileInfo(path);
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
                    50,
                    50,
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
                                    50,
                                    50,
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

void LocalSong::saveList(const QString &fName)
{
    QString fileName = QString("%1.xml").arg(fName);
    QFile file(fileName);
    if(false==file.open(QIODevice::ReadWrite|QIODevice::Truncate))
        return;
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();// 写文档头
    writer.writeStartElement("Local_Music_List");
    for (int i = 0; i < songlist.length(); i++)
    {
        filePath = getSong(i+1);
        writer.writeStartElement("song");
        writer.writeAttribute("ID", QString::number(i+1, 10));
        writer.writeTextElement("path", filePath);
        writer.writeEndElement();
    }
    writer.writeEndElement();
    writer.writeEndDocument();
    qDebug()<<"本地列表保存完成";
    file.close();
}
void LocalSong::readList(const QString &fName)
{
    QString fileName = QString("%1.xml").arg(fName);
    QFile file(fileName);
    if (false==file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    QXmlStreamReader reader;
    reader.setDevice(&file);
    songlist.clear();//清空歌曲路径列表
    while (false==reader.atEnd())
    {
        reader.readNext();
        if (reader.isStartElement())
        {
            if (reader.name() == "path")
            {
                QString songPath=reader.readElementText().toUtf8();
                addSongList(songPath);//读取列表
                setInfo(songPath);
            }
        }
    }
    qDebug()<<"本地列表读取完成";
    unsigned long dirLength=songlist.length();
    qDebug()<<"歌曲总数为"<<dirLength;
    emit dirCount(dirLength);
    file.close();
}

void LocalSong::rmSong(const int& SongIndex)
{
    qDebug()<<"正在删除第"<<SongIndex<<"首歌曲";
    songlist.removeAt(SongIndex);
    singerlist.removeAt(SongIndex);
    snamelist.removeAt(SongIndex);
    albumlist.removeAt(SongIndex);
    lrclist.removeAt(SongIndex);
    int listLength=songlist.length();
    if(listLength>0)
    {
    qDebug()<<"当前歌曲总数为"<<listLength;
    emit setSongPath("",listLength);
    saveList("locallist");
    }
    return;
}
void LocalSong::reset()
{
    songlist.clear();
    singerlist.clear();
    snamelist.clear();
    albumlist.clear();
    lrclist.clear();
}
