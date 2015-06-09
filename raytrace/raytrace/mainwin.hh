/***************************************************************************
**  mainwin.hh
****************************************************************************/

#ifndef MAINWIN_H
#define MAINWIN_H

#include <qwidget.h>
#include <qlabel.h>
#include <qcursor.h>
#include "gpackage.hh"
#include "raytrace.hh"

#define RESOLUTION_X 1200
#define RESOLUTION_Y 800

class MainWindow : public QWidget
{
	Q_OBJECT  //needed if any signals or slots are used
	
protected:
	QLabel 	  *mlabel;
	GP_Object 	  *gpobject;
	
	renderer        *raycamera;
	raytraceobjects  objects;
	
	//QCursor       mouse;
	
	
public:
		MainWindow( QWidget* parent = 0, const char* name = 0 );
	~MainWindow();
	void getName();
	
signals:
		void setLineMode(LineMode);
	void setObjectMode(ObjectMode);
	void message(const QString&);
	void clearScr();
	
	public slots:
		void clearCanvas();
	
	
	
};


#endif // MAINWIN_H
