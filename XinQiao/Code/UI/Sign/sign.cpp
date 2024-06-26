#include "sign.h"
#include "ui_sign.h"
Sign::Sign(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Sign)
{
    ui->setupUi(this);
}

Sign::~Sign()
{
    delete ui;
}

void Sign::on_ConfirmSign_clicked()
{
    
    if (ui->PwEdit->text().toUInt() == 0 || ui->PwEdit->text().toUInt() >= 65535 || ui->UserNameEdit->text().toStdString() == "" || ui->PwEdit->text().toUInt() != ui->ConfirmPwEdit->text().toUInt()) {
        QMessageBox::information(nullptr, "err", "Registration information is incorrect");
        return;
    }
    if (ui->PwEdit->text().toUInt() == ui->ConfirmPwEdit->text().toUInt()) {
        uint16_t id;
        SignUser(ui->UserNameEdit->text().toStdString(), ui->PwEdit->text().toUInt(), id);
        UiManager::GetInstance()->JumpBack();
    }
}


void Sign::on_pushButton_clicked()
{
    UiManager::GetInstance()->JumpBack();
}

