#ifndef MESSAGE_H
#define MESSAGE_H
#include"../Manager/Manager.h"
#include <QWidget>

namespace Ui {
class Message;
}

class Message : public QWidget
{
    Q_OBJECT

public:
    explicit Message(uint16_t recvId,QWidget *parent = nullptr);
    ~Message();

private slots:
    void on_pushButton_clicked();
protected:
    void closeEvent(QCloseEvent* event) override;

private:
    Ui::Message *ui;


    friend class UiManager;
private:
    uint16_t recvId;

public:
    void AddRecvMessage(std::string message,uint64_t times);
    void AddSendMessage(std::string message,uint64_t times);
};

#endif // MESSAGE_H
