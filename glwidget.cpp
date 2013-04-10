#include <QtGui>
#include <QtOpenGL>
#include <gl/GLU.h>

 #include <math.h>

 #include "glwidget.h"
#include "armadillo.h"

 #ifndef GL_MULTISAMPLE
 #define GL_MULTISAMPLE  0x809D
 #endif

#define BUFSIZE 1024
#define pi 3.14159265359

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
 {
     a = 0;
     xRot = 0;
     yRot = 0;
     zRot = 0;
     bPersp = bWire = method = false;
     wwidth = wheight =0;
     eye[0] = 0;
     eye[1] = 0;
     eye[2] = 8;
     center[0] = center[1] = center[2] =0;
     deep = 0;
	 rubberIsShown=false;//不显示选框
 }

 GLWidget::~GLWidget()
 {
 }

 QSize GLWidget::minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize GLWidget::sizeHint() const
 {
     return QSize(640, 640);
 }

 void GLWidget::initializeGL()
 {
     qglClearColor(Qt::black);

     a = new armadillo;
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glShadeModel(GL_SMOOTH);
     glEnable(GL_LIGHTING);
     glEnable(GL_LIGHT0);
     glEnable(GL_MULTISAMPLE);
     static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
     glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
 }

 void GLWidget::paintGL()
 {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();

     gluLookAt(eye[0],eye[1],eye[2],center[0],center[1],center[2],0,1,0);
     if (bWire) {
             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
     else {
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         }
     glColorMaterial(GL_FRONT,GL_DIFFUSE);
     glEnable(GL_COLOR_MATERIAL);
     glEnable(GL_NORMALIZE);
     glScalef(0.02f, 0.02f, 0.02f);
     //glTranslatef(0.0, 0.0, -10.0);
     glRotatef(xRot, 1.0, 0.0, 0.0);
     glRotatef(yRot+180, 0.0, 1.0, 0.0);
     //glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
     if(!method)
     {a->draw_scene();
         //glTranslatef(50.0, 0.0, 0.0);
         //armadillo *b = new armadillo;
         //b->draw_scene();
     }
     else
         a->draw_vertex();
	 if(rubberIsShown)//如果显示选框
	 {
	     /*glMatrixMode(GL_PROJECTION);
		 glPushMatrix();
		 glLoadIdentity();
		 glOrtho(0,width(),height(),0,0,100);//?
		 glMatrixMode(GL_MODELVIEW);
		 glPushMatrix();
		 glLoadIdentity();*/
		 glColor4f(1,1,0,0.7);
		 glEnable(GL_BLEND);
		 glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		 glBegin(GL_QUADS);
				 //glNormal3f(0,0,-1);
				 glVertex2f(rubbercorner1.x(),rubbercorner1.y());
				 glVertex2f(rubbercorner1.x(),rubbercorner2.y());
				 glVertex2f(rubbercorner2.x(),rubbercorner2.y());
				 glVertex2f(rubbercorner2.x(),rubbercorner1.y());
		 glEnd();
		 glDisable(GL_BLEND);
		 glColor3f(1,1,1);
		 /*glPopMatrix();
		 glMatrixMode(GL_PROJECTION);
		 glPopMatrix();
		 glMatrixMode(GL_MODELVIEW);
		 updateGL();*/
	 }
 }

 void GLWidget::resizeGL(int w, int h)
 {
     wwidth = w;
     wheight = h;
     int side = qMin(w, h);
     glViewport((w - side) / 2, (h - side) / 2, side, side);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     if (!bPersp){
             gluPerspective(45,1,0.1f,100);
         }
     else
             glOrtho(-3 ,3, -3, 3,-100,100);
     glMatrixMode(GL_MODELVIEW);
 }

void GLWidget::startPicking(int x, int y)
{
    GLint viewport[4];
    glSelectBuffer(BUFSIZE, selectBuf);

    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix(x, viewport[3]-y, 5, 5, viewport);
        //gluPerspective(60, 1, 0.1, 1000);
    if (!bPersp) {
        gluPerspective(45,1,0.1f,100);
            //glFrustum(-3, 3, -3, 3, 3,100);
    } else {
        glOrtho(-3 ,3, -3, 3,-100,100);
    }
    glMatrixMode(GL_MODELVIEW);
    glInitNames();
    glPushName(0);
}

void GLWidget::processHits(int hits, GLuint buffer[])
{
    unsigned int i, j;
    GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

    qDebug("hits = %d\n", hits);
    ptr = (GLuint *)buffer;
    minZ = 0xffffffff;

    if(hits!=-1){
    for (i=0; i < hits; i++) {
        names = *ptr;
        ptr++;

        if (*ptr < minZ) {
            numberOfNames = names;
            minZ = *ptr;
            ptrNames = ptr+2;
        }

        ptr += names+2;
    }
    qDebug("The closest hit names are ");
    ptr = ptrNames;
	if(a->reselect()==0)
		a->setptr(*ptr);
	else
		a->setfixv(*ptr);
    for (j=0; j<numberOfNames; j++, ptr++) {
        qDebug("%d ", *ptr);
    }
    }
    qDebug("\n");
}

void GLWidget::stopPicking()
{
    int hits;

    // restoring the original projection matrix
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    // returning to normal rendering model
    hits = glRenderMode(GL_RENDER);

    // if there are hits process them
    if (hits != 0)
        processHits(hits, selectBuf);
}

 void GLWidget::mousePressEvent(QMouseEvent *event)//单击鼠标后执行的事件
 {
     lastPos = event->pos();
	 rubbercorner1=rubbercorner2=event->pos();
     /*if(event->buttons() & Qt::LeftButton)//单击左键执行选点操作
     {
		 startPicking(lastPos.x(),lastPos.y());
         a->draw_vertex();
         stopPicking();
         GLint    viewport[4];
         glGetIntegerv(GL_VIEWPORT, viewport);
         glReadPixels(event->x(),viewport[3]-event->y(), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &deep);//记录移动点的深度信息
         updateGL();
     }*/
 }

 void GLWidget::mouseMoveEvent(QMouseEvent *event)//拖动鼠标执行的事件
 {
     if (event->buttons() & Qt::MidButton) {//拖动鼠标中键旋转视角
		 GLfloat dx = GLfloat(event->x()-lastPos.x()) / width();
		 GLfloat dy = GLfloat(event->y()-lastPos.y()) / height();
         xRot += dy * 180;
         yRot += dx * 180;
         updateGL();
		 lastPos = event->pos();
     }

	 if((event->buttons() & Qt::RightButton)&&(a->getptr() != -1)&&(a->reselect()==0))//拖动鼠标右键移动选中点
     {
         /*float x = 0.2*dx * cos(yRot /16 * pi /180);
         float y = -0.2*dy * cos(xRot /16 * pi /180);
         float z = 0.2*(dx * sin(yRot /16 * pi /180) -dy * sin(xRot /16 * pi /180));*/
         GLint    viewport[4];
         GLdouble modelview[16];
         GLdouble projection[16];
         GLdouble  winX, winY;
         GLdouble posX, posY, posZ;

         glPushMatrix();
         glMatrixMode(GL_MODELVIEW);
         glGetIntegerv(GL_VIEWPORT, viewport); // 得到的是最后一个设置视口的参数
         glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
         glGetDoublev(GL_PROJECTION_MATRIX, projection);
         glPopMatrix();

         winX = event->x();
         winY = viewport[3]-event->y();
         gluUnProject(winX, winY, deep, modelview, projection, viewport, &posX, &posY, &posZ);
         a->movevertex(posX,posY,posZ);
         updateGL();
		 lastPos = event->pos();
     }

	 if(event->buttons() & Qt::LeftButton)
	 {
		 rubberIsShown=true;
		 rubbercorner2=event->pos();
	 }
     
 }

 void GLWidget::mouseReleaseEvent(QMouseEvent *event)
 {
	 if(event->button()==Qt::LeftButton)//单击左键执行选点操作
     {
		 int x=(rubbercorner1.x()+rubbercorner2.x())/2;
		 int y=(rubbercorner1.y()+rubbercorner2.y())/2;
		 startPicking(x,y);
         a->draw_vertex();
         stopPicking();
         GLint    viewport[4];
         glGetIntegerv(GL_VIEWPORT, viewport);
         glReadPixels(x,viewport[3]-y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &deep);//记录移动点的深度信息
         updateGL();
     }
	 if(rubberIsShown)
	 {
		 rubberIsShown=false;
	 }
 }

 void GLWidget::xleft()
 {
     eye[0] += 0.2f;
     center[0] += 0.2f;
     updateGL();
 }

 void GLWidget::xright()
 {
     eye[0] -= 0.2f;
     center[0] -= 0.2f;
     updateGL();
 }

 void GLWidget::yup()
 {
     eye[1] -= 0.2f;
     center[1] -= 0.2f;
     updateGL();
 }

 void GLWidget::ydown()
 {
     eye[1] += 0.2f;
     center[1] += 0.2f;
     updateGL();
 }

 void GLWidget::znear()
 {
     eye[2] *= 0.95;
     updateGL();
 }

 void GLWidget::zfar()
 {
     eye[2] *= 1.05;
     updateGL();
 }

 void GLWidget::methodchange()
 {
     method = !method;
     updateGL();
 }

 void GLWidget::perspchange()
 {
     bPersp = !bPersp;
     resizeGL(wwidth,wheight);
     updateGL();
 }

 void GLWidget::wirechange()
 {
     bWire = !bWire;
     updateGL();
 }
 
 void GLWidget::selectfix()
 {
	 a->cselect(true);
	 updateGL();
 }

 void GLWidget::selectnew()
 {
	 a->cselect(false);
	 updateGL();
 }

 void GLWidget::setalpha(int value)
 {
	 a->setalpha(value);
 }

 void GLWidget::freshv()
 {
	 a->refreshfix();
	 updateGL();
 }