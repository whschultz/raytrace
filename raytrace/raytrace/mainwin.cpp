/****************************************************************************
** mainwin.cpp
****************************************************************************/


#include <qpushbutton.h>
#include <qslider.h>
#include <qlayout.h>
#include <qframe.h>
#include <qmenubar.h>
#include <qpopupmenu.h>
#include <qapplication.h>
#include <qkeycode.h>
#include <qstring.h>
#include "mainwin.hh"
#include "gpackage.hh"
#include "namewin.hh"

#include <iostream>


//Constructor for the main window.  It is here that all the work is carried out
//necessary to create the window and all its parts (buttons, menus, etc...)

MainWindow::MainWindow( QWidget* parent, const char* name )
: QWidget( parent, name )
{
	std::cout << "MainWindow initialization..." << std::endl;
	QFrame* f = new QFrame( this, "frame" );
	gpobject = new GP_Object( f, "gpobject");
	
	raycamera = new renderer(gpobject);
	raycamera->setobjects(&objects);
	
	// begin default initializations:
	// copied straight from mainwin's initialization.
	raycamera->setpos(0,4,10);
	raycamera->setlookat(0,-5,-10);
	raycamera->settop(0,1,0);
	
	objects.setbackground(15.0f,10.0f,25.5f);
	objects.set_attenuation( 1500 );

	sphere *sparesphere=new sphere;
	plane  *spareplane=new plane;
	lightsource *sparelight=new lightsource;
	
	spareplane->setpoint(0,-3,0);
	spareplane->setnormal(0,1,0);
	spareplane->setambient(.0);
	spareplane->setdiffuse(.6);
	spareplane->setspecular(.4);
	spareplane->setphong( 30 );
	spareplane->setcolor(255,255,255);
	objects.addobject(spareplane);
	
	/*spareplane=new plane;
	spareplane->setpoint(0,0,15);
	spareplane->setnormal(0,0,-1);
	spareplane->setambient(.0);
	spareplane->setdiffuse( 0 );
	spareplane->setspecular( .8 );
	spareplane->setphong( 30 );
	spareplane->setcolor(255,255,255);
	objects.addobject(spareplane);

	spareplane=new plane;
	spareplane->setpoint(0,0,-15);
	spareplane->setnormal(0,0,1);
	spareplane->setambient(.0);
	spareplane->setdiffuse( 0 );
	spareplane->setspecular( .8 );
	spareplane->setphong( 30 );
	spareplane->setcolor(255,255,255);
	objects.addobject(spareplane);

	spareplane=new plane;
	spareplane->setpoint(-15,0,0);
	spareplane->setnormal(1,0,0);
	spareplane->setambient(.0);
	spareplane->setdiffuse( 0 );
	spareplane->setspecular( .8 );
	spareplane->setphong( 30 );
	spareplane->setcolor(255,255,255);
	objects.addobject(spareplane);

	spareplane=new plane;
	spareplane->setpoint(15,0,0);
	spareplane->setnormal(-1,0,0);
	spareplane->setambient(.0);
	spareplane->setdiffuse( 0 );
	spareplane->setspecular( .8 );
	spareplane->setphong( 30 );
	spareplane->setcolor(255,255,255);
	objects.addobject(spareplane);
	
	spareplane=new plane;
	spareplane->setpoint(0,15,0);
	spareplane->setnormal(0,-1,0);
	spareplane->setambient(.0);
	spareplane->setdiffuse( 0 );
	spareplane->setspecular( .8 );
	spareplane->setphong( 30 );
	spareplane->setcolor(255,255,255);
	objects.addobject(spareplane);*/

	sparesphere->setcenter(-3,0,0);
	sparesphere->setradius(3);
	sparesphere->setambient(.0);
	sparesphere->setdiffuse(.8);
	sparesphere->setspecular(.8);
	sparesphere->setphong( 30 );
	sparesphere->setcolor(255,255,255);
	objects.addobject(sparesphere);
	
	sparesphere=new sphere;
	sparesphere->setcenter(3,0,0);
	sparesphere->setradius(3);
	sparesphere->setambient(.0);
	sparesphere->setdiffuse(.8);
	sparesphere->setspecular(.8);
	sparesphere->setphong( 30 );
	sparesphere->setcolor(255,255,255);
	objects.addobject(sparesphere);
	
	sparelight->setcolor(255,0,0);
	sparelight->setposition(-4,6,4);
	sparelight->setradius( .3 );
	objects.addlight(sparelight);
	
	sparelight=new lightsource;
	sparelight->setcolor(0,0,255);
	sparelight->setposition(4,6,4);
	sparelight->setradius( .3 );
	objects.addlight(sparelight);
	
	sparelight=new lightsource;
	sparelight->setcolor(0,255,0);
	sparelight->setposition(-4,6,-4);
	sparelight->setradius( .3 );
	objects.addlight(sparelight);
	
	sparelight=new lightsource;
	sparelight->setcolor(255,255,0);
	sparelight->setposition( 4, 6,-4 );
	sparelight->setradius( .3 );
	objects.addlight(sparelight);
	
	std::cout << "Objects initialized..." << std::endl;
	
	//mouse.setShape(PointingHandCursor);
	//setCursor(mouse);
	
	// Create top-level layout manager  (vertical)
	QVBoxLayout* vlayout = new QVBoxLayout( this, 20, 20, "vlayout");
	
	// Create a menu item "File"
	QPopupMenu *file = new QPopupMenu();
	file->insertItem( "Open",  raycamera, SLOT(openfile()));
	file->insertItem( "Exit",  qApp, SLOT(quit()), CTRL+Key_Q );
	
	//Create menu item "Camera"
	QPopupMenu *raytrace = new QPopupMenu();
	raytrace->insertItem("Draw Screen", raycamera, SLOT(drawscreen()));
	raytrace->insertItem("Set pixel resolution...", raycamera, SLOT(setresolution()));
	raytrace->insertSeparator();
	raytrace->insertItem("Set Camera position in world coordinates", 
						 raycamera, SLOT(set_camera_position_world()));
	raytrace->insertItem("Set lookat in world coordinates",
						 raycamera, SLOT(set_lookat_world()));
	raytrace->insertItem("Set top in world coordinates",
						 raycamera, SLOT(set_top_world()));
	raytrace->insertSeparator();
	raytrace->insertItem("Move Camera, camera coordinates",
						 raycamera, SLOT(set_camera_position_camera()));
	raytrace->insertItem("Set relative lookat position",
						 raycamera, SLOT(set_lookat_camera()));
	raytrace->insertItem("Set relative top", raycamera, SLOT(set_top_camera()));
	raytrace->insertSeparator();
	raytrace->insertItem("Set light attenuation value",
						 raycamera, SLOT(set_attenuation()));
	
	//Create menu item "Edit Objects"
	QPopupMenu *edit = new QPopupMenu();
	edit->insertItem("Add Sphere", raycamera, SLOT(add_sphere()));
	edit->insertItem("Add Plane",  raycamera, SLOT(add_plane()));
	edit->insertItem("Add Light",  raycamera, SLOT(add_light()));
	edit->insertItem("Set New Background Color", raycamera, SLOT(setbackground()));
	edit->insertSeparator();
	edit->insertItem("Remove Last Object", raycamera, SLOT(remove_last_object()));
	edit->insertItem("Remove Last Light",  raycamera, SLOT(remove_last_light()));
	
	// Create a menu bar
	QMenuBar *m = new QMenuBar( this );
	m->setSeparator( QMenuBar::InWindowsStyle );
	m->insertItem("&File", file );
	m->insertItem("&Camera",raytrace);
	m->insertItem("&Edit Objects", edit);
	vlayout->setMenuBar( m );
	
	std::cout << "Menus created..." << std::endl;
	
	// Create a nice frame to put around the gpobject widget
	
	f->setFrameStyle( QFrame::Sunken | QFrame::Panel );
	f->setLineWidth( 10 );
	vlayout->addWidget( f, 1 );
	
	std::cout << "Frame created..." << std::endl;
	
	// Create a layout manager for the gpobject widget
	QHBoxLayout* flayout = new QHBoxLayout( f, 2, 2, "flayout");
	
	std::cout << "Layout manager created..." << std::endl;
	
	// Create an gpobject widget
	if ( !gpobject->isValid() )
    {
		std::cout << "gpobject creation failed!!!" << std::endl;
		fatal("Failed to create Gpobject rendering context on this display");
    }
	
	std::cout << "gpobject created successfully..." << std::endl;
	
	gpobject->setMinimumSize( RESOLUTION_X,RESOLUTION_Y );
	gpobject->setMaximumSize( RESOLUTION_X, RESOLUTION_Y );
	gpobject->setGeometry( 0, 0, RESOLUTION_X, RESOLUTION_Y );
	
	std::cout << "gpobject size set..." << std::endl;
	
	//must enable mouse tracking to grab mouse location
	//gpobject->setMouseTracking( TRUE );
	//but we don't care about mouse
	
	gpobject->makeCurrent();
	std::cout << "gpobject made current..." << std::endl;
	flayout->addWidget( gpobject, 1 );
	flayout->activate();
	
	std::cout << "gpobject initialized..." << std::endl;
	
	//Label for messages to user
	mlabel = new QLabel(this);
	CHECK_PTR( mlabel );
	mlabel->setFrameStyle( QFrame::Panel | QFrame::Raised );
	mlabel->setAlignment( AlignBottom | AlignHCenter);
	mlabel->setLineWidth( 1 );
	mlabel->setFixedHeight( 25 );
	mlabel->setText( "" );
	mlabel->setFont(QFont("Helvetica", 10));
	vlayout->addWidget( mlabel, 1 ); 
	//This connection must remain here to wait for ulabel's creation
	connect(gpobject, SIGNAL(glmessage(const QString&)), 
			mlabel, SLOT(setText(const QString&)));
	connect(this, SIGNAL(message(const QString&)), 
			mlabel, SLOT(setText(const QString&)));
	
	// Start the geometry management
	//game->setBreakout(0);
	gpobject->GP_UpdateScreen();
	vlayout->activate();
	
	std::cout << "MainWin initialization complet." << std::endl;
}



//deallocate resources

MainWindow::~MainWindow()
{
	delete gpobject;
	delete mlabel;
}



//emits a signal to the gpobject canvas to clear it to black 

void MainWindow::clearCanvas()
{
	emit clearScr();
}   

/* void MainWindow::setLineMode(LineMode input)
{
	gpobject->newLineMode(input);
} */


//gets the user's name and displays it

void MainWindow::getName()
{
	QString before, after;
	
	before.sprintf("Hello ");
	after.sprintf("! To begin, select \"Start Game\" from the Game menu.");
	
	
	//create the name dialog window and set its size and location
	NameWindow *w;
	w = new NameWindow(this, "namewin");
	w->setGeometry(500, 300, 100, 100);
	
	//the actions in if(w->exec()) will be carried out if the user selects ok
	//else it wil carry out whats in the else portion of the statement
	if(w->exec())
    {
		QString name;
		
		name = w->returnName();
		
		before += name;
		before += after;
		emit message(before);
    }
	else
		emit message("Hello!  To begin, select \"Start Game\" from the Game menu.");
	
    delete w;
}
