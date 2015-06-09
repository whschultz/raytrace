//  file:  objects.cpp

#include "raytrace.hh"
#include <iostream>
#include <qstring.h>
#include <qdatastream.h>


/*****************************************************/
//  functions for sphere

sphere::sphere(void)
{
	laVector newcenter(0,0,0);
	setcenter(newcenter);
	setradius(20);
	setcolor(255,255,255);
}


sphere::~sphere(void)
{
	
	return;
}


/*****************************************************/
//  functions for lightsource

lightsource::lightsource(void)
{
	position.setrectangular(0,0,0);
	color.setrectangular(1,1,1);
	attenuation=3;
	radius = .1;
}


lightsource::~lightsource(void)
{
	
}


void lightsource::setposition(float x,float y,float z)
{
	position.setrectangular(x,y,z);
}


laVector lightsource::diffuse(laVector point,laVector norm,laVector direction,float diffuse,float phong)
{
	laVector outputcolor;
	laVector reflecteddir=direction;
	float adder1,adder2,dotp;
	if (norm.norm() != 1)
		norm.makeunit();
		
	reflecteddir.reflect(norm);
	reflecteddir.makeunit();

	laVector dirtolight=(position-point);
	float distance=dirtolight.norm();
	dirtolight.makeunit();
	adder1=diffuse*dot(norm,dirtolight);
	dotp=dot(reflecteddir,dirtolight);
	if (!(dotp<0)) // shark recommended optimization
		adder2=pow(dotp,phong);
	else
		adder2=0;
	
	const float product = attenuation*(adder1+adder2)/distance;
	outputcolor = product * color;

	return (outputcolor);
}


laVector lightsource::specular(laVector direction,laVector norm,laVector intersection,float specular,float phong)
{
	float distance;
	laVector reflection_vector=direction;
	laVector dirtolight=position-intersection;
	distance=dirtolight.norm();
	dirtolight.makeunit();
	reflection_vector.reflect(norm);
	return ((specular*pow(dot(reflection_vector,dirtolight),phong)*color)/distance);
}


void lightsource::setcolor( float r, float g, float b)
{
	color.setrectangular(r,g,b);
}


void lightsource::setcolor(int r,int g,int b)
{
	color.setrectangular(static_cast<float>(r)/255.0,
						 static_cast<float>(g)/255.0,
						 static_cast<float>(b)/255.0);
}


/*********************************************************/
//  function calls for plane

plane::plane(void)
{
	normal.setrectangular(0,0,1);
	point.setrectangular(0,0,0);
}


plane::~plane(void)
{
	
}


laVector plane::intersect(laVector start,laVector direction,float *t)
{
	if (!dot(direction,normal))
		*t=-1;
	else
		*t=(dot(point,normal)-dot(start,normal))/dot(direction,normal);
	return line(start,*t,direction);
}


laVector plane::intersectcolor(laVector start,laVector direction,float t,lightsource light)
{
	if (!dot(direction,normal))
    {
		return background;
    }

	laVector intersection = line(start,t,direction);
	laVector lightfactor;
	
	laVector outputcolor=light.diffuse(intersection,normal,direction,diffuse,phong);
	
	return outputcolor;
}


laVector plane::reflect(laVector start,laVector direction,float *t)
{
	laVector reflected_vector=direction;
	reflected_vector.reflect(normal);
	return reflected_vector;
}
