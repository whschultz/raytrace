/****************************************************************************
** namewin.hh
****************************************************************************/
#ifndef DIALOG_H_
#define DIALOG_H_

#include <qwidget.h>
#include <qlabel.h>
#include <qdialog.h>
#include <qlineedit.h>
#include <qapplication.h>
#include <qstring.h>



class NameWindow: public QDialog
{
	Q_OBJECT
	
public:
	NameWindow( QWidget *parent, const char *name);
	~NameWindow();
	QString returnName();
	
	public slots:
		void newName();
	
private:
		QLineEdit *le;
	QLabel    *nlabel;
	QPushButton *ok, *cancel;
	QString name;
};

#endif
