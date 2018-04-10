// Para compilar en MacOSX
// g++ -o practica01 practica01.cpp -framework OpenGL -framework GLUT
// Para compilar desde terminal Ubuntu
//g++ -o practica01V2 practica01V2.cpp  -lGL -lglut -lGLU
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#ifdef __APPLE__
#include "GLUT/glut.h"
#else
#include "GL/glut.h"
#endif

using namespace std;

#define NUMVTX (8)
#define NUMVTXZ (98)
#define NUMVTXQ (24)
#define NUMVTXH (30)

static int winwidth,winheight;
static int mx,my;
static int flag=0;
static float rotx=0.0f, roty=0.0f;

GLfloat v[NUMVTX][3]={{-1,0,1},{-1,0,-1},
                      {0,-1,1},{0,-1,-1},
                      {1,1,1},{1,1,-1},
                      {-1,0,1},{-1,0,-1}};

GLfloat c[NUMVTX][3]={{1,0,0},{1,0,0},
                      {0,1,0},{0,1,0},
                      {0,0,1},{0,0,1},
                      {1,0,0},{1,0,0}};

/******************ZQH****************************/
GLfloat vZ[NUMVTXZ][3]={{-3,1,0},{-2,1,0},
                      {-3,3,0},{-2,2,0},
                      {3,3,0},{1,2,0},
                      {3,2,0},{1,2,0},
                      {1,0,0},{1,2,0},
                      {1,0,0},{0,1,0}, //12
                      {1,0,0},{-1,1,0},
                      {1,0,0},{-0.5,0.5,0},
                      {1.5,-0.5,0},{-0.5,0.5,0},
                      {0.5,-0.5,0},{-0.5,0.5,0}, //20 Dx
                      {-1,-2,0},{-3,-2,0}, //22
                  	  {-1,-2,0},{-3,-3,0},
                  	  {2,-2,0},{3,-3,0},
                  	  {2,-1,0},{3,-1,0},
                  	  //espejo
                  	  {2,-1,-1},{3,-1,-1},
                  	  {2,-2,-1},{3,-3,-1},
                  	  {-1,-2,-1},{-3,-3,-1},
                  	  {-3,-2,-1},{-1,-2,-1}, //36
                  	  {-0.5,0.5,-1},{0.5,-0.5,-1},
                  	  {-0.5,0.5,-1},{1.5,-0.5,-1},
                  	  {-0.5,0.5,-1},{1,0,-1},
                  	  {-1,1,-1},{1,0,-1},
                  	  {0,1,-1},{1,0,-1}, //46
                  	  {1,2,-1},{1,0,-1},
                  	  {1,2,-1},{3,2,-1},
                  	  {1,2,-1},{3,3,-1},
                  	  {-2,2,-1},{-3,3,-1},
                  	  {-2,1,-1},{-3,1,-1}, //56
                  	  //
                  	  {-2,1,0},{-3,1,-1},
                  	  {-3,1,0},{-3,1,-1},
                  	  {-3,3,0},{-3,3,-1},
                  	  {3,3,0},{3,3,-1},
                  	  {3,2,0},{3,2,-1},
                  	  {1,0,0},{1,0,-1}, //68
                  	  {1.5,-0.5,0},{1.5,-0.5,-1},
                  	  {0.5,-0.5,0},{0.5,-0.5,-1},
                  	  {-1,-2,0},{-1,-2,-1},
                  	  {2,-2,0},{2,-2,-1},
                  	  {2,-1,0},{2,-1,-1}, //78
                  	  {3,-1,0},{3,-1,-1},
                  	  {3,-3,0},{3,-3,-1},
                  	  {-3,-3,0},{-3,-3,-1},
                  	  {-3,-2,0},{-3,-2,-1},
                  	  {-0.5,0.5,0},{-0.5,0.5,-1}, //88
                  	  {-1,1,0},{-1,1,-1},
                  	  {0,1,0},{0,1,-1},
                  	  {1,2,0},{1,2,-1},
                  	  {-2,2,0},{-2,2,-1},
                  	  {-2,1,0},{-2,1,-1}, //98
                  	};                 

GLfloat nZ[NUMVTXZ][3]={{-1,-1,1},{1,-1,1}, //-1,-1,1},{1,-1,1
						{-1,1,1},{1,-1,1},
						{1,1,1},{-0.7071,-0,2929},
						{1.7071,0.7071,1},{-0.7071,-0,2929},
						{1.414,0,1},{-0.7071,-0,2929},
						{1.414,0,1},{-0.7071,1.7071,1},
						{1.414,0,1},{-0.7071,0.2929,1},
						{1.414,0,1},{-1.414,0,1},
						{0.7071,-0.2929,1},{-1.414,0,1},
						{0.7071,-1.7071,1},{-1.414,0,1}, //20
						{0.7071,0.2929,1},{-1.7071,0.7071,1},
						{0.7071,0.2929,1},{-1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,1,1},
						//espejo
						{-1,1,1},{1,-1,1}, //30
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //40
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //50
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //60
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //70
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //80
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //90
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1},
						{-1,1,1},{1,-1,1}, //98
						};


