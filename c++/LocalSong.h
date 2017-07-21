#ifndef LOCALSONG_H
#define LOCALSONG_H

#include <QLabel>
#include <QStringList>
#include <QList>
#include <QStandardPaths>
#include <QImage>
#include <QUrl>
#include <QByteArray>
#include <tag.h>
#include <fileref.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <mpegfile.h>
#include <mp4file.h>

class LocalSong: public QLabel
{
    Q_OBJECT
public:
    explicit LocalSong(QWidget *parent = 0);
    ~LocalSong();
    static QImage imageForFile(const QString *file);
    static QImage imageForTag(TagLib::ID3v2::Tag *tag);
    const QStringList musicPaths =
            QStandardPaths::standardLocations(QStandardPaths::MusicLocation);

    TagLib::FileRef *fileRef;//第三方调用
    QString filePath;//文件路径
    QString locDir;//本地目录
    QString fileExtension;//文件格式检测
    QStringList singerlist;//存放歌手链表
    QStringList snamelist;//存放单曲链表
    QStringList albumlist;//存放专辑链表
    QStringList songlist;//存放单曲路径
    QStringList lrclist;//存放歌词路径
    QList<QImage> covPiclist;//存放封皮图片

    Q_INVOKABLE QImage getCoverPicture(QString path);

signals:
    /*
     * @brief 发射本地单曲路径
     * @param songPath 单曲地址
     * @param listLength 链表长度
     */
    void setSongPath(const QString &songPath,const int listLength);

    /*
     * @brief  发射本地单曲信息
     * @param singer 歌手名
     * @param sname 歌曲名
     * @param album 专辑名
     */
    void setSongInfo(const QString &singer,const QString &sname,const QString &album,const QUrl &covUrl);

    /*
     * @brief 目录文件数
     * @param length 单曲列表长度地址
     */
    void dirCount(const int&length);
public slots:
    QString openFile();//打开文件
    void addDir();//扫描目录
    void reset();//重置

    /*
     * @brief  移除列表歌曲
     * @param SongIndex 单曲编号
     */
    void rmSong(const int& SongIndex);

    /*
     * @brief  读取xml信息
     * @param fName xml文件名
     */
    void readList(const QString &fName);

    /*
     * @brief 保存本地列表信息到xml文件
     * @param fName xml文件名
     */
    void saveList(const QString &fName);

    /*
     * @brief 添加至列表
     * @param filePath 单曲文件路径
     */
    void addSongList(const QString &filePath);

    /*
     * @brief 解析本地单曲信息
     * @param filePath  单曲文件路径
     */
    void setInfo(const QString &filePath);

    /*
     * @brief  获取本地单曲路径
     * @param SongIndex 单曲编号
     */
    QString getSong(int SongIndex);

    /*
     * @brief  获取本地单曲歌手名
     * @param SongIndex 单曲编号
     */
    QString getSinger(int SongIndex);

    /*
     * @brief  获取本地单曲歌曲名
     * @param SongIndex 单曲编号
     */
    QString getSname(int SongIndex);

    /*
     * @brief  获取本地单曲专辑名
     * @param SongIndex 单曲编号
     */
    QString getAlbum(int SongIndex);

    /*
     * @brief  获取本地单曲封皮信息
     * @param SongIndex 单曲编号
     */
    QUrl getCovPic(int SongIndex);
};

#endif // LOCALSONG_H
