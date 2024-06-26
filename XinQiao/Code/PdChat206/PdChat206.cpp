#pragma once
#include"PdChat206.h"
PdChat206::PdChat206() {
    m_MOrC = INIT_STATUS;
    m_value.messageData.contentLen = 0;
    m_value.messageData.recvId = 0;
    m_value.messageData.sendId = 0;
    m_value.messageData.time = 0;
    m_value.messageData.content = nullptr;
}
PdChat206::PdChat206(const PdChat206& other) {
    m_MOrC = other.m_MOrC;
    switch (other.m_MOrC)
    {
    case MESSAGE:
        m_value.messageData.contentLen = other.m_value.messageData.contentLen;
        m_value.messageData.recvId = other.m_value.messageData.recvId;
        m_value.messageData.sendId = other.m_value.messageData.sendId;
        m_value.messageData.time = other.m_value.messageData.time;
        if (other.m_value.messageData.contentLen == 0) {
            m_value.messageData.content = nullptr;
        }
        else {
            m_value.messageData.content = (byte*)malloc(sizeof(byte) * other.m_value.messageData.contentLen);
            memcpy(m_value.messageData.content, other.m_value.messageData.content, other.m_value.messageData.contentLen);
        }
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
PdChat206::~PdChat206() {
    switch (m_MOrC)
    {
    case MESSAGE:
        if (m_value.messageData.content != nullptr)
            free(m_value.messageData.content);
        m_value.messageData.content = nullptr;
        break;
    case HEART:
        if (m_value.heart.str != nullptr)
            free(m_value.heart.str);
        m_value.heart.str = nullptr;
        break;
    case FRIEND:
        for (int i = 0; i < m_value.friendData.length; i++) {
            free(m_value.friendData.friendInfo[i].userName);
        }
        free(m_value.friendData.friendInfo);
        break;
    case SIGN:
        if (m_value.sign.userName != nullptr)
            free(m_value.sign.userName);
        break;
    case USER_INFO:
        if (m_value.UserInfo.data != nullptr)
            free(m_value.UserInfo.data);
        break;
    default:
        break;
    }
}

bool PdChat206::BinPrase(IN byte* data) {
    bool ret = true;
    m_MOrC = BintoInt(data, 0, PKG_CONTROL_HEAD_SIZE * BYTE_LEN);
    switch (m_MOrC) {
    case MESSAGE:
        m_value.messageData.recvId = BintoInt(data, RECV_ID, SEND_ID);
        m_value.messageData.sendId = BintoInt(data, SEND_ID, CON_LEN);
        m_value.messageData.contentLen = BintoInt(data, CON_LEN, SEND_TIME);
        m_value.messageData.time = BintoInt(data, SEND_TIME, CON_INDEX);
        m_value.messageData.content = (byte*)calloc(m_value.messageData.contentLen, sizeof(byte));
        memcpy(m_value.messageData.content, data + MESSAGE_HEAD_SIZE, m_value.messageData.contentLen);
        break;
    case LOGIN:
        m_value.control.id = BintoInt(data, L_ID, L_PW);
        m_value.control.pw = BintoInt(data, L_PW, L_YN);
        m_value.control.yn = BintoInt(data, L_YN, C_YN_SIZE * BYTE_LEN + L_YN);
        break;
    case HEART:
        m_value.heart.str = (byte*)calloc(HEART_PKG_MSG_SIZE, sizeof(byte));
        memcpy(m_value.heart.str, data + PKG_CONTROL_HEAD_SIZE, HEART_PKG_MSG_SIZE);
        break;
    case FRIEND:
        BinPrase_FRIEND(data);
        break;
    case SIGN:
        m_value.sign.id = BintoInt(data, S_ID, S_PW);
        m_value.sign.pw = BintoInt(data, S_PW, USER_NAME_LENGTH);
        m_value.sign.userNameLength = BintoInt(data, USER_NAME_LENGTH, USER_NAME);
        m_value.sign.userName = (byte*)calloc(m_value.sign.userNameLength, sizeof(byte));
        memcpy(m_value.sign.userName, data + USER_NAME / BYTE_LEN, m_value.sign.userNameLength);
        break;
    case ADD_FRIEND:
        m_value.addFriend.addId = BintoInt(data, ADD_ID, SRC_ID);
        m_value.addFriend.srcId = BintoInt(data, SRC_ID, ADD_FRIEND_YN);
        m_value.addFriend.yn = BintoInt(data, ADD_FRIEND_YN, ADD_FRIEND_YN + ADD_FRIEND_YN_SIZE * BYTE_LEN);
        break;
    case USER_INFO:
        m_value.UserInfo.id = BintoInt(data, U_INFO_ID, U_INFO_CONTROL);
        m_value.UserInfo.control = BintoInt(data, U_INFO_CONTROL, U_INFO_NAME_LEN);
        m_value.UserInfo.userNameLen = BintoInt(data, U_INFO_NAME_LEN, U_INFO_MOTTO_LEN);
        m_value.UserInfo.mottoLen = BintoInt(data, U_INFO_MOTTO_LEN, U_INFO_PROPIC_LEN);
        m_value.UserInfo.profilePictureLen = BintoInt(data, U_INFO_PROPIC_LEN, U_INFO_DATA);
        m_value.UserInfo.data = (byte*)calloc(m_value.UserInfo.userNameLen + m_value.UserInfo.mottoLen + m_value.UserInfo.profilePictureLen, sizeof(byte));
        memcpy(m_value.UserInfo.data, data + U_INFO_DATA / BYTE_LEN, m_value.UserInfo.userNameLen + m_value.UserInfo.mottoLen + m_value.UserInfo.profilePictureLen);
        break;
    default:
        ret = false;
        break;
    }
    return ret;
}


bool PdChat206::CreatBinStream(OUT byte*& data, OUT uint64_t* length) {
    bool ret = true;
    CheckData();
    switch (m_MOrC) {
    case MESSAGE:CreatBinStreamForMessage(data, length); break;
    case LOGIN:CreatBinStreamForControl(data, length); break;
    case HEART:CreatBinStreamForHeart(data, length); break;
    case FRIEND:CreatBinStreamForFirend(data, length); break;
    case SIGN:CreatBinStreamForSign(data, length); break;
    case ADD_FRIEND:CreatBinStreamForAddFirend(data, length); break;
    case USER_INFO:CreatBinStreamForUserInfo(data, length); break;
    default:
        ret = false;
        break;
    }
    return ret;
}

PdChat206::PdChat206(IN byte* data) {
    BinPrase(data);
}

void PdChat206::BinPrase_FRIEND(IN byte* data) {
    byte* p = data;
    m_value.friendData.length = BintoInt(p, FRIEND_LENGTH, FRIEND_ID);
    m_value.friendData.srId = BintoInt(p, SR_ID, FRIEND_LENGTH);
    m_value.friendData.friendInfo = (FriendInfo_t*)calloc(m_value.friendData.length, sizeof(FriendInfo_t));
    for (int i = 0; i < m_value.friendData.length; i++) {
        m_value.friendData.friendInfo[i].id = BintoInt(p, FRIEND_ID, FRIEND_NAME_LENGTH);
        m_value.friendData.friendInfo[i].userNameLength = BintoInt(p, FRIEND_NAME_LENGTH, FRIEND_NAME);
        m_value.friendData.friendInfo[i].userName = (byte*)calloc(m_value.friendData.friendInfo[i].userNameLength, sizeof(byte));
        memcpy(m_value.friendData.friendInfo[i].userName, p + FRIEND_NAME / BYTE_LEN, m_value.friendData.friendInfo[i].userNameLength);
        p += FRIEND_HEAD_SIZE + m_value.friendData.friendInfo[i].userNameLength;
    }
}


bool PdChat206::SetMessageContent(IN const byte* data, IN uint len) {
    if (m_MOrC != 0) {
        return false;
    }
    m_value.messageData.content = (byte*)calloc(len, sizeof(byte));
    memcpy(m_value.messageData.content, data, len);
}

void PdChat206::CreatBinStreamForMessage(OUT byte*& data, uint64_t* length) {
    if (data != nullptr) {
        free(data);
    }
    *length = MESSAGE_HEAD_SIZE + m_value.messageData.contentLen;
    data = (byte*)calloc(*length, sizeof(byte*));
    PTR_CHECK(data);
    *data = (byte)m_MOrC;
    void* p = &m_value.messageData;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, p, MESSAGE_HEAD_SIZE - PKG_CONTROL_HEAD_SIZE);
    memcpy(data + MESSAGE_HEAD_SIZE, m_value.messageData.content, m_value.messageData.contentLen);
}
void PdChat206::CreatBinStreamForControl(OUT byte*& data, uint64_t* length) {
    if (data != nullptr) {
        free(data);
    }
    *length = CONTROL_SIZE + PKG_CONTROL_HEAD_SIZE;
    data = (byte*)calloc(*length, sizeof(byte*));
    PTR_CHECK(data);
    *data = (byte)m_MOrC;
    void* p = &m_value.control;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, p, sizeof(m_value.control));
}

