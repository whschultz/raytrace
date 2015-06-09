/****************************************************************************
**  glwidget.cpp
****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <qgl.h>
#include "gpackage.hh"
#include "mainwin.hh"


// Constructor

GP_Object::GP_Object( QWidget* parent, const char* name )
: QGLWidget( parent, name )
{
	black=new QColor(0,0,0);
	white=new QColor(255,255,255);
	
	background = white;
	lineColor = *black;
	polyColor = *black;
	
	X = -1;
	Y = -1; 
	lastX = -1; 
	lastY = -1;
	toggleFree = 0;
	num_points = 0;
	shapes = 0;
	
	currentL = CONTINUOUS;
	currentO = LINE;
	emode    = DRAW;
	
	glEnable(GL_POINT_SMOOTH);
}


//  Release allocated resources

GP_Object::~GP_Object()
{
	delete black;
	delete white;
	//while (shapes->previous)
	//  delete shapes->previous;
	if (points) delete [] points;
}


//handle the event that any mouse button is pressed

void GP_Object::mousePressEvent( QMouseEvent *e )
{
	float x=e->pos().x(),y=RESOLUTION_Y-e->pos().y();
	if( e->button() == LeftButton )
    {
		if (emode == DRAW)
			GP_AddPoints( x, y , 0);
		else if (emode == MODIFY)
			GP_UpdatePoly( GP_Proximity(x,y,3) );
		else
			GP_DeletePoly( GP_Proximity(x,y,3) );
    }
	else if (e->button() == RightButton )
    {
		if (emode == DRAW)
			GP_AddPoints( x, y , 1);
		else if (emode == MODIFY)
			GP_UpdatePoly( GP_Proximity(x,y,3) );
		else
			GP_DeletePoly( GP_Proximity(x,y,3) );
    }
	
	GP_UpdateScreen();
}


void GP_Object::setLineColor()
{
	lineColor = QColorDialog::getColor(*black);
}


void GP_Object::setPolyColor()
{
	polyColor = QColorDialog::getColor(*black);
}
