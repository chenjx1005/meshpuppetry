#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>


class armadillo;
class QTextBrowser;

 class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
     GLWidget(QWidget *parent = 0);
     ~GLWidget();
     QSize minimumSizeHint() const;
     QSize sizeHint() const;

 public slots:
     void xleft();
     void xright();
     void yup();
     void ydown();
     void znear();
     void zfar();
	 void left();
	 void right();
	 void up();
	 void down();
     void methodchange();
     void perspchange();
     void wirechange();
	 void selectfix();
	 void selectnew();
	 void setalpha(int value);
	 void setradius(int r);
	 void setmethod1();
	 void setmethod2();
	 void setmethod3();
	 void freshv();
	 void transform();
	 void backup();
	 void ReLoad();
	 void Export(QString Exportname);
 signals:

 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int w, int h);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void startPicking(int x,int y);
     void stopPicking(int x, int y);
     void processHits(int hits,GLuint* selectBuf);
	 
 public:
	 QTextBrowser *text;
 private:
	 armadillo *a;
     int xRot;
     int yRot;
     int zRot;
     QPoint lastPos;
     bool bPersp,bWire,method;
     int wwidth,wheight;
     float eye[3],center[3];
     GLuint selectBuf[1024];
     GLfloat deep;
	 QPoint rubbercorner1;
	 QPoint rubbercorner2;
	 bool rubberIsShown;
 };

 #endif
