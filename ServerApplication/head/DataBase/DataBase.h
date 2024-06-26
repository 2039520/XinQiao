#pragma once
#ifndef DATABASE_H
#define DATABASE_H
#include"../PdChat/PdChat206.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
using namespace sql;

class DataBase {
private:
	string ip;
	uint port;
	string userName;
	string pw;
	mysql::MySQL_Driver* driver;
	Connection* con ;
public:
	DataBase() :ip(""), port(0), userName(""), pw("") ,driver(nullptr), con(nullptr){
	}
	DataBase(string ip, uint port, string userName, string pw) {
		this->ip = ip;
		this->port = port;
		this->userName = userName;
		this->pw = pw;
	}
	~DataBase() {
		if(con!= nullptr)
			delete con;
	}

	bool ConnectDataBase();
	void SetConnectInfo(string ip, uint port, string userName, string pw);
	static DataBase * GetInstance();
	uint16_t GetPw(uint16_t id);
	bool RegistUser(OUT uint16_t& id, IN uint16_t pw, IN string userName);
	MALLOC bool GetFriendList_M(OUT FriendInfo_t*& friendInfo ,IN uint16_t srcId,OUT uint64_t& length);
	bool AddFriend(IN uint16_t idOne, IN uint16_t idTwo);
	MALLOC bool GetUserInfo(OUT UserInfo_t& userInfo);
	MALLOC bool SetUserInfo(IN UserInfo_t userInfo);
	void SaveMessage(PdChat206 pd);
	MALLOC void GetMessage(OUT PdChat206*& pd, IN uint16_t id, OUT uint64_t& n);
};
#endif // !DATABASE_H



