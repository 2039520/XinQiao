#include"DataBase.h"
DataBase db;

bool DataBase::ConnectDataBase() {
    bool ret = false;
    driver = mysql::get_mysql_driver_instance();
    con = driver->connect(ip + ":" + to_string(port), userName, pw);//"tcp://localhost:3306", "username", "password"
    if (con == nullptr) {
        LOG << "Connect Fail" << endl;
    }
    else {
        LOG << "Connect Successd" << endl;
        ret = true;
    }
    con->setSchema("XinQiao");
    return ret;
}


uint16_t DataBase::GetPw(uint16_t id) {
    Statement* stmt = con->createStatement();
    string cmd = "select password from UserInfo where id = " + to_string(id) +"; ";
    ResultSet* res = stmt->executeQuery(cmd);
    if (res->next()) {
        return res->getUInt("password");
    }
    return 0;
}

DataBase* DataBase::GetInstance() {
    return &db;
}

void DataBase::SetConnectInfo(string ip, uint port, string userName, string pw) {
    this->ip = ip;
    this->port = port;
    this->userName = userName;
    this->pw = pw;
}

bool DataBase::RegistUser(OUT uint16_t& id, IN uint16_t pw, IN string userName) {
    Statement* stmt = con->createStatement();
    string cmd = "INSERT INTO UserInfo (username, password) VALUES ('" + userName + "'" + "," + to_string(pw) + "); ";
    int ret = stmt->executeUpdate(cmd);
    if (ret > 0) {
        ResultSet * result =  stmt->executeQuery("SELECT LAST_INSERT_ID();");
        result->next();
        id = result->getUInt(1);
        return true;
    }
    else
        return false;
}

MALLOC bool DataBase::GetFriendList_M(OUT FriendInfo_t*& friendInfo, IN uint16_t srcId, OUT uint64_t& length) {
    if (friendInfo != nullptr) {
        free(friendInfo);
    }
    Statement* stmt = con->createStatement();
    string cmd = "SELECT COUNT(*) AS row_count                  \
        FROM(                                                   \
            SELECT friend.user_id, UserInfo.UserName            \
            FROM friend                                         \
            JOIN UserInfo ON friend.user_id = UserInfo.id       \
            WHERE friend.id =  " + to_string(srcId)  + "        \
        ) AS subquery;";
    ResultSet* resCount = stmt->executeQuery(cmd);
    if (resCount->next()) {
        length = resCount->getUInt(1);
    }
    else {
        return true;
    }
    cmd = "SELECT friend.user_id, UserInfo.UserName FROM friend JOIN UserInfo ON friend.user_id = UserInfo.id WHERE friend.id = "+to_string(srcId)+ ";";
    ResultSet* res = stmt->executeQuery(cmd);
    friendInfo = (FriendInfo_t*)calloc(length, sizeof(FriendInfo_t));
    for (uint i = 0; i < length; i++) {
        res->next();
        friendInfo[i] = GetFriendInfo_M(res->getUInt("user_id"), (const byte*)res->getString("username").c_str(), res->getString("username").length());
    }
    return true;
}


bool DataBase::AddFriend(IN uint16_t idOne, IN uint16_t idTwo) {
    Statement* stmt = con->createStatement();
    string cmd = "insert into friend (id, user_id) values("+std::to_string(idOne)+","+ std::to_string(idTwo)+"); ";
    int ret = stmt->executeUpdate(cmd);
    if (ret < 0) {
        return false;
        //[!]需要加异常处理
    }
    cmd = "insert into friend (id, user_id) values(" + std::to_string(idTwo) + "," + std::to_string(idOne) + "); ";
    ret = stmt->executeUpdate(cmd);
    if (ret > 0) {
        return true;
    }
}


