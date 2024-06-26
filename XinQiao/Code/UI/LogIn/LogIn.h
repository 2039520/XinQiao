#pragma once
#ifndef XINQIAO_H
#define XINQIAO_H
#include <QtWidgets/QWidget>
#include "ui_LogIn.h"
#include"../Manager/Manager.h"

class XinQiao : public QWidget
{
    Q_OBJECT
public:
    XinQiao(QWidget* parent = nullptr);
    ~XinQiao();


private slots:
    void on_LoginButton_clicked();
    void on_SignButton_clicked();

public:
    Ui::XinQiaoClass ui;
};
#endif // XINQIAO_H



