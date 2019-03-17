#include "mainwindow.h"
#include "ui_mainwindow.h"

mainWindow::mainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mainWindow)
{
    ui->setupUi(this);

    _tcpSocket = new QTcpSocket(this);
    connect(_tcpSocket,SIGNAL(readyRead()),this,SLOT(readSocket()));

//    _nrReply = new QNetworkReply(this);

    _tPing = new QTimer(this);
    connect(_tPing,SIGNAL(timeout()),this,SLOT(pingTimer()));
    _tPing->start(300000);

    _tStory = new QTimer(this);
    connect(_tStory,SIGNAL(timeout()),this,SLOT(getStory()));
    _tStory->start(300000);
}

mainWindow::~mainWindow()
{
    this->disconnectFromServer();
    _tcpSocket->close();
    _tcpSocket->deleteLater();
    delete ui;
}

void mainWindow::readSocket()
{
    QByteArray baReadLine = _tcpSocket->readAll();
    QString sReadLine = baReadLine;
    QListWidgetItem *lwi = new QListWidgetItem(QString(">> ").append(QTime::currentTime().toString()).append(" | ").append(sReadLine));
    lwi->setBackgroundColor(QColor(Qt::lightGray));
    this->ui->lw_log->addItem(lwi);
    this->ui->lw_log->scrollToBottom();

    QStringList slLines = sReadLine.split("\r\n");
    QStringList slMessageData;
    for (int iline=0;iline<slLines.size();++iline)
    {
        if (slLines.at(iline).isEmpty()) {continue;}
        slMessageData = this->parseIrcMessage(slLines[iline]);
        if (slMessageData.last().isEmpty()) {continue;}
        if (QString::compare(slMessageData.first(),"PING ")==0)
        {this->sendMessage("PONG :tmi.twitch.tv\r\n");}
        if (slMessageData.size() !=7) {continue;}

        if (slMessageData.last().at(0) == '!')
        {this->executeCommand(slMessageData);return;}

        if (slMessageData.last().contains("привет",Qt::CaseInsensitive) && bGreetingRelease)
        {
            bGreetingRelease = false;
            QTimer::singleShot(60000,this,SLOT(releaseGreetingTimer()));
            this->sendPrivmsg(" HeyGuys");
        }
        else if (slMessageData.last().startsWith("#") && bHashtagRelease)
        {
            bHashtagRelease = false;
            QTimer::singleShot(60000,this,SLOT(releaseHashtagTimer()));
            this->sendPrivmsg(slMessageData.last());
        }
//        else if (QString::compare(slMessageData.at(1),"onlytheoceanknows")==0 && bOnlytheoceanknowsRelease)
//        {
//            bOnlytheoceanknowsRelease = false;
//            this->sendPrivmsg("onlytheoceanknows Я знаю твой страшный секрет! https://imgur.com/a/ccPmVqX");
//        }
//        else if (QString::compare(slMessageData.at(1),"golpazex")==0 && bGolpazexRelease)
//        {
//            bGolpazexRelease = false;
//            this->sendPrivmsg("golpazex Чем праща хороша?");
//        }
//        else if (QString::compare(slMessageData.at(1),"tpaypp")==0 && bTpayppRelease)
//        {
//            bTpayppRelease = false;
//            this->sendPrivmsg("tpaypp Стример будет фидить, не переживай");
//        }
//        else if (QString::compare(slMessageData.at(1),"henrymanning94")==0 && bHenrymanningRelease)
//        {
//            bHenrymanningRelease = false;
//            this->sendPrivmsg("henrymanning94 Когда стрим с пятью шавухами?");
//        }
//        else if (QString::compare(slMessageData.at(1),"multistrong999")==0 && bMultistrongRelease)
//        {
//            bMultistrongRelease = false;
//            this->sendPrivmsg("multistrong999 Мм, я чувствую аромат крепкого кофе...");
//        }
//        else if (QString::compare(slMessageData.at(1),"garryp0")==0 && bGarryRelease)
//        {
//            bGarryRelease = false;
//            this->sendPrivmsg("garryp0 שלום. יש חדשות על הג'ינג'ים?");
//        }
//        else if (QString::compare(slMessageData.at(1),"moarblues")==0 && bMoeRelease)
//        {
//            bMoeRelease = false;
//            this->sendPrivmsg("moarblues СТРИМЬ ДАВАЙ, ХВАТИТ В ЧАТ ПИСАТЬ, МУДЕНЬ!!!!11один");
//        }
//        else if (QString::compare(slMessageData.at(1),"empire_metal")==0 && bMetalkaRelease)
//        {
//            bMetalkaRelease = false;
//            this->sendPrivmsg("empire_metal HeyGuys (oo) гладкое бритьё и прозрачные щупальца");
//        }
//        else if (QString::compare(slMessageData.at(1),"death391")==0 && bDeathRelease)
//        {
//            bDeathRelease = false;
//            this->sendPrivmsg("death391 Санкт-Петербург город хип-хопа! Чё как по новым трекам?");
//        }
//        else if (QString::compare(slMessageData.at(1),"dimodd")==0 && bDimoddRelease)
//        {
//            bDimoddRelease = false;
//            this->sendPrivmsg("dimodd ЛАЙК А РЭЙНБОУ ИН ЗЕ ДАРК! \\m/");
//        }
//        else if (QString::compare(slMessageData.at(1),"mora_tong")==0 && bMoratongRelease)
//        {
//            bMoratongRelease = false;
//            this->sendPrivmsg("mora_tong https://imgur.com/a/VY9xaRl (Металка сделала)");
//        }
//        else if (QString::compare(slMessageData.at(1),"norn904")==0 && bNornRelease)
//        {
//            bNornRelease = false;
//            this->sendPrivmsg("norn904 компрометирующие факты не найдены, исполняю стандартное приветствие HeyGuys");
//        }
//        else if (QString::compare(slMessageData.at(1),"volchishe")==0 && bVolchisheRelease)
//        {
//            bVolchisheRelease = false;
//            this->sendPrivmsg("volchishe АУУУУУУУУУУУУУУ!");
//        }
//        else if (QString::compare(slMessageData.at(1),"gooha1")==0 && bGoohaRelease)
//        {
//            bGoohaRelease = false;
//            this->sendPrivmsg("gooha1 Как дела на фабрике подгузников в Епонии?");
//        }
        else if (slMessageData.last().contains("GetViewers",Qt::CaseInsensitive))
        {this->sendPrivmsg(QString("/ban ").append(slMessageData.at(1)));}
    }
}

