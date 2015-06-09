/*
 *  laVector.h
 *  Modeler
 *
 *  Created by William H. Schultz  on Wed Mar 13 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef NEWVECTORWHS
#define NEWVECTORWHS

#include <math.h>

#include <iostream>

#include <vecLib/vectorOps.h>
#include <vecLib/vecLibTypes.h>

//#define dot(x,y) (x*y)

//#define vector3 laVector  // this is done to trick out the compiler
#define pi 3.1415926535897946264338
//const float pi = 2*acos(0);


class laVector
{
public:
	
	laVector(void);
	laVector(float,float,float);
	
	laVector(const laVector & startingPoint)
	: x_coor	( reinterpret_cast<float *>(coords)[0] ),
	y_coor		( reinterpret_cast<float *>(coords)[1] ),
	z_coor		( reinterpret_cast<float *>(coords)[2] ),
	null0		( reinterpret_cast<float *>(coords)[3] ),
	r 			( reinterpret_cast<float *>(coords)[4] ),
	theta		( reinterpret_cast<float *>(coords)[5] ),
	phi			( reinterpret_cast<float *>(coords)[6] ),
	null1		( reinterpret_cast<float *>(coords)[7] )
	{
		vScopy( 8, startingPoint.coords, coords );
		mode = startingPoint.mode;
	}
	
	//~laVector();
	
	void set_equal_to(const laVector &startingPoint)
	{
		vScopy( 8, startingPoint.coords, coords );
		mode = startingPoint.mode;
	}
	int  check_equal(const laVector &);
	
	void setrectangular(float,float,float);  // preferred
	void set_spherical(float,float,float);  // preferred
	
	void makeunit(void);
	
	float get_x(void);
	float get_y(void);
	float get_z(void);
	float get_r(void);
	float get_theta(void);
	float get_phi(void);
	
	// legacy support
	float x(void) {return get_x();}
	float y(void) {return get_y();}
	float z(void) {return get_z();}
	
	// laVector arithmetic
	//float dot(laVector);
	laVector cross(laVector);
	laVector add(laVector);
	laVector subtract(laVector);
	float norm(void);
	//void make_unit(void);
	inline void reflect(laVector & norm) { *this=(*this+(-2*dot(*this,norm)*norm)); }

	// scalar arithmetic
	laVector multiply(float);
	laVector divide(float);
	
	
	// legacy support
	friend laVector operator*(float a, laVector v) {return v.multiply(a);}
	friend float operator*(laVector a, laVector b) { return dot( a, b ); }
	friend inline laVector operator+(laVector a,laVector b) { return line( a, 1, b ); }
	friend laVector operator/(laVector a,float b);
	friend inline laVector operator-(laVector a,laVector b) {return line( a, -1, b );}
	friend float dot(laVector &a,laVector &b);
	friend laVector line( laVector & start, float t, laVector & dir );
	
	bool operator==(const laVector & b) {return check_equal(b);}
	void operator=( const laVector & startingPoint) 
	{
		vScopy( 8, startingPoint.coords, coords );
		mode = startingPoint.mode;
	}
	
	void cropforcolor(void);
	
protected:
			
	void update_rectangular(void);
	void update_spherical(void);
	
private:
	vFloat coords[2];
	float & x_coor;
	float & y_coor;
	float & z_coor;
	float &null0;
	float &r,&theta,&phi;
	float &null1;
	short mode;  //  1 for rectangular, 2 for spherical, 3 for both
};


laVector cross(laVector,laVector);
inline float dot(laVector &a,laVector &b)
{
	if ( a.mode == 2 )
		a.update_rectangular();
	if ( b.mode == 2 )
		b.update_rectangular();
	
	return vSdot( 4, a.coords, b.coords );
	
    //return ((a.x_coor * b.x_coor) +
    //        (a.y_coor * b.y_coor) +
    //        (a.z_coor * b.z_coor));
}

inline laVector line( laVector & start, float t, laVector & dir )
{
	if ( start.mode == 2 )
		start.update_rectangular();
	if ( dir.mode == 2 )
		dir.update_rectangular();
		
	laVector output;
	
	vSzaxpy( 4, t, dir.coords, start.coords, output.coords );
	output.mode = 1;
	
	/*std::cout 	<< "calculated a line:  < " << start.get_x() << ", " << start.get_y() << ", " << start.get_z() << " > + "
				<< t << " * < " << dir.get_x() << ", " << dir.get_y() << ", " << dir.get_z() << " > == < "
				<< output.get_x() << ", " << output.get_y() << ", " << output.get_z() << ">" << std::endl;*/
	
	return output;
}

#endif
