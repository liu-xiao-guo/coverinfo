#ifndef ReadWrite_H
#define ReadWrite_H

#include <QObject>
#include <QString>
#include <QSettings>

//IO操作类

class ReadWrite : public QObject
{
    Q_OBJECT
public:
    explicit ReadWrite(QObject *parent = 0);
    ~ReadWrite();
    QString lrcPath;//歌词文件路径
    QString lrcContent;//歌词条目
    QString skinPath;//皮肤图片路径
    QSettings* rwIni;//INI文件临时指针

public slots:
    /*
     * @brief  读入歌单文件
     * @param fName 歌单文件名
     */
    QString readFile(const QString &fName);

    /*
     * @brief  读入歌词文件
     * @param songPath 同名歌曲路径
     */
    QString readLrc(const QString& songPath);

    /*
     * @brief  写入歌单文件
     * @param content  本地列表歌单条目
     */
    void saveFile(const QString &fName,QString content);

    /*
     * @brief  保存网络歌词
     * @param lrcContent 歌词条目
     */
    void saveLrc(const QString &sName,QString lrcContent);

    /*
     * @brief  保存皮肤Path
     * @param skinPath 皮肤图片路径
     */
    void saveSkin(const QString& skinPath);

    QString loadSkin();//载入皮肤Path

};

#endif // ReadWrite_H
