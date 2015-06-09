/*
 *  laVector.mm
 *  Modeler
 *
 *  Created by William H. Schultz  on Wed Mar 13 2002.
 *  Copyright (c) 2001 __MyCompanyName__. All rights reserved.
 *
 */

//#import "laVector.h"
#include "vector.hh"

#include <vecLib/vfp.h>


/*laVector::~laVector(void)
{
	return;
}*/

laVector::laVector(void)
: x_coor	( reinterpret_cast<float *>(coords)[0] ),
y_coor		( reinterpret_cast<float *>(coords)[1] ),
z_coor		( reinterpret_cast<float *>(coords)[2] ),
null0		( reinterpret_cast<float *>(coords)[3] ),
r 			( reinterpret_cast<float *>(coords)[4] ),
theta		( reinterpret_cast<float *>(coords)[5] ),
phi			( reinterpret_cast<float *>(coords)[6] ),
null1		( reinterpret_cast<float *>(coords)[7] )
{
    x_coor = 0;
    y_coor = 0;
    z_coor = 0;
    null0 = 0;
    
    r = 0;
    theta = 0;
    phi = 0;
    null1 = 0;
    
    mode = 3;  // both are valid
}


laVector::laVector(float x,float y,float z)
: x_coor	( reinterpret_cast<float *>(coords)[0] ),
y_coor		( reinterpret_cast<float *>(coords)[1] ),
z_coor		( reinterpret_cast<float *>(coords)[2] ),
null0		( reinterpret_cast<float *>(coords)[3] ),
r 			( reinterpret_cast<float *>(coords)[4] ),
theta		( reinterpret_cast<float *>(coords)[5] ),
phi			( reinterpret_cast<float *>(coords)[6] ),
null1		( reinterpret_cast<float *>(coords)[7] )
{
    x_coor = x;
    y_coor = y;
    z_coor = z;
    null0 = 0;

    r = 0;
    theta = 0;
    phi = 0;
    null1 = 0;

    mode = 1;  // only rectangular is valid
}


/*laVector::laVector(const laVector &startingPoint)
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
}*/

/*void laVector::set_equal_to(const laVector &startingPoint)
{
	vScopy( 8, startingPoint.coords, coords );
    mode = startingPoint.mode;
}*/

int  laVector::check_equal(const laVector &reference)
{
    switch (mode)
	{
        case 1:
        case 3:
            return ((x_coor == reference.x_coor) &&
                    (y_coor == reference.y_coor) &&
                    (z_coor == reference.z_coor));
            break;
        case 2:
            return ((r == reference.r) &&
                    (theta == reference.theta) &&
                    (phi == reference.phi));
            break;
        default:
            return 0;
	}
}




float laVector::get_x(void)
{
    if (mode == 2)  // then this is not a valid request
        update_rectangular();
    return x_coor;
}


float laVector::get_y(void)
{
    if (mode == 2)  // then this is not a valid request
        update_rectangular();
    return y_coor;
}


float laVector::get_z(void)
{
    if (mode == 2)  // then this is not a valid request
        update_rectangular();
    return z_coor;
}

float laVector::get_r(void)
{
    if (mode == 1)  // then this is not a valid request
        update_spherical();
    return r;
}

float laVector::get_theta(void)
{
    if (mode == 1)  // then this is not a valid request
        update_spherical();
    return theta;
}

float laVector::get_phi(void)
{
    if (mode == 1)  // then this is not a valid request
        update_spherical();
    return phi;
}

void laVector::setrectangular(float new_x,float new_y,float new_z)
{
    x_coor = new_x;
    y_coor = new_y;
    z_coor = new_z;
    mode = 1;  //  rectangular valid
}

void laVector::set_spherical(float new_r,float new_theta,float new_phi)
{
    r = new_r;
    theta = new_theta;
    phi = new_phi;
    mode = 2;  //  spherical valid
}


void laVector::update_spherical(void)
{
    if (mode >= 2)
        return;  // spherical is already valid
	
    //r=hypot(hypot(x_coor,y_coor),z_coor);
    r = sqrt( x_coor*x_coor + y_coor*y_coor + z_coor*z_coor );
    if (r>0)
	{
        //  set theta
        if (x_coor != 0)
            theta=atan(y_coor/x_coor);
        else
		{
            if (y_coor > 0)
                theta=pi/2;
            if (y_coor < 0)
                theta=-pi/2;
            if (y_coor == 0)
                theta=0;
		}
        if ( x_coor>0 )
		{
            /*if (y_coor >= 0)
			return;*/
            if (y_coor<0)
                theta+=2*pi;
            //break;
		}
        else if (x_coor<0)
		{
            theta+=pi;
            //break;
		}
	}
    else
        theta=0;
    // the breaks should put us here
    
    //  set phi
    
    if (r>0)
        phi = acos(z_coor/r);
    else
        phi=0;
	
    mode = 3;  //  both are valid now
    
    return;
}

