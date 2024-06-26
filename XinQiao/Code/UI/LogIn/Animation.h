// #ifndef ANIMATION_H
// #define ANIMATION_H
// #include <QPropertyAnimation>
// #include <QSequentialAnimationGroup>
// #include <QColor>
// #include <QPushButton>
// #include <qobject.h>

// void BottonRainBow(QObject &button){
//     //// 创建彩虹颜色扫描动画
//     //QSequentialAnimationGroup animationGroup;

//     //for (int i = 0; i < 7; ++i) {
//     //    // 设置按钮初始颜色和结束颜色
//     //    QColor startColor = QColor::fromHsv((i * 30) % 360, 255, 255);
//     //    QColor endColor = QColor::fromHsv(((i + 1) * 30) % 360, 255, 255);

//     //    // 创建颜色属性动画
//     //    QPropertyAnimation *colorAnimation = new QPropertyAnimation(&button, "styleSheet");
//     //    colorAnimation->setStartValue(QString("background-color: %1").arg(startColor.name()));
//     //    colorAnimation->setEndValue(QString("background-color: %1").arg(endColor.name()));
//     //    colorAnimation->setDuration(500);  // 设置动画持续时间

//     //    // 将颜色动画添加到动画组
//     //    animationGroup.addAnimation(colorAnimation);
//     //}
//     //// 连接按钮点击信号槽
//     //QObject::connect(&button, &QPushButton::clicked, [&]() {
//     //    // 启动动画组
//     //    animationGroup.start();
//     //});
// }

// #endif // ANIMATION_H
