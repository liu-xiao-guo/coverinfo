#ifndef MYIMAGEPROVIDER_H
#define MYIMAGEPROVIDER_H

#include <QQuickImageProvider>
class QNetworkAccessManager;

class MyImageProvider : public QQuickImageProvider
{
public:
    MyImageProvider(ImageType type, Flags flags = 0);
    ~MyImageProvider();
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

protected:
    QNetworkAccessManager *manager;
private:
    QImage getCoverPicture(QString filePath);

private:
    QString fileExtension;//文件格式检测
};

#endif // MYIMAGEPROVIDER_H
