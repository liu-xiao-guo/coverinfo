#include "c++/ReadWrite.h"
#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QDebug>

ReadWrite::ReadWrite(QObject *parent) : QObject(parent)
{
}

ReadWrite::~ReadWrite()
{

}

QString ReadWrite::readFile(const QString &fName)
{
    QString fileName = QString("%1.txt").arg(fName);
    QFile file(fileName);
    if (false==file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return "";//文件IO异常
    }
    return file.readAll();
}
QString ReadWrite::readLrc(const QString& songPath)
{
    QString temp=songPath;//判断文件路径
    if(temp.contains(".mp3"))
    {
        lrcPath=temp.replace("mp3","lrc");//替换同名后缀
    }
    else
    {
        lrcPath=temp+".lrc";
    }
    QFile file(lrcPath);//解析文件
    if (false==file.open(QIODevice::ReadOnly | QIODevice::Text)){
        file.close();
        return "error";
    }
    else
    {
        lrcContent.clear();//暴力清空
        lrcContent=QString(file.readAll());
        file.close();
        return lrcContent;
    }
}
void ReadWrite::saveFile(const QString &fName, QString content)
{
    QString fileName = QString("%1.txt").arg(fName);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;//文件IO异常
    }
    file.write(content.toUtf8());
    file.close();
    return;
}
void ReadWrite::saveLrc(const QString &sName,QString lrcContent)
{
    QString fileName = QString("%1.lrc").arg(sName);
    QFile file(fileName);
    if(true==file.exists())
    {//歌词文件已存在则跳出，避免重复写入歌词
        qDebug()<<"File is exists!";
        return;
    }
    else
    {
        if (false==file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return;//文件IO异常
        }
        file.write(lrcContent.toUtf8());//写入歌词
        file.close();
        return;
    }
}
void ReadWrite::saveSkin(const QString &skinPath)
{


    QString iniPath = QString("skin.ini");
    rwIni=new QSettings(iniPath,QSettings::IniFormat);
    rwIni->setValue("skinPath",skinPath);
    qDebug()<<"Saving your skin...";
    delete rwIni;
}
QString ReadWrite::loadSkin()
{
    QString iniPath = QString("skin.ini");
    QFile file(iniPath);
    if(false==file.exists())
    {//不存在则返回错误
        qDebug()<<"File is not exists!";
        return "error";
    }
    else
    {
    rwIni=new QSettings(iniPath,QSettings::IniFormat);
    skinPath=rwIni->value("skinPath").toString();
    delete rwIni;
    qDebug()<<"Loading your skin...";
    return skinPath;
    }
}

