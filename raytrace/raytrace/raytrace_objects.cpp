//  file:  raytrace_objects.cpp

#include "raytrace.hh"
#include <iostream>
#include <qstring.h>
#include <qdatastream.h>


/*****************************************************/
//  functions for raytraceobjects

raytraceobjects::raytraceobjects()
{
	objects=new raytraceobject * [MAXOBJECTS];
	lights=new lightsource * [MAXLIGHTS];
	for(int x=0;x<MAXOBJECTS;x++)
		objects[x]=NULL;
	for(int x=0;x<MAXLIGHTS;x++)
		lights[x]=NULL;
	numobjects=0;
	numlights=0;
	attenuation=1000;
	setbackground(0,0,0);
}


void raytraceobjects::setbackground(float r,float g,float b)
{
	background.setrectangular(r,g,b); 
	background.cropforcolor();
	for(int x=0;x<numobjects;x++)
		objects[x]->setbackground(background);
}


laVector raytraceobjects::intersect(laVector start,laVector direction,
								  float *location,int *objectnumber, int ignore )
{
	float t=1,smallest=-2;
	int numofsmallest=-1;
	*location=-1;
	*objectnumber=-1;
	for(int x=0;x<numobjects;x++)
    {
		if ( x == ignore )
			continue;
		objects[x]->intersect(start,direction,&t);
		if (t<=.00001)
			continue;
		else
		{
			if (numofsmallest==-1)
			{
				smallest=t;
				numofsmallest=x;
				continue;
			}
			if (t<smallest)
			{
				smallest=t;
				numofsmallest=x;
			}
		}
    }
	if (numofsmallest != -1)
    {
		*location=smallest;
		*objectnumber=numofsmallest;
    }
	return line(start,smallest,direction);
}


laVector raytraceobjects::intersect(laVector start,laVector direction,float *location, int ignore )
{
	int objectnumber;
	return intersect(start,direction,location,&objectnumber, ignore );
}


raytraceobjects::~raytraceobjects(void)
{
	deleteallobjects();
	delete [] objects;
	delete [] lights;
}


void raytraceobjects::deleteallobjects(void)
{
	while (numobjects--)
		delete objects[numobjects];
	while (numlights--)
		delete lights[numlights];
	numobjects=0;
	numlights=0;
}


void raytraceobjects::remove_one_object(void)
{
	if (numobjects==0)
		return;
	numobjects--;
	delete objects[numobjects];
}


void raytraceobjects::remove_one_light(void)
{
	if (numlights==0)
		return;
	numlights--;
	delete lights[numlights];
}


bool raytraceobjects::addobject(raytraceobject *newobject)
{
	if (numobjects >= MAXOBJECTS)
		return 0;
	newobject->setbackground(background);
	objects[numobjects]=newobject;
	numobjects++;
	return 1;
}


bool raytraceobjects::addlight(lightsource *newlight)
{
	if (numlights >= MAXLIGHTS)
		return 0;
	
	newlight->setattenuation(attenuation);
	lights[numlights]=newlight;
	numlights++;
	return 1;
}


void raytraceobjects::set_attenuation(float newatt)
{
	for(int x=0;x<numlights;x++)
		lights[x]->setattenuation(newatt);
	attenuation=newatt;
}


laVector raytraceobjects::followrayspecular(laVector start,laVector dir,float t,int objectnumber, int ignore )
{
	laVector intersection;
	laVector outputcolor(0,0,0);
	laVector norm;
	laVector spare;
	
	if (t>0)
    {
		norm=objects[objectnumber]->getnorm(intersection);
		for(int x=0;x<numlights;x++)
			outputcolor=outputcolor+
				(lights[x]->specular(dir,norm,intersection,objects[objectnumber]->getspecular(),
									 objects[objectnumber]->getphong()));
		//outputcolor.cropforcolor();
    }
	return outputcolor;
}


laVector raytraceobjects::followraydiffuse(laVector start,laVector direction,float t,int numofsmallest, int ignore )
{
	laVector outputcolorv(0,0,0);
	laVector spare;
	laVector intersectionpoint,newdirection;

	if (numofsmallest != -1)
    {
		for(int y=0;y<numlights;y++)
		{
			outputcolorv = outputcolorv + intersect( start, direction, t, lights[y], objects[numofsmallest], ignore );
		}
		
		outputcolorv = laVector( outputcolorv.x() * objects[numofsmallest]->getcolor().x() / 255.0f,
								 outputcolorv.y() * objects[numofsmallest]->getcolor().y() / 255.0f, 
								 outputcolorv.z() * objects[numofsmallest]->getcolor().z() / 255.0f ) + objects[numofsmallest]->getambient() * objects[numofsmallest]->getcolor();
    }
	else
		outputcolorv=background;
	
	return outputcolorv;
}


