#include "message.h"
#include "ui_message.h"

Message::Message(uint16_t recvId, QWidget* parent)
	: QWidget(parent)
	, ui(new Ui::Message)
{
	ui->setupUi(this);
	this->recvId = recvId;
}

Message::~Message()
{
	delete ui;
}

void Message::on_pushButton_clicked()
{
	std::string message = ui->Edit->toPlainText().toStdString();
	ui->Edit->clear();
	time_t tim = time(nullptr);
	AddSendMessage(message, tim);
	SendXQMessage(this->recvId, UiManager::GetInstance()->GetId(), message, tim);
}

void Message::AddRecvMessage(std::string message, uint64_t times) {
	//[!]保存到本地聊天记录数据库
	std::tm* timeinfo = localtime((time_t*)&times);
	char str[20] = { '\0' };
	sprintf(str, "%d-%02d-%02d %02d:%02d\n\0", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min);
	std::string itemMessage = std::string(str)+message;
	QListWidgetItem* item = new QListWidgetItem(itemMessage.c_str());
	item->setForeground(Qt::red);
	ui->MessageWidget->addItem(item);
}
void Message::AddSendMessage(std::string message, uint64_t times) {
	//[!]保存到本地聊天记录数据库
	std::tm* timeinfo = localtime((time_t*)&times);
	char str[20] = { '\0' };
	sprintf(str, "%d-%02d-%02d %02d:%02d\n\0", timeinfo->tm_year, timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min);
	std::string itemMessage = std::string(str) + message;
	QListWidgetItem* item = new QListWidgetItem(itemMessage.c_str());
	item->setForeground(Qt::blue);
	ui->MessageWidget->addItem(item);
}

void Message::closeEvent(QCloseEvent* event) {
	UiManager::GetInstance()->DeleteMessageWindow(recvId);
	event->accept();
}

