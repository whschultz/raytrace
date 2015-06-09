/****************************************************************************
** namewin.cpp
****************************************************************************/
#include <qpushbutton.h>
#include <qlayout.h>
#include <qapplication.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qdialog.h>
#include <qwidget.h>
#include <qstring.h>

#include "namewin.hh"



//Constructor for the name dialog window
//Here all the work required to set up the window is done

NameWindow::NameWindow( QWidget *parent, const char *name ) 
:QDialog( parent, name, TRUE )
{
	
	// Create a top level layout manager (vertical)
	QVBoxLayout *vbox = new QVBoxLayout( this, 5, 5 );
	
	//create another layout managaer (horizantal)
	QHBoxLayout *hbox = new QHBoxLayout( 0, "hbox" );
	
	setCaption("Enter Name");  //Sets the caption for the window
	
	vbox->addLayout( hbox );  //addd the horizontal layout to the vertical one
	
	
	//create a label to prompt the user for their name
	nlabel = new QLabel(this, "nlabel");
	nlabel->setText("Please Enter Your Name");
	nlabel->setFixedHeight( 20 );
	nlabel->setFixedWidth( 150 );
	hbox->addWidget(nlabel, 1);
	
	//create a line edit box to get the users name as input
	le = new QLineEdit(this, "Edit");
	CHECK_PTR( le );
	le->setFixedHeight( 18 ); 
	le->setText("");  		//can be used to set defualt text
	vbox->addWidget(le, 1);  
	
	
	hbox = new QHBoxLayout( 0, "hbox2" );
	vbox->addLayout( hbox );
	
	//ok and puch buttons are connected by signals to carry out certain
	//functions
	ok = new QPushButton( this, "OK" );
	CHECK_PTR( ok );
	ok->setText(" OK "); 
	ok->setFixedHeight( 20 );
	ok->setFixedWidth( 50 );
	connect(ok, SIGNAL(clicked()), SLOT(newName()));
	hbox->addWidget( ok, 1 );
	
	hbox->addSpacing(40);
	
	cancel = new QPushButton(this, "Cancel");
	CHECK_PTR( cancel );
	cancel->setText(" Cancel ");
	cancel->setFixedHeight( 20 );
	cancel->setFixedWidth( 50 );
	connect(cancel, SIGNAL(clicked()), SLOT(reject()));
	hbox->addWidget( cancel, 1);
	
	vbox->activate();     //activates the top level manager(will activate all
						  //managers under it)
	
}



/****************************************************************************/

NameWindow::~NameWindow()
{
	delete le;
	delete nlabel;
	delete ok;
	delete cancel;  
}



/****************************************************************************/
//simply grabs the text from the line edit box and puts it in name

void NameWindow::newName()
{
	name = le->text(); 
	emit accept(); 
}



/****************************************************************************/
// just returns the name entered by the user and stored in name

QString NameWindow::returnName()
{
	return(name);
}
