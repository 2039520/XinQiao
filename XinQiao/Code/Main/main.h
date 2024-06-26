#pragma once
#ifndef MAIN_ZP
#define MAIN_ZP
#include "../UI/Manager/Manager.h"
#include <QtWidgets/QApplication>
#include <mutex>
#include <condition_variable>
#define MAX_RECV_PDMSG_SIZE   10000
#endif // MAIN_ZP

class XQControl {
public:
    Transfer* m_Transfer;
    UiManager* m_WindowManager;
public:
    //窟僕�潦鵡卞霓慟�
    //！！！！！！！！！！！！！！
    std::mutex mtx;
    std::condition_variable cv;
    bool notified = false;
    std::counting_semaphore<MAX_RECV_PDMSG_SIZE> m_Semaphore{0};
    //！！！！！！！！！！！！！！

public:
    static XQControl* GetInstance();
    XQControl();
public:
    void NotifiedSendMassage();
};




int StartUi(int argc, char* argv[]);

void ServiceThread();

void SendThread();

void ReceiveThread();

int main(int argc, char* argv[]);