void PdChat206::CreatBinStreamForHeart(OUT byte*& data, uint64_t* length) {
    if (data != nullptr) {
        free(data);
    }
    *length = PKG_CONTROL_HEAD_SIZE + HEART_PKG_MSG_SIZE;
    data = (byte*)calloc(*length, sizeof(byte*));
    PTR_CHECK(data);
    *data = (byte)m_MOrC;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, m_value.heart.str, HEART_PKG_MSG_SIZE);
}
void PdChat206::CreatBinStreamForFirend(OUT byte*& data, OUT uint64_t* length) {
    *length = sizeof(m_MOrC) + sizeof(m_value.friendData.length) + sizeof(m_value.friendData.srId) + m_value.friendData.length * (sizeof(m_value.friendData.friendInfo->id) + sizeof(m_value.friendData.friendInfo->userNameLength));
    for (int i = 0; i < m_value.friendData.length; i++) {
        *length += m_value.friendData.friendInfo[i].userNameLength;
    }
    data = (byte*)malloc(*length);
    *data = (byte)m_MOrC;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, &m_value.friendData.srId, sizeof(m_value.friendData.length) + sizeof(m_value.friendData.srId));
    byte* byteP = data + PKG_CONTROL_HEAD_SIZE + sizeof(m_value.friendData.length) + sizeof(m_value.friendData.srId);
    FriendInfo_t* p = m_value.friendData.friendInfo;
    for (int i = 0; i < m_value.friendData.length; i++, p++) {
        memcpy(byteP, p, FRIEND_HEAD_SIZE);
        memcpy(byteP + FRIEND_HEAD_SIZE, p->userName, p->userNameLength);
        byteP += FRIEND_HEAD_SIZE + p->userNameLength;
    }
}

