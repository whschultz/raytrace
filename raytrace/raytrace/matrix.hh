//  file:  matrix.hh

#ifndef MATRIX
#define MATRIX

#define USEWITHVECTOR

#ifdef USEWITHVECTOR
#include "vector.hh"
#endif

#include <iostream>

#ifndef LLIST
typedef enum {noErr=0, dimMismatch, dimRange} Err;
#else

typedef enum {dimMismatch=1, dimRange} Err;
#endif

class matrix
{
public:
	//constructors, destructors
	matrix(int,int);
	matrix(void) { };
	~matrix(void) {deldata();}
	
	//simple ops
	float get(int,int);
	void set(int,int,float);
	void setdim(int,int);
	int  getdimr(void) {return dimr;}
	int  getdimc(void) {return dimc;}
	
	//arithmetic ops
	friend matrix  operator+(matrix,matrix);
	friend matrix  operator*(matrix,matrix);
#ifdef USEWITHVECTOR
	//friend matrix  operator*(matrix,laVector);
	friend matrix  operator*(matrix,laVector);
#endif
	friend matrix  operator*(float,matrix);
	friend matrix  operator^(matrix,int);
	friend std::ostream &operator<<(std::ostream&,matrix);
	bool operator=(matrix);
	matrix transpose(void);
	matrix inverse(void);
	
	//useful ops
	void makeidentity(void);                      //don't change dim, 1's on diag
	void makerotate(float theta);                //2x2 matrix rotate in 2D
	void makerotate(float x,float y,float z);  //3x3 matrix rotate in 3D
	void makescale(float,float);                //2x2 matrix scale in 2D
	void makescale(float,float,float);         //3x3 matrix scale in 3D
	void maketranslate(float,float);            //3x3 matrix translate in 2D
	void maketranslate(float,float,float);     //4x4 matrix translate in 3D
	
	//error ops
	Err checkerror(void) {return error;}
	
	//row ops
	void swap(int,int);
	void multiply(int,float);
	void add(int,int);
	void multiply_add(int,float,int);
	
private:
		void deldata(void);  //this being public would make bugs easier to create
	float *data;        // a 2D array of doubles, represented as 1D row major order
	int    dimr,dimc;    // rows and columns
	Err error;
};

#endif
