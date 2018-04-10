#include <iostream>
#include "Angel-yjc.h"
using namespace std;

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;


//vectores unitarios wordl espace
extern vec3 X;
extern vec3 Y;
extern vec3 Z;

extern GLuint program;       // shader program object id 
extern GLuint cube_buffer;   // vertex buffer object id for cube
extern GLuint esfera_buffer;
extern GLuint piso_buffer;
extern GLuint ejes_buffer;
extern int esfera_NumVertices;
extern int piso_NumVertices;
extern int ejes_NumVertices;
extern const int cube_NumVertices; // (6 faces)*(2 triangles/face)*(3 vertices/triangle)


extern GLfloat  fovy;  // Field-of-view in Y direction angle (in degrees)
extern GLfloat  aspect;       // Viewport aspect ratio
extern GLfloat  zNear;
extern GLfloat  zFar;

//incrementos en idle
extern GLfloat angle; // rotation angle in degrees
extern GLfloat giro;
extern GLfloat recorrido;
extern vec3 deplazamiento;
extern vec3 vecGiro;
extern vec3 A;
extern vec3 B;
extern vec3 C;

extern vec4 init_eye;
extern vec4 eye;               // current viewer position

extern int animationFlag; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'
extern int cubeFlag;   // 1: solid cube; 0: wireframe cube. Toggled by key 'c' or 'C'
extern int sphereFlag;
extern int beginRollFlag;
extern int sphereRollFlag;

extern point3 esfera_points[3072];
extern color3 esfera_colors[3072];
extern point3 piso_points[6];
extern color3 piso_colors[6];
extern point3 ejes_points[6];
extern color3 ejes_colors[6];
extern point3 cube_points[100];
extern color3 cube_colors[100];
extern point3 vertices[8];
extern color3 vertex_colors[8];
extern int Index;

int primGL_triangles=0;
int primGL_lines=1;

void quad( int a, int b, int c, int d);
void colorcube();
void floor();
void display(void);

void init();
void drawObj(GLuint buffer, int num_vertices, int primGL);
void idle (void);
void keyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
//menu
void menu();
void choseShading(int id);
void wireFrameSphere(int id);
void EnableLighting(int id);
void demo_menu(int id);
//
void reshape(int width, int height);



// quad(): generate two triangles for each face and assign colors to the vertices
void quad( int a, int b, int c, int d ){
    cube_colors[Index] = vertex_colors[a]; cube_points[Index] = vertices[a]; Index++;
    cube_colors[Index] = vertex_colors[b]; cube_points[Index] = vertices[b]; Index++;
    cube_colors[Index] = vertex_colors[c]; cube_points[Index] = vertices[c]; Index++;

    cube_colors[Index] = vertex_colors[c]; cube_points[Index] = vertices[c]; Index++;
    cube_colors[Index] = vertex_colors[d]; cube_points[Index] = vertices[d]; Index++;
    cube_colors[Index] = vertex_colors[a]; cube_points[Index] = vertices[a]; Index++;
}

