SDK_PATH = /
#SDK_PATH = /Volumes/Boot
#CC = $(SDK_PATH)/usr/bin/g++ -B$(SDK_PATH)/usr/bin -mmacosx-version-min=10.4 -isysroot $(SDK_PATH) -g -O3 -falign-loops=16 -mdynamic-no-pic -funroll-loops -mcpu=7400 -faltivec -maltivec
#CC = $(SDK_PATH)/sw/lib/gcc4.2/bin/g++ --openmp -B$(SDK_PATH)/sw/lib/gcc4.2/bin -mmacosx-version-min=10.4 -isysroot $(SDK_PATH) -g -O3 -falign-loops=16 -funroll-loops -march=prescott -mfpmath=sse -malign-double -msse3 # -mdynamic-no-pic
#CC = $(SDK_PATH)/usr/bin/g++ -B$(SDK_PATH)/usr/bin -mmacosx-version-min=10.10 -isysroot $(SDK_PATH) -g -O3 -funroll-loops -mfpmath=sse -msse3 -mdynamic-no-pic # -falign-loops=16 -malign-double -march=prescott
CC = $(SDK_PATH)/usr/bin/g++ -m64
QTDIR=/opt/local/lib/qt3

DEBUGER = ddd
OBJS = namewin.o mainwin.o main.o raytrace.o vector.o matrix.o camera.o raytrace_objects.o objects.o
MOC_OBJS = moc_namewin.o moc_mainwin.o moc_raytrace.o moc_gpackage.o
SRCS = namewin.cpp mainwin.cpp main.cpp raytrace.cpp vector.cpp matrix.cpp camera.cpp raytrace_objects.cpp objects.cpp
MOC_SRCS = moc_namewin.cpp moc_mainwin.cpp moc_raytrace.cpp moc_gpackage.cpp
GP_SRCS = gpackage.cpp glwidget.cpp 
GP_INCLUDES = gpackage.hh
GP_OBJS = gpackage.o glwidget.o 
INCLUDES = namewin.hh mainwin.hh raytrace.hh vector.hh matrix.hh
TARGET = raytrace
MOC = $(QTDIR)/bin/moc
QT_LIBS = -lqt-mt -lqui
#QT_LIBS = -lqt
#GL_LIBS = -lGL -lGLU
#MESA_LIBS = -lMesaGL -lMesaGLU
X_LIBS = -lXmu -lX11 -lXext
LIBS = $(QT_LIBS) $(GL_LIBS) $(X_LIBS) -lm -lmpi -llammpi++ -llam -framework Accelerate

.SUFFIXES: .cpp .cxx .cc .C .c
.cpp.o:
	$(CC) -c  $(CXXFLAGS) $(CXXOPTIONS) -o $@ $< 

#
#LINUX-SPECIFIC
#
#QT_INCL = /usr/lib/qt-2.0.2/include
#LDFLAGS = -L/usr/lib/qt-2.0.2/lib -L/usr/include/GL
#CXXFLAGS =  -I$(QT_INCL) 
#
#SUNOS-SPECIFIC
#
#QT_INCL = -I/afs/uncc/coe/unix/apps56/gnu/qt202/include
#LDFLAGS = -L/afs/uncc/coe/public/krs/lib/
#CXXFLAGS = -I/afs/uncc/coe/public/krs/include $(QT_INCL)

#
#OS X Specific
#
QT_INCL = -I$(QTDIR)/include
GL_INCL = -I/opt/X11/include
MPI_INCL = -I/opt/local/include
LDFLAGS = -L/opt/local/lib -L/opt/X11/lib -L/usr/lib -bind_at_load $(FRAMEWORK_INCL) #-Xlinker -syslibroot -Xlinker $(SDK_PATH)
GL_LIBS = -lGL -lGLU
FRAMEWORK_INCL = -iframework $(SDK_PATH)/System/Library/Frameworks/Accelerate.framework/Versions/A/Frameworks
CXXFLAGS = $(GL_INCL) $(QT_INCL) $(MPI_INCL) $(FRAMEWORK_INCL) -Wall 




CXXOPTIONS = #-O2 -funroll-all-loops -fstrict-prototypes -Wall


