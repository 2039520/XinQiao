#pragma once
#ifndef MANAGER_H
#define MANAGER_H
#include "../SignalManager/SignalManager.h"
#include "../../PdSdk/SqList.h"
class XinQiao;
class Mnue;
class Message;

class UiManager:public QObject {
public:
    UiManager(QObject* parent = nullptr);

public:
    XinQiao* m_LogIn;
    Mnue* m_Mnue;
    QWidget* m_ActiveWindow;
    SqList<Message*>m_Message;
    uint16_t m_id;
    std::stack< QWidget* >m_WindowJumpHistory;
public:
    static UiManager* GetInstance();
    void ChangeWindow(QWidget* window);
    void JumpTo(QWidget* window);
    void JumpBack();
    void OpenWin(QWidget* window);
    void CloseWin(QWidget* window);
    uint16_t GetId();
    void SetId(uint16_t id);
    Message* AddMessageWindow(uint16_t id);
    void DeleteMessageWindow(uint16_t id);

    friend class SendSignal;
private slots:
    //外部信号触发函数
    //————————————————————————————————
    void ShowAddFriend(IN std::string userName, IN uint16_t userId);
    void DoLogIn(PdChat206* msg);
    void DoAddFriend(PdChat206* msg);
    void DoSignUser(PdChat206* msg);
    void DoRefreshFirendList(PdChat206* msg);
    void DoRecvXQMessage(PdChat206* msg);
    void DoUpdateUserInfo(PdChat206* msg);
    //————————————————————————————————
};
#endif // !MANAGER_H


