#ifndef _SPATRIX_H_
#define _SPATRIX_H_

#include "Matrix.h"
#include <mkl_spblas.h>
#include <mkl_pardiso.h>

/* only for dss solver
#include "mkl_dss.h"
#if defined(MKL_ILP64)
#define MKL_INT long long
#else
#define MKL_INT int
#endif
*/

class Spatrix
{	
public:
	//CSR format arrays
	float * value;
	int * column;
	int * index;
	int row, col, size;

	//Pardiso related paramters
	static int pt[64], maxfct, mnum, mtype, phase;
	static int iparm[64], msglvl, error, idum;
	static double ddum;
	
	//constructors and destructors
	Spatrix() throw(): row(0), col(0), size(0),
		value(NULL), column(NULL), index(NULL){}
	Spatrix(int r, int c, int s) throw(): row(r), col(c), size(s)
	{
		value = new float[size];
		column = new int[size];
		index = new int[row+1];
	}
	Spatrix(const Matrix& m) throw(): row(m.row), col(m.col), size(0)
	{
		int l = row * col;
		for(int i=0; i<l; ++i)
		{
			if(m.num[i]!=0)
				++size;
		}
		value = new float[size];
		column = new int[size];
		index = new int[row+1];
		this->sparse(m);
	}
	Spatrix(const Spatrix& s) throw(): row(s.row), col(s.col), size(s.size)
	{
		value = new float[size];
		column = new int[size];
		index = new int[row+1];
		cblas_scopy(size, s.value, INC, value, INC); //
		memcpy(column, s.column, sizeof(int)*size);  //
		memcpy(index, s.index, sizeof(int)*(row+1));
	}
	~Spatrix() throw()
	{
		if(value != NULL) delete [] value;
		if(column != NULL) delete [] column;
		if(index != NULL) delete [] index;
	}

	//dense and sparse conversion
	void dense(Matrix& m) throw()
	{
		if(index[0] == 1) this->zerobase();
		assert(m.row==row && m.col==col);
		m.zerolize();
		for(int i=0; i<row; ++i)
			for(int j=index[i]; j<index[i+1]; ++j)
				*(m.num+i*col+column[j]) = value[j]; //
	}
	//be careful about the uorl
	bool sparse(const Matrix& m, char uorl=0) throw()
	{
		row = m.row, col = m.col;
		int l = row * col;
		for(int i=0; i<l; ++i)
		{
			if(m.num[i]!=0)
				++size;
		}
		if(value != NULL) delete [] value;
		if(index != NULL) delete [] index;
		if(column != NULL) delete [] column;
		column = new int[size];
		value = new float[size];
		index = new int[row+1];

		int flag, job[6] = {0, 0, 0, 2, size, 1}; //
		if(uorl=='U' || uorl=='u')	job[3] = 1;
		else if (uorl=='L' || uorl=='l')	job[3] = 0;
		if(job[3]!=2 && row!=col)	return false;
		mkl_sdnscsr(job, &row, &col, m.num, &col, value, column, index, &flag);
		return flag?false:true;
	}
	//before call of ssadd or ssmult
	void onebase() throw()
	{
		for(int i=0; i<size; ++i)
			++ column[i];
		for(int i=0; i<=row; ++i)
			++ index[i];
	}
	//before call of other function
	void zerobase() throw()
	{
		for(int i=0; i<size; ++i)
			-- column[i];
		for(int i=0; i<=row; ++i)
			-- index[i];
	} 
	//dispaly the csr parameters
	void show() const throw()
	{
		printf("%d\t%d\t%d\n", row, col, size);
		for(int i=0; i<size; i++)	printf("%f\t", value[i]);	printf("\n"); //
		for(int i=0; i<size; i++)	printf("%d\t", column[i]);	printf("\n"); //
		for(int i=0; i<=row; i++)	printf("%d\t", index[i]);	printf("\n"); //
	}
	
	//computation
	Spatrix& operator=(const Spatrix& s) throw()
	{
		if(size != s.size)
		{
			if(value != NULL) delete [] value;
			if(column != NULL) delete [] column;
			size = s.size;
		}
		if(row != s.row)
		{
			if(index != NULL) delete [] index;
			row = s.row;
		}
		col = s.col;
		value = new float[size];
		column = new int[size];
		index = new int[row+1];
		memcpy(value, s.value, sizeof(float)*size);
		memcpy(column, s.column, sizeof(int)*size);
		memcpy(index, s.index, sizeof(int)*(row+1));
		return *this;
	}

	Spatrix& operator*=(const float alpha) throw()
	{
		for(int i=0; i<size; ++i)
			value[i] *= alpha;
		return *this;
	}
	//data related
	void resize(int r, int c, int s) throw()
	{
		row = r;
		col = c;
		size = s;
		if(value != NULL)
			delete [] value;
		if(column != NULL)
			delete [] column;
		if(index != NULL)
			delete [] index;
		value = new float[size];
		column = new int[size];
		index = new int[row+1];
	}
	void read(const char* file) throw()
    {
        FILE *fp;
        fopen_s(&fp, file, "r");
        assert(fp);

        fscanf_s(fp, "%d %d %d", &row, &col, &size);
        resize(row, col, size);
		for(int i=0; i<size; ++i)
			fscanf_s(fp, "%f", value+i);
		for(int i=0; i<size; ++i)
			fscanf_s(fp, "%d", column+i);
		for(int i=0; i<=row; ++i)
			fscanf_s(fp, "%d", index+i);
		fclose(fp);
    }
	void write(const char* file) throw()
    {
        FILE *fp;
        fopen_s(&fp, file, "w");
        assert(fp);

        fprintf_s(fp, "%d %d %d\n", row, col, size);
        for(int i=0; i<size; ++i)
			fprintf_s(fp, "%f ", *(value+i));
		for(int i=0; i<size; ++i)
			fprintf_s(fp, "%d ", *(column+i));
		for(int i=0; i<=row; ++i)
			fprintf_s(fp, "%d ", *(index+i));
        fclose(fp);
    }

};

//b!=x, b and c is colomun major
bool sdsolve(Spatrix& a, Matrix& b, Matrix& x) throw();
//x input row major, output col major
bool sdsolve(Spatrix& a, Matrix& x) throw();
//be careful about t
void sdmult(float alpha, char t, Spatrix& a, Matrix& b, float beta, Matrix& c) throw();
//c can be a or b
bool ssadd(Spatrix& a, float beta, char t, Spatrix& b, Spatrix& c) throw();
//c can be a or b
bool ssmult(char t, Spatrix& a, Spatrix& b, Spatrix& c) throw();
#endif