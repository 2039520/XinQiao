#pragma once
#ifndef PDCHAT206_H
#define	PDCHAT206_H
#include"../PdSdk/Pd.h"
#define INIT_STATUS 0
#define MESSAGE     1
#define LOGIN       2
#define HEART       3
#define FRIEND      4
#define SIGN        5
#define ADD_FRIEND  6
#define USER_INFO   7

#define PKG_CONTROL    0 INDEX       //控制信息和消息信息标识位


// 传输身份验证信息
// ————————————————————
#define L_ID        8   INDEX       //账户
#define L_PW        24  INDEX       //密码
#define L_YN        40  INDEX       //1表示登录成功  0表示登录失败
//————————————————————


//注册信息
//————————————————————
#define S_ID                8   INDEX       //账户
#define S_PW                24  INDEX       //密码
#define USER_NAME_LENGTH    40  INDEX       //【-】返回包：1代表成功  0表示失败
#define USER_NAME           48  INDEX
//————————————————————


//传输消息
//————————————————————
#define RECV_ID     8       INDEX       //接收方ID
#define SEND_ID     24      INDEX       //发送方ID
#define CON_LEN     40      INDEX       //消息长度
#define SEND_TIME   56      INDEX       //客户端发送时的时间
#define CON_INDEX   120     INDEX       //传输的信息
//————————————————————


//传输心跳包
//————————————————————
#define HEART_PKG   8   INDEX
//————————————————————


//传输好友列表
//————————————————————
#define SR_ID               8   INDEX       //客户端的id
#define FRIEND_LENGTH       24  INDEX      
#define FRIEND_ID           40  INDEX       
#define FRIEND_NAME_LENGTH  56  INDEX
#define FRIEND_NAME         64  INDEX
//————————————————————


//好友申请
//————————————————————
#define ADD_ID             8   INDEX       //接收方ID
#define SRC_ID             24  INDEX       //发送方ID
#define ADD_FRIEND_YN      40  INDEX       //1表示同意，0表示拒绝，2表示申请消息
//————————————————————


//获取或者设置用户信息
//—————————————————————
#define U_INFO_ID           8       INDEX
#define U_INFO_CONTROL      24      INDEX   //0表示获取，1表示更新，获取的时候，没有ProPic数据
#define U_INFO_NAME_LEN     32      INDEX
#define U_INFO_MOTTO_LEN    40      INDEX
#define U_INFO_PROPIC_LEN   48      INDEX
#define U_INFO_DATA         112     INDEX
//—————————————————————


#define MAX_CHATDATA_PKG_SIZE       1*MB          //  88/8+2*16 + 1
#define PKG_CONTROL_HEAD_SIZE           1       

//身份验证
#define C_PW_SIZE                   2       
#define C_ID_SIZE                   2       
#define C_YN_SIZE                   1       

//传输信息
#define MESSAGE_HEAD_SIZE           (CON_INDEX/BYTE_LEN)  
#define CONTROL_SIZE                (PKG_CONTROL_HEAD_SIZE+C_PW_SIZE+C_ID_SIZE+C_YN_SIZE)    

//心跳包
#define HEART_PKG_MSG_SIZE          2       
#define HEART_PKG_SIZE              (HEART_PKG_MSG_SIZE + PKG_CONTROL_HEAD_SIZE)        
#define HEART_PKG_MSG   "zp"

//好友列表
#define FRIEND_HEAD_SIZE            ((FRIEND_NAME - FRIEND_ID)/BYTE_LEN)      //注意这里的头，指的是好友信息的头，不是好友列表的头

//注册
#define SIGN_HEAD_SIZE              ((USER_NAME-S_ID)/BYTE_LEN)

//好友申请
#define ADD_FRIEND_YN_SIZE          1




//协议定义，注意下面的所有实现都要看一遍，看有没有需要添加的


#pragma pack(push, 1) // 保存当前的对齐状态，并设置为紧凑对齐
struct AddFriend_t {
    uint16_t addId;
    uint16_t srcId;
    uint8_t   yn;
};

struct FriendInfo_t {
    uint16_t  id;
    uint8_t  userNameLength;
    byte* userName;
};

struct Friend_t {
    uint16_t  srId;
    uint16_t  length;
    FriendInfo_t* friendInfo;
};

struct Sign_t {
    uint16_t id;
    uint16_t pw;
    uint8_t userNameLength;
    byte* userName;
};

struct Message_t {
public:
    uint16_t recvId;
    uint16_t sendId;
    uint16_t contentLen;
    uint64_t time;
    byte* content;
};


class UserInfo_t {
    friend class  PdChat206;
public:
    uint16_t  id;
    uint8_t   control;
    uint8_t userNameLen;
    uint8_t mottoLen;
    uint64_t profilePictureLen;
private:
    byte* data;
public:
    void Update(IN uint8_t userNameLen, IN uint8_t mottoLen, IN uint64_t profilePictureLen, IN const byte* data);
    void UpdateProfilePicture(IN uint64_t profilePictureLen, IN const byte* data);
    std::string GetXQUserName();
    std::string GetMotto();
    std::string GetProfilePicture();
    byte* GetData();
};

#pragma pack(pop)




struct ConTrol_t {
public:
    uint16_t id;
    uint16_t pw;
    uint8_t yn;
};

struct Heart_t {
public:
    byte* str;
};

