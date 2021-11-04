// Copyright 2021 byteihq <kotov038@gmail.com>

#include <UserConversation.h>
#include <QMessageBox>

UserConversation::UserConversation(QWidget * parent, std::shared_ptr < QThread > clientThread,
                                   std::shared_ptr < QThread > serverThread,
const std::shared_ptr <boost::asio::io_service> &ioService_, Mode
mode) :
Resizable(parent,
640, 480),
clientConnection_ (std::make_unique<ClientConnection>()),
serverConnection_(std::make_unique<Server>(ioService_)),
clientThread_(
        std::move(
        clientThread)),
serverThread_(
        std::move(
        serverThread)),
qgrid_(std::make_unique<QGridLayout>(
        this)),
leftList_(
        std::make_unique<QListWidget>()),
rightList_(
        std::make_unique<QListWidget>()),
progress_(std::make_unique<QProgressBar>()),
lineEdit_(
        std::make_unique<QLineEdit>()),
sendBtn_(
        std::make_unique<QPushButton>(
                "Send",
                this)),
disconnectBtn_(
        std::make_unique<QPushButton>(
                "Disconnect",
                this)), msgNumber_(0) {

    if (mode == Mode::Dark) {
        StyleSettings::setDarkMode(this);
    } else {
        StyleSettings::setLightMode(this);
    }

    qgrid_->addWidget(leftList_.get(), 0, 0, 3, 4);
    qgrid_->addWidget(rightList_.get(), 0, 4, 3, 4);
    qgrid_->addWidget(progress_.get(), 3, 0, 1, 4);
    qgrid_->addWidget(lineEdit_.get(), 3, 4, 1, 3);
    qgrid_->addWidget(sendBtn_.get(), 3, 7);
    qgrid_->addWidget(disconnectBtn_.get(), 4, 0, 1, 8);

    connect(sendBtn_.get(), &QPushButton::clicked, this, &UserConversation::sendBtnClicked);

}

void UserConversation::sendBtnClicked() {
    if (lineEdit_->text().isEmpty()) {
        return;
    }
    ++msgNumber_;
    switch (connectionMode_) {
        case ServerMode:
            serverConnection_->sendMessage(serverConnection_->handler_->reply(sender_, lineEdit_->text().toStdString(), Requests::Msg));
            break;
        case ClientMode:
            clientConnection_->sendMessage(clientConnection_->handler_->reply(sender_, lineEdit_->text().toStdString(), Requests::Msg));
            break;
    }
    rightList_->addItem(QString::fromStdString(sender_) + ": " + lineEdit_->text());
    rightList_->item(msgNumber_ - 1)->setBackgroundColor(QColor(60, 100, 100));
    lineEdit_->clear();

}

void UserConversation::setSender(const std::string &sender) {
    sender_ = sender;
}

void UserConversation::setDarkMode() {
    StyleSettings::setDarkMode(this);
}

void UserConversation::setLightMode() {
    StyleSettings::setLightMode(this);
}

void UserConversation::startClient(const QString &ip) {
    connectionMode_ = ConnectionMode::ClientMode;
    clientConnection_->moveToThread(clientThread_.get());
    clientThread_->start();
    auto status = clientConnection_->Connect(ip.toStdString(), 2002);
    if (status == -2) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Server not responding");
        msgBox.exec();
        return;
    }
    if (status == -1) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("Error");
        msgBox.setText("Ip is wrong");
        msgBox.exec();
        return;
    }
    clientConnection_->listen();
}

void UserConversation::startServer() {
    connectionMode_ = ConnectionMode::ServerMode;
    serverConnection_->moveToThread(serverThread_.get());
    serverThread_->start();
    serverConnection_->accept();
    serverConnection_->listen();
}
