#pragma once
#ifndef ADDFIREND_H
#define ADDFIREND_H
#include"../Manager/Manager.h"

#include <QWidget>

namespace Ui {
class AddFirend;
}

class AddFirend : public QWidget
{
    Q_OBJECT

public:
    explicit AddFirend(std::string userName, uint16_t userId, QWidget* parent = nullptr);
    ~AddFirend();

private slots:
    void on_ignore_clicked();

    void on_Confirm_clicked();

private:
    Ui::AddFirend *ui;
private:
    std::string m_userName;
    uint16_t m_userId;
};

#endif // ADDFIREND_H