void mainWindow::readNamReply()
{
    QJsonObject joReply = QJsonDocument::fromJson(_nrReply->readAll()).object();
    _nrReply->deleteLater();
    this->parseRequestMessage(joReply);
}

void mainWindow::connectToServer()
{
//
//    :moarbotx!moarbotx@moarbotx.tmi.twitch.tv
    _tcpSocket->connectToHost("irc.chat.twitch.tv",6667);

    this->sendMessage(QString("PASS ").append(this->ui->le_oauth->text()).append("\r\n"));
    this->sendMessage("NICK moarbotx\r\n");
//    _tcpSocket->write("USER moarbotx\r\n");

    if (_tcpSocket->canReadLine()) {this->readSocket();}
}

void mainWindow::disconnectFromServer()
{
    this->sendMessage("QUIT Getting out of here!\r\n");
}

void mainWindow::releaseHashtagTimer()
{
    bHashtagRelease = true;
}

void mainWindow::releaseGreetingTimer()
{
    bGreetingRelease = true;
}

void mainWindow::pingTimer()
{
    this->sendMessage("PING :tmi.twitch.tv\r\n");
}

void mainWindow::startPoll(const QString &sPollData)
{
    _slPollData.clear();
    _slPollResults.clear();
    bPollRelease = false;
    QStringList slSeparators;
    slSeparators << "*" << "?" << "!";
    short currentSeparator = 0;
    QStringList slPollData;
    QString sItem;
    for (int iitm=0;iitm<sPollData.size();++iitm)
    {
        if (QString::compare(sPollData.at(iitm),slSeparators.at(currentSeparator)) == 0)
        {
            slPollData << sItem;
            sItem.clear();
            if (currentSeparator<slSeparators.size()-1) {currentSeparator++;}
            continue;
        }
        sItem.append(sPollData.at(iitm));
    }
    slPollData << sItem;
    if (slPollData.size()>12) {this->sendPrivmsg("Не более девяти вариантов ответа");bPollRelease = true;return;}
    if (slPollData.size()<4) {this->sendPrivmsg("Неверная структура запроса голосования");bPollRelease = true;return;}
    QString sTemp;
    sTemp.append("Голосование \"").append(slPollData.at(2)).append("?\" запущено на ").append(slPollData.at(1))
            .append(" секунд. Варианты: ");
    _slPollData.append(slPollData.at(2));
    for (int iitm=3;iitm<slPollData.size();++iitm)
    {
        sTemp.append("!v ").append(QString::number(iitm-2)).append(" - ").append(slPollData.at(iitm)).append(" ");
        _slPollData.append(slPollData.at(iitm));
    }
    QTimer::singleShot(slPollData.at(1).toShort()*1000,this,SLOT(completePoll()));
    this->sendPrivmsg(sTemp);
}

