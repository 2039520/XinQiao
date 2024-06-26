#pragma once
#include "head/main.h"
void SetFlashMode() {
    fflush(stdout);
    setvbuf(stdout, NULL, _IONBF, 0);//设置标准输出为无缓冲模式，立即显示数据
}

void ErrorPrint(const Str file, int line) {
    Str err = strerror(errno); //暂时并发量不够高，没必要使用线程独享errno数据
    Str tempFile = strrchr(file, '/');
    P(&g_Control.m_printPv);
    printf("[%s:%d]%s", tempFile + 1, line, err);
    V(&g_Control.m_printPv);
}


UserListMgt::UserListMgt() {
    m_PdList.emplace_back();
    m_fd.resize(0xffff);
    //查询数据库，有哪些信息没发送，如果在线就添加到发送队列
}

void UserListMgt::AddSendPd(IN SendPd_t messageData) {
    int i = 0;
    for (; i < m_PdList.size();i++) {
        SendPdQueue_t& value = m_PdList[i];
        if (value.sendPd.size() < ONE_PD_QUEUE_SIZE) {
            P(&value.mtx);
            value.sendPd.push(messageData);
            POST(&value.sem);
            V(&value.mtx);
            break;
        }
    }
    if (i == m_PdList.size()) {
        m_PdList.resize(m_PdList.size() + 1);
        P(&m_PdList[i].mtx);
        m_PdList[i].sendPd.push(messageData);
        POST(&m_PdList[i].sem);
        V(&m_PdList[i].mtx);
    }
}

void UserListMgt::GetFd(IN uint16_t id, OUT int& fd) {
    fd = m_fd[IdtoIdIndex(id)];
}

void UserListMgt::FdUpdate(IN uint16_t id, IN int fd) {
    m_fd[IdtoIdIndex(id)] = fd;
}

void UserListMgt::RegistrationId(IN int fd,IN uint16_t id) {
    LOG << id << " Sign" << endl;
    m_fd[IdtoIdIndex(id)] = fd;
}

void UserListMgt::LogOffId(IN uint16_t idIndex) {
    //[!]更新数据库
}

void UserListMgt::LogOut(IN uint16_t id) {
    printf("%d LogOut\n", id);
    m_fd[IdtoIdIndex(id)] = INVALID_SOCKET;
}

uint16_t UserListMgt::IdtoIdIndex(IN uint16_t id){
    return id;
}

uint16_t UserListMgt::FdtoId(IN int fd) {
    return fd;
}

PV void UserListMgt::GetPmessage(IN uint64_t num, OUT int& fd, OUT SendPd_t& message) {
    SendPdQueue_t& value = m_PdList[num];
    WAIT(&value.sem);
    P(&value.mtx);
    if (value.sendPd.empty()) {
        LOG << "pdQueue Err" << endl;
        exit(1);
    }
    message = value.sendPd.front();
    value.sendPd.pop();
    V(&value.mtx);
    fd = m_fd[IdtoIdIndex(message.msg.GetRecvId())];
}

ThreadMgt::ThreadMgt() {
    m_AcceptThread.function = &AcceptThread;
    m_AcceptThread.param = NULL;
    m_ExecuteThread.function = &ExecuteThread;
    m_ExecuteThread.param = NULL;
    m_SendThread.function = &SendThread;
    m_SendThread.param = NULL;
    m_RecvThread.function = &RecvThread;
    m_RecvThread.param = NULL;

}

InitInfo::InitInfo() {
    m_ListenProt = LISTENPROT;
    m_MaxClientNumber = MAX_CLIENT_NUMBER;
}

Control::Control() {
    m_RecvLength = 0;
    m_NeedRecvLength = 0;
    m_SendLength = 0;
    m_SendedLength = 0;
    struct sockaddr_in local;
    int result;
    int prama = 1;
    memset(&local, 0, sizeof(local));


    m_printPv = PTHREAD_MUTEX_INITIALIZER;
    m_ListenEpoll = epoll_create(m_InitInfo.m_MaxClientNumber);
    CHECK_CALL(m_ListenEpoll);
    m_ClientEpoll = epoll_create(m_InitInfo.m_MaxClientNumber);
    CHECK_CALL(m_ClientEpoll);
    m_SocketListen = socket(AF_INET, SOCK_STREAM, NULL);
    CHECK_CALL(m_SocketListen);
    result = ioctl(m_SocketListen, FIONBIO, &prama);   // 将套接字设置为非阻塞模式
    CHECK_CALL(result);
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);
    local.sin_port = htons(m_InitInfo.m_ListenProt);

    result = setsockopt(m_SocketListen, SOL_SOCKET, SO_REUSEADDR, (char*)&prama, sizeof(prama));
    CHECK_CALL(result);
    result = bind(m_SocketListen, (struct sockaddr*)&local, sizeof(local));
    CHECK_CALL(result);
    result = listen(m_SocketListen, m_InitInfo.m_MaxClientNumber);

}
bool LogOnVerification(PdChat206 chatData) {
    uint16_t pw = DataBase::GetInstance()->GetPw(chatData.m_value.control.id);
    if (pw == chatData.m_value.control.pw)
        return true;
    return false;
}

