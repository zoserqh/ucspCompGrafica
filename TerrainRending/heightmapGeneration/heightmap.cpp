/************************************************************
 * Handout: rotate-cube-new.cpp (A Sample Code for Shader-Based OpenGL ---
                                 for OpenGL version 3.1 and later)
 * Originally from Ed Angel's textbook "Interactive Computer Graphics" 6th Ed
              sample code "example3.cpp" of Chapter 4.
 * Moodified by Yi-Jen Chiang to include the use of a general rotation function
   Rotate(angle, x, y, z), where the vector (x, y, z) can have length != 1.0,
   and also to include the use of the function NormalMatrix(mv) to return the
   normal matrix (mat3) of a given model-view matrix mv (mat4).

   (The functions Rotate() and NormalMatrix() are added to the file "mat-yjc-new.h"
   by Yi-Jen Chiang, where a new and correct transpose function "transpose1()" and
   other related functions such as inverse(m) for the inverse of 3x3 matrix m are
   also added; see the file "mat-yjc-new.h".)

 * Extensively modified by Yi-Jen Chiang for the program structure and user
   interactions. See the function keyboard() for the keyboard actions.
   Also extensively re-structured by Yi-Jen Chiang to create and use the new
   function drawObj() so that it is easier to draw multiple objects. Now a floor
   and a rotating cube are drawn.

** Perspective view of a color cube using LookAt() and Perspective()

** Colors are assigned to each vertex and then the rasterizer interpolates
   those colors across the triangles.
**************************************************************/
//g++ -std=c++0x heightmap.cpp InitShader.cpp -o heightmap -lGL -lglut -lGLU -lGLEW
//./heightmap
#include <iostream>
#include "Angel-yjc.h"
#include "funcionesER.h"
#include "graficosER.h"
using namespace std;

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;

GLuint Angel::InitShader(const char* vShaderFile, const char* fShaderFile);
/*
//esfera
float **esfera;
float **esfera_colors;
int numT; //numero de triangulos
*/

//camara
vec4 init_eye(0.0, 2.0, 0.0, 1.0); // initial viewer position
vec4 init_at(0.0, 2.0, 2.0, 1.0);
vec4 init_up(0.0, 1.0, 0.0, 0.0);
vec4 eye = init_eye;               // current viewer position
vec4 at=init_at;
vec4 up=init_up;
vec4 binormEye=vec4(normalize(cross((at-eye),up))) - vec4(0.0,0.0,0.0,1.0) ;
int flagwasd=0;
int flaggh=0; // flag de giro plano horizontal
int flagah=0; //flag antihorario
int flagh=0; // flag horario
int flaggv=0; // flag de giro plano vertical
int flagmA=0; // flag mirar arriba
int flagma=0; // flag mirar abajo


//vec4 up(0.0, 0.707106781, 0.707106781, 0.0);

float da=0; // desplazamiento en x a-d
float dw=0; // desplazamiento en z w-s
float gh=0.1; // giro horizontal Z
float gv=0.1; // giro vertical -X
//vectores unitarios wordl espace
vec3 X(1.0,0.0,0.0);
vec3 Y(0.0,1.0,0.0);
vec3 Z(0.0,0.0,1.0);

//esfera
/*
int esfera_NumVertices;
point3 esfera_points[3072];
color3 esfera_colors[3072];
*/

//grilla
Floor n_floor;

//ejes
int ejes_NumVertices=6;
point3 ejes_points[6]={point3(0.0,0.0,0.0),
                       point3(10.0,0.0,0.0),
                       point3(0.0,0.0,0.0),
                       point3(0.0,10.0,0.0),
                       point3(0.0,0.0,0.0),
                       point3(0.0,0.0,10.0)
                      };
color3 ejes_colors[6]={color3(1.0,0.0,0.0),
                       color3(1.0,0.0,0.0),
                       color3(1.0,0.0,1.0),
                       color3(1.0,0.0,1.0),
                       color3(0.0,0.0,1.0),
                       color3(0.0,0.0,1.0)
                      };

//GLuint esfera_buffer;
GLuint ejes_buffer;
GLuint program;       /* shader program object id */
/*
// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 0.5, zFar = 3.0;
*/
// Projection transformation parameters
GLfloat  fovy = 45.0;  // Field-of-view in Y direction angle (in degrees)
GLfloat  aspect;       // Viewport aspect ratio
GLfloat  zNear = 1, zFar = 18.0;

//incrementos en idle
GLfloat angle = 0.0; // rotation angle in degrees
GLfloat giro = 0.0;
GLfloat recorrido = 0.0;
vec3 vecGiro(-1.0,0.0,0.0);
vec3 A(3.0,1.0,5.0);
vec3 B(-1.0,1.0,-4);
vec3 C(3.5,1.0,-2.5);
vec3 deplazamiento=A;


int animationFlag=1; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'
int cubeFlag=1;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
//int sphereFlag=1;
int beginRollFlag=0;
//int sphereRollFlag=1;

//RGBA color esfera en funcionesER

int Index = 0; // YJC: This must be a global variable since quad() is called
               //      multiple times and Index should then go up to 36 for
               //      the 36 vertices and colors

int main(int argc, char *argv[]){ 
  int err;
  n_floor.ReadFloor();
  cout<<"eye: "<<eye<<endl;
  
  cout<<"//////////"<<endl;
  cout<<"eye: "<<eye<<endl;
  cout<<"at: "<<at<<endl;
  cout<<"up: "<<up<<endl;
  //readObject(argc,argv);

  //cout<<"sizeof(esfera): "<<sizeof(esfera)<<endl;
  //cout<<"sizeof(&esfera): "<<sizeof(&esfera)<<endl;
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(600, 600);
  // glutInitContextVersion(3, 2);
  // glutInitContextProfile(GLUT_CORE_PROFILE);
  glutCreateWindow("SphereRoll");
  menu();
  /* Call glewInit() and error checking */
  err = glewInit();
  if (GLEW_OK != err){ 
    printf("Error: glewInit failed: %s\n", (char*) glewGetErrorString(err)); 
    exit(1);
  }
  
  
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
	glutMouseFunc(myMouse);
  printf("xD\n");
  init();
  glutMainLoop();
  return 0;
}


//&argc, argv
