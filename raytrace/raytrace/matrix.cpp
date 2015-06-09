//  file:  matrix.cpp

#include "matrix.hh"
#include <iostream>

using namespace std;

ostream &operator<<(ostream &cout,matrix a)
{
	std::cout << std::endl << "[";
	for(int x=0;x<a.dimr;x++)
    {
		if (x>0)
			std::cout << " ";
		std::cout << "[ ";
		for(int y=0;y<a.dimc;y++)
		{
			std::cout << a.get(x,y);
			if (y < a.dimc-1)
				std::cout << ", ";
		}
		std::cout << " ]";
		if (x < a.dimr-1)
			std::cout << std::endl;
    }
	std::cout << "]" << std::endl;
	return std::cout;
}


matrix::matrix(int row,int column)
{
	data=NULL;
	setdim(row,column);
	makeidentity();
}


void matrix::setdim(int row,int column)
{
	if (data != NULL)
		deldata();
	
	data = new float[row*column];
	dimr = row;
	dimc = column;
	error = noErr;
}


void matrix::deldata(void)
{
	//  if (data != NULL)
	//  delete [] data;
}


float matrix::get(int row,int column)
{
	if ((row>=dimr) || (column>=dimc))
    {
		std::cout << "error in matrix::get";
		std::cout << *this << "(" << row << "," << column << ")\n";
		error=dimRange;
		return 0;
    }
	else
		error=noErr;
	
	return data[dimc*row+column];
}


void matrix::set(int row,int column,float value)
{
	if ((row>=dimr) || (column>=dimc))
    {
		std::cout << "error in matrix::set";
		std::cout << *this << "(" << row << "," << column << ")\n";
		error=dimRange;
		return;
    }
	else
		error=noErr;
	data[dimc*row+column]=value;
}


matrix operator+(matrix a,matrix b)
{
	matrix c(a.dimr,a.dimc);
	if ((a.dimr != b.dimr) || (a.dimc != b.dimc))
    {
		std::cout << "error in matrix::operator+";
		std::cout << a << b << std::endl;
		c.error=dimMismatch;
		return c;
    }
	else
		c.error=noErr;
	for(int x=0;x<c.dimr;x++)
    {
		for(int y=0;y<c.dimc;y++)
		{
			c.set(x,y,a.get(x,y)+b.get(x,y));
		}
    }
	return c;
}


matrix operator*(matrix a,matrix b)
{
	matrix c(a.dimr,b.dimc);
	if (a.dimc != b.dimr)
    {
		std::cout << "error in matrix::operator*";
		std::cout << a << b << std::endl;
		c.error=dimMismatch;
		return c;
    }
	else
		c.error=noErr;
	
	int x=0;
	int y=0;
	int z=0;
	
	for(x=0;x<c.dimr;x++)
    {
		for(y=0;y<c.dimc;y++)
		{
			c.set(x,y,0);
			for(z=0;z<a.dimc;z++)
				c.set(x,y,c.get(x,y)+a.get(x,z)*b.get(z,y));
		}
    }
	return c;
}


bool matrix::operator=(matrix a)
{
	//  std::cout << a;
	int x,y;
	setdim(a.dimr,a.dimc);
	for(x=0;x<dimr;x++)
    {
		for(y=0;y<dimc;y++)
		{
			set(x,y,a.get(x,y));
		}
    }
	return 1;
}


#ifdef USEWITHVECTOR
/*matrix operator*(matrix a,laVector b)
{
	if ((a.dimc != 2) && (a.dimc != 3))
    {
		std::cout << "error in matrix*laVector\n";
		//std::cout << a << b << std::endl;
		a.error=dimMismatch;
		return a;
    }
	matrix c(a.dimc,1);
	c.set(0,0,b.x());
	c.set(1,0,b.y());
	if (a.dimc==3)
		c.set(2,0,1);
	return a*c;
}*/


matrix operator*(matrix a,laVector b)
{
	if ((a.dimc != 3) && (a.dimc != 4))
    {
		std::cout << "error in matrix*laVector\n";
		//std::cout << a << b << std::endl;
		a.error=dimMismatch;
		return a;
    }
	matrix c(a.dimc,1);
	c.set(0,0,b.x());
	c.set(1,0,b.y());
	c.set(2,0,b.z());
	if (a.dimc == 4)
		c.set(3,0,1);
	return a*c;
}
#endif

matrix operator*(float a,matrix b)
{
	matrix c(b.dimr,b.dimc);
	for(int x=0;x<b.dimr;x++)
		for(int y=0;y<b.dimc;y++)
			c.set(x,y,c.get(x,y)*a);
	return c;
}


matrix operator^(matrix a,int pow)
{
	matrix b=a;
	
	if (a.dimr != a.dimc)
    {
		std::cout << "error in matrix^";
		a.error=dimMismatch;
		return a;
    }
	else
		b.error=noErr;
	
	for(int x=1;x<pow;x++)
		b=a*a;
	return b;
}


void matrix::makeidentity(void)
{
	for(int x=0;x<dimr;x++)
		for(int y=0;y<dimc;y++)
		{
			if (x==y)
				set(x,y,1);
			else
				set(x,y,0);
		}
}


