#pragma once
#ifndef MNUE_H
#define MNUE_H
#include <QStandardItem>
#include <QStandardItemModel>
#include <QWidget>
#include "../Manager/Manager.h"

#define USER_INFO_PATHR "Img/"
#define USER_INFO_PROFILEPICTURE_PATH_NAME "ProfilePicture/image.jpg"

namespace Ui {
class Mnue;
}

class Mnue : public QWidget
{
    Q_OBJECT

public:
    Ui::Mnue* ui;
    QStandardItemModel* m_FriendListModel;
    QStandardItem* m_FriendListItems;
    FriendInfo_t* m_FriendListInfo;
    uint16_t m_FriendListLength;
    UserInfo_t m_UserInfo;
public:
    //friend void UiManager::DoRefreshFirendList(PdChat206& msg);
public:
    explicit Mnue(QWidget *parent = nullptr);
    ~Mnue();
    friend class Setting;
private slots:
    void FirendListItemDoubleClicked(const QModelIndex& index);
    
    void on_pushButton_clicked();

    void on_buttonSetting_clicked();

    void SetProfilePictureS(UserInfo_t* buffer);

public:
    void RefreshFirendList();
    void UpdateUserInfo();
    void UpdateUserInfo(UserInfo_t info);
    void SetProfilePicture(std::string param);
};
#endif // MNUE_H
