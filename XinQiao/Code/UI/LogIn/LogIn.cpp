#pragma once
#ifndef XIN
#define XIN
#include"LogIn.h"

XinQiao::XinQiao(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

XinQiao::~XinQiao()
{}

void XinQiao::on_LoginButton_clicked()
{
    QString id = ui.UserIdEdit->text();
    QString pw = ui.PwEdit->text();
    uint temp = pw.toUInt();
    if (temp > 0xffff || temp==0) {
        QMessageBox::information(nullptr, "err", "Password format error");
        return;
    }
    LogIn(id.toUShort(), pw.toUShort());
}
void XinQiao::on_SignButton_clicked()
{
    Sign* window = new Sign();
    UiManager::GetInstance()->JumpTo(window);
}

#endif // !XIN


