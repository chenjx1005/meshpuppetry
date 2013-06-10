#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <mkl.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>
#define  INC  1
#define  ZERO 1
class Matrix;
class matrix;

class Vector
{
private:
	float * data;
	int size;
public:
//-------------------------------------------------------
//------------constructor and destructor-----------------
//-------------------------------------------------------
	Vector() throw(): data(NULL), size(0){}
	Vector(const int& n) throw(): data(new float[n]), size(n){}
    Vector(const int& n, const float * p) throw(): size(n)
	{
		data = new float[size];
		cblas_scopy(size, p, INC, data, INC);
	}
	Vector(const Vector& v) throw(): size(v.size)
	{
		data = new float[size];
		cblas_scopy(size, v.data, INC, data, INC);
	}
	~Vector() throw()
	{
		if(data != NULL)
            delete [] data;
	}
//-------------------------------------------------------
//---------------characters of a Vector------------------
//-------------------------------------------------------
	inline float magsum() const throw(){
		return cblas_sasum(size, data, INC);
	}
	inline float norm2() const throw() {
		return cblas_snrm2(size, data, INC);
	}
	inline int maxindex() const throw(){
		return cblas_isamax(size, data, INC);
	}
	inline int minindex() const throw(){
		return cblas_isamin(size, data, INC);
	}
//-------------------------------------------------------
//------------------binary operation---------------------
//-------------------------------------------------------
	inline float dot(const Vector& v) const throw(){
        return cblas_sdot(size, data, INC, v.data, INC);
	}
    inline void rot(Vector& v, const float& c, const float& s) throw(){
		cblas_srot(size, data, INC, v.data, INC, c, s);
	}	//this = c*this + s*v; v = c*v - s*this;
	inline void swap(Vector& v) throw(){
		cblas_sswap(size, data, INC, v.data, INC);
	}
    inline void axpy(const float& a, const Vector& v) throw(){
		cblas_saxpy(size, a, v.data, INC, data, INC);
	}	//this = a*v + this
	inline void compare(const Vector& v) const throw()
	{
		bool flag = true;
		if(size != v.size)
			flag = false;
		else
		{
			for(int i=0; i<size; i++)
			{
				if(fabs(*(data+i)-*(v.data+i)) > ZERO)
				{
					flag = false;
					break;
				}
			}
		}
		if(flag == true)
			printf("true\n");
		else
			printf("false\n");
	}
//-------------------------------------------------------
//------------------unary operation----------------------
//-------------------------------------------------------
    inline float& operator[](const int& i) const throw(){
        return *(data+i);
	}
	inline float operator()(const int& i) const throw(){
        return *(data+i);
	}
	inline Vector operator-() const throw(){
		Vector v(*this);
		cblas_saxpy(size, -2.0f, data, INC, v.data, INC);
		return v;
	}
	inline void show() const throw()
	{
		for(int i=0; i<size; i++)
			printf("%f\t", *(data+i));
		printf("\n");
	}
//-------------------------------------------------------
//-------------operators that contain = -----------------
//-------------------------------------------------------
	inline Vector& operator=(const Vector& v) throw(){
		if(data != NULL)
			delete [] data;
		size = v.size;
		data = new float[size];
		cblas_scopy(size, v.data, INC, data, INC);
		return *this;
	}
	inline Vector& operator+=(const Vector& v) throw(){
		if(size != v.size)
		{
			printf("Vectors with different dimension can't be added\n");
			return *this;
		}
		cblas_saxpy(size, 1.0f, v.data, INC, data, INC);
		return *this;
	}
	inline Vector& operator-=(const Vector& v) throw()
	{
		if(size != v.size)
		{
			printf("Vectors with different dimension can't be subtracked\n");
			return *this;
		}
		cblas_saxpy(size, -1.0f, v.data, INC, data, INC);
		return *this;
	}
    inline Vector& operator*=(const float& a) throw(){
        cblas_sscal(size, a, data, INC);
		return *this;
    }
	Vector& operator*=(const Matrix& m) throw();
	Vector& operator*=(const matrix& m) throw();
	inline float operator*(const Vector& v) throw(){
		return v.dot(*this);
	}
//-------------------------------------------------------
//---------------global binary operation-----------------
//-------------------------------------------------------
    inline friend void swap(Vector& v1, Vector& v2) throw();
	inline friend void rot(Vector& v1, Vector& v2, const float& c, const float& s) throw();

	inline friend Vector operator+(const Vector& v1, const Vector& v2) throw();
	inline friend Vector operator-(const Vector& v1, const Vector& v2) throw();
    inline friend Vector operator*(const Vector& v, const float& a) throw();
    inline friend Vector operator*(const float& a, const Vector& v) throw();

	friend bool solve(const matrix& m, const Vector& v, Vector& r) throw();
	inline friend bool solve(const Matrix& m, Vector& v, char t) throw();
};

inline void rot(Vector& v1, Vector& v2, const float& c, const float& s) throw(){
        v1.rot(v2, c, s);
}
inline void swap(Vector& v1, Vector& v2) throw(){
		v1.swap(v2);
}

inline Vector operator+(const Vector& v1, const Vector& v2) throw(){
	Vector temp(v1);
	return (temp += v2);
}
inline Vector operator-(const Vector& v1, const Vector& v2) throw(){
	Vector temp(v1);
	return (temp -= v2);
}
inline Vector operator*(const float& a, const Vector& v) throw(){
	Vector temp(v);
	return (temp *= a);
}
inline Vector operator*(const Vector& v, const float& a) throw(){
	Vector temp(v);
	return (temp *= a);
}

#endif

	





	
	