GLfloat cZ[NUMVTXZ][3]={{0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //12
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //22
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      //espejo
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //36
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //46
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //56
                      //
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //68
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //78
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //88
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //98
                  }; 


GLfloat vQ[NUMVTXQ][3]={{1.5,-1,0},{3,-2,0},
                      {1.5,1,0},{3,2,0},
                      {1,1.5,0},{2,3,0},
                      {-1,1.5,0},{-2,3,0},
                      {-1.5,1,0},{-3,2,0},
                      {-1.5,-1,0},{-3,-2,0}, //12
                      {-1,-1.5,0},{-2,-3,0},
                      {1,-1.5,0},{2,-3,0}, //14
                      {1,-1,0},{2,-3,0}, //16
                      {1.5,-1,0},{2,-3,0},
                      {3,-2,0},{2,-3,0}, //
                  	  {4,-2.5,0},{2.5,-4,0}};

GLfloat nQ[NUMVTXQ][3]={{-1,1,1},{1.7071,-0.7071,1},
						{-1.7071,-0.7071,1},{1.7071,0.7071,1},
						{-0.7071,-1.7071,1},{0.7071,1.7071,1},
						{0.7071,-1.7071,1},{-0.7071,1.7071,1},
						{1.7071,-0.7071,1},{-1.7071,0.7071,1}, //10
						{1.7071,0.7071,1},{-1.7071,-0.7071,1},
						{0.7071,1.7071,1},{-0.7071,-1.7071,1},
						{-1.,1.,1},{-0.7071,-1.7071,1}, //16
						{-1.,1.,1},{-0.7071,-1.7071,1},
						{1.7071,0.7071,1},{-0.7071,-1.7071,1},
						{1.1543,0.1873,1},{-0.1873,-1.6015,1},

						};

GLfloat cQ[NUMVTXQ][3]={{0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //12
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},//14
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //22
                      {0,1,0},{0,1,0}};

GLfloat vH[NUMVTXH][3]={{-2,0,0},{-1,1,0},
                      {-2,0,0},{-1,3,0},
                      {-2,0,0},{-3,3,0},
                      {-2,0,0},{-3,-3,0},
                      {-2,0,0},{-1,-3,0},
                      {-2,0,0},{-1,-1,0}, //12
                      {-1,1,0},{-1,-1,0},
                      {1,1,0},{1,-1,0}, //14
                      {2,0,0},{1,-1,0},
                      {2,0,0},{1,-3,0},
                      {2,0,0},{3,-3,0}, //22
                  	  {2,0,0},{3,3,0},
                  	  {2,0,0},{1,3,0},
                  	  {2,0,0},{1,1,0},
                  	  {1,-1,0},{1,1,0}};

GLfloat nH[NUMVTXH][3]={{0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1}, //10
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1}, //20
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1},
					   {0,0,1},{0,0,1}, //30
						};

GLfloat cH[NUMVTXH][3]={{0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //12
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},//14
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}, //22
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0},
                      {0,1,0},{0,1,0}};

GLfloat amarillo[3]={1,1,0};

void drawgraphix();
void reshapefunc(int width,int height);
void mousefunc(int button, int state, int x, int y);
void motionfunc(int x, int y);
void keyboardfunc(unsigned char key,int x,int y);
void idlefunc();

void local2world();
void world2clip();

int main(int argc,char **argv){
   winwidth=winheight=1024;

   glutInit(&argc,argv);
   glutInitWindowSize(winwidth,winheight);
   glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   glutCreateWindow("Practica 1 - Ejercicio 1.1");

   local2world();
   world2clip();

   glutDisplayFunc(drawgraphix);
   glutReshapeFunc(reshapefunc);
   glutMouseFunc(mousefunc);
   glutMotionFunc(motionfunc);
   glutKeyboardFunc(keyboardfunc);
   glutIdleFunc(idlefunc);

   glutMainLoop();

   return(0);
}

