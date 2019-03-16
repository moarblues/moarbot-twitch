#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QTimer>
#include <QDateTime>

namespace Ui {
class mainWindow;
}

class mainWindow : public QMainWindow
{
    Q_OBJECT

    QTimer *_tPing;

    QString _sChannel;
    QTcpSocket *_tcpSocket;
    bool bHashtagRelease = true;
    bool bGreetingRelease = true;
    bool bDimaas63Release = true;

    QString _sPollName;
    QStringList _slPollData;
    QStringList _slPollResults;

    QNetworkReply *_nrReply;
    QStringList _slNamRequest;
    QStringList _slNamData;

    bool bOnlytheoceanknowsRelease = true;
    bool bGolpazexRelease = true;
    bool bTpayppRelease = true;
    bool bHenrymanningRelease = true;
    bool bMultistrongRelease = true;
    bool bGarryRelease = true;
    bool bMoeRelease = true;
    bool bMetalkaRelease = true;
    bool bDeathRelease = true;
    bool bDimoddRelease = true;
    bool bMoratongRelease = true;
    bool bNornRelease = true;
    bool bVolchisheRelease = true;
    bool bGoohaRelease = true;

public:
    explicit mainWindow(QWidget *parent = 0);
    ~mainWindow();

signals:

private slots:
    void readSocket();
    void readNamReply();
    void connectToServer();
    void disconnectFromServer();
    void releaseHashtagTimer();
    void releaseGreetingTimer();
    void pingTimer();
    void startPoll(const QString &sPollData);
    void completePoll();
    void sendMessage(QString sMsg);
    void sendPrivmsg(QString sMsg);
    void sendNamRequest(const QString &sNamRequestUrl);
    QStringList parseIrcMessage(const QString &sMsg);
    void parseRequestMessage(const QJsonObject &joData);
    void executeCommand(const QStringList &slMessageData);
    void executeNamCommand(const QStringList &slMessageData);
    void replyError(const QNetworkReply::NetworkError &neState);
    void replySslErrors(const QList<QSslError> &errList);

    void on_btn_connect_clicked();
    void on_btn_join_clicked();
    void on_btn_sendprivmsg_clicked();

    void on_btn_test_clicked();

private:
    Ui::mainWindow *ui;
};

#endif // MAINWINDOW_H
