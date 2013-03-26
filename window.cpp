#include <QtGui>
#include "window.h"
#include "ui_window.h"
#include "glwidget.h"

Window::Window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Window)
{
    ui->setupUi(this);
    glWidget = new GLWidget;
    ui->verticalLayout->addWidget(glWidget);
	connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),glWidget,SLOT(setalpha(int)));
	connect(ui->radioButton,SIGNAL(clicked()),glWidget,SLOT(selectfix()));
	connect(ui->radioButton_2,SIGNAL(clicked()),glWidget,SLOT(selectnew()));
	connect(ui->pushButton,SIGNAL(clicked()),glWidget,SLOT(freshv()));
    //connect(ui->checkBox,SIGNAL(clicked(bool)),...);
    //connect(ui->spinBox,SIGNAL(valueChanged(int)),...);
}

Window::~Window()
{
    delete ui;
}

void Window::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()){
    //if (e->key() == Qt::Key_Escape)
        //close();
    //else
        //QWidget::keyPressEvent(e);
    case Qt::Key_Escape :{close();break;}
    case Qt::Key_A :{glWidget->xleft();break;}
    case Qt::Key_D :{glWidget->xright();break;}
    case Qt::Key_W :{glWidget->yup();break;}
    case Qt::Key_S :{glWidget->ydown();break;}
    case Qt::Key_Z :{glWidget->znear();break;}
    case Qt::Key_C :{glWidget->zfar();break;}
    case Qt::Key_P :{glWidget->perspchange();break;}
    case Qt::Key_E :{glWidget->methodchange();break;}
    case Qt::Key_O :{glWidget->wirechange();break;}
    }
}
