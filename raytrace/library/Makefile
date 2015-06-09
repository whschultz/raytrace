CC = g++
QT_LIBS = -lqt-mt
GL_LIBS = -framework OpenGL
MESA_LIBS = -lMesaGL -lMesaGLU
X_LIBS = -L /usr/X11R6/lib -lXmu -lX11 -lXext
LIBS = $(QT_LIBS) $(GL_LIBS) -lm

.SUFFIXES: .cpp .cxx .cc .C .c

.cpp.o:
	$(CC) -c  $(CXXFLAGS) -o $@ $< 

#LINUX SPECIFIC
#QT_INCL = /usr/lib/qt-2.0.2/include
#LDFLAGS = -L/usr/lib/qt-2.0.2/lib -L/usr/include/GL
#CXXFLAGS = -I$(QT_INCL) -O6 -funroll-all-loops -fstrict-prototypes -Wall -ffast-math

#SOLARIS SPECIFIC
#QT_INCL = -I/afs/uncc/coe/unix/apps56/gnu/qt202/include
#LDFLAGS = -L/afs/uncc/coe/public/krs/lib/
#CXXFLAGS = -I/afs/uncc/coe/public/krs/include $(QT_INCL)

#
#OS X Specific
#
QT_INCL = -I/sw/include/qt/
GL_INCL = -I/usr/X11R6/include/
#GL_INCL = /usr/include
LDFLAGS = -L/sw/lib
CXXFLAGS = $(QT_INCL) $(GL_INCL)




objs: gpackage.o glwidget.o Makefile moc_gpackage.cpp

moc_gpackage.cpp: gpackage.hh
	moc -o moc_gpackage.cpp gpackage.hh
clean:
	rm -f *.o moc_gpackage.cpp

backup: gpackage.cpp gpackage.hh glwidget.cpp
	tar -cvf gpackage.tar Makefile gpackage.hh glwidget.cpp gpackage.cpp
	gzip -9 gpackage.tar

restore: clean
	tar -zxvf gpackage.tar.gz

gpackage.o: Makefile
glwidget.o: Makefile