void mainWindow::completePoll()
{
    QString sResults;
    QList<short> lsResults;

    for (int iitm=1;iitm<_slPollData.size();++iitm)
    {lsResults.append(0);}

    for (int iitm=0;iitm<_slPollResults.size();++iitm)
    {++lsResults[_slPollResults.at(iitm).toShort()-1];}
//    this->sendPrivmsg(sResults);

    sResults = QString("Голосование \"").append(_slPollData.at(0)).append("?\" закончено. Результаты: ");
    for (int iitm=1;iitm<_slPollData.size();++iitm)
    {sResults.append(_slPollData.at(iitm)).append(" - ").append(QString::number(lsResults.at(iitm-1)).append(", "));}
    short imax = 0;
    short winCol = 0;
    for (int iitm=0;iitm<lsResults.size();++iitm)
    {if(lsResults.at(iitm)>imax) {imax = lsResults.at(iitm);winCol=iitm;}}
    sResults.append("Победитель \"").append(_slPollData.at(winCol+1)).append("\" с результатом ").append(QString::number(imax));
    bPollRelease=true;
    this->sendPrivmsg(sResults);
}

void mainWindow::getStory()
{
    QThread *tReadStory = new QThread(this);
    files *fObj = new files();
    fObj->moveToThread(tReadStory);
    connect(fObj,SIGNAL(sendData(QString)),this,SLOT(postStory(QString)));
    connect(tReadStory,SIGNAL(started()),fObj,SLOT(readStory()),Qt::QueuedConnection);
    connect(this,SIGNAL(storyFinished()),tReadStory,SLOT(quit()));
    connect(tReadStory,SIGNAL(finished()),fObj,SLOT(deleteLater()));
    connect(fObj,SIGNAL(destroyed()),tReadStory,SLOT(deleteLater()));
    tReadStory->start();
}

void mainWindow::postStory(const QString &sStory)
{
    emit this->storyFinished();
    this->sendPrivmsg(QString(sStory).append(" KomodoHype"));
}

void mainWindow::sendMessage(QString sMsg)
{
    QListWidgetItem *lwi = new QListWidgetItem(QString("<< ").append(QTime::currentTime().toString()).append(" | ").append(sMsg));
//    lwi->setBackgroundColor(QColor(Qt::lightGray));
    this->ui->lw_log->addItem(lwi);
    this->ui->lw_log->scrollToBottom();

    QTextStream tsStream(_tcpSocket);
    tsStream.setCodec("UTF-8");

    tsStream << sMsg;
    tsStream.flush();
}

void mainWindow::sendPrivmsg(QString sMsg)
{
    this->sendMessage(QString("PRIVMSG #").append(_sChannel).append(" :").append(sMsg).append("\r\n").toStdString().c_str());
}

void mainWindow::sendNamRequest(const QString &sNamRequestUrl)
{
    QNetworkAccessManager *namManager = new QNetworkAccessManager(this);

    QNetworkRequest nrRequest;
    nrRequest.setUrl(QUrl(sNamRequestUrl));
    nrRequest.setRawHeader("Client-ID", "fr5hyjbajq4ltuhr45vttxca79aap7");

    _nrReply = namManager->get(nrRequest);
    connect(_nrReply, SIGNAL(readyRead()), this, SLOT(readNamReply()));
    connect(_nrReply, SIGNAL(error(QNetworkReply::NetworkError)),this, SLOT(replyError(QNetworkReply::NetworkError)));
    connect(_nrReply, SIGNAL(sslErrors(QList<QSslError>)),this, SLOT(replySslErrors(QList<QSslError>)));
}

