//  file:  camera.cpp

#include "raytrace.hh"
#include <iostream>
#include <fstream>
#include <qstring.h>
#include <qdatastream.h>
#include <qdatetime.h>
#include <qprogressdialog.h>
#include <qprogressbar.h>
#include <qfile.h>
#include <qfiledialog.h>
#include <unistd.h>


#ifndef _OPENMP
	#include <mpi.h>
#else
	#include <omp.h>
#endif

#include "mainwin.hh"

using namespace std;

/*****************************************************/
//  functions for camera

renderer::renderer(GP_Object *newgp)
{
	//std::cout << "camera with gpobject created." << std::endl;
	gpobject=newgp;
	camera();
}


camera::camera(void)
{
	//std::cout << "camera without gpobject created." << std::endl;
	lookat.setrectangular(0,-1,-1);
	lookat.makeunit();
	head.setrectangular(0,1,0);
	position.setrectangular(0,10,10);
	camera_resolution=1;
	setaxes();
	zoom = 1.0f;
}


void renderer::drawscreen(void)
{
	tracescreen();
}


void camera::setlookat(float x, float y, float z)
{
	lookat.setrectangular(x,y,z);
	lookat.makeunit();
	setaxes();
}


void camera::settop(float x, float y, float z)
{
	head.setrectangular(x,y,z);
	setaxes();
}


void camera::setpos(float x,float y,float z)
{
	position.setrectangular(x,y,z);
}


laVector camera::getvector(void)
{
	laVector output(0,0,0);
	float x,y,z;
	cout << "\nPlease input three values (x y z):  ";
	cin >> x >> y >> z;
	cout << std::endl;
	
	output.setrectangular(x,y,z);
	
	return output;
}


laVector camera::getcolor(void)
{
	laVector output(0,0,0);
	float x,y,z;
	cout << "\nPlease input three integers [0,255] (r g b):  ";
	cin >> x >> y >> z;
	cout << std::endl;
	
	output.setrectangular(x,y,z);
	output.cropforcolor();
	
	return output;
}


void camera::setaxes(void)
{
	right=cross(lookat,head);
	head=cross(right,lookat);  // make head normal to lookat
	right.makeunit();
	head.makeunit();
	lookat.makeunit();
}

