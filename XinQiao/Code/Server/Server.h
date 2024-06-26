#pragma once
#include"../Transfer/Transfer.h"
#include <qmessagebox.h>

MALLOC void GetFriendsList_M(IN PdChat206 msg, OUT FriendInfo_t*& friendData,OUT uint16_t& length);

//Manager同步下达指令操作
//————————————————————————
void AddFriend(uint16_t selfId, uint16_t addId);

void LogIn(std::uint16_t id, std::uint16_t pw);

void SignUser(IN std::string userName, IN uint16_t pw, OUT uint16_t& id);

void SendXQMessage(uint16_t recvId, uint16_t sendId,std::string message, uint64_t times);
//————————————————————————
