//  file:  raytrace.cpp
//     screen res:  RESOLUTION_XxRESOLUTION_Y
#include "raytrace.hh"
#include <iostream>
#include <qdatastream.h>


/*****************************************************/
//  functions for raytraceobject

raytraceobject::raytraceobject(void)
{
	ambient=0;
	diffuse=0;
	specular=0;
	phong=1;
	center.setrectangular(0,0,0);
	color.setrectangular(255,255,255);
	background.setrectangular(150,100,255);
	radius=5;
}


void raytraceobject::setambient(float a)
{
	ambient=a;
	if (ambient+diffuse+specular > 1)
    {
		diffuse=diffuse/(diffuse+specular);
		specular=1-(ambient+diffuse);
    }
	return;
}


void raytraceobject::setdiffuse(float d)
{
	diffuse=d;
	if (ambient+diffuse+specular > 1)
    {
		ambient=ambient/(ambient+specular);
		specular=1-(ambient+diffuse);
    }
	return;
}



void raytraceobject::setspecular(float s)
{
	specular=s;
	if (ambient+diffuse+specular > 1)
    {
		ambient=ambient/(ambient+diffuse);
		diffuse=1-(ambient+specular);
    }
	return;
}


laVector raytraceobject::intersect(laVector start, laVector direction, float *t)
{
	laVector v=start - center;
	const float b = 2.0f*dot(v,direction);
	const float a = dot(direction,direction);
	const float c = dot(v,v) - radius*radius;
	
	const float radical = b*b-4*a*c;
	
	if ( radical <= 0.0f )
	{
		*t=(-1);
		return (start-direction);
	}
	
	const float root = sqrt( radical );
	const float t1 = ( -b + root ) / ( 2.0f * a );
	const float t2 = ( -b - root ) / ( 2.0f * a );
	
	if ( t1 > 0.0f )
	{
		if ( t2 > 0.0f )
		{
			*t = t1 < t2 ? t1 : t2;
		}
		else
		{
			*t = t1;
		}
	}
	else
	{
		if ( t2 > 0.0f )
		{
			*t = t2;
		}
		else
		{
			*t=(-1);
		}
	}
	
	return line(start,*t,direction);
}


laVector raytraceobject::intersectcolor(laVector start,laVector direction,
									  float t, lightsource light)
{
	laVector outputcolor=color;
	laVector norm;
	laVector intersection = line(start,t,direction);
	laVector lightfactor;
	laVector output=background;
	
	norm=intersection-center;
	norm.makeunit();
	outputcolor=light.diffuse(intersection,norm,direction,diffuse,phong);
	
	return outputcolor;
}


void raytraceobject::setcenter(laVector newcenter)
{
	center=newcenter;
}


void raytraceobject::setradius(float newradius)
{
	radius=newradius;
}


void raytraceobject::setcolor(int r,int g,int b)
{
	color.setrectangular(r,g,b);
}


laVector raytraceobject::reflect(laVector start,laVector direction,float t)
{
	laVector output=direction;
	laVector pointnorm=getnorm(line(start,t,direction));
	output.reflect(pointnorm);
	return output;
}


laVector raytraceobject::getnorm(laVector atpoint)
{
	laVector norm=(atpoint-center);
	norm.makeunit();
	return norm;
}