QStringList mainWindow::parseIrcMessage(const QString &sMsg)
{
    QStringList slParsed;
    QStringList slSeparators;
    QString sItem;
    short currentSeparator=0;
    slSeparators << ":" << "!" << "@" << " " << " " << ":" << "..";
    for (int iitm=0;iitm<sMsg.size();++iitm)
    {
        if (QString::compare(sMsg.at(iitm),slSeparators.at(currentSeparator)) == 0)
        {
            slParsed << sItem;
            sItem.clear();
            currentSeparator++;
            continue;
        }
        sItem.append(sMsg.at(iitm));
    }
    slParsed << sItem;

    return slParsed;
}

void mainWindow::parseRequestMessage(const QJsonObject &joData)
{
//    QJsonArray jsonArray = jsonObject.value("data").toArray();
    QVariantMap vmRoot = joData.toVariantMap();
//    qDebug() << "ROOT" << vmRoot;
    QVariantList vlData = vmRoot.value("data").toList();
    if (vlData.isEmpty()) {this->sendPrivmsg("Стримлер оффлаен");return;}
    QVariant vData = vlData.at(0);
//    qDebug() << "ROOTDATA" << vmRoot.value("data");
//    qDebug() << "DATA" << vData.toMap().value("started_at").toString();
    QString sTemp = vData.toMap().value("started_at").toString();
//    this->sendPrivmsg(QString("стрим начался в ").append(sTemp).append(""));
    sTemp.chop(1);
    QStringList slStart = sTemp.split("T");
    QDateTime dtStart(QDate(QDate::fromString(slStart.at(0),"yyyy-MM-dd")),QTime(QTime::fromString(slStart.at(1),"hh:mm:ss")));
    int itmp = dtStart.secsTo(QDateTime::currentDateTime());
    int jtmp = itmp/3600;
    sTemp.clear();
    sTemp.append(QString::number(jtmp-6)).append("ч");
    itmp = itmp-jtmp*3600;
    jtmp = itmp/60;
    sTemp.append(QString::number(jtmp)).append("м");
    itmp = itmp-jtmp*60;
    sTemp.append(QString::number(itmp)).append("с");
//    qDebug() << dtStart.secsTo(QDateTime::currentDateTime());
    this->sendPrivmsg(QString("с начала стрима прошло ").append(sTemp));
}