void PdChat206::CreatBinStreamForSign(OUT byte*& data, OUT uint64_t* length) {
    *length = PKG_CONTROL_HEAD_SIZE + SIGN_HEAD_SIZE + m_value.sign.userNameLength;
    data = (byte*)calloc(*length, sizeof(byte));
    *data = (byte)m_MOrC;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, &m_value.sign, SIGN_HEAD_SIZE);
    memcpy(data + SIGN_HEAD_SIZE + PKG_CONTROL_HEAD_SIZE, m_value.sign.userName, m_value.sign.userNameLength);
}

void PdChat206::CreatBinStreamForAddFirend(OUT byte*& data, OUT uint64_t* length) {
    *length = ADD_FRIEND_YN / BYTE_LEN + ADD_FRIEND_YN_SIZE;
    data = (byte*)calloc(*length, sizeof(byte));
    *data = (byte)m_MOrC;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, &m_value.addFriend, *length - PKG_CONTROL_HEAD_SIZE);
}

void PdChat206::CreatBinStreamForUserInfo(OUT byte*& data, OUT uint64_t* length) {
    *length = U_INFO_DATA / BYTE_LEN;
    data = (byte*)calloc(*length, sizeof(byte));
    *data = (byte)m_MOrC;
    memcpy(data + PKG_CONTROL_HEAD_SIZE, &m_value.UserInfo.id, *length - PKG_CONTROL_HEAD_SIZE);
    *length += m_value.UserInfo.userNameLen + m_value.UserInfo.mottoLen + m_value.UserInfo.profilePictureLen;
    data = (byte*)realloc(data, *length);
    memcpy(data + U_INFO_DATA / BYTE_LEN, m_value.UserInfo.data, *length - U_INFO_DATA / BYTE_LEN);
}