MALLOC bool DataBase::GetUserInfo(OUT UserInfo_t& userInfo) {
    //SELECT user.username, UserInfo.motto FROM user JOIN UserInfo ON user.id = UserInfo.id WHERE user.id= 1;
    Statement* stmt = con->createStatement();
    string cmd = "SELECT UserName, motto FROM UserInfo WHERE id= " + to_string(userInfo.id) + ";";
    ResultSet* res = stmt->executeQuery(cmd);
    string userName;
    string motto;
    if (res->next()) {
        userName = res->getString(1);
        motto = res->getString(2);
    }
    byte* data = (byte*)malloc(userName.length() + motto.length());
    memcpy(data, userName.c_str(), userName.length());
    memcpy(data+ userName.length(), motto.c_str(), motto.length());
    userInfo.Update(userName.length(), motto.length(), 0, data);
    free(data);
}

MALLOC bool DataBase::SetUserInfo(IN UserInfo_t userInfo) {
    //UPDATE UserInfo SET motto = 'Come', UserName = 'Zhang Peng' WHERE id = 1;
    Statement* stmt = con->createStatement();
    string cmd = "  UPDATE  UserInfo SET  motto = '"+userInfo.GetMotto()+"',"+"UserName = '"+userInfo.GetXQUserName()+"' WHERE   id = "+to_string(userInfo.id)+";";
    int ret = stmt->executeUpdate(cmd);
    if (1 != ret ) {
        return false;
    }
    return true;
}

void DataBase::SaveMessage(PdChat206 pd) {
    //insert into chat(send_id, receive_id, no_receive_msg, at_send_time) values(1, 2, '哈哈', 2454252443);
    Statement* stmt = con->createStatement();
    string cmd = "insert into chat (send_id, receive_id,no_receive_msg,at_send_time) values(" + 
        std::to_string(pd.m_value.messageData.sendId)               + ","   + 
        std::to_string(pd.m_value.messageData.recvId)               + ","   +"'"    +
        std::string((char*)pd.m_value.messageData.content,pd.m_value.messageData.contentLen)  + "'"   + ","   +
        std::to_string(pd.m_value.messageData.time) +"); ";
    int ret = stmt->executeUpdate(cmd);
    if (ret < 0) {
        //[!]需要加异常处理
    }
}

void DataBase::GetMessage(OUT PdChat206*& pd,IN uint16_t id,OUT uint64_t& n) {
    //SELECT send_id, no_receive_msg,no_receive_msg, DATE_FORMAT(at_send_time, '%Y%m%d%H%i%s') FROM chat where send_id = 1;
    if (pd != nullptr) {
        delete pd;
        pd = nullptr;
    }
    Statement* stmt = con->createStatement();
    // 获取总行数
    uint64_t count;
    ResultSet* countRes = stmt->executeQuery("SELECT COUNT(*) AS total_rows FROM chat WHERE receive_id = " + to_string(id));
    if (countRes->next()) {
        count = countRes->getInt("total_rows");
    }
    n = count;

    pd = new PdChat206[count];
    string cmd = "SELECT send_id,receive_id, no_receive_msg, at_send_time FROM chat where receive_id = "+to_string(id)+";";
    ResultSet* res = stmt->executeQuery(cmd);
    for (int i = 0; i < count; i++) {
        if (res->next()) {
            pd[i].m_MOrC = MESSAGE;
            pd[i].m_value.messageData.sendId = res->getUInt(1);
            pd[i].m_value.messageData.recvId = res->getUInt(2);
            pd[i].m_value.messageData.content = (byte*)malloc(res->getString(3).length());
            memcpy(pd[i].m_value.messageData.content, res->getString(3).c_str(), res->getString(3).length());
            pd[i].m_value.messageData.contentLen = res->getString(3).length();
            pd[i].m_value.messageData.time = res->getUInt64(4);
        }
    }

    //DELETE FROM chat WHERE receive_id = 3;
    cmd = "DELETE FROM chat WHERE receive_id = " + to_string(id) + ";";
    int ret = stmt->executeUpdate(cmd);
    if (ret < 0) {
        //[!]需要加异常处理
    }
}