// Note that this function will only be called by id 0
// The function antialias has been set up to do both antialiasing
// and to serve as the origin for calculation by the client
// processes.
void renderer::tracescreen(void)
{
#ifdef _OPENMP
	omp_set_num_threads( 9 );
#else
	//int   numprocs;
	int   currentproc, chunksize=1;
	//float camera_coordinates[9];  // this will be sent from the function that does this
	//float modified_shapes[10];  // this will be sent from the function that does this
	int   command[4];
	
	short *completed_columns;   // this tells us how much of a section each slave has 
								//completed, so that we only send work when necessary.
	short next_column=0;        // this tells us which column to send next (after initial transmission)
	
	MPI_Status  status;
	MPI_Request request;
	
	QProgressDialog dialog("tracing rays...","Cancel",RESOLUTION_X);
#endif

	int line_colors[3*RESOLUTION_Y+1];
	
	while (!gpobject->GP_IsEmpty())
		((GP_Object *)gpobject)->GP_DeletePoly(0);
	QString labeltext;
	
	QTime    timer;
	laVector  spare;
	laVector  Xdir,Ydir;
	QColor  current;
	
	Xdir=getdX();
	Ydir=getdY();
	
	spare=getBottomLeft();
	
	timer.start();
	
	//  This is where the master program initiates the transfer to the other processes.
	
#ifndef _OPENMP
	MPI_Comm_size(MPI_COMM_WORLD,&numprocs);

	// If there is more than one processors available, this process becomes the master.
	// Otherwise, it does all the work itself.
	if (numprocs > 1)
    {
		//MPI_Barrier(MPI_COMM_WORLD);
		completed_columns = new short [numprocs];
		
		for(int x=1;x<numprocs;x++)
		{
			completed_columns[x]=0;
			command[0]=1;  // this means draw
			command[1]=chunksize*(x-1)*camera_resolution;   //  10 | RESOLUTION_X  is good
			command[2]=(chunksize*x-1)*camera_resolution; //  so this would be 10 columns to computer
			command[3]=camera_resolution;
			MPI_Send(command,4,MPI_INT,x,0,MPI_COMM_WORLD);
		}
		
		next_column = ((numprocs-1)*chunksize)*camera_resolution;
		
		int flag;
		
		currentproc=0;
		for(int X=0;X<RESOLUTION_X;X+=camera_resolution)
		{
			do
			{
				usleep( 50000 ); // sleep for 50ms
				
				// Here, we are checking to see if other processes have sent anything to us.
				currentproc++;
				if (currentproc == numprocs)
					currentproc=1;
				//std::cout << "Main checking to see if " << currentproc << " sent anything." << std::endl;
				line_colors[0]=-1; // we will receive a positive int from the slave
				flag=0;
				MPI_Iprobe(currentproc,1,MPI_COMM_WORLD,&flag,&status);
				if (flag == 1)
				{
					//std::cout << "Main should have a package waiting from " << currentproc << std::endl;
					MPI_Recv(line_colors,3*RESOLUTION_Y+1,MPI_INT,currentproc,1,MPI_COMM_WORLD,&status);
					//std::cout << "Main got package." << std::endl;
				}
			} while (line_colors[0] == -1);
			//std::cout << "Main got package from " << currentproc << std::endl;
			
			(completed_columns[currentproc])++;
			//std::cout << "Main says that " << currentproc << " has completed " 
			//     << completed_columns[currentproc] << " columns." << std::endl;
			if (completed_columns[currentproc] == chunksize)
			{
				//std::cout << "Main says " << currentproc << "completed 10 columns; sending new packet..." << std::endl;
				command[0]=1;
				command[1]=next_column;
				command[2]=next_column+(chunksize-1)*camera_resolution;
				if (next_column < RESOLUTION_X)
				{
					if (next_column > RESOLUTION_X-chunksize)
						command[2]=RESOLUTION_X-1;
					// that's because main won't be waiting for those packets and the clients will deadlock
					
					next_column+=(chunksize*camera_resolution);
					MPI_Isend(command,3,MPI_INT,currentproc,0,MPI_COMM_WORLD,&request);
					// we want main to continue
					completed_columns[currentproc]=0;
				}
			}
			
			laVector currentv;

			for(int Y=0;Y<=RESOLUTION_Y;Y+=camera_resolution)
			{
				currentv.setrectangular(line_colors[3*Y],
							   			line_colors[3*Y+1],
							   			line_colors[3*Y+2]);
				currentv.cropforcolor();
				
				current.setRgb(currentv.get_x(),
							   currentv.get_y(),
							   currentv.get_z());
				if (camera_resolution<=1)
					((GP_Object *)gpobject)->GP_DrawPixel(line_colors[3*RESOLUTION_Y],Y,current);
				else
				{
					for(int a=line_colors[3*RESOLUTION_Y];a<=line_colors[3*RESOLUTION_Y]+camera_resolution-1;a++)
						for(int b=Y;b<=Y+camera_resolution-1;b++)
							((GP_Object *)gpobject)->GP_DrawPixel(a,b,current);
				}
			}
			dialog.setProgress(X);
			if (static_cast<int>(static_cast<float>(X)/static_cast<float>(camera_resolution))%5 == 0)
				((GP_Object *)gpobject)->GP_RefreshScreen();
			float current=(float(timer.elapsed())/float(1000));
			float total=current*(float(RESOLUTION_X)/(float(X+1)));
			float remaining=total-current;
			labeltext="";
			labeltext.sprintf("\nEstimated total time:     %i seconds\nEstimated remaining time: %i\nElapsed:  %i",
							  int(total),int(remaining),int(current));
			dialog.setLabelText(labeltext);
			
			std::cout << labeltext << std::endl << std::endl;
			
			if (dialog.wasCancelled())
				return;
		}
		std::cout << "Total elapsed time:  " << timer.elapsed()/1000 << " seconds" << std::endl; 
		
		delete completed_columns;
    }
	else
#endif
    {
		laVector  topleft,topright,bottomleft,bottomright;
		
		topright=(zoom*lookat+right+(RESOLUTION_Y*head)/RESOLUTION_X);
		topleft=(zoom*lookat-right+(RESOLUTION_Y*head)/RESOLUTION_X);
		bottomleft=(zoom*lookat-right-(RESOLUTION_Y*head)/RESOLUTION_X);
		bottomright=(zoom*lookat+right-(RESOLUTION_Y*head)/RESOLUTION_X);
		
		Xdir=topright-topleft;
		Xdir= Xdir/RESOLUTION_X;
		Ydir=(topleft-bottomleft)/RESOLUTION_Y;
		
		//Xdir.update_rectangular();
		//Xdir.update_spherical();
		//Ydir.update_rectangular();
		//Ydir.update_spherical();
				
		//position.update_rectangular();
		//position.update_spherical();

		for(int X=0;X<=RESOLUTION_X;X+=camera_resolution)
		{
			
		#pragma omp parallel for shared( X, line_colors )
			for(int Y=0;Y<=RESOLUTION_Y;Y+=camera_resolution)
			{
				laVector currentxy=spare + X*camera_resolution*Xdir + Y*camera_resolution*Ydir;

				laVector direction=(zoom*lookat+currentxy);
				direction.makeunit();
				
				//direction.update_rectangular();
				//direction.update_spherical();
				
			#ifndef ANTIALIASING
				laVector currentv=objects->followray(position,direction,0);
			#else
				laVector currentv=antialias(direction,Xdir,Ydir,ANTIALIASING_RESOLUTION);
			#endif
				currentv.cropforcolor();
				
				/*current.setRgb(currentv.get_x(),
							   currentv.get_y(),
							   currentv.get_z());*/

				line_colors[3*Y]=currentv.x();   // red
				line_colors[3*Y+1]=currentv.y(); // green
				line_colors[3*Y+2]=currentv.z(); // blue
				line_colors[3*RESOLUTION_Y]=X;

				/*if (camera_resolution<=1)
					gpobject->GP_DrawPixel(X,Y,current);
				else
				{
					for(int a=X;a<=X+camera_resolution;a++)
						for(int b=Y;b<=Y+camera_resolution;b++)
							gpobject->GP_DrawPixel(a,b,current);
				}*/
				//currentxy=currentxy+camera_resolution*Ydir;
			}
			
			#pragma omp barrier

			laVector currentv;

			for(int Y=0;Y<=RESOLUTION_Y;Y+=camera_resolution)
			{
				currentv.setrectangular(line_colors[3*Y],
							   			line_colors[3*Y+1],
							   			line_colors[3*Y+2]);
				currentv.cropforcolor();
				
				current.setRgb(currentv.get_x(),
							   currentv.get_y(),
							   currentv.get_z());
				if (camera_resolution<=1)
					((GP_Object *)gpobject)->GP_DrawPixel(line_colors[3*RESOLUTION_Y],Y,current);
				else
				{
					for(int a=line_colors[3*RESOLUTION_Y];a<=line_colors[3*RESOLUTION_Y]+camera_resolution-1;a++)
						for(int b=Y;b<=Y+camera_resolution-1;b++)
							((GP_Object *)gpobject)->GP_DrawPixel(a,b,current);
				}
			}

			//dialog.setProgress(X);
			if (int(static_cast<float>(X)/static_cast<float>(camera_resolution))%(5) == 0)
				((GP_Object *)gpobject)->GP_RefreshScreen();
			float current=(float(timer.elapsed())/float(1000));
			float total=current*(float(RESOLUTION_X)/(float(X+1)));
			float remaining=total-current;
			labeltext="";
			labeltext.sprintf("\nEstimated total time:     %i seconds\nEstimated remaining time: %i\nElapsed:  %i",
							  int(total),int(remaining),int(current));
			//dialog.setLabelText(labeltext);
			//if (dialog.wasCancelled())
			//	return;
			std::cout << labeltext << std::endl << std::endl;
		}
		std::cout << "Total elapsed time:  " << timer.elapsed()/1000 << " seconds" << std::endl; 
    }
}


