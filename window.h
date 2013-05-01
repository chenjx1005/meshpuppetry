#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class GLWidget;

namespace Ui {
class Window;
}

class Window : public QWidget
{
    Q_OBJECT
    
public:
    explicit Window(QWidget *parent =0);
    ~Window();
    void keyPressEvent(QKeyEvent *event);
    
private:
	Ui::Window *ui;
    GLWidget *glWidget;
};

#endif // WINDOW_H
