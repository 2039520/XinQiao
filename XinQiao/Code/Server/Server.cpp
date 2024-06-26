#pragma once
#include"Server.h"


void LogIn(uint16_t id, uint16_t pw) {
	PdChat206 msg;
	msg.m_MOrC = LOGIN;
	msg.m_value.control.id = id;
	msg.m_value.control.pw = pw;
	Transfer::GetInstance()->AddPdMessage(msg);
}


void SignUser(IN std::string userName, IN uint16_t pw ,OUT uint16_t& id) {
	PdChat206 msg;
	msg.m_MOrC = SIGN;
	msg.m_value.sign.userName = (byte*)malloc(userName.length());
	memcpy(msg.m_value.sign.userName, userName.c_str(), userName.length());
	msg.m_value.sign.userNameLength = userName.length();
	msg.m_value.sign.pw = pw;
	Transfer::GetInstance()->AddPdMessage(msg);
}

/**
 * @brief *
 * @param param1 FriendInfo_t类型的数组，注意是数组，不是单个的指针 
 * @param param2 数组长度
 * @param param3 *
 * @return *
 */
MALLOC void GetFriendsList_M(IN PdChat206 msg, OUT FriendInfo_t*& friendData, OUT uint16_t& length) {
	if (friendData != nullptr) {
		free(friendData);
	}
	friendData = (FriendInfo_t*)malloc(msg.m_value.friendData.length * sizeof(FriendInfo_t));
	for (uint i = 0; i < msg.m_value.friendData.length; i++) {
		friendData[i].id = msg.m_value.friendData.friendInfo[i].id;
		friendData[i].userNameLength = msg.m_value.friendData.friendInfo[i].userNameLength;
		friendData[i].userName = (byte*)calloc(friendData[i].userNameLength, sizeof(byte));
		memcpy(friendData[i].userName, msg.m_value.friendData.friendInfo[i].userName, friendData[i].userNameLength);
	}
	length = msg.m_value.friendData.length;
}

void AddFriend(uint16_t selfId,uint16_t addId) {
	PdChat206 msg;
	msg.m_MOrC = ADD_FRIEND;
	msg.m_value.addFriend.srcId = selfId;
	msg.m_value.addFriend.addId = addId;
	msg.m_value.addFriend.yn = 2;
	Transfer::GetInstance()->AddPdMessage(msg);
}

void SendXQMessage(uint16_t recvId, uint16_t sendId,std::string message, uint64_t times) {
	PdChat206 msg;
	msg.m_MOrC = MESSAGE;
	msg.m_value.messageData.recvId = recvId;
	msg.m_value.messageData.sendId = sendId;
	msg.m_value.messageData.content = (byte*)malloc(message.length());
	memcpy(msg.m_value.messageData.content, message.c_str(), message.length());
	msg.m_value.messageData.contentLen = message.length();
	msg.m_value.messageData.time = times;
	Transfer::GetInstance()->AddPdMessage(msg);
}
