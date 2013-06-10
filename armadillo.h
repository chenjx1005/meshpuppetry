#ifndef ARMADILLO_H
#define ARMADILLO_H

#include <QObject>
#include <gl/glut.h>
#include <vector>
#include "common.h"
#include "..\Matrix\Matrix\Spatrix.h"

class armadillo : public QObject
{
    Q_OBJECT
public:
    explicit armadillo(QObject *parent = 0);
	~armadillo();
    void draw_scene();
    void draw_vertex();
    void setptr(int p);
    int getptr();
    void movevertex(float x,float y,float z);
	void setfixv(int p);
	void setalpha(int value);
	void setmethod(int m);
	void setradius(int r);
	int reselect();  //返回当前的armadillo选点方法
	void laplacian(int t);
	void transform();
	void initialTrans();
	void InitialModel(QString filename);
	void Export(QString Exportname);
	void up();
	void down();
	void left();
	void right();

private:
    int cptr;
	int fixv[20];//数组fixv记录指定不变的点的index， fixv_num是点的数量
    int fixv_num;
	float alpha; //两个约束的权重，数值为0到1，由UI上设计一个滑块控制
	int select;  //当前选择的点是固定点还是移动点，1为固定点，0为移动点

	int method;
	float radius;

	Matrix * vertex;
	Matrix * normal;
	int * face;
	int nvertex, nface;

	//method 3
	std::vector<float> * coff;
	std::vector<int> * topo;
	Matrix * face_normal;

	Spatrix sLtL, sAtA, sA;
	Spatrix sL, sLt;
	Matrix *LtLx, *cLtLx;
	Matrix *A, *xhat, *len;

	Matrix * ori, * Lx;
signals:
    
public slots:
	void refreshfix();//删除已选的固定点
	void cselect(bool value);//改变当前的选点方法
    
};

#endif // ARMADILLO_H