// generate 12 triangles: 36 vertices and 36 colors
void colorcube(){
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

// OpenGL initialization
void init(){
  colorcube();


 // Create and initialize a vertex buffer object for cube, to be used in display()//////////////
    glGenBuffers(1, &cube_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, cube_buffer);


    glBufferData(GL_ARRAY_BUFFER, 
                 sizeof(point3)*cube_NumVertices + sizeof(color3)*cube_NumVertices,
     NULL, GL_STATIC_DRAW);
    cout<<"sizeof(float): "<<sizeof(float)<<" sizeof(point3): "<<sizeof(point3)<<" cube_NumVertices: "<<cube_NumVertices<<endl;
    glBufferSubData(GL_ARRAY_BUFFER, 0, 
                    sizeof(point3) * cube_NumVertices, cube_points);
    glBufferSubData(GL_ARRAY_BUFFER, 
                    sizeof(point3) * cube_NumVertices, 
                    sizeof(color3) * cube_NumVertices,
                    cube_colors);
    
    //create and initialize a vertex buffer object for esfera, to be used in display()//////////
    glGenBuffers(1, &esfera_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, esfera_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*esfera_NumVertices + sizeof(color3)*esfera_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*esfera_NumVertices, esfera_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*esfera_NumVertices, sizeof(color3)*esfera_NumVertices,
                    esfera_colors);

    //create and initialize a vertex buffer object for piso, to be used in display()//////////
    glGenBuffers(1, &piso_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, piso_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*piso_NumVertices + sizeof(color3)*piso_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*piso_NumVertices, piso_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*piso_NumVertices, sizeof(color3)*piso_NumVertices,
                    piso_colors);


    //create and initialize a vertex buffer object for ejes, to be used in display()//////////
    glGenBuffers(1, &ejes_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, ejes_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*ejes_NumVertices + sizeof(color3)*ejes_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*ejes_NumVertices, ejes_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*ejes_NumVertices, sizeof(color3)*ejes_NumVertices,
                    ejes_colors);
    
  printf("XD1\n");
 // Load shaders and create a shader program (to be used in display())
    program = InitShader("Shaders/vshader42.glsl", "Shaders/fshader42.glsl");
    
    printf("XD2\n");
    glEnable( GL_DEPTH_TEST );
    //glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glClearColor( 0.529, 0.807, 0.92, 1.0 ); 
    glLineWidth(1.0);
}

// drawObj(buffer, num_vertices):
//   draw the object that is associated with the vertex buffer object "buffer"
//   and has "num_vertices" vertices.
//
void drawObj(GLuint buffer, int num_vertices, int primGL){
    //--- Activate the vertex buffer object to be drawn ---//
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    /*----- Set up vertex attribute arrays for each vertex attribute -----*/
    GLuint vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(0) );

    GLuint vColor = glGetAttribLocation(program, "vColor"); 
    glEnableVertexAttribArray(vColor);
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
        BUFFER_OFFSET(sizeof(point3) * num_vertices) ); 
      // the offset is the (total) size of the previous vertex attribute array(s)

    /* Draw a sequence of geometric objs (triangles) from the vertex buffer
       (using the attributes specified in each enabled vertex attribute array) */
    if(primGL==primGL_triangles){
      glDrawArrays(GL_TRIANGLES, 0, num_vertices);
    }

    else if(primGL==primGL_lines){
      glDrawArrays(GL_LINES, 0, num_vertices); 
    }
    /*--- Disable each vertex attribute array being enabled ---*/
    glDisableVertexAttribArray(vPosition);
    glDisableVertexAttribArray(vColor);
}

