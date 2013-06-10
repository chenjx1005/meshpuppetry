#include <QtGui>
#include "window.h"
#include "ui_window.h"
#include "glwidget.h"

extern QString filename;

Window::Window(QMainWindow *parent) :
    QMainWindow(parent),
		ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	glWidget = new GLWidget();
	ui->verticalLayout_3->addWidget(glWidget);
	connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),glWidget,SLOT(setalpha(int)));
	connect(ui->horizontalSlider_2,SIGNAL(valueChanged(int)),glWidget,SLOT(setradius(int)));
	
	connect(ui->radioButton,SIGNAL(clicked()),glWidget,SLOT(selectfix()));
	connect(ui->radioButton_2,SIGNAL(clicked()),glWidget,SLOT(selectnew()));
	connect(ui->radioButton_3,SIGNAL(clicked()),glWidget,SLOT(setmethod1()));
	connect(ui->radioButton_4,SIGNAL(clicked()),glWidget,SLOT(setmethod2()));
	connect(ui->radioButton_5,SIGNAL(clicked()),glWidget,SLOT(setmethod3()));
	
	connect(ui->pushButton,SIGNAL(clicked()),glWidget,SLOT(freshv()));

    //connect(ui->checkBox,SIGNAL(clicked(bool)),...);
    //connect(ui->spinBox,SIGNAL(valueChanged(int)),...);
	createmenu();
}

Window::~Window()
{
    delete ui;
}

void Window::createmenu()
{
	menu=menuBar()->addMenu(tr("&File"));
	menu->addAction(tr("&Open..."),this,SLOT(openfile()),QKeySequence::Open);
	menu->addAction(tr("&Export..."),this,SLOT(savefile()),QKeySequence::SaveAs);
}

void Window::openfile()
{
    filename = QFileDialog::getOpenFileName(this);
	glWidget->ReLoad();
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
	case Qt::Key_G :{glWidget->left();break;}
	case Qt::Key_J :{glWidget->right();break;}
	case Qt::Key_Y :{glWidget->up();break;}
	case Qt::Key_H :{glWidget->down();break;}
    case Qt::Key_P :{glWidget->perspchange();break;}
    case Qt::Key_E :{glWidget->methodchange();break;}
    case Qt::Key_O :{glWidget->wirechange();break;}
	case Qt::Key_M :{glWidget->transform();break;}
	case Qt::Key_N :{	glWidget->backup();
						glWidget->setalpha(ui->horizontalSlider->value());
						break;	}

    }
}

void Window::savefile()
{
	QString ExportfileName = QFileDialog::getSaveFileName(this);
    if (!ExportfileName.isEmpty()) {
		glWidget->Export(ExportfileName);
    }
}
