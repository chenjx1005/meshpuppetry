#include "natrix.h"

bool matrix::tolu() throw()
{
	assert(row == col);
	float temp;
	for(int i=1; i<row; i++)
	{
		for(int j=i; j<row; j++)
		{
			temp = 0;
			for(int k=0; k<i-1; k++)
				temp += *(num+j*col+k) * *(num+k*col+i-1);
			*(num+j*col+i-1) = (*(num+j*col+i-1)-temp)/ *(num+(i-1)*col+i-1);
		
			temp = 0;
			for(int k=0; k<i; k++)
				temp += *(num+i*col+k) * *(num+k*col+j); 
			*(num+i*col+j) = *(num+i*col+j) - temp;
		}
	}
	return true;
}

bool solve(const matrix& m, const Vector& v, Vector& r) throw()
{
	assert(m.row == v.size);
	int i, j;
	float temp;
	for(i=0; i<v.size; i++)
	{
		temp = 0;
		for(j=0; j<i; j++)
			temp += *(m.num+i*m.col+j) * *(r.data+j);
		*(r.data+i) = *(v.data+i) - temp;
	}
	for(i=v.size-1; i>=0; i--)
	{
		temp = 0;
		for(j=i+1; j<v.size; j++)
			temp += *(m.num+i*m.col+j) * *(r.data+j);
		if(fabs(*(m.num+i*m.col+i)) < ZERO)
			return false;
		*(r.data+i) = (*(r.data+i)-temp) / *(m.num+i*m.col+i);
	}
	return true;
}




