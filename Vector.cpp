#include "natrix.h"
#include "matrix.h"

Vector& Vector::operator*=(const Matrix& m) throw()
{
	if(m.cols() != size)
		printf("Matrix.col doesn't match Vector.size in *=");
	else
	{
		Vector temp(*this);
		if(m.rows() > size)
		{
			delete [] data;
			size = m.rows();
			data = new float[size];
		}
		cblas_sgemv(R, N, m.rows(), m.cols(), 1.0f,
			m.nums(), m.cols(), temp.data, INC, 0.0f, data, INC);
	}
	return *this;
}

Vector& Vector::operator*=(const matrix& m) throw()
{
	if(m.cols() != size)
		printf("Matrix.col doesn't match Vector.size in *=");
	else
	{
		Vector temp(*this);
		if(m.rows() > size)
		{
			delete [] data;
			size = m.rows();
			data = new float[size];
		}
		cblas_sgemv(R, N, m.rows(), m.cols(), 1.0f,
			m.nums(), m.cols(), temp.data, INC, 0.0f, data, INC);
	}
	return *this;
}
