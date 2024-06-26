#include "setting.h"
#include "ui_setting.h"
#include <QFileDialog>


Setting::Setting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Setting)
{
    ui->setupUi(this);
    connect(this, &Setting::DoSetProfilePictureS_QS, UiManager::GetInstance()->m_Mnue, &Mnue::SetProfilePictureS);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_findImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, QString::fromUtf8("open image file"), "", "Image Files (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty())
    {
        m_ProfilePicturePath = fileName.toStdString();
        QPixmap pixmap(fileName); 
        ReadFile(m_ProfilePicturePath, m_ProfilePictureBuffer, std::ios::binary | std::ios::out);
        ui->imageLabel->SetImage(m_ProfilePictureBuffer);
    }
}

 
void Setting::on_back_clicked()
{
    UiManager::GetInstance()->CloseWin(this);
}


void Setting::on_ok_clicked()
{
    std::string data;
    ReadFile(m_ProfilePicturePath, data, std::ios_base::binary);
    QString motto = ui->lineEdit_motto->text();
    QString userName = ui->lineEdit_userName->text();
    PdChat206 msg;
    msg.m_MOrC = USER_INFO;
    msg.m_value.UserInfo.id = UiManager::GetInstance()->GetId();
    msg.m_value.UserInfo.control = 1;
    msg.m_value.UserInfo.Update(userName.length(), motto.length(), data.length(), (const byte*)(userName.toStdString() + motto.toStdString() + data).c_str());
    emit DoSetProfilePictureS_QS(&msg.m_value.UserInfo);
    Transfer::GetInstance()->AddPdMessage(msg);
    UiManager::GetInstance()->CloseWin(this);
}

