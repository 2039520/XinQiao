#include "mnue.h"
#include "ui_mnue.h"


Mnue::Mnue(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::Mnue), m_FriendListModel(nullptr), m_FriendListItems(nullptr), m_FriendListInfo(nullptr), m_FriendListLength(0)
{
    ui->setupUi(this);

    m_FriendListModel = new QStandardItemModel(ui->FriendList);
    ui->FriendList->setModel(m_FriendListModel);
    ui->FriendList->setRootIsDecorated(false);
    ui->FriendList->setEditTriggers(QTreeView::NoEditTriggers);			
    ui->FriendList->header()->setVisible(false);

    connect(ui->FriendList, &QTreeView::doubleClicked, this, &Mnue::FirendListItemDoubleClicked);
    RefreshFirendList();

    UpdateUserInfo();
}

Mnue::~Mnue()
{
    if (m_FriendListInfo != nullptr) {
        for (uint32_t i = 0; i < m_FriendListLength; i++) {
            free(m_FriendListInfo[i].userName);
        }
        free(m_FriendListInfo);
    }
    if (m_FriendListItems != nullptr)
        delete [] m_FriendListItems;
    if (ui != nullptr)
        delete ui;
}

void Mnue::FirendListItemDoubleClicked(const QModelIndex& index) {
    QStandardItem* item = m_FriendListModel->itemFromIndex(index);
    UiManager::GetInstance()->AddMessageWindow(m_FriendListInfo[index.row()].id);
}

void Mnue::RefreshFirendList() {
    PdChat206 msg;
    msg.m_MOrC = FRIEND;
    msg.m_value.friendData.srId = UiManager::GetInstance()->GetId();
    msg.m_value.friendData.length = 0;
    Transfer::GetInstance()->AddPdMessage(msg);
}

void Mnue::UpdateUserInfo() {
    //获取本地的数据
    //————————————————————————————————————
    std::string buffer;
    if (ReadFile(MAIN_PATHR + std::string(USER_INFO_PATHR)+std::to_string(UiManager::GetInstance()->GetId())+"/"+ USER_INFO_PROFILEPICTURE_PATH_NAME, buffer, std::ios::binary) == SUC) {
        SetProfilePicture(buffer);
    }
    //————————————————————————————————————

    PdChat206 msg;
    msg.m_MOrC = USER_INFO;
    msg.m_value.UserInfo.id = UiManager::GetInstance()->GetId();
    msg.m_value.UserInfo.control = 0;
    Transfer::GetInstance()->AddPdMessage(msg);
}


void Mnue::on_pushButton_clicked()
{
   uint16_t id =  ui->search->text().toUInt();
   for (uint i = 0; i < m_FriendListLength; i++) {
       if (id == m_FriendListInfo[i].id) {
           QMessageBox::information(nullptr, "remind", "You have already added this friend");
           return;
       }
   }
   AddFriend(UiManager::GetInstance()->GetId(), id);
   QMessageBox::information(nullptr, "remind", "Friend request has been sent.");
}


void Mnue::UpdateUserInfo(UserInfo_t info) {
    m_UserInfo = info;
    m_UserInfo.Update(info.userNameLen, info.mottoLen, info.profilePictureLen, info.GetData());
    std::string userName = info.GetXQUserName();
    std::string motto = info.GetMotto();
    std::string imageDateStr = info.GetProfilePicture();
    WriteFile(MAIN_PATHR + std::string(USER_INFO_PATHR) + std::to_string(UiManager::GetInstance()->GetId()) + "/" + USER_INFO_PROFILEPICTURE_PATH_NAME, imageDateStr, std::ios::binary);
    ui->userName->setText(QString::fromStdString(userName));
    ui->motto->setText(QString::fromStdString(motto));
    ui->profilePicture->SetImage(imageDateStr);
}

void Mnue::SetProfilePicture(std::string buffer) {
    ui->profilePicture->SetImage(buffer);
}

void Mnue::on_buttonSetting_clicked()
{
    Setting *settingWin = new Setting();
    UiManager::GetInstance()->OpenWin(settingWin);
}

void Mnue::SetProfilePictureS(UserInfo_t* param) {
    UpdateUserInfo(*param);
}