void mainWindow::executeCommand(const QStringList &slMessageData)
{
    QString sCommand = slMessageData.last().split(" ").at(0);
    if (QString::compare(sCommand,"!хрюшка")==0)
    {this->sendPrivmsg("(oo)");}
    else if (QString::compare(sCommand,"!димас")==0)
    {this->sendPrivmsg("dimaas63 Димас, где чат?");}
    else if (QString::compare(sCommand,"!followage")==0)
    {
        this->sendPrivmsg("в разработке");
//        _slNamRequest << "getstreamerid" << "getfollowerid" << "getfollowage";this->executeNamCommand(slMessageData);
    }
    else if (QString::compare(sCommand,"!минигусь")==0)
    {this->sendPrivmsg("(*)>");}
    else if (QString::compare(sCommand,"!гусь")==0)
    {this->sendPrivmsg(QString(slMessageData.at(1)).append(" Гуся те на весь чат захотелось? Убирать за ним ты будешь?"));}
    else if (QString::compare(sCommand,"!uptime")==0)
    {_slNamRequest << "getuptime";this->executeNamCommand(slMessageData);}
    else if (QString::compare(sCommand,"!poll")==0)
    {
        if (!bPollRelease) {this->sendPrivmsg("Одно голосование уже запущено");return;}
        if(QString::compare(slMessageData.at(1),"moarblues")==0 ||
                QString::compare(slMessageData.at(1),"multistrong999")==0 ||
                QString::compare(slMessageData.at(1),"empire_metal")==0 ||
                QString::compare(slMessageData.at(1),"gooha1")==0)
        {this->startPoll(slMessageData.last());}
        else
        {this->sendPrivmsg("Пока что только батя и избранные могут запускать голосовалки");return;}
    }
    else if (QString::compare(sCommand,"!v")==0)
    {
        if (bPollRelease) {this->sendPrivmsg("Никаких голосований сейчас не идёт");return;}
        QStringList slTemp = slMessageData.last().split(" ");
        if (slTemp.size()!=2) {this->sendPrivmsg("Ты втираешь мне какую-то дичь");return;}
        if (!slTemp.at(1).at(0).isDigit()) {this->sendPrivmsg("Ты втираешь мне какую-то дичь");return;}
        if (slTemp.at(1).toShort()>=_slPollData.size() || slTemp.at(1).toShort()<=0)
        {this->sendPrivmsg("Столько вариантов нет");return;}
        _slPollResults.append(slMessageData.last().split(" ").at(1));
    }
    else if (QString::compare(sCommand,"!startstories")==0)
    {
        if(QString::compare(slMessageData.at(1),"moarblues")==0 || QString::compare(slMessageData.at(1),"empire_metal")==0)
        {
            if (_tStory->isActive()) {this->sendPrivmsg("Истории уже работают");return;}
            _tStory->start(300000);
            this->getStory();
        }
        else {this->sendPrivmsg("Вы не наделены правом запускать истории KevinTurtle");return;}
    }
    else if (QString::compare(sCommand,"!stopstories")==0)
    {
        if(QString::compare(slMessageData.at(1),"moarblues")==0 || QString::compare(slMessageData.at(1),"empire_metal")==0)
        {
            if (!_tStory->isActive()) {this->sendPrivmsg("Истории и не работали");return;}
            _tStory->stop();
        }
        else {this->sendPrivmsg("Вы не наделены правом останавливать истории KevinTurtle");return;}
    }
    else if (QString::compare(sCommand,"!8")==0)
    {
        QStringList sl8Data = slMessageData.last().split(" ");
        if (sl8Data.size()!=2) {this->sendPrivmsg("Гадать надо на что-то (через пробел)");return;}

        qsrand((uint)QTime::currentTime().msec());
        short max=0;
        QString sEnding = "";
        if (QString::compare(sl8Data.at(1),"нос")==0) {max = 10;sEnding = "см WutFace";}
        else if (QString::compare(sl8Data.at(1),"писюн")==0) {max = 30;sEnding = "см PogChamp";}
        else if (QString::compare(sl8Data.at(1),"iq")==0) {max = 250;sEnding = " MingLee";}
        else if (QString::compare(sl8Data.at(1),"сиськи")==0) {max = 250;sEnding = "см в обхвате HotPokket";}
        if(max==0) {return;}
        QString sKey = QString::number(qrand() % ((max + 1) - 1) + 1);

        this->sendPrivmsg(QString().append("У ").append(slMessageData.at(1)).append(" ")
                          .append(sl8Data.at(1)).append(" ").append(sKey).append(sEnding));
    }
    else if (QString::compare(sCommand,"!код")==0)
    {this->sendPrivmsg("https://github.com/moarblues/moarbot-twitch");return;}
}

void mainWindow::executeNamCommand(const QStringList &slMessageData)
{
    if (_slNamRequest.isEmpty()) {_slNamData.clear(); return;}
    if (QString::compare(_slNamRequest.at(0),"getstreamerid")==0)
    {this->sendNamRequest(QString("https://api.twitch.tv/helix/users?login=").append(slMessageData.at(1)));}
    else if (QString::compare(_slNamRequest.at(0),"getfollowerid")==0)
    {this->sendNamRequest(QString("https://api.twitch.tv/helix/users?login=").append(slMessageData.at(1)));}
    else if (QString::compare(_slNamRequest.at(0),"getfollowage")==0)
    {this->sendNamRequest(QString("https://api.twitch.tv/helix/users?login=").append(slMessageData.at(1)));}
    else if (QString::compare(_slNamRequest.at(0),"getuptime")==0)
    {this->sendNamRequest(QString("https://api.twitch.tv/helix/streams?user_login=").append("moarblues"));}
}

void mainWindow::replyError(const QNetworkReply::NetworkError &neState)
{
    qDebug() << neState;
}

void mainWindow::replySslErrors(const QList<QSslError> &errList)
{
    qDebug() << errList;
}

void mainWindow::on_btn_connect_clicked()
{
    this->connectToServer();
}

void mainWindow::on_btn_join_clicked()
{
    _sChannel = this->ui->le_channel->text();
    this->sendMessage(QString("JOIN #").append(_sChannel).append("\r\n").toStdString().c_str());
    this->sendPrivmsg("HeyGuys");
}

void mainWindow::on_btn_sendprivmsg_clicked()
{
    this->sendPrivmsg(this->ui->te_privmsg->toPlainText());
    this->ui->te_privmsg->clear();
}

void mainWindow::on_btn_test_clicked()
{
//    this->executeCommand("!followage");
}
