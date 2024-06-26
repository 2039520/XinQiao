#ifndef SETTING_H
#define SETTING_H

#include <QWidget>
#include "../Manager/Manager.h"
namespace Ui {
class Setting;
}

class Setting : public QWidget
{
    Q_OBJECT
public:
    friend class UiManager;
    explicit Setting(QWidget *parent = nullptr);
    ~Setting();
private:
    std::string m_ProfilePicturePath;
    std::string m_ProfilePictureBuffer;
private slots:
    void on_back_clicked();

    void on_ok_clicked();

    void on_findImage_clicked();

signals: void DoSetProfilePictureS_QS(UserInfo_t* param);

private:
    Ui::Setting *ui;
};

#endif // SETTING_H