laVector camera::tocamera(laVector input)
{
	matrix transform(3,3),intermediate(3,1);
	laVector output,spare;
	
	transform.set(0,0,right.x());
	transform.set(0,1,head.x());
	transform.set(0,2,lookat.x());
	
	transform.set(1,0,right.y());
	transform.set(1,1,head.y());
	transform.set(1,2,lookat.y());
	
	transform.set(2,0,right.y());
	transform.set(2,1,head.y());
	transform.set(2,2,lookat.y());
	
	transform.inverse();
	spare=input-position;
	
	intermediate=transform*spare;
	
	output.setrectangular(intermediate.get(0,0),
						  intermediate.get(1,0),
						  intermediate.get(2,0));
	return output;
}


laVector camera::toworld(laVector input)
{
	laVector output;
	output=(position+(input.x()*right)+(input.y()*head)+(input.z()*lookat));
	return output;
}


void renderer::openfile(void)
{
#ifdef _OPENMP
	omp_set_num_threads( 1 );
#endif

	lightsource    *light;
	raytraceobject *object;
	QFile           inputfile;
	QFileDialog     dialog;
	float           **surfaces;
	
	inputfile.setName(dialog.getOpenFileName("./","*.dat",(GP_Object *)gpobject,
											 "Open Data File..."));
	ifstream file((const char*)inputfile.name(),ios::in);
	
	float x,y,z,ambient,radius;
	int   num,numsurfaces,surfaceID;
	char  c;
	
	objects->deleteallobjects();
	
	if (file >> x >> y >> z)
    {
		std::cout << "Ambient:    " << x << " " << y << " " << z << std::endl;
		ambient=(x+y+z)/3;
    }
	else
    {
		std::cout << "ERROR IN FILE at 1!!!!!!!" << std::endl;
		return;
    }
	if (file >> num)
    {
		std::cout << "Point:      " << num << std::endl;
		while (num--)
		{
			file >> x >> y >> z;
			std::cout << "           " << x << " " << y << " " << z << std::endl;
			light = new lightsource;
			light->setposition(x,y,z);
			light->setcolor( 255, 255, 255 );
			objects->addlight(light);
		}
    }
	else
    {
		std::cout << "ERROR IN FILE at 2!!!!!!!!!!" << std::endl;
		return;
    }
	if (file >> numsurfaces)
    {
		std::cout << "Surfaces:      " << numsurfaces << std::endl;
		surfaces = new float * [numsurfaces];
		for(int q=0;q<numsurfaces;q++)
		{
			surfaces[q]= new float[10];
			for(int m=0;m<10;m++)
			{
				file >> surfaces[q][m];
				std::cout << surfaces[q][m] << " ";
				if ((m%3)==2)
					std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
    }
	else
    {
		std::cout << "ERROR IN FILE at 3!!!!!!!!!!" << std::endl;
		return;
    }
	if (file >> num)
    {
		std::cout << "Objects       " << num << std::endl;
		for(int q=0;q<num;q++)
		{
			file >> c >> x >> y >> z >> radius >> surfaceID;
			std::cout << "        " << c << " " << x << " " << y << " " << z << " " 
			<< radius << " " <<surfaceID << std::endl;
			switch (c)
			{
				case ('S'):
				default:
					object = new sphere;
					object->setcenter(x,y,z);
					object->setradius(radius);
					break;
			}
			float r,g,b;
			r=surfaces[surfaceID-1][3];
			g=surfaces[surfaceID-1][4];
			b=surfaces[surfaceID-1][5];
			object->setcolor(int(255*r),int(255*g),int(255*b));
			object->setambient(surfaces[surfaceID-1][0]);
			object->setdiffuse((r+g+b)/3);
			object->setspecular(surfaces[surfaceID-1][6]);
			object->setphong(surfaces[surfaceID-1][9]);
			
			object->setcolor(255,255,255);
			
			objects->addobject(object);
			
		}
    }
	else
    {
		std::cout << "ERROR IN FILE at 4!!!!!!!!!!" << std::endl;
		return;
    }
	
	for(int q=0;q<numsurfaces;q++)
		delete surfaces[q];
	delete [] surfaces;
	std::cout << "Success opening file" << std::endl;
}


void renderer::setresolution(void)
{
	std::cout << "\nPlease input a new drawing resolution.  (a value of 3 means rays are 3 pixels wide):  ";
	cin >> camera_resolution;
	if (camera_resolution < 1)
		camera_resolution=1;
}


void renderer::set_camera_position_world(void)
{
	int command[4];
	float cameracoor[3];
	command[0]=2;  // camera is moving
	command[1]=2;  // camera location
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	position=getvector();
	cameracoor[0]=position.get_x();
	cameracoor[1]=position.get_y();
	cameracoor[2]=position.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
}


void renderer::set_camera_position_camera(void)
{
	int command[4];
	float cameracoor[3];
	command[0]=2;  // camera is moving
	command[1]=2;  // camera location
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif

	
	laVector newposition=getvector();
	position=(position+(newposition.x()*right)+(newposition.y()*head)
			  +(newposition.z()*lookat));
	
	cameracoor[0]=position.get_x();
	cameracoor[1]=position.get_y();
	cameracoor[2]=position.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
}


void renderer::set_lookat_world(void)
{
	int command[4];
	float cameracoor[3];
	
	lookat=getvector();
	setaxes();
	
	command[0]=2;  // camera is moving
	command[1]=0;  // camera lookat
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	cameracoor[0]=lookat.get_x();
	cameracoor[1]=lookat.get_y();
	cameracoor[2]=lookat.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
	command[0]=2;  // camera is moving
	command[1]=1;  // camera up
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	cameracoor[0]=head.get_x();
	cameracoor[1]=head.get_y();
	cameracoor[2]=head.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
}


void renderer::set_lookat_camera(void)
{
	int command[4];
	float cameracoor[3];
	
	laVector newlookat=getvector();
	lookat=((newlookat.x()*right)+(newlookat.y()*head)+
			(newlookat.z()*lookat));
	setaxes();
	
	command[0]=2;  // camera is moving
	command[1]=0;  // camera lookat
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	cameracoor[0]=lookat.get_x();
	cameracoor[1]=lookat.get_y();
	cameracoor[2]=lookat.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
	command[0]=2;  // camera is moving
	command[1]=1;  // camera up
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	cameracoor[0]=head.get_x();
	cameracoor[1]=head.get_y();
	cameracoor[2]=head.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
}


void renderer::set_top_world(void)
{
	int command[4];
	float cameracoor[3];
	
	head=getvector();
	setaxes();
	
	command[0]=2;  // camera is moving
	command[1]=1;  // camera up
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	cameracoor[0]=head.get_x();
	cameracoor[1]=head.get_y();
	cameracoor[2]=head.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
}


void renderer::set_top_camera(void)
{
	int command[4];
	float cameracoor[3];
	
	laVector newtop=getvector();
	head=((newtop.x()*right)+(newtop.y()*head)+
		  (newtop.z()*lookat));
	setaxes();
	
	command[0]=2;  // camera is moving
	command[1]=1;  // camera up
#ifndef _OPENMP
	MPI_Bcast(command,4,MPI_INT,0,MPI_COMM_WORLD);
#endif
	cameracoor[0]=head.get_x();
	cameracoor[1]=head.get_y();
	cameracoor[2]=head.get_z();
#ifndef _OPENMP
	MPI_Bcast(cameracoor,3,MPI_FLOAT,0,MPI_COMM_WORLD);
#endif
}


void renderer::set_attenuation(void)
{
	float newatt;
	std::cout << "Please input a new light attenuation factor:  ";
	cin >> newatt;
	objects->set_attenuation(newatt);
}



void renderer::add_sphere(void)
{
	sphere *newsphere=new sphere;
	float radius;
	float amb,diff,spec,phong;
	std::cout << "Please input a center point:\n";
	laVector center=getvector();
	std::cout << "Please input a radius:  ";
	cin >> radius;
	std::cout << "\nPlease input a color:\n";
	laVector color=getcolor();
	std::cout << "Please input ambient reflection coeficient:  ";
	cin >> amb;
	std::cout << "\nPlease input diffuse reflection coeficient:  ";
	cin >> diff;
	std::cout << "\nPlease input specular reflection coeficient:  ";
	cin >> spec;
	std::cout << "\nPlease input phong value:  ";
	cin >> phong;
	std::cout << "\n";
	newsphere->setcenter(center);
	newsphere->setradius(radius);
	newsphere->setcolor(color.x(),color.y(),color.z());
	newsphere->setambient(amb);
	newsphere->setdiffuse(diff);
	newsphere->setspecular(spec);
	newsphere->setphong(phong);
	
	objects->addobject(newsphere);
	
}


void renderer::add_plane(void)
{
	plane *newplane=new plane;
	float amb,diff,spec,phong;
	std::cout << "Please input a reference point:\n";
	laVector point=getvector();
	std::cout << "Please input the normal laVector:  ";
	laVector norm=getvector();
	std::cout << "\nPlease input a color:\n";
	laVector color=getcolor();
	std::cout << "Please input ambient reflection coeficient:  ";
	cin >> amb;
	std::cout << "\nPlease input diffuse reflection coeficient:  ";
	cin >> diff;
	std::cout << "\nPlease input specular reflection coeficient:  ";
	cin >> spec;
	std::cout << "\nPlease input phong value:  ";
	cin >> phong;
	std::cout << "\n";
	newplane->setpoint(point.x(),point.y(),point.z());
	newplane->setnormal(norm.x(),norm.y(),norm.z());
	newplane->setcolor(color.x(),color.y(),color.z());
	newplane->setambient(amb);
	newplane->setdiffuse(diff);
	newplane->setspecular(spec);
	newplane->setphong(phong);
	
	objects->addobject(newplane);
	
}


void renderer::add_light(void)
{
	lightsource *light=new lightsource;
	std::cout << "Please input the position:  ";
	laVector pos=getvector();
	std::cout << "\nPlease input a color:\n";
	laVector color=getcolor();
	light->setcolor(int(color.x()),int(color.y()),int(color.z()));
	light->setposition(pos.x(),pos.y(),pos.z());
	objects->addlight(light);
}


void renderer::remove_last_light(void)
{
	objects->remove_one_light();
}


void renderer::remove_last_object(void)
{
	objects->remove_one_object();
}


void renderer::setbackground(void)
{
	std::cout << "Please input a new background color:\n";
	laVector color=getcolor();
	objects->setbackground(color.x(),color.y(),color.z());
}


laVector camera::antialias(laVector direction,laVector Xdir,laVector Ydir,int resolution)
{
	laVector antialiasing(0,0,0);
	laVector antialiasing_reminder(0,0,0);
	laVector currentv(0,0,0);
	
	for(int antix=0;antix < resolution;antix++)
    {
		for(int antiy=0;antiy < resolution;antiy++)
		{
			laVector new_color = objects->followray(position,direction+antialiasing,0, -1 );
			new_color.cropforcolor();
			currentv=currentv+new_color;
			antialiasing=antialiasing+Ydir/resolution;
			//std::cout << "antialiasing:  " << antix << " " << antiy << std::endl;
		}
		antialiasing_reminder=antialiasing_reminder+Xdir/static_cast<float>(resolution);
		antialiasing=antialiasing_reminder;
    }
	return currentv/static_cast<float>(resolution*resolution);
}


laVector camera::getdX(void)
{
	laVector topright,topleft;
	topright=getTopRight();
	topleft=getTopLeft();
	return ((topright-topleft)/static_cast<float>(RESOLUTION_X));
}


laVector camera::getdY(void)
{
	laVector topleft,bottomleft;
	topleft=getTopLeft();
	bottomleft=getBottomLeft();
	return ((topleft-bottomleft)/static_cast<float>(RESOLUTION_Y));
}


laVector camera::getBottomLeft(void)
{
	return (zoom*lookat-right-(static_cast<float>(RESOLUTION_Y)*head)/static_cast<float>(RESOLUTION_X));
}


laVector camera::getBottomRight(void)
{
	return (zoom*lookat+right-(static_cast<float>(RESOLUTION_Y)*head)/static_cast<float>(RESOLUTION_X));
}


laVector camera::getTopLeft(void)
{
	return (zoom*lookat-right+(static_cast<float>(RESOLUTION_Y)*head)/static_cast<float>(RESOLUTION_X));
}


laVector camera::getTopRight(void)
{
	return (zoom*lookat+right+(static_cast<float>(RESOLUTION_Y)*head)/static_cast<float>(RESOLUTION_X));
}
