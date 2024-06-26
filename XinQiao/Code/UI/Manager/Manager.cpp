#include"Manager.h"

UiManager::UiManager(QObject* parent ) :QObject(parent), m_LogIn(nullptr), m_Mnue(nullptr), m_ActiveWindow(nullptr), m_id(0) {



}


void UiManager::ChangeWindow(QWidget* window) {
    if (m_ActiveWindow != nullptr) {
        m_ActiveWindow->close();
        m_ActiveWindow->deleteLater();
    }
    delete m_ActiveWindow;
    m_ActiveWindow = window;
    m_ActiveWindow->show();
    
}

void UiManager::JumpTo(QWidget* window) {
    m_WindowJumpHistory.push(m_ActiveWindow);
    m_ActiveWindow->hide();
    m_ActiveWindow = window;
    window->show();
}

void UiManager::JumpBack() {
    QWidget* window = m_WindowJumpHistory.top();
    window->show();
    m_ActiveWindow->close();
    m_ActiveWindow->deleteLater();
    delete m_ActiveWindow;
    m_ActiveWindow = window;
    m_WindowJumpHistory.pop();
}


void UiManager::OpenWin(QWidget* window) {
    window->show();
}

void UiManager::CloseWin(QWidget* window) {
    window->close();
    window->deleteLater();
    delete window;
    window = nullptr;
}

UiManager* UiManager:: GetInstance() {
    static UiManager* uiManager = nullptr;
    if (uiManager == nullptr) {
        uiManager = new UiManager;
    }
    return uiManager;
}



uint16_t UiManager::GetId() {
    return this->m_id;
}


void UiManager::SetId(uint16_t id) {
    this->m_id = id;
}

void UiManager::ShowAddFriend(std::string userName, uint16_t userId) {
    AddFirend* addfiend = new AddFirend(userName,userId);
    addfiend->show();
} 
Message* UiManager::AddMessageWindow(uint16_t id) {
    //如果窗口已经打开，就展示到已经打开的窗口
    Message* messageWin =  nullptr;
    uint i = 0;
    for (; i < m_Message.length; i++) {
        if (id == m_Message.data[i]->recvId) {
            messageWin = m_Message.data[i];
            messageWin->show();
            return messageWin;
        }
    }
    messageWin = new Message(id);
    m_Message.SqListInsert(m_Message.length, messageWin);
    messageWin->show();
    return messageWin;
}


void UiManager::DeleteMessageWindow(uint16_t id) {
    uint i = 0;
    for (; i < m_Message.length; i++) {
        if (id == m_Message.data[i]->recvId) {
            break;
        }
    }
    //Message* messageWin = nullptr;
    m_Message.SqListDelete(i);
    //if (nullptr != messageWin) {
    //    delete messageWin;
    //}
}


void UiManager::DoAddFriend(PdChat206* msg) {
    if (msg->m_value.addFriend.yn == 1) {
            ((Mnue*)m_Mnue)->RefreshFirendList();
        QMessageBox::information(nullptr, "Remind",(std::to_string(msg->m_value.addFriend.srcId)+" has agreed to add you as a friend !").c_str());
    }
    else if (msg->m_value.addFriend.yn == 2) {
        ShowAddFriend(std::to_string(msg->m_value.addFriend.srcId), msg->m_value.addFriend.srcId);
    }
    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}

void UiManager::DoLogIn(PdChat206* msg) {
    if (msg->m_value.control.yn == 1) {
        uint temp= m_LogIn->ui.UserIdEdit->text().toUInt();
        SetId(temp);
        Mnue* mnue = new Mnue();
        m_Mnue = mnue;
        ChangeWindow(mnue);
        return;
    }
    else if (msg->m_value.control.yn == 0) {
        QMessageBox::information(nullptr, "error", "Incorrect account or password");
    }
    else {
        QMessageBox::information(nullptr, "error", "The network is not connected or the server is not online");
    }
    Transfer::GetInstance()->CloseServer();
    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}

void UiManager::DoSignUser(PdChat206* msg) {
    if (msg->m_value.sign.userNameLength == 1) {
        m_id = msg->m_value.sign.id;
        QMessageBox::information(nullptr, "Usser ID", (std::string("Your successfully registered ID is:") + std::to_string(m_id) + ". Please note this ID to log in to XinQiao").c_str());
    }
    else if (msg->m_value.sign.userNameLength == 0) {
        QMessageBox::information(nullptr, "err", "Username is already taken");
    }
    Transfer::GetInstance()->CloseServer();
    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}


void UiManager::DoRefreshFirendList(PdChat206* msg) {
    if (m_Mnue->m_FriendListInfo != nullptr) {
        free(m_Mnue->m_FriendListInfo);
        m_Mnue->m_FriendListInfo = nullptr;
    }
    GetFriendsList_M(*msg, m_Mnue->m_FriendListInfo, m_Mnue->m_FriendListLength);
    if (m_Mnue->m_FriendListItems != nullptr) {
        delete [] m_Mnue->m_FriendListItems;
        m_Mnue->m_FriendListItems = nullptr;
    }
    m_Mnue->m_FriendListModel->clear();
    m_Mnue->m_FriendListItems = new QStandardItem[m_Mnue->m_FriendListLength];
    for (uint i = 0; i < m_Mnue->m_FriendListLength; i++) {
        m_Mnue->m_FriendListItems[i].setText(std::string((const char*)m_Mnue->m_FriendListInfo[i].userName, m_Mnue->m_FriendListInfo[i].userNameLength).c_str());
        m_Mnue->m_FriendListModel->appendRow(&m_Mnue->m_FriendListItems[i]);
    }
    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}

void UiManager::DoRecvXQMessage(PdChat206* msg) {
    uint16_t sendId = msg->m_value.messageData.sendId;
    Message * win = AddMessageWindow(sendId);
    win->AddRecvMessage(std::string((char*)msg->m_value.messageData.content, msg->m_value.messageData.contentLen), msg->m_value.messageData.time);
    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}

void UiManager::DoUpdateUserInfo(PdChat206* msg) {
    if (m_Mnue != nullptr) {
        m_Mnue->UpdateUserInfo(msg->m_value.UserInfo);
    }
    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}
