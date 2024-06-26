#include"main.h"
#include <QGraphicsDropShadowEffect>
XQControl g_Control;
int StartUi(int argc, char* argv[]) {
    QApplication a(argc, argv);
    // 设置编码
    g_Control.m_WindowManager->m_LogIn = new XinQiao();
    g_Control.m_WindowManager->ChangeWindow(g_Control.m_WindowManager->m_LogIn);
    return a.exec();
}


void ServiceThread() {
    g_Control.m_Transfer = Transfer::GetInstance();
    if (g_Control.m_Transfer == nullptr) {
        //[zp!]连接服务器失败
    }
    else {
        //[zp!]连接服务器成功
    }
	
}

void SendThread() {
    PdChat206 msg;
    while (true) {
        std::unique_lock<std::mutex> lock(g_Control.mtx); // 创建互斥锁的独占封装
        g_Control.cv.wait(lock, [] { return g_Control.notified; }); // 等待通知，同时释放锁

        LOG << "send massage" << std::endl;
        uint16_t length = g_Control.m_Transfer->m_PdMessageQueue.size();
        for (uint i = 0; i < length; i++) {
            g_Control.m_Transfer->GetPdMessage(msg);
            g_Control.m_Transfer->SendPdMessage(msg);
        }
        g_Control.notified = false;
    }
}

void ExecuteThread() {
    while (1) {
        PdChat206 *msg = new PdChat206;
        g_Control.m_Transfer->GetRecvPdMsg(*msg);
        switch (msg->m_MOrC) {
        case ADD_FRIEND:
            emit SendSignal::GetInstance()->DoAddFriend_QS(msg);
            break;
        case LOGIN:
            emit SendSignal::GetInstance()->DoLogIn_QS(msg);
            break;
        case FRIEND:
            emit SendSignal::GetInstance()->DoRefreshFirendList_QS(msg);
            break;
        case SIGN:
            emit SendSignal::GetInstance()->DoSignUser_QS(msg);
            break;
        case MESSAGE:
            emit SendSignal::GetInstance()->DoRecvXQMessage_QS(msg);
            break;
        case USER_INFO:
            emit SendSignal::GetInstance()->DoUpdateUserInfo_QS(msg);
            break;
        }
    }
}

void ReceiveThread() {
    while (1) {
        //[注意]：msg是需要在各个处理信息函数处理完后释放内存
        g_Control.m_Transfer->RecvPdMessage();
    }
}


XQControl* XQControl::GetInstance() {
    return &g_Control;
}
int main(int argc, char* argv[]) {
    std::thread serviceThread(ServiceThread);
    serviceThread.join();
    g_Control.m_WindowManager = UiManager::GetInstance();
    std::thread sendThread(SendThread);
    std::thread receiveThread(ReceiveThread);
    std::thread executeThread(ExecuteThread);
    sendThread.detach();
    receiveThread.detach();
    executeThread.detach();
    return StartUi(argc, argv);
}

void XQControl::NotifiedSendMassage() {
  notified = true; // 设置通知标志为 true
  cv.notify_one(); // 发送通知给一个等待线程
}


XQControl::XQControl() {

}

