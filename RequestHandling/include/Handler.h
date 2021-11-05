//Copyright 2021 byteihq <kotov038@gmail.com>

#ifndef TCPCLIENT_HANDLER_H
#define TCPCLIENT_HANDLER_H

#include <string>
#include <QObject>
#include <map>
#include <QString>
#include <nlohmann/json.hpp>
#include <NetworkCommunication.h>

class Handler : public QObject {
Q_OBJECT
private:
    void showErrMsg(const std::string& msg);
public:
    typedef std::map<std::string, std::string> StringMap;
signals:

    void authorizeFailed();

    void authorizeSucceed();

    void unknownStatus();

    void newMsg(const QString& sender, const QString& msg, const QString& status);

    void newUserMsg(const QString& sender, const QString& msg);

    void users(const Handler::StringMap& );

    void newInvite(const QString& userName);

    void startServer();

    void startClient(const QString& ip);

    void connectionAbort();

    void sendAllFiles();

    void setAllFiles(const QList<QString>& paths, const QList<QString>& names);

public:
    Handler() = default;

    void request(const std::string &request);

    nlohmann::json reply(const std::string& sender, const std::string &reply, Requests requestType);
};

#endif //TCPCLIENT_HANDLER_H
