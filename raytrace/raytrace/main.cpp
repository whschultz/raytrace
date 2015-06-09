//
// Qt OpenGL example: Box
//
// A small example showing how a GLWidget can be used just as any Qt widget
// 
// File: main.cpp
//
// The main() function 
// 

#include "raytrace.hh"
#include "mainwin.hh"
#include <qapplication.h>
#include "matrix.hh"
#include "mpi.h"
#include <unistd.h>

#ifndef _OPENMP
	#include <mpi.h>
#else
	#include <omp.h>
#endif


/*
 The main program is here. 
 */

int main( int argc, char **argv )
{
#ifndef _OPENMP

	int numprocs, myid;
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD,&myid);
#endif

	int return_value = 0;

#ifndef _OPENMP
	if (myid == 0)
    {
#endif
		std::cout << "Hi, I'm the main process." << std::endl;
		QApplication::setColorSpec( QApplication::CustomColor );
		QApplication a(argc,argv);
		
		MainWindow w;
		//w.resize( 800, 600 );
		a.setMainWidget( &w );
		w.show();
		
		//w.getName();
		
		return_value = a.exec();
		
		//MPI_Bcast(QUIT!);
		
#ifndef _OPENMP
    }
	else
    {
		std::cout << myid << " says it is a slave and wants to be emancipated." << std::endl;
		// communication variables
		float camera_coordinates[3]; // this is for moving the camera.  It would be BAD without this.  ;)
		int line_colors[3*RESOLUTION_Y+1];  // the plus one is to designate which column to draw 
		float modified_shapes[10];   // this designates which object type was added
		int   command[4];            // tells the slaves what the user is doing
		
		int min_column,max_column,current_column,resolution;
		
		MPI_Status status;
		MPI_Request request;
		
		laVector the_color;
		laVector dX,dY,bottomleft;
		
		std::cout << myid << " creating camera..." << std::endl;
		
		camera raycamera;
		
		std::cout << myid << " success!!!" << std::endl;
		raytraceobjects objects;
		raycamera.setobjects(&objects);
		
		// rewrite this to do what main sends
		
		// begin default initializations:
		// copied straight from mainwin's initialization.
		raycamera.setpos(0,40,100);
		raycamera.setlookat(0,-4,-10);
		raycamera.settop(0,1,0);
		raycamera.setZoom(4.0f);
		
		objects.setbackground(15.0f,10.0f,25.5f);
		objects.set_attenuation( 3000 );

		sphere *sparesphere= NULL; //new sphere;
		plane  *spareplane=new plane;
		lightsource *sparelight=NULL;//new lightsource;
		
		spareplane->setpoint(0,-3,0);
		spareplane->setnormal(0,1,0);
		spareplane->setambient(.0);
		spareplane->setdiffuse(.6);
		spareplane->setspecular(.4);
		spareplane->setphong( 30 );
		spareplane->setcolor(255,255,255);
		objects.addobject(spareplane);
		
		float x = 0.0f, y = 0.0f;
		
		const float dx = 6.0f;
		const float dy = sqrt(3) * 3.0f;
		const float dz = dy;
		
		float x0 = 0.0f;
		
		const int dim = 5;
		
		for( int k = 0; k < dim; k++ )
		{
			y = float(k) * 3.0f;
			x0 = 0.0f;
			
			for( int i = 0; i < dim - k; i++ )
			{
				x = x0;
				
				for( int j = 0; j <= i; j++ )
				{
					if ( k > 0 )
					{
						sparelight = new lightsource;
						sparelight->setcolor(255,255,255);
						sparelight->setposition( x, float(k)*dz - 4, y );
						sparelight->setradius( .1 );
						objects.addlight(sparelight);
					}

					sparesphere = new sphere;
					sparesphere->setcenter( x, float(k)*dz, y );
					sparesphere->setradius( 3 );
					sparesphere->setambient( .0 );
					sparesphere->setdiffuse( 1.0f - float(k)/dim );
					sparesphere->setspecular( float(k)/dim );
					sparesphere->setphong( 30 );
					
					if ( k < dim )
					{
						sparesphere->setcolor( 255.0f*cos( float(i)*pi/( 2.0f*float(dim-k) ) ),
											   255.0f*cos( float(i-j-dim)*pi/(2.0f*float(dim-k)) ),
											   255.0f*sin( float(j)*pi/(2.0f*float(dim-k)) ) );
					}
					else
					{
						sparesphere->setcolor( 255, 255, 255 );
					}
					objects.addobject(sparesphere);
					
					x += dx;
				}
				
				x0 -= dy/2;
				y += dy;
			}
		}
		
		
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
		objects.addobject(spareplane);

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
		objects.addobject(sparesphere);*/
		
		
		//for( double k = 0.0f; k < 64.0f; k += 32.0f )
		{
			sparelight = new lightsource;
			sparelight->setcolor(255,0,0);
			sparelight->setposition( -32, 16, 32 );
			sparelight->setradius( .3 );
			objects.addlight(sparelight);
			
			sparelight = new lightsource;
			sparelight->setcolor(0,0,255);
			sparelight->setposition( 32, 16, 32 );
			sparelight->setradius( .3 );
			objects.addlight(sparelight);
			
			sparelight = new lightsource;
			sparelight->setcolor(0,255,0);
			sparelight->setposition( -32, 16, -32);
			sparelight->setradius( .3 );
			objects.addlight(sparelight);
			
			sparelight = new lightsource;
			sparelight->setcolor(128,128,0);
			sparelight->setposition( 32, 16,-32 );
			sparelight->setradius( .3 );
			objects.addlight(sparelight);
		}

		// end defaults initializations
		
		//MPI_Barrier(MPI_COMM_WORLD);
		
		//  begin main slave event loop
		do
		{
			std::cout << myid << " waiting for command from main..." << std::endl;
			
			int flag = 0;
			
			MPI_Irecv(command,4,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&request);

			do
			{
				usleep( 50000 ); // sleep for 50ms
				
				// Here, we are checking to see if other processes have sent anything to us.
				flag=0;
				MPI_Test( &request, &flag, &status );
			} while ( flag == 0 );

			//MPI_Recv(command,4,MPI_INT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
			std::cout << myid << " got command from main...  checking..." << std::endl;
			//  The main process is sending a lot of these, so the slaves
			//  will wait.  The result of this tells us what to do next.
			
			switch (command[0])
			{
				case (0):  // this means quit
					std::cout << myid << " got quit message from main." << std::endl;
					continue;// do-while loop will end on its own.
					break;
				case (1):  // this means draw
					std::cout << myid << " got draw message from main." << std::endl;
					dX = raycamera.getdX();
					dY = raycamera.getdY();
					bottomleft = raycamera.getBottomLeft();
					
					min_column = command[1]; // done strictly for clarity of code
					max_column = command[2]; // there would be slightly less communication w/o
					resolution = command[3];
					raycamera.setresolution(resolution);
					
					for(current_column=min_column;current_column<=max_column;current_column+=resolution)
					{
						for(int row=0;row<RESOLUTION_Y;row++)
						{
							the_color=raycamera.antialias(bottomleft+(row*dY)+(current_column*dX),dX,dY,ANTIALIASING_RESOLUTION);
							the_color.cropforcolor();
							line_colors[3*row]=the_color.x();   // red
							line_colors[3*row+1]=the_color.y(); // green
							line_colors[3*row+2]=the_color.z(); // blue
						}
						line_colors[3*RESOLUTION_Y]=current_column;
						//std::cout << myid << " finished a column, sending to main..." << std::endl;
						MPI_Ssend(line_colors,3*RESOLUTION_Y+1,MPI_INT,0,1,MPI_COMM_WORLD);
						//std::cout << myid << " main got my packet!" << std::endl;
						// the main process is checking for a lot of these, so we want the slave
						// process to wait for the main process.  It must be a blocking send.
					}
						break;
				case (2):  // this means move the camera
					std::cout << myid << " got camera move command from main." << std::endl;
					MPI_Recv(camera_coordinates,3,MPI_FLOAT,0,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
					//  Since the camera has been moved, the main process is sending a lot of these
					//  so we want the slave process to wait for the main process.
					switch (command[1])
					{
						case (0):
							raycamera.setlookat(camera_coordinates[0],
												camera_coordinates[1],
												camera_coordinates[2]);
							break;
						case (1):
							raycamera.settop (  camera_coordinates[0],
												camera_coordinates[1],
												camera_coordinates[2]);
							break;
						case (2):
							raycamera.setpos (  camera_coordinates[0],
												camera_coordinates[1],
												camera_coordinates[2]);
							break; 
					}
						break;
				case (3):  // this means add objects
					break;
				case (4):  // this means remove objects
					break;
			}
			
			usleep( 1000 );  // sleep 1ms
		} while (command[0] != -1);
		// don't forget to do whatever shutdown procedures are necessary here
    }
    
    std::cout << myid << " Just thought you would like to know.  I quit with:" << return_value <<std::endl;
#endif

	return return_value;
}
