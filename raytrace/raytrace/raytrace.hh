//  file:  raytrace.hh

#ifndef RAYTRACE
#define RAYTRACE

#include "gpackage.hh"
#include "vector.hh"
#include "matrix.hh"
#include <qdatetime.h>
#include <qcursor.h>
#include <qcolor.h>
#include <qstring.h>

#define MAXOBJECTS 512
#define MAXLIGHTS  512
#define MAXSPECDEPTH 20

// comment out these lines to turn off antialiasing:
// IMPORTANT:  resolution is one dimension of a SQUARE!!!
#define ANTIALIASING
#define ANTIALIASING_RESOLUTION 1

#define SOFT_SHADOWS 1
#define SOFT_SHADOW_LEVEL 1
#define SOFT_SHADOWS_STEPS 1

typedef enum {PLAIN,CHECKERED} pattern;

class lightsource
{
public:
	lightsource(void);
	~lightsource(void);
	
	laVector specular(laVector dir,laVector norm,laVector intersect,float spec,float phong);
	laVector diffuse(laVector point,laVector norm,laVector direction,
				   float diffuse,float phong);
	
	void setcolor(float, float, float); // 0.0 to 1.0
	void setcolor(int,int,int);          // 0 to 255
	void setposition(float,float,float);
	void setattenuation(float newatt) {attenuation=newatt;}
	void setradius( float newradius ) { radius = newradius; }
	float getradius( void ) { return radius; }
	
	laVector getpos(void) {return position;}
private:
	laVector position;
	laVector color; // range from 0 to 1
	float  attenuation;
	float  radius;
};


class raytraceobject
{
public:
	raytraceobject(void);
	virtual ~raytraceobject(void) { };
	virtual laVector intersect(laVector,laVector,float *); // returns intersect point
	virtual laVector intersectcolor(laVector,laVector,float,lightsource); // returns color
	virtual laVector reflect(laVector,laVector,float);
	virtual void setcenter(laVector);
	virtual void setcenter(float x,float y,float z) {center.setrectangular(x,y,z);}
	virtual void setradius(float);
	virtual void setcolor(int,int,int);
	virtual void setbackground(laVector b) {background=b;}
	virtual void setbackground(int r,int g,int b) {background.setrectangular(r,g,b);}
	virtual void setambient(float a);
	virtual void setdiffuse(float d);
	virtual void setspecular(float s);
	virtual void setphong(float p) {phong=p;}
	virtual laVector getnorm(laVector);
	virtual float getphong(void) {return phong;}
	virtual float getspecular(void) {return specular;}
	virtual float getdiffuse(void) {return diffuse;}
	virtual float getambient(void) {return ambient;}
	virtual laVector getcolor(void) {return color;}
	
protected:
	laVector  center;
	float   radius;
	laVector  color;
	laVector  background;
	float   ambient;   // recode this to a laVector
	float   diffuse;   // recode this to a laVector
	float   specular;  // recode this to a laVector
	float   phong;
};


class sphere : public raytraceobject
{
public:
	sphere(void);
	~sphere(void);
};


class plane : public raytraceobject
{
public:
	plane(void);
	~plane(void);
	
	virtual laVector intersect(laVector,laVector,float *);
	virtual laVector intersectcolor(laVector,laVector,float,lightsource); // returns color
	virtual laVector reflect(laVector,laVector,float *);
	virtual void setpoint(float x,float y,float z) {point.setrectangular(x,y,z);}
	virtual void setnormal(float x,float y,float z) {normal.setrectangular(x,y,z);normal.makeunit();}
	virtual laVector getnorm(laVector irrelevant) {return normal;}
private:
	laVector point;
	laVector normal;
	pattern p;
};


class raytraceobjects
{
public:
	raytraceobjects(void);
	~raytraceobjects(void);
	
	bool addobject(raytraceobject *);
	bool addlight(lightsource *);
	void setbackground(float,float,float);
	void deleteallobjects(void);
	void set_attenuation(float);
	
	laVector followray(laVector,laVector,int, int ignore );
	laVector followraydiffuse(laVector,laVector,float,int, int ignore );
	laVector followrayspecular(laVector,laVector,float,int, int ignore );
	
	laVector intersect(laVector,laVector,float *, int ignore );
	laVector intersect(laVector,laVector,float *,int *, int ignore );
	laVector intersect(laVector start,laVector direction,float t, lightsource * pLight, raytraceobject * pObject, int ignore );
	
	void remove_one_light(void);
	void remove_one_object(void);
	
private:
	laVector background;
	raytraceobject **objects;
	lightsource    **lights;
	int numobjects,numlights;
	float attenuation;
};


class camera
{
public:
	camera(void);
	~camera(void) {}
	
	laVector antialias(laVector,laVector,laVector,int);
	laVector tocamera(laVector);
	laVector toworld(laVector);
	void setobjects(raytraceobjects *newobjs) {objects=newobjs;}
	void setaxes(void);
	
	void setlookat(float,float,float);
	void settop(float,float,float);
	void setpos(float,float,float);
	
	void setZoom(float mag) {zoom=mag; }
	float getZoom(void) { return zoom; }
	
	laVector getvector(void);
	laVector getcolor(void);
	
	laVector getdX(void);
	laVector getdY(void);
	
	laVector getBottomLeft(void);
	laVector getBottomRight(void);
	laVector getTopLeft(void);
	laVector getTopRight(void);
	
	void setresolution(int newres) {camera_resolution=newres;}
	
protected:
	laVector  lookat;
	laVector  head;
	laVector  right;
	laVector  position;
	raytraceobjects *objects;
	matrix transform;
	int camera_resolution;
	float zoom;
};


class renderer : public QWidget, public camera
{
	Q_OBJECT
public:
	renderer(GP_Object *);
	void tracescreen(void);
	
	public slots:
	
	void drawscreen(void);
	void openfile(void);
	void setresolution(void);
	void set_camera_position_world(void);
	void set_camera_position_camera(void);
	void set_lookat_world(void);
	void set_lookat_camera(void);
	void set_top_world(void);
	void set_top_camera(void);
	void set_attenuation(void);
	void add_sphere(void);
	void add_plane(void);
	void add_light(void);
	void remove_last_light(void);
	void remove_last_object(void);
	void setbackground(void);
	
private:
	GP_Object *gpobject;
	int numprocs;
};


#endif
