#include"SignalManager.h"

SendSignal::SendSignal(QObject* parent) :QObject(parent) {
    connect(this, &SendSignal::ShowAddFriend_QS,        UiManager::GetInstance(), &UiManager::ShowAddFriend,        Qt::AutoConnection);
    connect(this, &SendSignal::DoLogIn_QS,              UiManager::GetInstance(), &UiManager::DoLogIn,              Qt::AutoConnection);
    connect(this, &SendSignal::DoAddFriend_QS,          UiManager::GetInstance(), &UiManager::DoAddFriend,          Qt::AutoConnection);
    connect(this, &SendSignal::DoSignUser_QS,           UiManager::GetInstance(), &UiManager::DoSignUser,           Qt::AutoConnection);
    connect(this, &SendSignal::DoRefreshFirendList_QS,  UiManager::GetInstance(), &UiManager::DoRefreshFirendList,  Qt::AutoConnection);
    connect(this, &SendSignal::DoRecvXQMessage_QS,      UiManager::GetInstance(), &UiManager::DoRecvXQMessage,      Qt::AutoConnection);
    connect(this, &SendSignal::DoUpdateUserInfo_QS,     UiManager::GetInstance(), &UiManager::DoUpdateUserInfo,      Qt::AutoConnection);
}


SendSignal* SendSignal::GetInstance() {
    static SendSignal* da = nullptr; 
    if (da == nullptr) {
        da = new SendSignal;
    }
    return da;
}