void display( void ){
  GLuint  model_view;  // model-view matrix uniform shader variable location
  GLuint  projection;  // projection matrix uniform shader variable location

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glUseProgram(program); // Use the shader program

    model_view = glGetUniformLocation(program, "model_view" );
    projection = glGetUniformLocation(program, "projection" );

    /*---  Set up and pass on Projection matrix to the shader ---*/
    mat4  p = Perspective(fovy, aspect, zNear, zFar);

    glUniformMatrix4fv(projection, 1, GL_TRUE, p); // GL_TRUE: matrix is row-major

    /*---  Set up and pass on Model-View matrix to the shader ---*/
    // eye is a global variable of vec4 set to init_eye and updated by keyboard()
    /*vec4    at(0.0, 0.0, 0.0, 1.0);
    vec4    up(0.0, 1.0, 0.0, 0.0);*/
    vec4    at(0.0, 0.0, 0.0, 1.0);
    vec4    up(0.0, 1.0, 0.0, 0.0);

    mat4  mv = LookAt(eye, at, up);

    //cout<<"mv: "<<mv<<endl;
    /*----- Set Up the Model-View matrix for the cube -----*/
	    // The following is to verify that Rotate() about (0,2,0) is RotateY():
      // Commenting out Rotate() and un-commenting RotateY()
      // gives the same result.
  		//
  	  // The set-up below gives a new scene (scene 2), using Correct LookAt().
    //////
    /*
      mv = mv * Translate(0.0, 0.0, 0.0) * Scale (1.0, 1.0, 1.0) 
              * Rotate(angle, 0.0, 2.0, 0.0);
    */
    ////
	   // * RotateY(angle);
	  //
	  // The set-up below gives the original scene (scene 1), using Correct LookAt().
	  //  mv = Translate(0.0, 0.5, 0.0) * mv * Scale (1.4, 1.4, 1.4) 
	  //               * Rotate(angle, 0.0, 2.0, 0.0);
		        // * RotateY(angle); 
	//
	// The set-up below gives the original scene (scene 1), when using previously 
	//     Incorrect LookAt() (= Translate(1.0, 1.0, 0.0) * correct LookAt() )
	//  mv = Translate(-1.0, -0.5, 0.0) * mv * Scale (1.4, 1.4, 1.4) 
	//               * Rotate(angle, 0.0, 2.0, 0.0);
	    // * RotateY(angle);
	/////
  /*
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if (cubeFlag == 1) // Filled cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else              // Wireframe cube
       glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    drawObj(cube_buffer, cube_NumVertices,primGL_triangles);  // draw the cube
  */
  ////
    /*----- Set Up the Model-View matrix for the sphere -----*/
    // The following is to verify that Rotate() about (0,2,0) is RotateY():
      // Commenting out Rotate() and un-commenting RotateY()
      // gives the same result.
      //
      // The set-up below gives a new scene (scene 2), using Correct LookAt().
      
      mv = LookAt(eye, at, up) * Translate(deplazamiento[0],deplazamiento[1],deplazamiento[2]) /** Scale (1.0, 1.0, 1.0)*/
              * Rotate(giro,vecGiro[0],vecGiro[1],vecGiro[2]);
     // * RotateY(angle);
    //
    // The set-up below gives the original scene (scene 1), using Correct LookAt().
    //  mv = Translate(0.0, 0.5, 0.0) * mv * Scale (1.4, 1.4, 1.4) 
    //               * Rotate(angle, 0.0, 2.0, 0.0);
            // * RotateY(angle); 
  //
  // The set-up below gives the original scene (scene 1), when using previously 
  //     Incorrect LookAt() (= Translate(1.0, 1.0, 0.0) * correct LookAt() )
  //  mv = Translate(-1.0, -0.5, 0.0) * mv * Scale (1.4, 1.4, 1.4) 
  //               * Rotate(angle, 0.0, 2.0, 0.0);
      // * RotateY(angle);
  //
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if(sphereFlag==1){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    drawObj(esfera_buffer, esfera_NumVertices,primGL_triangles);  // draw the sphere


    /*----- Set Up the Model-View matrix for the piso -----*/
    mv = LookAt(eye, at, up);
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj(piso_buffer, piso_NumVertices,primGL_triangles);  // draw the piso


    /*----- Set Up the Model-View matrix for the ejes -----*/
    mv = LookAt(eye, at, up);
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj(ejes_buffer, ejes_NumVertices,primGL_lines);  // draw the ejes

    glutSwapBuffers();
}

void idle (void){

  //angle += 0.02;
  
  static float recTAB=sqrt(dot(B-A,B-A));
  static float recTBC=sqrt(dot(C-B,C-B));
  static float recTCA=sqrt(dot(A-C,A-C));
  static int tramo=0;
  
  if(sphereRollFlag==1 && beginRollFlag==1){
    angle += 0.01;    //YJC: change this value to adjust the cube rotation speed.
    giro += 0.1;
    recorrido += 0.1*0.0174532925; // r*dtheta*(pi/180)=recorrido
    if(recorrido<recTAB && tramo==0){
      static vec3 direcdesplaz=normalize(B-A);
      deplazamiento=A+direcdesplaz*recorrido;
      vecGiro=cross(Y,direcdesplaz);
      //cout<<"recorrido: "<<recorrido<<" recTAB: "<<recTAB<<endl;
    }
    static int key0=1;
    if(recorrido>recTAB && key0==1 && tramo==0) {recorrido=0; tramo=1; key0=0;}

    if(recorrido<recTBC && tramo==1){
      static vec3 direcdesplaz=normalize(C-B);
      deplazamiento=B+direcdesplaz*recorrido;
      vecGiro=cross(Y,direcdesplaz);
      //cout<<"recorrido: "<<recorrido<<" recTBC: "<<recTBC<<endl;
    }
    static int key1=1;
    if(recorrido>recTBC && key1==1 && tramo==1) {recorrido=0; tramo=2; key1=0;}  
    
    if(recorrido<recTCA && tramo==2){
      static vec3 direcdesplaz=normalize(A-C);
      deplazamiento=C+direcdesplaz*recorrido;
      vecGiro=cross(Y,direcdesplaz);
      //cout<<"recorrido: "<<recorrido<<" recTCA: "<<recTCA<<endl;
    }
    static int key2=1;
    if(recorrido>recTCA && key2==1 && tramo==2) {recorrido=0; tramo=0; key2=0;}  
    if(key0==0 && key1==0 && key2==0) {key0=1; key1=1; key2=1;}
  //cout<<"vecGiro: "<<vecGiro<<endl;
  }
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
  switch(key) {
    case 033: // Escape Key
    case 'q': case 'Q':
      exit( EXIT_SUCCESS );
      break;

    case 'X': eye[0] += 1.0; break;
	  case 'x': eye[0] -= 1.0; break;
    
    case 'Y': eye[1] += 1.0; break;
	  case 'y': eye[1] -= 1.0; break;
        
    case 'Z': eye[2] += 1.0; break;
	  case 'z': eye[2] -= 1.0; break;

    case 'a': case 'A': // Toggle between animation and non-animation
      animationFlag = 1 -  animationFlag;
      if (animationFlag == 1) glutIdleFunc(idle);
      else  glutIdleFunc(NULL);
      break;
	   
    case 'c': case 'C': // Toggle between filled and wireframe cube
      cubeFlag = 1 -  cubeFlag;   
      break;

    case ' ':  // reset to initial viewer/eye position
	    eye = init_eye;
	    break;
    case 'b': case 'B': //begin sphere rolling
      beginRollFlag=1;
      break;
  }	
  glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
    sphereRollFlag=1- sphereRollFlag;
  }
}

