#include <QApplication>
 #include <QDesktopWidget>
 #include "window.h"

//extern void initial();
QString filename = "";

 int main(int argc, char *argv[])
 {
	 //initial();
     QApplication app(argc, argv);
     Window window;
     window.resize(window.sizeHint());
     int desktopArea = QApplication::desktop()->width() *
                    QApplication::desktop()->height();
     int widgetArea = window.width() * window.height();
     if (((float)widgetArea / (float)desktopArea) < 0.75f)
         window.show();
     else
         window.showMaximized();
     return app.exec();
 }
