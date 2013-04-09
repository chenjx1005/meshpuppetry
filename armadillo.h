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
	int reselect();//���ص�ǰ��armadilloѡ�㷽��
private:
    int cptr;
    GLfloat Vertex[VERTEX][3];
    GLfloat Normal[VERTEX][3];
    unsigned int Face[FACE][3];
	//int fixv[20];//����fixv��¼ָ������ĵ��index�� fixv_num�ǵ������
	vector <int> fixv;
    //int fixv_num;
    int newv[20]; //newv��¼ָ���ƶ��ĵ��index
    int newvp[20][3];//newvp�����Ӧ��ָ��λ��
    int newv_num;//newv_num�ǵ������
	float alpha;//����Լ����Ȩ�أ���ֵΪ0��1����UI�����һ���������
	int select;//��ǰѡ��ĵ��ǹ̶��㻹���ƶ��㣬1Ϊ�̶��㣬0Ϊ�ƶ���
signals:
    
public slots:
	void refreshfix();//ɾ����ѡ�Ĺ̶���
	void cselect(bool value);//�ı䵱ǰ��ѡ�㷽��
    
};

#endif // ARMADILLO_H
