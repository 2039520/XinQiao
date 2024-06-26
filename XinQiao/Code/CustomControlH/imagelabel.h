#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include <QPainter>
#include <QLabel>

class ImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ImageLabel(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent* event);
    void SetImage(std::string imageData);
};

#endif // IMAGELABEL_H