class PdChat206 {
public:
    uint8_t m_MOrC{};
    union Value {
        Message_t messageData;
        ConTrol_t control;
        Heart_t heart;
        Friend_t  friendData;
        Sign_t  sign;
        AddFriend_t addFriend;
        UserInfo_t  UserInfo;
    }m_value{};
public:
    PdChat206();
    PdChat206(const PdChat206& src);
    ~PdChat206();
    PdChat206& operator=(const PdChat206& other) {
        if (this != &other) {
            this->m_MOrC = other.m_MOrC;
            switch (m_MOrC)
            {
            case MESSAGE:
                m_value.messageData.recvId = other.m_value.messageData.recvId;
                m_value.messageData.contentLen = other.m_value.messageData.contentLen;
                m_value.messageData.sendId = other.m_value.messageData.sendId;
                m_value.messageData.time = other.m_value.messageData.time;
                m_value.messageData.content = (byte*)malloc(sizeof(byte) * other.m_value.messageData.contentLen);
                memcpy(m_value.messageData.content, other.m_value.messageData.content, other.m_value.messageData.contentLen);
                break;
            case LOGIN:
                m_value.control.id = other.m_value.control.id;
                m_value.control.pw = other.m_value.control.pw;
                m_value.control.yn = other.m_value.control.yn;
                break;
            case HEART:
                m_value.heart.str = (byte*)malloc(HEART_PKG_MSG_SIZE);
                memcpy(m_value.heart.str, other.m_value.heart.str, HEART_PKG_MSG_SIZE);
                break;
            case FRIEND:
                m_value.friendData.length = other.m_value.friendData.length;
                m_value.friendData.srId = other.m_value.friendData.srId;
                m_value.friendData.friendInfo = (FriendInfo_t*)malloc(sizeof(FriendInfo_t) * m_value.friendData.length);
                memcpy(m_value.friendData.friendInfo, other.m_value.friendData.friendInfo, m_value.friendData.length);
                for (int i = 0; i < m_value.friendData.length; i++) {
                    m_value.friendData.friendInfo[i].id = other.m_value.friendData.friendInfo[i].id;
                    m_value.friendData.friendInfo[i].userNameLength = other.m_value.friendData.friendInfo[i].userNameLength;
                    m_value.friendData.friendInfo[i].userName = (byte*)malloc(sizeof(byte) * m_value.friendData.friendInfo[i].userNameLength);
                    memcpy(m_value.friendData.friendInfo[i].userName, other.m_value.friendData.friendInfo[i].userName, m_value.friendData.friendInfo[i].userNameLength);
                }
                break;
            case SIGN:
                m_value.sign.id = other.m_value.sign.id;
                m_value.sign.pw = other.m_value.sign.pw;
                m_value.sign.userNameLength = other.m_value.sign.userNameLength;
                m_value.sign.userName = (byte*)calloc(m_value.sign.userNameLength, sizeof(byte));
                memcpy(m_value.sign.userName, other.m_value.sign.userName, m_value.sign.userNameLength);
                break;
            case ADD_FRIEND:
                m_value.addFriend.addId = other.m_value.addFriend.addId;
                m_value.addFriend.srcId = other.m_value.addFriend.srcId;
                m_value.addFriend.yn = other.m_value.addFriend.yn;
                break;
            case USER_INFO:
                m_value.UserInfo.id = other.m_value.UserInfo.id;
                m_value.UserInfo.control = other.m_value.UserInfo.control;
                m_value.UserInfo.mottoLen = other.m_value.UserInfo.mottoLen;
                m_value.UserInfo.profilePictureLen = other.m_value.UserInfo.profilePictureLen;
                m_value.UserInfo.userNameLen = other.m_value.UserInfo.userNameLen;
                m_value.UserInfo.data = (byte*)calloc(other.m_value.UserInfo.mottoLen + other.m_value.UserInfo.profilePictureLen + other.m_value.UserInfo.userNameLen, sizeof(byte));
                memcpy(m_value.UserInfo.data, other.m_value.UserInfo.data, other.m_value.UserInfo.mottoLen + other.m_value.UserInfo.profilePictureLen + other.m_value.UserInfo.userNameLen);
                break;
            default:
                break;
            }
        }
        return *this;
    }
    explicit PdChat206(IN byte* data);//[!不需修改]

    //数据解析    
    //——————————————————————————————
    bool BinPrase(IN byte* data);
    void BinPrase_FRIEND(IN byte* data);
    //——————————————————————————————


    //数据构造
    //——————————————————————————————
    bool CreatBinStream(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForMessage(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForControl(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForHeart(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForFirend(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForSign(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForAddFirend(OUT byte*& data, OUT uint64_t* length);
    void CreatBinStreamForUserInfo(OUT byte*& data, OUT uint64_t* length);
    //———————————————————————————————

    


    bool CheckData() const;//[!不需修改]
    bool SetMessageContent(IN const byte* data, IN uint len);//[!]等待完善
    uint64_t GetDataStrLen();

    //对外接口
    //———————————————————————————————
    FREE void SetFriend_F(IN FriendInfo_t* friendInfo, IN uint64_t n, IN uint16_t id);
    //———————————————————————————————
};

MALLOC FriendInfo_t GetFriendInfo_M(IN uint16_t id, IN const byte* name, IN uint8_t nameLeng);

//最终包封装
//———————————————————————————————
void CreatPdMessage(OUT byte*& data,OUT uint64_t& n);
//PdChat206 RemovePdMessageHead();

//———————————————————————————————

#endif // !PDCHAT206_H

