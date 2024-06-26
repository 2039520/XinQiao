#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <wininet.h>
#include"../PdChat206/PdChat206.h"
#include"../PdSdk/WinPd.h"
#include<queue>

class TcpInfo {

public:
	const char* m_servIp ;
	uint m_host ;
public:
	TcpInfo();
};



class Transfer {
private:
	TcpInfo m_Info;
	SOCKET m_ServerSokect;
	uint64_t m_RecvLength;
	uint64_t m_Length;
	uint64_t m_NeedSendLength;
	uint64_t m_SendLength;
	std::queue<PdChat206>  m_PdMessageQueue;
	std::queue<PdChat206>  m_RecvPdMsg;

public:
	friend void SendThread();
	friend void ReceiveThread();
public:
	Transfer();
	Transfer(TcpInfo info);
public:
	WPA bool ConnectServer();
	WPA void CloseServer();
	WPA bool SendPdMessage(PdChat206 mes);
	WPA MALLOC void RecvPdMessage();
	void AddPdMessage(PdChat206& msg);
	void GetPdMessage(PdChat206& msg);
	void AddRecvPdMsg(PdChat206& msg);
	void GetRecvPdMsg(PdChat206& msg);
	static Transfer* GetInstance();
};