$(TARGET): $(GP_OBJS) $(OBJS) $(MOC_OBJS) Makefile $(INCLUDES)
	$(CC) $(OBJS) $(MOC_OBJS) $(GP_OBJS) $(CXXFLAGS) $(CXXOPTIONS) -o $@ $(LDFLAGS) $(LIBS)
	rm -f core
edit:
	edit Makefile $(SRCS) $(INCLUDES) $(GP_SRCS) $(GP_INCLUDES) &
print:
	a2ps manual.txt ../library/Makefile $(GP_INCLUDES) $(GP_SRCS) Makefile $(INCLUDES) $(SRCS) | lp
backup: $(TARGET).tar.gz 
$(TARGET).tar.gz: Makefile $(INCLUDES) $(SRCS) $(GP_SRCS) manual.txt
	tar -cvf $(TARGET).tar $(SRCS) $(INCLUDES) $(GP_SRCS) $(GP_INCLUDES) Makefile manual.txt *.dat
	gzip -9 $(TARGET).tar
restore: clean
	tar -zxvf $(TARGET).tar.gz
debug: $(TARGET)
	$(DEBUGER) $(TARGET) core &
clean:
	rm -rf core $(OBJS) $(MOC_OBJS) $(MOC_SRCS) $(GP_OBJS) $(TARGET) *~
depend: $(SRCS)
	makedepend $(SRCS) $(CXX_FLAGS) $(CXX_OPTIONS) -V
moc_namewin.cpp: namewin.hh
	$(MOC) namewin.hh -o moc_namewin.cpp
moc_mainwin.cpp: mainwin.hh
	$(MOC) mainwin.hh -o moc_mainwin.cpp
moc_gpackage.cpp: gpackage.hh
	$(MOC) gpackage.hh -o moc_gpackage.cpp
moc_raytrace.cpp: raytrace.hh
	$(MOC) raytrace.hh -o moc_raytrace.cpp


# DO NOT DELETE

