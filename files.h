#ifndef FILES_H
#define FILES_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

class files : public QObject
{
    Q_OBJECT

public:
    explicit files(QObject *parent = 0);

signals:
    void sendData(const QString &sData);

public slots:
    void readStory();
};

#endif // FILES_H