uint64_t PdChat206::GetDataStrLen() {
    switch (m_MOrC) {
    case MESSAGE:return MESSAGE_HEAD_SIZE + m_value.messageData.contentLen;
    case LOGIN:return CONTROL_SIZE;
    case HEART:return HEART_PKG_MSG_SIZE + PKG_CONTROL_HEAD_SIZE;
    }
}

FREE void PdChat206::SetFriend_F(IN FriendInfo_t* friendInfo, IN uint64_t n, IN uint16_t id) {
    m_MOrC = FRIEND;
    m_value.friendData.srId = id;
    m_value.friendData.length = n;
    m_value.friendData.friendInfo = (FriendInfo_t*)calloc(n, sizeof(FriendInfo_t));
    for (uint64_t i = 0; i < m_value.friendData.length; i++) {
        m_value.friendData.friendInfo[i].id = friendInfo[i].id;
        m_value.friendData.friendInfo[i].userNameLength = friendInfo[i].userNameLength;
        m_value.friendData.friendInfo[i].userName = (byte*)calloc(friendInfo[i].userNameLength, sizeof(byte));
        memcpy(m_value.friendData.friendInfo[i].userName, friendInfo[i].userName, friendInfo[i].userNameLength);
    }


    for (int i = 0; i < n; i++) {
        free(friendInfo[i].userName);
    }
    free(friendInfo);
}


MALLOC FriendInfo_t GetFriendInfo_M(IN uint16_t id, IN const byte* name, IN uint8_t nameLength) {
    FriendInfo_t friendInfo;
    friendInfo.id = id;
    friendInfo.userNameLength = nameLength;
    friendInfo.userName = (byte*)calloc(nameLength, sizeof(byte));
    strcpy((char*)friendInfo.userName, (const char*)name);
    return friendInfo;
}
bool PdChat206::CheckData() const {
    bool ret = true;
    switch (m_MOrC) {
    case MESSAGE:if (m_value.messageData.content == nullptr || m_value.messageData.contentLen == 0 || m_value.messageData.recvId == 0 || m_value.messageData.sendId == 0)ret = false; break;
    case LOGIN:if (m_value.control.id == 0 || m_value.control.pw == 0)ret = false; break;
    case HEART:if (m_value.heart.str == nullptr)ret = false; break;
    case FRIEND:if (m_value.friendData.length <= 0 || m_value.friendData.friendInfo == nullptr)ret = false; break;
    default:
        ret = false;
        break;
    }
    return ret;
}

void UserInfo_t::Update(IN uint8_t userNameLen, IN uint8_t mottoLen, IN uint64_t profilePictureLen, IN const byte* data) {
    this->userNameLen = userNameLen;
    this->mottoLen = mottoLen;
    this->profilePictureLen = profilePictureLen;
    this->data = (byte*)malloc(userNameLen + mottoLen + profilePictureLen);
    memcpy(this->data, data, userNameLen + mottoLen + profilePictureLen);
}

void UserInfo_t::UpdateProfilePicture(IN uint64_t profilePictureLen, IN const byte* data) {
    this->profilePictureLen = profilePictureLen;
    this->data = (byte*)realloc(this->data, this->userNameLen + this->mottoLen + profilePictureLen);
    memcpy(this->data + this->userNameLen + this->mottoLen, data, profilePictureLen);
}


std::string UserInfo_t::GetXQUserName() {
    return std::string((char*)this->data, this->userNameLen);
}

std::string UserInfo_t::GetMotto() {
    std::string temp((char*)this->data);
    std::string motto(temp.substr(this->userNameLen, this->mottoLen));
    return motto;
}

std::string UserInfo_t::GetProfilePicture() {
    std::string temp((char*)this->data, userNameLen + mottoLen + profilePictureLen);
    std::string profilePictureLen(temp.substr(this->userNameLen+this->mottoLen, this->profilePictureLen));
    return profilePictureLen;
}

byte* UserInfo_t::GetData() {
    return this->data;
}

void CreatPdMessage(OUT byte*& data,OUT uint64_t& n) {
    byte*temp = (byte*)malloc(n + 8);
    *(uint64_t*)temp = n+8;
    memcpy(temp + 8, data, n);
    free(data);
    data = temp;
    n = n + 8;
}