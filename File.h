#ifndef _FILE_H_
#define _FILE_H_

#include <stdio.h>
#include <assert.h>

class file
{
private:
	FILE * fp;
	char buf;
public:
	file() throw(): fp(NULL), buf(0){}
	file(const char *path, const char *mode) throw(): buf(0)
	{
		fp = fopen(path, mode);
		assert(fp != NULL);
	}
	~file() throw()
	{
		if(fp != NULL)
			fclose(fp);
	}

	inline bool open(const char* path, const char* mode) throw()
	{
		if(fp != NULL)
			fclose(fp);
		fp = fopen(path, mode);
		return (fp != NULL);
	}
	inline int eof() const throw()
	{
		return feof(fp);
	}
	inline FILE* p() throw()
	{
		return fp;
	}

	inline char getch() throw()
	{
		return fgetc(fp);
	}
	inline int geti(int &i) throw()
	{
		return fscanf(fp, "%d", &i);
	}
	inline int getf(float &f) throw()
	{
		return fscanf(fp, "%f", &f);
	}
	inline int getd(double &d) throw()
	{
		return fscanf(fp, "%lf", &d);
	}
	inline void gets(char *s, char c='\n') throw()
	{
		int i = 0;
		while((s[i]=fgetc(fp)) != c)
			i ++;
		s[i] = 0;
	}

	inline int putch(const char& c) throw()
	{
		return fputc(c, fp);
	}
	inline int puti(const int& i) throw()
	{
		return fprintf(fp, "%d", i);
	}
	inline int putf(const float& f) throw()
	{
		return fprintf(fp, "%f", f);
	}
	inline int putd(const double& d) throw()
	{
		return fprintf(fp, "%lf", d);
	}
	inline int puts(const char* s) throw()
	{
		return fputs(s, fp);
	}
	inline bool ignore(char c='\n') throw()
	{
		char t;
		while((t=fgetc(fp)) != c)
		{	
			if(t == EOF) return false;
		}
		return true;
	}
};
	


#endif