matrix matrix::transpose(void)
{
	matrix output;
	output.setdim(dimc,dimr);
	for(int x=0;x<dimc;x++)
    {
		for(int y=0;y<dimr;y++)
		{
			output.set(x,y,get(y,x));
		}
    }
	return output;
}


void matrix::makerotate(float theta)
{
	setdim(3,3);
	set(0,0, cos(theta) ); //set matrix to:
	set(0,1, sin(theta) ); // [[ cos(theta)  sin(theta)]
	set(1,0,-sin(theta) ); //  [-sin(theta)  cos(theta)]]
	set(1,1, cos(theta) ); //
}


void matrix::makerotate(float x,float y, float z)
{
	matrix X(4,4),Y(4,4),Z(4,4);
	
	Z.set(0,0, cos(z) ); //set matrix to:
	Z.set(0,1, sin(z) ); // [[ cos(z)  sin(z)  0]
	Z.set(1,0,-sin(z) ); //  [-sin(z)  cos(z)  0]
	Z.set(1,1, cos(z) ); //  [ 0           0    1]]
	
	X.set(1,1, cos(x) ); //set matrix to:
	X.set(1,2, sin(x) ); // [[ 1  0       0     ]
	X.set(2,1,-sin(x) ); //  [ 0  cos(x)  sin(x)]
	X.set(2,2, cos(x) ); //  [ 0 -sin(x)  cos(x)]]
	
	Y.set(2,2, cos(y) ); //set matrix to:
	Y.set(2,0, sin(y) ); // [[ cos(y)  0 -sin(y)]
	Y.set(0,2,-sin(y) ); //  [ 0       1  0     ]
	Y.set(2,2, cos(y) ); //  [ sin(y)  0  cos(y)]]
	
	*this=X*Y*Z;
	//return output;
}


void matrix::makescale(float x,float y)
{
	setdim(3,3);
	makeidentity();
	set(0,0,x);
	set(1,1,y);
}


void matrix::makescale(float x,float y,float z)
{
	setdim(4,4);
	makeidentity();
	set(0,0,x);
	set(1,1,y);
	set(2,2,z);
}

void matrix::maketranslate(float x,float y)
{
	setdim(3,3);
	makeidentity();
	set(0,2,x);
	set(1,2,y);
}


void matrix::maketranslate(float x,float y,float z)
{
	setdim(4,4);
	makeidentity();
	set(0,3,x);
	set(1,3,y);
	set(2,3,z);
}


void matrix::swap(int rowa,int rowb)
{
	matrix intermediate(1,dimc);
	for(int x=0;x<dimc;x++)
    {
		intermediate.set(0,x,get(rowa,x));
		set(rowa,x,get(rowb,x));
		set(rowb,x,intermediate.get(0,x));
    }
}


void matrix::multiply(int row,float value)
{
	for(int x=0;x<dimc;x++)
		set(row,x,value*get(row,x));
}


void matrix::add(int rowa,int rowb)
{
	for(int x=0;x<dimc;x++)
		set(rowb,x,get(rowb,x)+get(rowa,x));
}


void matrix::multiply_add(int rowa,float factor,int rowb)
{
	for(int x=0;x<dimc;x++)
		set(rowb,x,get(rowb,x)+factor*get(rowa,x));
}


matrix matrix::inverse(void)
{
	if (dimr != dimc)
		return *this;
	matrix unit(dimr,dimc);
	matrix work(dimr,2*dimc);
	matrix output=*this;
	unit.makeidentity();
	for(int x=0;x<dimr;x++)
		for(int y=0;y<dimc;y++)
		{
			work.set(x,y,get(x,y));
			work.set(x+dimr,y,unit.get(x,y));
		}
			int x=0,y=0;
	bool notuppertriangular=1;
	
	while (notuppertriangular)
    {
		y=x;
		if (work.get(x,x)==0)
			while (++y<dimr)
			{
				if (work.get(x,y)==0)
					continue;
				else
				{
					work.swap(x,y);
					break;
				}
			}
				if (y==dimr)
					return output;
		if (work.get(x,x) != 1)
			work.multiply(x,1/(work.get(x,x)));
		y=x;
		while (++y<dimr)
		{
			if (work.get(x,y) != 0)
				work.multiply_add(x,-work.get(x,y),y);
		}
		if (x==dimc)
			notuppertriangular=0;
		x++;
    }
	
	int notdone=1;
	
	x=dimr-1;
	
	while (notdone)
    {
		y=x;
		//if (work.get(x,x)==0)  // we know this can't happen
		//while (++y<dimr)
		//  {
		//    if (work.get(x,y)==0)
		//      continue;
		//    else
		//      {
		//	work.swap(x,y);
		//	break;
		//      }
		//  }
		//if (y==dimr)
		//return output;
		//if (work.get(x,x) != 1)
		//work.multiply(x,1/(work.get(x,x)));
		//y=x;
		while (--y>=0)
		{
			if (work.get(x,y) != 0)
				work.multiply_add(x,-work.get(x,y),y);
		}
		if (x==0)
			notdone=0;
		x--;
    }
	
	for(int x=dimc;x<2*dimc;x++)
		for(int y=0;y<dimr;y++)
			output.set(y,x-dimc,work.get(y,x));
	
	return output;
}