void OnceSendPdMessage(PdChat206& chatData,int fd) {
    byte* data = nullptr;
    uint64_t length;
    chatData.CreatBinStream(data, &length);
    CreatPdMessage(data, length);
    int result = send(fd, data, length, MSG_NOSIGNAL);
    epoll_ctl(g_Control.m_ClientEpoll, EPOLL_CTL_DEL, fd, NULL);
    close(fd);
}

void MessageProcess(PdChat206 chatData) {
    g_Control.m_QueueMgt.AddSendPd(chatData);
}

void ControlProcess(PdChat206 chatData, int fd) {
    if (LogOnVerification(chatData)) {
        chatData.m_value.control.yn = 1;
        g_Control.m_QueueMgt.RegistrationId(fd, chatData.m_value.control.id);
        //处理积压的信息
        PdChat206* msg = nullptr;
        g_Control.m_QueueMgt.AddSendPd(chatData);
        uint64_t len = 0;
        DataBase::GetInstance()->GetMessage(msg, chatData.GetRecvId(),len);
        for (uint i = 0; i < len; i++) {
            g_Control.m_QueueMgt.AddSendPd(msg[i]);
        }
    }
    else {
        chatData.m_value.control.yn = 0;
        OnceSendPdMessage(chatData, fd);
    }
    
}
void SignProcess(PdChat206& chatData, int clientSocket) {
    string userName((char*)chatData.m_value.sign.userName, chatData.m_value.sign.userNameLength);
    bool ret = DataBase::GetInstance()->RegistUser(chatData.m_value.sign.id, chatData.m_value.sign.pw, userName);
    if (ret) {
        chatData.m_value.sign.userNameLength = 1;
    }
    else {
        chatData.m_value.sign.userNameLength = 0;
    }
    OnceSendPdMessage(chatData, clientSocket);
}

void FriendProcess(PdChat206& chatData) {
    if (chatData.m_value.friendData.length == 0) {//请求好友信息
        FriendInfo_t* friendInfo = nullptr;
        uint64_t length;
        DataBase::GetInstance()->GetFriendList_M(friendInfo, chatData.m_value.friendData.srId, length);
        chatData.SetFriend_F(friendInfo, length, chatData.m_value.friendData.srId);
        g_Control.m_QueueMgt.AddSendPd(chatData);
    }
    else {//加好友
    
    
    }
}

void* AcceptThread(void* param) {
    sockaddr_in addr;
    socklen_t  len = sizeof(addr);
    int result;
    epoll_event ev;
    int ClientSocket;
    epoll_event clientEv;
    int temp = 1;
    memset(&clientEv, 0, sizeof(clientEv));
    while (1) {
        //针对监听端口的可读事件触发
        result = epoll_wait(g_Control.m_ListenEpoll, &ev, g_Control.m_InitInfo.m_MaxClientNumber, -1);
        if (result < 0 && errno == EINTR) {
            continue;
        }
        while (1) {
            ClientSocket = accept(g_Control.m_SocketListen, (sockaddr*)&addr, (socklen_t*)&len);
            if (ClientSocket < 0)
                break;
            result = ioctl(ClientSocket, FIONBIO, &temp);
            CHECK_CALL(result);
            clientEv.events = EPOLLIN | EPOLLET  | EPOLLHUP;
            MessageLength_t* send = new MessageLength_t;
            clientEv.data.ptr = (void*)send;
            LOG << clientEv.data.ptr << endl;
            send->fd = ClientSocket;
            send->recvBuffer = (byte*)malloc(ONCE_MAX_CHATDATA_PKG_SIZE);
            result = epoll_ctl(g_Control.m_ClientEpoll, EPOLL_CTL_ADD, ClientSocket, &clientEv);
            CHECK_CALL(result);
        }
    }
    free(&clientEv);
}