laVector raytraceobjects::intersect(laVector start,laVector direction,float t, lightsource * pLight, raytraceobject * pObject, int ignore )
{
	//laVector outputcolorv(0,0,0);
	laVector intersectionpoint,newdirection;
	laVector point = pLight->getpos();


	laVector dir;
	
	float success = 0;
	float total = 0;
	
	float previous_success = -1;
	float previous_total = -1;
	bool do_more_calcs = true;
	
	float shadowpoint = 0, distance = 0;
	
	intersectionpoint=line(start,t,direction);
	
	laVector intersect_color = pObject->intersectcolor( start, direction, t, *(pLight) );

#if SOFT_SHADOWS
	for( float layer = SOFT_SHADOW_LEVEL; layer > 0; layer-- )
	{
		if ( total > .5f && success < .5f )
		{
			// Then we've been around the outer shell and nothing created
			// a shadow.  The inner shell definitely won't either.
			break;
		}
			
		float current_radius = (layer/static_cast<float>( SOFT_SHADOW_LEVEL ))*pLight->getradius();
		
		float dTheta = 2.000001*pi/( SOFT_SHADOWS_STEPS * layer + 1 );
		float dPhi = pi/( SOFT_SHADOWS_STEPS * layer + 2);
		
		for( float theta = 0; theta < 2*pi; theta += dTheta )
		{
			for( float phi = dPhi; phi < pi; phi += dPhi )
			{
				if ( do_more_calcs )
				{
					dir.set_spherical( current_radius, theta, phi );
					
					shadowpoint = 0.0f;
					
					laVector light_pos = point+dir;
									
					newdirection = light_pos - intersectionpoint;
					distance = newdirection.norm();
					newdirection.makeunit();
	
					// dot product is related to the cosine of the angle between the two.
					// If the dot product is positive, the angle is acute.  Otherwise, it's
					// right or obtuse.  In which case, the light is irrelevant.
					if ( pObject->getnorm(intersectionpoint) * newdirection > 0 )
					{
						intersect(intersectionpoint,newdirection,&shadowpoint, ignore);
						
						if ((shadowpoint < 0) || (shadowpoint > distance))
						{
							success++;
						}
					}
				}

				total++;
			}
		}
		
		if ( success <= previous_success +.5f )
		{
			// This shell failed to find any intersections.  We have no reason
			// to intersect the shell inside it.
			do_more_calcs = false;
		}
		
		previous_total = total;
		previous_success = success;
	}
#else
	newdirection = point - intersectionpoint;
	distance = newdirection.norm();
	newdirection.makeunit();

	if ( pObject->getnorm(intersectionpoint) * newdirection > 0 )
	{
		intersect(intersectionpoint,newdirection,&shadowpoint, ignore);
		
		if ((shadowpoint < 0) || (shadowpoint > distance))
		{
			success++;
		}
	}

	total++;
#endif

	return (success/total)*intersect_color;
}


laVector raytraceobjects::followray(laVector start,laVector direction,int currentdepth, int ignore )
{
	float smallest=-2;
	int numofsmallest=-1;
	laVector outputcolorv(background);
	laVector spare;
	laVector intersectionpoint,newdirection=direction;
	laVector intersectnorm;
	
	intersectionpoint=intersect(start,direction,&smallest,&numofsmallest, ignore);
	if (numofsmallest != -1)
    {
		outputcolorv=followraydiffuse(start,direction,smallest,numofsmallest, numofsmallest );
		//	+followrayspecular(start,direction,smallest,numofsmallest);
		if ((currentdepth < MAXSPECDEPTH) && (objects[numofsmallest]->getspecular() > 0))
		{
			intersectnorm = objects[numofsmallest]->getnorm(intersectionpoint);
			newdirection.reflect( intersectnorm );
			
			//newdirection.update_rectangular();
			//newdirection.update_spherical();

			outputcolorv=outputcolorv+((objects[numofsmallest]->getspecular())
									   *(followray(intersectionpoint,newdirection,currentdepth+1, numofsmallest )));
		}
    }
	
	//outputcolorv.cropforcolor();
	
	return outputcolorv;
}