namewin.o: namewin.hh
mainwin.o: mainwin.hh gpackage.hh /usr/include/math.h
mainwin.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
mainwin.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
mainwin.o: /usr/include/AvailabilityInternal.h raytrace.hh vector.hh
mainwin.o: matrix.hh namewin.hh
main.o: raytrace.hh gpackage.hh /usr/include/math.h /usr/include/sys/cdefs.h
main.o: /usr/include/sys/_symbol_aliasing.h
main.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
main.o: /usr/include/AvailabilityInternal.h vector.hh matrix.hh mainwin.hh
main.o: /usr/include/unistd.h /usr/include/_types.h /usr/include/sys/_types.h
main.o: /usr/include/machine/_types.h /usr/include/i386/_types.h
main.o: /usr/include/sys/_pthread/_pthread_types.h /usr/include/sys/unistd.h
main.o: /usr/include/sys/_types/_posix_vdisable.h
main.o: /usr/include/sys/_types/_seek_set.h /usr/include/sys/_types/_size_t.h
main.o: /usr/include/_types/_uint64_t.h /usr/include/sys/_types/_ssize_t.h
main.o: /usr/include/sys/_types/_uid_t.h /usr/include/sys/_types/_gid_t.h
main.o: /usr/include/sys/_types/_intptr_t.h /usr/include/sys/_types/_off_t.h
main.o: /usr/include/sys/_types/_pid_t.h
main.o: /usr/include/sys/_types/_useconds_t.h /usr/include/sys/_types/_null.h
main.o: /usr/include/sys/select.h /usr/include/sys/appleapiopts.h
main.o: /usr/include/sys/_types/_fd_def.h /usr/include/sys/_types/_timespec.h
main.o: /usr/include/sys/_types/_timeval.h /usr/include/sys/_types/_time_t.h
main.o: /usr/include/sys/_types/_suseconds_t.h
main.o: /usr/include/sys/_types/_sigset_t.h
main.o: /usr/include/sys/_types/_fd_setsize.h
main.o: /usr/include/sys/_types/_fd_set.h /usr/include/sys/_types/_fd_clr.h
main.o: /usr/include/sys/_types/_fd_isset.h
main.o: /usr/include/sys/_types/_fd_zero.h /usr/include/sys/_types/_fd_copy.h
main.o: /usr/include/sys/_select.h /usr/include/sys/_types/_dev_t.h
main.o: /usr/include/sys/_types/_mode_t.h /usr/include/sys/_types/_uuid_t.h
main.o: /usr/include/gethostuuid.h
raytrace.o: raytrace.hh gpackage.hh /usr/include/math.h
raytrace.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
raytrace.o: /usr/include/sys/_posix_availability.h
raytrace.o: /usr/include/Availability.h /usr/include/AvailabilityInternal.h
raytrace.o: vector.hh matrix.hh
vector.o: vector.hh /usr/include/math.h /usr/include/sys/cdefs.h
vector.o: /usr/include/sys/_symbol_aliasing.h
vector.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
vector.o: /usr/include/AvailabilityInternal.h
matrix.o: matrix.hh vector.hh /usr/include/math.h /usr/include/sys/cdefs.h
matrix.o: /usr/include/sys/_symbol_aliasing.h
matrix.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
matrix.o: /usr/include/AvailabilityInternal.h
camera.o: raytrace.hh gpackage.hh /usr/include/math.h
camera.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
camera.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
camera.o: /usr/include/AvailabilityInternal.h vector.hh matrix.hh
camera.o: /usr/include/unistd.h /usr/include/_types.h
camera.o: /usr/include/sys/_types.h /usr/include/machine/_types.h
camera.o: /usr/include/i386/_types.h
camera.o: /usr/include/sys/_pthread/_pthread_types.h
camera.o: /usr/include/sys/unistd.h /usr/include/sys/_types/_posix_vdisable.h
camera.o: /usr/include/sys/_types/_seek_set.h
camera.o: /usr/include/sys/_types/_size_t.h /usr/include/_types/_uint64_t.h
camera.o: /usr/include/sys/_types/_ssize_t.h /usr/include/sys/_types/_uid_t.h
camera.o: /usr/include/sys/_types/_gid_t.h
camera.o: /usr/include/sys/_types/_intptr_t.h
camera.o: /usr/include/sys/_types/_off_t.h /usr/include/sys/_types/_pid_t.h
camera.o: /usr/include/sys/_types/_useconds_t.h
camera.o: /usr/include/sys/_types/_null.h /usr/include/sys/select.h
camera.o: /usr/include/sys/appleapiopts.h /usr/include/sys/_types/_fd_def.h
camera.o: /usr/include/sys/_types/_timespec.h
camera.o: /usr/include/sys/_types/_timeval.h
camera.o: /usr/include/sys/_types/_time_t.h
camera.o: /usr/include/sys/_types/_suseconds_t.h
camera.o: /usr/include/sys/_types/_sigset_t.h
camera.o: /usr/include/sys/_types/_fd_setsize.h
camera.o: /usr/include/sys/_types/_fd_set.h /usr/include/sys/_types/_fd_clr.h
camera.o: /usr/include/sys/_types/_fd_isset.h
camera.o: /usr/include/sys/_types/_fd_zero.h
camera.o: /usr/include/sys/_types/_fd_copy.h /usr/include/sys/_select.h
camera.o: /usr/include/sys/_types/_dev_t.h /usr/include/sys/_types/_mode_t.h
camera.o: /usr/include/sys/_types/_uuid_t.h /usr/include/gethostuuid.h
camera.o: mainwin.hh
raytrace_objects.o: raytrace.hh gpackage.hh /usr/include/math.h
raytrace_objects.o: /usr/include/sys/cdefs.h
raytrace_objects.o: /usr/include/sys/_symbol_aliasing.h
raytrace_objects.o: /usr/include/sys/_posix_availability.h
raytrace_objects.o: /usr/include/Availability.h
raytrace_objects.o: /usr/include/AvailabilityInternal.h vector.hh matrix.hh
objects.o: raytrace.hh gpackage.hh /usr/include/math.h
objects.o: /usr/include/sys/cdefs.h /usr/include/sys/_symbol_aliasing.h
objects.o: /usr/include/sys/_posix_availability.h /usr/include/Availability.h
objects.o: /usr/include/AvailabilityInternal.h vector.hh matrix.hh