void AddFriendProcess(PdChat206 &chatData) {
    if (1 == chatData.m_value.addFriend.yn ) {
        DataBase::GetInstance()->AddFriend(chatData.m_value.addFriend.addId, chatData.m_value.addFriend.srcId);
        //向同意方发送更新好友列表命令
        FriendInfo_t* friendInfo = NULL;
        uint64_t length;
        PdChat206 message;
        DataBase::GetInstance()->GetFriendList_M(friendInfo, chatData.m_value.addFriend.srcId, length);
        message.SetFriend_F(friendInfo, length, chatData.m_value.addFriend.srcId);
        g_Control.m_QueueMgt.AddSendPd(message);
    }
    else if (2 == chatData.m_value.addFriend.yn) {
    
    }
    g_Control.m_QueueMgt.AddSendPd(chatData);
}

void UserInfoProcess(PdChat206& chatData) {
    if (0 == chatData.m_value.UserInfo.control) {
        //更新除了头像的数据
        DataBase::GetInstance()->GetUserInfo(chatData.m_value.UserInfo);
        //更新头像数据
        std::string buffer;
        //[!]改为项目文件相对路径
        ReadFile(USER_INFO_PATHR + to_string(chatData.m_value.UserInfo.id) + "/" + USER_INFO_PROFILEPICTURE_PATH_NAME, buffer, std::ios::binary);
        chatData.m_value.UserInfo.UpdateProfilePicture(buffer.length(), (const byte *)buffer.c_str());
        g_Control.m_QueueMgt.AddSendPd(chatData);
    }
    else if (1 == chatData.m_value.UserInfo.control) {
        DataBase::GetInstance()->SetUserInfo(chatData.m_value.UserInfo);
        std::string buffer = chatData.m_value.UserInfo.GetProfilePicture();
        WriteFile(USER_INFO_PATHR + to_string(chatData.m_value.UserInfo.id)+ "/" + USER_INFO_PROFILEPICTURE_PATH_NAME, buffer, std::ios::binary | std::ios::out);
    }
}
void* ExecuteThread(void* param) {
    while (1) {
        while (g_Control.m_RecvMessageQueue.empty()==false) {
            RecvMessage data = g_Control.m_RecvMessageQueue.front();
            g_Control.m_RecvMessageQueue.pop();
            PdChat206 chatData = data.m_PdChat;
            int a = chatData.m_MOrC;
            LOG << "Execute message type:" << a << endl;
            switch (chatData.m_MOrC)
            {
            case MESSAGE:
                MessageProcess(chatData);
                break;
            case LOGIN:
                ControlProcess(chatData, data.m_Fd);
                break;
            case SIGN:
                SignProcess(chatData, data.m_Fd);
                break;
            case FRIEND:
                FriendProcess(chatData);
                break;
            case ADD_FRIEND:
                AddFriendProcess(chatData);
                break;
            case USER_INFO:
                UserInfoProcess(chatData);
            default:
                break;
            }
        }
    }
}

FREE void FreeEpollEventParam(epoll_event& clientEv) {
    if (clientEv.data.ptr != nullptr) {
        if (((MessageLength_t*)clientEv.data.ptr)->recvBuffer != nullptr) {
            free(((MessageLength_t*)clientEv.data.ptr)->recvBuffer);
            ((MessageLength_t*)clientEv.data.ptr)->recvBuffer = nullptr;
        }
        delete clientEv.data.ptr;
        clientEv.data.ptr = nullptr;
    }
}

