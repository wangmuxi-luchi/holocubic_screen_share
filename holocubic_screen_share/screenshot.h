#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QWidget>
#include<QPixmap>



class Screenshot : public QWidget
{
    Q_OBJECT
public:
    explicit Screenshot(QWidget *parent = 0);
    ~Screenshot();

signals:

public:
    void shootScreen();
    QPixmap originalPixmap;


};

#endif // SCREENSHOT_H
