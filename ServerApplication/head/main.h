#pragma once
#ifndef TCP_HTTP_MAIN_H
#define TCP_HTTP_MAIN_H
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <queue>
#include <vector>
#include <semaphore.h>
#include "DataBase/DataBase.h"
#include "FileIO/FileIo.h"
#include <semaphore.h>
using namespace  std;
#define USER_INFO_PATHR "/code/ServerApplication/UserInfo/"
#define USER_INFO_PROFILEPICTURE_PATH_NAME "ProfilePicture/image.jpg"

//阻塞PV
#define P    pthread_mutex_lock
#define V    pthread_mutex_unlock
//信号量
#define WAIT    sem_wait 
#define POST    sem_post 

//非阻塞PV
#define TP(mutex)    sem_trywait(&mutex)
#define TV(mutex)    sem_post(&mutex)

#define IN
#define OUT
#define STR_END_LEN     1
#define BIT_SIZE        8
#define LISTENPROT      2039
#define MAX_CLIENT_NUMBER      128
#define FALSE           0
#define TRUE            1
#define HEART_PKG_TIME  300
#define HEAET_PKG_MAXTIME 600
#define INVALID_SOCKET 0
#define ONE_PD_QUEUE_SIZE  20
#define CHECK_CALL(val)                     \
    do {                                    \
        if ((val) < 0) {                    \
            ErrorPrint(__FILE__, __LINE__); \
            exit(1);                        \
        }                                   \
    } while (0)

#define CHECK_RETRUN(val)                   \
    if(val==false){                         \
        ErrorPrint(__FILE__, __LINE__);     \
        return false;                       \
    }
#define MOCK

void* AcceptThread(void* param);
void* ExecuteThread(void* param);
void* SendThread(void* param);
void HeartDetectionTimer(int single);

struct SendPd_t {
    PdChat206 msg;
    uint64_t send = 0;
    uint64_t needSend = 0;
    vector<byte> buf;
    SendPd_t(PdChat206 value):msg(value){
    }
    SendPd_t(){}
};

struct SendPdQueue_t {
   queue<SendPd_t> sendPd;
   pthread_mutex_t  mtx = PTHREAD_MUTEX_INITIALIZER;
   sem_t sem;
   SendPdQueue_t();
   ~SendPdQueue_t() {
       pthread_mutex_destroy(&mtx);
       sem_destroy(&sem);
   }
};

class UserListMgt {
private:
    std::vector<int> m_fd; //安装id序列对应存放fd
    vector<SendPdQueue_t> m_PdList;
public:
    friend void* AcceptThread(void* param);
    friend void* ExecuteThread(void* param);
    friend void* SendThread(void* param);
    friend void HeartDetectionTimer(int single);
public:
    UserListMgt();
private:
    uint16_t IdtoIdIndex(IN uint16_t id);
    uint16_t FdtoId(IN int fd);
public:
    //添加协议消息
    PV void AddSendPd(IN SendPd_t messageData);
    //获取id对应套接字
    void GetFd(IN uint16_t id, OUT int& fd);
    //更新id对应套接字
    void FdUpdate(IN uint16_t id, IN int fd);
    //登录id以及保存对应套接字
    void RegistrationId(IN int fd,IN uint16_t id);
    //保存登录失败
    //注销Id
    void LogOffId(IN uint16_t id);
    //获取一个消息
    PV void GetPmessage(IN uint64_t num, OUT int& fd, OUT SendPd_t& message);
    //关闭id对应套接字
    void LogOut(IN uint16_t id);

};

struct Thread {
    void* (*function)(void* param);
    void* param;
};

class ThreadMgt {
public:
    ThreadMgt();
    Thread  m_AcceptThread;
    pthread_t m_AcceptThreadFd;
    Thread  m_SendThread;
    pthread_t m_SendThreadFd;
    Thread  m_ExecuteThread;
    pthread_t m_ExecuteThreadFd;
    Thread  m_RecvThread;
    pthread_t m_RecvThreadFd;

};

class InitInfo {
public:
    InitInfo();
    uint   m_ListenProt;
    uint   m_MaxClientNumber;
};

struct MessageLength_t
{
    int fd = 0;
    uint64_t needRecv = 0;
    uint64_t recv = 0;
    byte* recvBuffer = nullptr;
};

class RecvMessage {
public:
    PdChat206 m_PdChat;
    int m_Fd;
public:
    RecvMessage(PdChat206 pdChat,int fd) :m_PdChat(pdChat),m_Fd(fd){}
};

class Control {
public:
    friend void* AcceptThread(void* param);
    friend void* ExecuteThread(void* param);
    friend void* SendThread(void* param);
    friend void* RecvThread(void* param);
    friend void HeartDetectionTimer(int single);
public:
    Control();
    queue<RecvMessage> m_RecvMessageQueue;
    UserListMgt   m_QueueMgt;
    InitInfo   m_InitInfo;
    pthread_mutex_t    m_printPv;
    int     m_SocketListen; //初始化后 开始监听
    int     m_ListenEpoll;
    int     m_ClientEpoll;
    uint64_t m_RecvLength;
    uint64_t m_NeedRecvLength;
    uint64_t m_SendLength;
    uint64_t m_SendedLength;


}g_Control;

timer_t timerid;


void SetFlashMode();
void ErrorPrint(const Str file, int line);

void* RecvThread(void* param);
#endif //TCP_HTTP_MAIN_H



