#ifndef ARMADILLO_H
#define ARMADILLO_H

#include <QObject>
#include <gl/glut.h>
#include <vector>

using namespace std;

#define VERTEX 172974
#define FACE 345944

class armadillo : public QObject
{
    Q_OBJECT
public:
    explicit armadillo(QObject *parent = 0);
    void draw_scene();
    void draw_vertex();
    void setptr(int p);
    int getptr();
    void movevertex(float x,float y,float z);
	void setfixv(int p);
	void setalpha(int value);
	int reselect();//返回当前的armadillo选点方法
private:
    int cptr;
    GLfloat Vertex[VERTEX][3];
    GLfloat Normal[VERTEX][3];
    unsigned int Face[FACE][3];
	//int fixv[20];//数组fixv记录指定不变的点的index， fixv_num是点的数量
	vector <int> fixv;
    //int fixv_num;
    int newv[20]; //newv记录指定移动的点的index
    int newvp[20][3];//newvp储存对应的指定位置
    int newv_num;//newv_num是点的数量
	float alpha;//两个约束的权重，数值为0到1，由UI上设计一个滑块控制
	int select;//当前选择的点是固定点还是移动点，1为固定点，0为移动点
signals:
    
public slots:
	void refreshfix();//删除已选的固定点
	void cselect(bool value);//改变当前的选点方法
    
};

#endif // ARMADILLO_H
