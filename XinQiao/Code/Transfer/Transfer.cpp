#pragma once
#include"Transfer.h"
#include"../Main/main.h"
Transfer g_transfer;



TcpInfo::TcpInfo() {
	m_servIp = "47.109.98.45";
	m_host = 2039;
	
}


Transfer::Transfer() {
	m_RecvLength = 0;
	m_Length = 0;
	m_NeedSendLength = 0;
	m_SendLength = 0;
	m_ServerSokect = INVALID_SOCKET;
	ConnectServer();
}
Transfer::Transfer(TcpInfo info) {
	m_RecvLength = 0;
	m_Length = 0;
	m_ServerSokect = INVALID_SOCKET;
	m_Info.m_servIp = info.m_servIp;
	m_Info.m_host = info.m_host;
	ConnectServer();
}
WPA bool Transfer::ConnectServer() {
	WSADATA wsaData;
	SOCKADDR_IN servAddr;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return false;
	}
	m_ServerSokect = socket(PF_INET, SOCK_STREAM, 0);
	if (m_ServerSokect == INVALID_SOCKET) {
		return false;
	}
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(m_Info.m_servIp);
	servAddr.sin_port = htons(m_Info.m_host);
	if (connect(m_ServerSokect, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		return false;
	}
	return true;
}


WPA bool Transfer::SendPdMessage(PdChat206 mes) {
	byte* str = NULL;
	uint64_t length;
	mes.CreatBinStream(str, &length);
	if (INVALID_SOCKET == m_ServerSokect) {
		if (!ConnectServer()) {
			return false;
		}
	}
	CreatPdMessage(str, length);
	m_NeedSendLength = length;
	A:
	int result = send(m_ServerSokect, (char*)str+ m_SendLength, length- m_SendLength, 0);
	if (result == -1) {
		closesocket(m_ServerSokect);
		m_ServerSokect = INVALID_SOCKET;
		return false;
	}
	m_SendLength += result;
	if (m_SendLength != m_NeedSendLength) {
		goto A;
	}
	m_NeedSendLength = 0;
	m_SendLength = 0;
	return true;
}


WPA MALLOC void Transfer::RecvPdMessage() {
	static char* buf = nullptr;
	if (buf == nullptr) {
		buf  = (char*)malloc(MAX_CHATDATA_PKG_SIZE);
		memset(buf, 0, MAX_CHATDATA_PKG_SIZE);
	}
	uint len = m_Length== 0? MAX_CHATDATA_PKG_SIZE: m_Length- m_RecvLength;
A:
	int result = recv(m_ServerSokect, buf+ m_RecvLength, len, 0);
	if (result == -1) {
		goto A;
	}
	if (m_Length == 0) {
		m_Length = *(uint64_t*)buf;
		if (m_Length > MAX_CHATDATA_PKG_SIZE) {
			buf = (char*)realloc(buf, m_Length);
		}
	}
	PdChat206* mes = new PdChat206;
	if (result == m_Length - m_RecvLength) {
		mes->BinPrase((byte*)buf + 8);
		m_Length = 0;
		m_RecvLength = 0;
		AddRecvPdMsg(*mes);
	}
	else if (result > m_Length - m_RecvLength) {
		do {
			mes->BinPrase((byte*)buf + 8);
			AddRecvPdMsg(*mes);
			memcpy(buf, buf + m_Length, result - (m_Length - m_RecvLength));
			result = result - (m_Length - m_RecvLength);
			m_RecvLength = 0;
			if (result == 0) {
				m_Length = 0;
				return;
			}
			m_Length = *(uint64_t*)buf;
		} while (result >= m_Length);
		if (result > 0) {
			m_RecvLength = result;
		}
	}
	else {
		m_RecvLength += result;
		goto A;
	}
	delete mes;
	free(buf);
	buf = nullptr;
}

WPA void Transfer::CloseServer() {
	closesocket(m_ServerSokect);
	m_ServerSokect = INVALID_SOCKET;
}

void Transfer::AddPdMessage(PdChat206& msg) {
	m_PdMessageQueue.push(msg);
	XQControl::GetInstance()->NotifiedSendMassage();
}

void Transfer::GetPdMessage(PdChat206& msg) {
	msg = m_PdMessageQueue.front();
	m_PdMessageQueue.pop();
}


void Transfer::AddRecvPdMsg(PdChat206& msg) {
	int a = msg.m_MOrC;
	LOG << "Recv Message" << a << std::endl;
	m_RecvPdMsg.push(msg);
	XQControl::GetInstance()->m_Semaphore.release();
}


void Transfer::GetRecvPdMsg(PdChat206& msg) {
	XQControl::GetInstance()->m_Semaphore.acquire();
	msg = m_RecvPdMsg.front();
	m_RecvPdMsg.pop();
}


Transfer* Transfer::GetInstance() {
	return &g_transfer;
}