void drawgraphix(){
	int i;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	///////////////////////////////////////////////////
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 0., 1., 0., 1.0 };
   	GLfloat mat_diffuse[] = { 150.0 };
   	//GLfloat lightpos[] = {1.f, 1.f, 0.f, 0.f};
   	//GLfloat lightpos[] = {-1.f, -1.f, 1.f, 0.f};
   	GLfloat lightpos[] = {0.f, 0.f, 1.f, 0.f};
   	glClearColor (0.0, 0.0, 0.0, 0.0);

   	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
   	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

   	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel (GL_SMOOTH);

	glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
   //////////////////////////////////////////////////////
	
	glPushMatrix();

	glTranslatef(0.0f,0.0f,-0.0f);

	glRotatef(rotx,0.0f,1.0f,0.0f);
	glRotatef(roty,1.0f,0.0f,0.0f);


	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTXZ; i++) {
		glNormal3d(nZ[i][0],nZ[i][1],nZ[i][2]);
		glColor3fv(cZ[i]);
		glVertex3fv(vZ[i]);
	}
	glEnd();
	
	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTXQ; i++) 
	{
		glNormal3d(nQ[i][0],nQ[i][1],nQ[i][2]);
		glColor3fv(cQ[i]);
		glVertex3fv(vQ[i]);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);
	for (i=0; i<NUMVTXH; i++) 
	{
		glNormal3d(nH[i][0],nH[i][1],nH[i][2]);
		glColor3fv(cH[i]);
		glVertex3fv(vH[i]);
	}
	glEnd();

	glPopMatrix();

	glutSwapBuffers();
}

void reshapefunc(int width,int height){
	winwidth=width;
	winheight=height;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0,(float)width/height,1.0,10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f);
}

void mousefunc(int button,int state,int x,int y){
	if (button==GLUT_LEFT_BUTTON) {
		if (state==GLUT_DOWN) {
			flag=1;
		} else {
			flag=0;
		}
	}
}

void motionfunc(int x,int y){
	if (flag>0) {
		if (flag>1) {
			rotx+=360.0f*(x-mx)/winwidth;
			roty+=360.0f*(y-my)/winheight;
		}

		mx=x;
		my=y;

		drawgraphix();

		flag=2;
	}
}

void keyboardfunc(unsigned char key,int x,int y){
	if (key=='q' || key==27) exit(0);
}

void idlefunc(){
	glutPostRedisplay();
}

void local2world(){
	cout<<cZ[1][1]<<endl;
	

	//escalamiento
	float Mscale[3][3]={{0.333,1,0},{0,0.333,0},{0,0,0.333}};
	for(int i=0;i<NUMVTXZ;i++){
		vZ[i][0]= Mscale[0][0]*vZ[i][0];
		vZ[i][1]= Mscale[1][1]*vZ[i][1];
		vZ[i][2]= Mscale[2][2]*vZ[i][2];
	}
	for(int i=0;i<NUMVTXQ;i++){
		vQ[i][0]= Mscale[0][0]*vQ[i][0];
		vQ[i][1]= Mscale[1][1]*vQ[i][1];
		vQ[i][2]= Mscale[2][2]*vQ[i][2];
	}
	for(int i=0;i<NUMVTXH;i++){
		vH[i][0]= Mscale[0][0]*vH[i][0];
		vH[i][1]= Mscale[1][1]*vH[i][1];
		vH[i][2]= Mscale[2][2]*vH[i][2];
	}

	//traslación
	float difxZ=-2.5;//-2.5;
	for(int i=0;i<NUMVTXZ;i++){
		vZ[i][0]=vZ[i][0]+difxZ;
	}

	float difxQ=0;
	for(int i=0;i<NUMVTXQ;i++){
		vQ[i][0]=vQ[i][0]+difxQ;
	}

	float difxH=2.5;
	for(int i=0;i<NUMVTXH;i++){
		vH[i][0]=vH[i][0]+difxH;
	}
}

void world2clip(){

	//busqueda de max para escalar
	float max=0;
	for(int i=0;i<NUMVTXZ;i++){
		for(int j=0;j<3;j++){
			if(vZ[i][j]>max){ max=vZ[i][j]; }
		}
	}
	for(int i=0;i<NUMVTXQ;i++){
		for(int j=0;j<3;j++){
			if(vQ[i][j]>max){ max=vQ[i][j]; }
		}
	}
	for(int i=0;i<NUMVTXH;i++){
		for(int j=0;j<3;j++){
			if(vH[i][j]>max){ max=vH[i][j]; }
		}
	}
	
	//scala
	for(int i=0;i<NUMVTXZ;i++){
		for(int j=0;j<3;j++){
			vZ[i][j]=vZ[i][j]/(max/1.5);
		}
	}
	for(int i=0;i<NUMVTXQ;i++){
		for(int j=0;j<3;j++){
			vQ[i][j]=vQ[i][j]/(max/1.5);
		}
	}
	for(int i=0;i<NUMVTXH;i++){
		for(int j=0;j<3;j++){
			vH[i][j]=vH[i][j]/(max/1.5);
		}
	}	
}



