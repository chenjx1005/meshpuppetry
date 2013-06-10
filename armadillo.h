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
	int reselect();  //���ص�ǰ��armadilloѡ�㷽��
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
	int fixv[20];//����fixv��¼ָ������ĵ��index�� fixv_num�ǵ������
    int fixv_num;
	float alpha; //����Լ����Ȩ�أ���ֵΪ0��1����UI�����һ���������
	int select;  //��ǰѡ��ĵ��ǹ̶��㻹���ƶ��㣬1Ϊ�̶��㣬0Ϊ�ƶ���

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
	void refreshfix();//ɾ����ѡ�Ĺ̶���
	void cselect(bool value);//�ı䵱ǰ��ѡ�㷽��
    
};

#endif // ARMADILLO_H
