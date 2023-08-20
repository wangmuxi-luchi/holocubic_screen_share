#include "screenshot.h"
#include <QScreen>
#include <QGuiApplication>



Screenshot::Screenshot(QWidget *parent) : QWidget(parent)
{
    shootScreen();

}

Screenshot::~Screenshot()
{

}


//void Screenshot::newScreenshot(){
//    QTimer::singleShot(5000 , this ,SLOT(shootScreen()));

//}


//void Screenshot::saveScreenshot(){


//    QString format = "png";
//    QString initialPath = QDir::currentPath() + tr("/untitled.")+ format;

//    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), initialPath,
//                                                        tr("%1 Files (*.%2);;All Files (*)").arg(format.toUpper()).arg(format));


//    if(!fileName.isEmpty()){

//        originalPixmap.save(fileName , format.toLatin1().constData());

//    }



//}


void Screenshot::shootScreen(){

    originalPixmap = QPixmap();

    QScreen * screen = QGuiApplication::primaryScreen();

    if(screen){
        originalPixmap = screen->grabWindow(0);

    }


}



//void Screenshot::updateScreenshotLabel(){


//    screenshotLabel->setPixmap(originalPixmap.scaled(screenshotLabel->size() , Qt::KeepAspectRatio,
//                                                     Qt::SmoothTransformation));
//}