void* RecvThread(void* param) {
    epoll_event* ev = (epoll_event*)malloc(sizeof(epoll_event) * g_Control.m_InitInfo.m_MaxClientNumber);
    while (1) {
        int epollNumber = epoll_wait(g_Control.m_ClientEpoll, ev, g_Control.m_InitInfo.m_MaxClientNumber, -1);
        for (int i = 0; i < epollNumber; i++) {
            if (ev[i].events & EPOLLHUP) {
                int fd = ((MessageLength_t*)ev[i].data.ptr)->fd;
                epoll_ctl(g_Control.m_ClientEpoll, EPOLL_CTL_DEL, fd, NULL);
                shutdown(fd, SHUT_RDWR);
                LOG << "Free fd:" << ((MessageLength_t*)ev[i].data.ptr)->fd << "data" << endl;
                FreeEpollEventParam(ev[i]);
                continue;
            }
            if (ev[i].events & EPOLLIN) {
                int clientSocket = ((MessageLength_t*)ev[i].data.ptr)->fd;
                uint64_t& recvLen = ((MessageLength_t*)ev[i].data.ptr)->recv;
                uint64_t& needRecvLen = ((MessageLength_t*)ev[i].data.ptr)->needRecv;
                byte*& buffer = ((MessageLength_t*)ev[i].data.ptr)->recvBuffer;
                uint64_t bufferLen = needRecvLen - recvLen;
                if (bufferLen == 0) {
                    bufferLen = ONCE_MAX_CHATDATA_PKG_SIZE;
                }
                int result = recv(clientSocket, buffer+ recvLen, bufferLen, NULL);
                if (result == -1) {
                    std::cerr << "Recv failed: " << strerror(errno) << std::endl;
                    continue;
                }
                if (needRecvLen == 0) {
                    needRecvLen = *(uint64_t*)buffer;
                    if (needRecvLen > ONCE_MAX_CHATDATA_PKG_SIZE) {
                        buffer = (byte*)realloc(buffer, needRecvLen);
                    }
                }
                if (result == needRecvLen - recvLen) {
                    PdChat206 pdChat(buffer + 8);
                    int temp = pdChat.m_MOrC;
                    LOG << "Recv massage type:" << temp << endl;
                    RecvMessage recvMessage(pdChat, clientSocket);
                    g_Control.m_RecvMessageQueue.push(recvMessage);
                    needRecvLen = 0;
                    recvLen = 0;
                }
                else if (result > needRecvLen - recvLen) {
                    do {
                        PdChat206 pdChat(buffer + 8);
                        int temp = pdChat.m_MOrC;
                        LOG << "Recv massage type:" << temp << endl;
                        g_Control.m_RecvMessageQueue.push(RecvMessage(pdChat, clientSocket));
                        memcpy(buffer, buffer + needRecvLen, result - (needRecvLen - recvLen));
                        result = result - (needRecvLen - recvLen);
                        recvLen = 0;
                        if (result == 0) {
                            needRecvLen = 0;
                            continue;
                        }
                        needRecvLen = *(uint64_t*)(buffer);
                    }while (result >= needRecvLen);
                    if (result > 0) {
                        recvLen = result;
                    }
                }
                else {
                    recvLen += result;
                }
            }
        }
    }
}

void* SendThread(void* param) {
    while (1) {
        int sendFd = 0;
        SendPd_t sendPd;
        g_Control.m_QueueMgt.GetPmessage(0, sendFd, sendPd);
        if (sendFd == INVALID_SOCKET) {
            DataBase::GetInstance()->SaveMessage(sendPd.msg);
            continue;
        }
        PdChat206 Pmessage = sendPd.msg;
        byte* data = sendPd.buf.data();
        uint64_t& sendLen = sendPd.send;
        uint64_t& needSendLen = sendPd.needSend;
        uint64_t length = sendPd.buf.size();
        if (needSendLen == 0) {
            Pmessage.CreatBinStream(data, &length);
            CreatPdMessage(data, length);
            sendPd.buf.assign(data, data + length);
            needSendLen = length;
            free(data);
            data = sendPd.buf.data();
        }
        int temp = Pmessage.m_MOrC;
        LOG << "reading send massage  type:" << temp << endl;
    A:
        int result = send(sendFd, data + sendLen, length - sendLen, NULL);
        if (result == -1) {
            LOG << "Send failed: " << strerror(errno) << errno <<std::endl;
            if (errno == EAGAIN) {
                g_Control.m_QueueMgt.AddSendPd(sendPd);
                continue;
            }
            if (sendPd.msg.m_MOrC == MESSAGE) {
                DataBase::GetInstance()->SaveMessage(sendPd.msg);
            }
            g_Control.m_QueueMgt.LogOut(Pmessage.GetRecvId());
            continue;
        }
        sendLen += result;
        if (sendLen != needSendLen) {
            goto A;
        }
        LOG << "send massage successed  type:" << temp << endl;
    }
}

