#include "addfirend.h"
#include "ui_addfirend.h"


AddFirend::AddFirend(std::string userName, uint16_t userId,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AddFirend)
{
    ui->setupUi(this);
    this->m_userId = userId;
    this->m_userName = userName;
    ui->label->setText((userName + "requests to be added as a friend !").c_str());
} 

AddFirend::~AddFirend()
{
    delete ui;
}


void AddFirend::on_ignore_clicked()
{
    this->destroy();
}


void AddFirend::on_Confirm_clicked()
{
    PdChat206 msg;
    msg.m_MOrC = ADD_FRIEND;
    msg.m_value.addFriend.addId = this->m_userId;
    msg.m_value.addFriend.srcId = UiManager::GetInstance()->GetId();
    msg.m_value.addFriend.yn = 1;
    Transfer::GetInstance()->AddPdMessage(msg);
    this->destroy();
}