void laVector::update_rectangular(void)
{
    if (mode != 2)  // then we're set
        return;
    
    const float sin_phi = sin(phi);
    
    x_coor = r*sin_phi*cos(theta);
    y_coor = r*sin_phi*sin(theta);
    z_coor = r*cos(phi);
	
    mode = 3;  //  because both are valid now
	
    return;
}


//float laVector::dot(laVector variable)
/*float operator*(laVector a,laVector b)
{
	a.update_rectangular();
	b.update_rectangular();
	
    return ((a.x_coor * b.x_coor) +
            (a.y_coor * b.y_coor) +
            (a.z_coor * b.z_coor));
}*/


laVector laVector::cross(laVector variable)
{
	if (mode == 2)  //  we want rectangular for this method
		update_rectangular();
	if (variable.mode == 2)
		variable.update_rectangular();
	laVector returnvalue(y_coor*variable.z_coor - z_coor*variable.y_coor,
					     z_coor*variable.x_coor - x_coor*variable.z_coor,
					     x_coor*variable.y_coor - y_coor*variable.x_coor);
	return returnvalue;
}	


laVector laVector::add(laVector variable)
{
	return line( *this, 1, variable );
	
	/*if (mode == 2)  //  we want rectangular for this method
		update_rectangular();
	if (variable.mode == 2)
		variable.update_rectangular();
	laVector returnvalue(x_coor + variable.x_coor,
					     y_coor + variable.y_coor,
					     z_coor + variable.z_coor);
	return returnvalue;*/
}


laVector laVector::subtract(laVector variable)
{
	return line( *this, -1, variable );
	
	/*laVector variablecopy;
	variablecopy.set_equal_to(variable);
	variablecopy.multiply(-1);
	return this->add(variablecopy);*/
}


float laVector::norm(void)
{
    if (mode != 1)
        return r;

	return vSnorm2( 4, coords );
	
    //return sqrt(x_coor * x_coor +  // hopefully, this is faster
    //            y_coor * y_coor +
    //            z_coor * z_coor);
}




laVector laVector::multiply(float variable)
{
	laVector v;
    if (mode != 1)
	{
		v.set_equal_to(*this);
        v.r *= variable;
	}
	if (mode != 2)
	{
		vSyax( 4, variable, coords, v.coords );
		v.mode = 1;
	}

    return v;
}


laVector laVector::divide(float variable)
{
	return multiply( 1/variable );
}


void laVector::makeunit(void)
{
    if (mode != 2)
	{
		float inv_length = 1/norm();
		vSscal( 4, inv_length, coords );
	}
    if (mode != 1)
	{
        r = 1;
	}
}


//void laVector::reflect(laVector & norm)
//{
	/*laVector normcopy;
	normcopy.set_equal_to(norm);
	normcopy.multiply(-2*dot(*this,norm));
	this->set_equal_to(this->add(normcopy));*/
	
//	*this=(*this+(-2*dot(*this,norm)*norm));
//}


//laVector operator+(laVector a,laVector b)
//{
//	return a.add(b);
//}


laVector operator/(laVector a,float b) 
{
	return a.multiply(1/b);
}


laVector cross(laVector a,laVector b)
{
	//a.update_rectangular();
	//b.update_rectangular();
	
	laVector output(a.get_y()*b.get_z()-a.get_z()*b.get_y(),
				    a.get_z()*b.get_x()-a.get_x()*b.get_z(),
				    a.get_x()*b.get_y()-a.get_y()*b.get_x());
	//cout << "cross product: " << a << "x" << b << " ?= " << output << std::endl;
	return output;
}



void laVector::cropforcolor(void)
{
	update_rectangular();
	if (x_coor < 0)
		x_coor=0;
	if (y_coor < 0)
		y_coor=0;
	if (z_coor < 0)
		z_coor=0;
	
	if (x_coor > 255)
		x_coor=255;
	if (y_coor > 255)
		y_coor=255;
	if (z_coor > 255)
		z_coor=255;
	mode=1;
}
