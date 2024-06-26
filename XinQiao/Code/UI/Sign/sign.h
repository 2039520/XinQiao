#pragma once
#ifndef SIGN_H
#define SIGN_H
#include "../Manager/Manager.h"
#include <QWidget>

namespace Ui {
class Sign;
}

class Sign : public QWidget
{
    Q_OBJECT

public:
    explicit Sign(QWidget *parent = nullptr);
    ~Sign();

private slots:
    void on_ConfirmSign_clicked();

    void on_pushButton_clicked();

private:
    Ui::Sign *ui;
};
#endif // SIGN_H
