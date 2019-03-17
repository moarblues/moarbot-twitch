#include "files.h"

files::files(QObject *parent) : QObject(parent)
{

}

void files::readStory()
{
//    qDebug() << QDateTime::currentMSecsSinceEpoch();
    QString sKey = QString::number(qrand() % ((331 + 1) - 1) + 1);

    QFile *dFile = new QFile(this);
    dFile->setFileName(QDir::currentPath().append("/malchik"));
    QString sTmp,sResult;
    bool bRead = false;

//    qDebug() << sKey;
    if (dFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream out(dFile);
        out.setCodec("UTF-8");

        do
        {
            sTmp = out.readLine();
            if (sTmp.isNull() || sTmp.isEmpty()) {continue;}
            if (sTmp.startsWith("#"))
            {
                if (bRead) {bRead=false;break;}
                sTmp.remove(0,1);
                if (QString::compare(sTmp,sKey)!=0) {continue;}
                else {bRead=true;sResult.append("#").append(sTmp).append(" ");continue;}
            }
            if (bRead)
            {sResult.append(sTmp).append(" ");}
        } while (!sTmp.isNull());
    }
    emit this->sendData(sResult);

    dFile->close();
    delete dFile;
}
