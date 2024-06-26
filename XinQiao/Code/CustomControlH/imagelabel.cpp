#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent)
{
}



void ImageLabel::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    if (pixmap().isNull()) {
        QLabel::paintEvent(event); // 如果没有 pixmap，使用默认绘制
        return;
    }

    // 按比例缩放图片以适应 QLabel 的宽度
    QPixmap scaledPixmap = pixmap().scaledToWidth(width(), Qt::SmoothTransformation);

    // 计算目标绘制矩形
    QRect targetRect(0, 0, scaledPixmap.width(), scaledPixmap.height());

    // 裁剪绘制区域到 QLabel 的大小
    QRect sourceRect = targetRect.intersected(QRect(0, 0, width(), height()));

    // 绘制图片
    painter.drawPixmap(0, 0, scaledPixmap, sourceRect.x(), sourceRect.y(), sourceRect.width(), sourceRect.height());
}

void ImageLabel::SetImage(std::string imageDataStr) {
    // 将字符串数据转换为 QByteArray 
    QByteArray byteArray(imageDataStr.c_str(), imageDataStr.size());

    // 将 QByteArray 转换为 QPixmap
    QPixmap pixmap;
    pixmap.loadFromData(byteArray);

    this->setPixmap(pixmap);
}