void menu(){
  int sub_menu1=glutCreateMenu(wireFrameSphere);
  glutAddMenuEntry("No",1);
  glutAddMenuEntry("Yes",2);

  int sub_menu2=glutCreateMenu(EnableLighting);
  glutAddMenuEntry("No",1);
  glutAddMenuEntry("Yes",2);

  int sub_menu3=glutCreateMenu(choseShading);
  glutAddMenuEntry("flat shading",1);
  glutAddMenuEntry("smooth shading",2);  

  glutCreateMenu(demo_menu);
  glutAddMenuEntry("Default View Point",1);
  glutAddSubMenu("Enable Lighting",sub_menu2);
  glutAddSubMenu("Shading",sub_menu3);
  glutAddMenuEntry("Light Source",4);
  glutAddSubMenu("Wire Frame Sphere",sub_menu1);
  glutAddMenuEntry("Quit",6);
  glutAttachMenu(GLUT_LEFT_BUTTON);


}
void choseShading(int id){
  switch(id){
    case 1:
      exit(0);
      break;
    case 2:
      exit(0);
      break;
  }
  glutPostRedisplay();
}

void EnableLighting(int id){
  switch(id){
    case 1:
      glDisable(GL_LIGHTING);
      break;
    case 2:
      glEnable(GL_LIGHTING);
      break;
  }
  glutPostRedisplay();
}

void wireFrameSphere(int id){
  switch(id){
    case 1:
      sphereFlag=0;
      break;
    case 2:
      sphereFlag=1;
  }
  glutPostRedisplay();
}

void demo_menu(int id){
  switch(id){
    case 1:
      eye = init_eye;
      break;
    case 2:
      exit(0);
      break;
    case 3:
      exit(0);
      break;
    case 4:
      exit(0);
      break;
    case 5:
      exit(0);
      break;
    case 6:
      exit( EXIT_SUCCESS );
      break;
  }
  glutPostRedisplay();
}

void reshape(int width, int height){
    glViewport(0, 0, width, height);
    aspect = (GLfloat) width  / (GLfloat) height;
    glutPostRedisplay();
}