void HeartDetectionTimer(int single) {
    //停止定时器
    //————————————————————
    struct itimerspec its;
    its.it_value.tv_sec = 0;
    its.it_value.tv_nsec = 0;
    timer_settime(timerid, 0, &its, NULL);
    //————————————————————


    for (uint i = 0; i < g_Control.m_QueueMgt.m_fd.size(); i++) {
        int& fd = g_Control.m_QueueMgt.m_fd[i];
        if (fd == INVALID_SOCKET)
            continue;
        PdChat206 pdMsg;
        pdMsg.m_MOrC = HEART;
        pdMsg.m_value.heart.str = (byte*)malloc(HEART_PKG_MSG_SIZE);
        memcpy(pdMsg.m_value.heart.str, HEART_PKG_MSG, HEART_PKG_MSG_SIZE);
        byte* data = nullptr;
        uint64_t lenth;
        pdMsg.CreatBinStream(data, &lenth);
        int result;
        result = send(fd, data, HEART_PKG_SIZE, NULL);
        if (result == -1) {
            LOG << "LogOut fd:" << fd << endl;
            // 发送失败，消息放到数据库
            g_Control.m_QueueMgt.LogOut(i);
        }
        free(data);
    }
    

    //重新开始定时器
    //——————————————————————
    struct itimerspec new_its;
    new_its.it_value.tv_sec = 600;
    new_its.it_value.tv_nsec = 0;
    new_its.it_interval.tv_sec = 600;
    new_its.it_interval.tv_nsec = 0;
    timer_settime(timerid, 0, &new_its, NULL);
    //——————————————————————
}
void DoBrokenPipe(int signal) {
    LOG << "BrokenPipe" << endl;
}

SendPdQueue_t::SendPdQueue_t() {
    int result = sem_init(&sem, 0, 0);
    CHECK_CALL(result);
}

int main() {
    DataBase::GetInstance()->SetConnectInfo("localhost", 3306, "zhangyuqing", "a1770287141$A.");
    DataBase::GetInstance()->ConnectDataBase();
    epoll_event ev;
    int result;
    SetFlashMode();


    ThreadMgt  threadMgt;
    memset(&ev, 0, sizeof(epoll_event));
    ev.data.fd = g_Control.m_SocketListen;
    ev.events = EPOLLIN | EPOLLET;
    result = epoll_ctl(g_Control.m_ListenEpoll, EPOLL_CTL_ADD, g_Control.m_SocketListen, &ev);
    CHECK_CALL(result);


    result = pthread_create(&threadMgt.m_AcceptThreadFd, NULL, threadMgt.m_AcceptThread.function, NULL);
    CHECK_CALL(result);
    result = pthread_create(&threadMgt.m_RecvThreadFd, NULL, threadMgt.m_RecvThread.function, NULL);
    CHECK_CALL(result);
    result = pthread_create(&threadMgt.m_ExecuteThreadFd, NULL, threadMgt.m_ExecuteThread.function, NULL);
    CHECK_CALL(result);
    result = pthread_create(&threadMgt.m_SendThreadFd, NULL, threadMgt.m_SendThread.function, NULL);
    CHECK_CALL(result);


    struct sigevent sev;
    struct itimerspec its;
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    sev.sigev_value.sival_ptr = &timerid;
    timer_create(CLOCK_REALTIME, &sev, &timerid);
    its.it_value.tv_sec = 600;
    its.it_value.tv_nsec = 0;
    its.it_interval.tv_sec = 600;
    its.it_interval.tv_nsec = 0;
    timer_settime(timerid, 0, &its, NULL);
    signal(SIGALRM, HeartDetectionTimer);


    struct sigaction sa; // 定义一个 sigaction 结构体实例 sa
    sa.sa_handler = DoBrokenPipe; // 将 sa_handler 字段设置为自定义的信号处理函数 handle_sigpipe
    sigemptyset(&sa.sa_mask); // 使用 sigemptyset 函数初始化信号集，将 sa_mask 清空，表示在处理 SIGPIPE 时不阻塞其他信号
    sa.sa_flags = SA_RESTART; // 设置 sa_flags 字段为 SA_RESTART，表示当信号中断某些系统调用时，系统调用会自动重启
    // 使用 sigaction 函数为 SIGPIPE 信号注册处理程序
    // 第一个参数 SIGPIPE 表示要捕捉的信号
    // 第二个参数 &sa 表示新的信号处理方式
    // 第三个参数 NULL 表示不保存旧的信号处理方式
    result = sigaction(SIGPIPE, &sa, NULL); 
    CHECK_CALL(result);



    pthread_join(threadMgt.m_AcceptThreadFd, NULL);
    pthread_join(threadMgt.m_RecvThreadFd, NULL);
    pthread_join(threadMgt.m_ExecuteThreadFd, NULL);
    pthread_join(threadMgt.m_SendThreadFd, NULL);
    return 0;
}
