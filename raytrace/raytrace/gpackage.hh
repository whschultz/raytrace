//  file:  gpackage.hh

#ifndef GPACKAGE
#define GPACKAGE

#include <qgl.h>
#include <qcolordialog.h>
#include <math.h>
//#include <qobject.h>


typedef enum {CONTINUOUS, DASHED, DOTTED} LineMode;
typedef enum {LINE, OPEN_POLYGON, CLOSED_POLYGON} ObjectMode;
typedef enum {DRAW, MODIFY, DELETE} EditMode;


class vertex
{
public:
	vertex(float a,float b) {x_coor=a; y_coor=b;};
	vertex(void) { };
	friend vertex operator+(vertex a,vertex b) {
		vertex c(a.x_coor+b.x_coor,a.y_coor + b.y_coor); return c;};
	friend vertex operator-(vertex a,vertex b) {
		vertex c(a.x_coor-b.x_coor,a.y_coor-b.y_coor); return c;};
	friend vertex operator*(vertex a,vertex b) {
		vertex c(a.x_coor * b.x_coor,a.y_coor * b.y_coor); return c;};
	friend vertex operator/(vertex a,vertex b) {
		vertex c(a.x_coor / b.x_coor,a.y_coor / b.y_coor); return c;};
	void set(float a,float b) {x_coor=a; y_coor=b;};
	float x(void) {return x_coor;};
	float y(void) {return y_coor;};
	//float distance(vertex v) {float d=pow(pow(x_coor - v.x_coor,2)+pow(y_coor-v.y_coor,2),.5);return d;}
	float distance(vertex v) {return hypot(x_coor-v.x_coor,y_coor-v.y_coor);}
private:
		float x_coor, y_coor;
};


class object
{
public:
	object(float [],int); //this int represents the number of vertices, not the size of the array
	object(void) { }
	~object() {delete [] vertices;};
	void setobject(float*,int);
	void drawobject(char *);
	void setmode(ObjectMode,LineMode,int,int,int,  int,int,int);  // line color RGB,  polygon color RGB
	void setmode(ObjectMode,LineMode,int,int,int);                //  just polygon color
	vertex getVertex(int a) {if (a<=number) return vertices[a]; else {vertex b(0,0); return b;}};
	void setVertex(int a,vertex b) {if (a<=number) vertices[a].set(b.x(),b.y());};
	int proximity(vertex,float);  //  point to compare at what distance
private:
		vertex *vertices;
	int number;
	ObjectMode fill_type;
	LineMode line_type;
	int lred,lgreen,lblue;  // line color
	int pred,pgreen,pblue;  // polygon interior color
};


class lobject 
{
public:
	object *theShape;
	lobject *next;
	lobject *previous;
	lobject() {theShape = 0; next = 0; previous = 0;};
	void set(float *data, int num, lobject *pd, lobject *nd) 
	{theShape = new object(data,num); next = nd; previous = pd;};
	~lobject();
	void drawobjects(char *);
	int proximity(vertex,float,int *);
};


class GP_Object : public QGLWidget
{
	Q_OBJECT  //needed if any signals and slots are used
public:
	GP_Object( QWidget* parent, const char* name );
	~GP_Object();
signals:
		void glmessage(const QString&);
	public slots:
		void clearScreen();
	void LineModeCONT(void)     { currentL = CONTINUOUS; }
	void LineModeDASH(void)     { currentL = DASHED; }
	void LineModeDOT(void)      { currentL = DOTTED; }
	void ObjectModeLINE(void)   { currentO = LINE; }
	void ObjectModeOPEN(void)   { currentO = OPEN_POLYGON; }
	void ObjectModeCLOSED(void) { currentO = CLOSED_POLYGON; }
	void EditModeDRAW(void)     { emode = DRAW; }
	void EditModeMODIFY(void)   { emode = MODIFY; } 
	void EditModeDELETE(void)   { emode = DELETE; }
	void setLineColor();
	void setPolyColor();
public:
		void initializeGL();
	virtual void mousePressEvent( QMouseEvent * );
	
	void GP_UpdateScreen(void);
	void GP_RefreshScreen(void) {updateGL();}
	void GP_AddPoints(float,float,int);
	void GP_Initialize();
	void GP_Terminate();
	void GP_Line(vertex,vertex);
	void GP_Polygon(float[],int);
	void GP_SetLineStyle(LineMode L) {currentL = L;};
	void GP_SetPolyStyle(ObjectMode M) {currentO = M;};
	void GP_SetColorPoly(int r,int g,int b) {polyColor.setRgb(r,g,b);}  // red, green, blue
	void GP_SetColorLine(int r,int g,int b) {lineColor.setRgb(r,g,b);};  // red, green, blue
	void GP_SetWorldCoordinates(float,float,float,float); //xMin,xMax,yMin,yMax
	int  GP_Proximity(float,float,int,int*);
	int  GP_Proximity(float,float,int);
	void GP_UpdatePoly(int);
	void GP_DeletePoly(int);
	int  GP_IsEmpty(void) { if (shapes==0) return 1; else if (shapes->theShape==0) return 1; else return 0;}
	void GP_Text(const QString &output) {emit glmessage(output);}
	void GP_DrawPixel(float,float,QColor);
	
protected:
		LineMode currentL;
	ObjectMode currentO;
	EditMode emode;
	int toggleFree;
	int X, Y, lastX, lastY;
	int num_points;
	float *points;
	lobject *shapes;
	QColor *black;
	QColor *white;
	QColor *background;
	QColor lineColor;
	QColor polyColor;
};

#endif

