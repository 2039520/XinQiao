#pragma once
#ifndef  SIGNAL_MANAGER_H
#define SIGNAL_MANAGER_H
#include <QtWidgets/QWidget>
#include <QMessageBox>
#include"../../Server/Server.h"
#include"../LogIn/LogIn.h"
#include"../Mnue/mnue.h"
#include"../Sign/sign.h"
#include"../AddFriend/addfirend.h"
#include"../Message/message.h"
#include"../../FileIo/FileIo.h"
#include"../Setting/setting.h"
#include <QCloseEvent>
class SendSignal :public QObject {
    Q_OBJECT
public:
    explicit  SendSignal(QObject* parent = nullptr);

//信号声明 
//——————————————————————————————————————
signals: void ShowAddFriend_QS(IN std::string userName, IN uint16_t userId);
signals: void DoLogIn_QS(PdChat206 * msg);
signals: void DoAddFriend_QS(PdChat206* msg);
signals: void DoSignUser_QS(PdChat206* msg);
signals: void DoRefreshFirendList_QS(PdChat206* msg);
signals: void DoRecvXQMessage_QS(PdChat206* msg);
signals: void DoUpdateUserInfo_QS(PdChat206* msg);
//———————————————————————————————————————

public:
    static SendSignal* GetInstance();
};
#endif // ! SIGNAL_MANAGER_H

