//  file:  gpackage.cpp

#include <qgl.h>
#include <qcolor.h>
#include "gpackage.hh"
#include <iostream>

using namespace std;

void GP_Object::GP_SetWorldCoordinates(double xMin,double xMax, double yMin, double yMax)
{  
  gluOrtho2D(xMin,xMax,yMin,yMax);
}


//  Set up the OpenGL rendering state, and define display list

void GP_Object::initializeGL()
{
  gluOrtho2D( 0, 720, 0, 486 );
  qglClearColor( *background ); // Let OpenGL clear to definable default -- defaults to white
  glClear( GL_COLOR_BUFFER_BIT );
}


void GP_Object::GP_Initialize(void)
{
  initializeGL();
}


void GP_Object::GP_AddPoints( double x, double y, int button )
{
  QString str;
  //  int spare;
  double *newpoints = 0;
  double *tobedeleted = 0;
  
  switch (button)
    {
    case 0 :
      if (num_points == 0)
	points = new double[2];
      newpoints = new double[(2*num_points + 2)];
      //if (newpoints == 0)
      //cout << "Invalid Pointer!!!" << endl;
      tobedeleted = points;
      for(int q=0;q< 2*num_points; q++)
	{
	  newpoints[q] = points[q];
	}

      newpoints[2*num_points] = x;
      newpoints[2*num_points+1] = y;
      num_points++;
      points = newpoints;
      delete [] tobedeleted;
      break;
    case 1 :
      if (num_points == 0)
	{
	  return;
	}
      else
	{
	  newpoints = new double[2*num_points + 2];
	}
      for(int q=0;q< 2*num_points; q++)
	newpoints[q] = points[q];
      tobedeleted = points;
      newpoints[2*num_points] = x;
      newpoints[2*num_points+1] = y;
      num_points++;
      points = newpoints;
      if (tobedeleted != 0)
	delete [] tobedeleted;
      else
	cout << "INVALID POINTER:  TOBEDELETED" << endl;

      GP_Polygon(points,num_points);

      num_points = 0;
      delete [] points;
      break;
    }
}


//Clears the canvas to black

void GP_Object::clearScreen()
{
  glClear( GL_COLOR_BUFFER_BIT );
  qglClearColor( *background ); // Let OpenGL clear to default
}


object::object(double input[],int verts)
{
  setobject(input,verts);
}


lobject::~lobject() 
{ 
  if (theShape != 0) 
    delete theShape;
  theShape = 0;
  if (next != 0) 
    {
      next->previous = previous; 
      previous->next = next;
    } 
};


void object::setobject(double input[],int verts)
{
  vertices = new vertex[verts+1];
  for(int x=0;x<verts;x++)
    {
      vertices[x].set(input[2 * x],input[2 * x+1]);
    }
  number = verts;
  return;
}


void GP_Object::GP_DrawPixel(double x,double y,QColor color)
{
  glEnable(GL_POINT_SMOOTH);
  glBegin(GL_POINTS);
  qglColor(color);
  glVertex2f(x,y);
  glEnd();
  return;
}


void object::drawobject(char *drawhere)
{
  QColor line(lred,lgreen,lblue), poly(pred,pgreen,pblue);
  if (lred >255)
    cout << "lred " << lred << endl;
  if (lgreen >255)
    cout << "lgreen " << lgreen << endl;
  if (lblue >255)
    cout << "lblue " << lblue << endl;
  int same = 0;
  if ((lred == pred) && (lgreen == pgreen) && (lblue == pblue))
    same = 1;   //  if line color == polygon color
  vertex current;

  glLineWidth(1.5);
  glEnable(GL_LINE_STIPPLE);
  switch (line_type)
    {
    case (CONTINUOUS):
      glLineStipple(1,65535);  //  16 bits, all on
      break;
    case (DASHED):
      glLineStipple(1,3855);   //  16 bits, alternating 4 on, 4 off
      break;
    case (DOTTED):
      glLineStipple(1,21845);  //  16 bits, alternating 1 on, 1 off
      break;
    }
  
  switch (fill_type)
    {
    case (CLOSED_POLYGON):
      glBegin(GL_POLYGON);
      for(int x=0;x<number;x++)
	{
	  current.set(vertices[x].x(),vertices[x].y());
	  ((GP_Object*)(drawhere))->qglColor(poly);
	  glVertex2f(vertices[x].x(),vertices[x].y());
	}
      glEnd();
      if (same)  //  if line and polygon colors are different, do the next case too.
	break;
    case (OPEN_POLYGON):       //  this uses line colors instead of poly colors
      glBegin(GL_LINE_LOOP);
      for(int x=0;x<number;x++)
	{
	  current.set(vertices[x].x(),vertices[x].y());
	  ((GP_Object*)(drawhere))->qglColor(line);
	  glVertex2f(vertices[x].x(),vertices[x].y());
	}
      glEnd();
      break;
    case (LINE):
      glBegin(GL_LINE_STRIP);
      for(int x=0;x<number;x++)
	{
	  current.set(vertices[x].x(),vertices[x].y());
	  ((GP_Object*)(drawhere))->qglColor(line);
	  glVertex2f(vertices[x].x(),vertices[x].y());
	}
      glEnd();
      break;
    } 
}


