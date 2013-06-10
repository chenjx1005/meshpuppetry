#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class GLWidget;
class QMenu;

namespace Ui {
class MainWindow;
}

class Window : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Window(QMainWindow *parent = 0);
    ~Window();
    void keyPressEvent(QKeyEvent *event);
    
private:
	void createmenu();
	QMenu *menu;
	Ui::MainWindow *ui;
    GLWidget *glWidget;

private slots:
	void openfile();
	void savefile();
};

#endif // WINDOW_H