void GP_Object::GP_Line(vertex a,vertex b)
{
  lobject *sparel;
  double vertices[4] = {a.x(),a.y(),b.x(),b.y()};
  if (shapes == 0)
    {
      shapes = new lobject;
      shapes->set(vertices,2,shapes,shapes);
    }
  else
    {
      sparel = shapes->previous;
      shapes->previous = new lobject;
      shapes->previous->set(vertices,2,sparel,sparel->next);
      sparel->next = shapes->previous;
    }
  
  shapes->previous->theShape->setmode(LINE,currentL,lineColor.red(),
				      lineColor.green(),lineColor.blue());
  shapes->drawobjects((char*)this);
}


void GP_Object::GP_UpdateScreen(void) 
{
  if ((shapes == 0) || (shapes->theShape == 0))
    {
      clearScreen();
      updateGL();
    }
  else
    {
      clearScreen();
      shapes->drawobjects((char*)this); 
      updateGL();
    }
}


int object::proximity(vertex v,double distance)
{
  int where = -1;
  int q=0;
  double d;
  while ((vertices[q].distance(v)+vertices[(q+1)%(number)].distance(v)-
	  vertices[q].distance(vertices[(q+1)%(number)]) > distance) && 
	 (q < number-1))
    {
      q++;
    }
  d=vertices[q].distance(v);
  if (vertices[q].distance(v)+vertices[(q+1)%(number)].distance(v)-
      vertices[q].distance(vertices[(q+1)%(number)]) <= distance)
    where = q;
  return where;
}


int lobject::proximity(vertex v,double distance,int *a)
{
  int where = -1;
  int q=-1;
  lobject *spare = this;
  do 
    {
      q++;
      spare = spare->next;
    } while ((spare != this) && (spare->theShape->proximity(v,distance) == -1));
  if (spare->theShape->proximity(v,distance) >= 0)
    {
      where = q+1;
      *a=spare->theShape->proximity(v,distance);
    }
  return where;
}


int GP_Object::GP_Proximity(double x,double y,int distance,int *a)
{
  if (shapes == 0)
    return -1;
  vertex thepoint(x,y);
  return shapes->proximity(thepoint,distance,a);
}

int GP_Object::GP_Proximity(double x,double y,int distance)
{
  int useless;
  if (shapes == 0)
    return -1;
  vertex thepoint(x,y);
  return shapes->proximity(thepoint,distance,&useless);
}


void GP_Object::GP_UpdatePoly(int p)
{
  if (p<0)
    return;
  lobject *spare = shapes;
  for(int q=0;q < p; q++)
    {
      spare = spare->next;
    }
  spare->theShape->setmode(currentO,currentL,
			   lineColor.red(),lineColor.green(),lineColor.blue(),
			   polyColor.red(),polyColor.green(),polyColor.blue());
}


void GP_Object::GP_DeletePoly(int p)
{
  if (p<0)
    return;
  lobject *spare = shapes;
  for(int q=0;q < p; q++)
    spare = spare->next;
  if (spare != shapes)
    delete spare;
  else
    {
      if (shapes->next != shapes)
	{
	  shapes = shapes->next;
	  delete spare;
	}
      else
	{
	  delete shapes;
	  1+1;     //  for some reason, this pause is necessary.
	           //  if you get brave enough to pull it out,
	           //  and your code breaks, don't say I didn't
	           //  warn you.  (Trust me on this one.)
	  shapes = 0;
	}
    }
}


void GP_Object::GP_Polygon(double *data,int num_verts)
{
  lobject *sparel;
  if (shapes == 0)
    {
      shapes = new lobject;
      shapes->set(data,num_verts,shapes,shapes);
    }
  else
    {
      sparel = shapes->previous;
      shapes->previous = new lobject;
      shapes->previous->set(data,num_verts,sparel,sparel->next);
      sparel->next = shapes->previous;
    }
  
  shapes->previous->theShape->setmode(currentO,currentL,
				      lineColor.red(),lineColor.green(),lineColor.blue(),
				      polyColor.red(),polyColor.green(),polyColor.blue());
  shapes->drawobjects((char*)this);
}


void object::setmode(ObjectMode no, LineMode nl,int r,int g,int b, int R, int G, int B)
{
  fill_type = no;
  line_type = nl;
  lred = r;
  lgreen = g;
  lblue = b;
  pred = R;
  pgreen = G;
  pblue = B;
}


void object::setmode(ObjectMode no, LineMode nl, int R, int G, int B)
{
  fill_type = no;
  line_type = nl;
  pred = R;
  pgreen = G;
  pblue = B;
}


void lobject::drawobjects(char *gpobject)
{
  if (this->theShape == 0)
    return;
  lobject *first = this, *current = this;
  do 
    {
      current->theShape->drawobject(gpobject);
      current = current->next;
    } while (current != first); 
}
