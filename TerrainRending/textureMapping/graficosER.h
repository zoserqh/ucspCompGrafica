#include <iostream>
#include "Angel-yjc.h"
#include "texture.h"
using namespace std;

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;

//textura
extern GLuint imagetex;
extern GLuint bumptex;
extern GLuint* image;
extern GLuint* bump;
extern int w, h, components;
extern int wi, hi, componentsi;

//camara
extern vec4 init_eye;
extern vec4 init_at;
extern vec4 eye;  // current viewer position
extern vec4 at;
extern vec4 up;
extern vec4 binormEye;
extern float da;
extern float dw;
extern float gh;
extern float gv;
extern int flagwasd;
extern int flaggh;
extern int flagah; //flag antihorario
extern int flagh; // flag horario
extern int flagmA; // flag mirar arriba
extern int flagma; // flag mirar abajo
extern int flaggv; // flag de giro plano vertical

//vectores unitarios wordl espace
extern vec3 X;
extern vec3 Y;
extern vec3 Z;

extern GLuint program;       // shader program object id 
//extern GLuint esfera_buffer;
extern GLuint ejes_buffer;
//extern int esfera_NumVertices;
extern int ejes_NumVertices;


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



extern int animationFlag; // 1: animation; 0: non-animation. Toggled by key 'a' or 'A'
//extern int sphereFlag;
//extern int beginRollFlag;
//extern int sphereRollFlag;

//extern point3 esfera_points[3072];
//extern color3 esfera_colors[3072];
extern point3 ejes_points[6];
extern color3 ejes_colors[6];
extern color3 vertex_colors[8];
extern int Index;

//grilla
extern Floor n_floor;

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
void texture();


// OpenGL initialization
void init(){
    /*
    //create and initialize a vertex buffer object for esfera, to be used in display()//////////
    glGenBuffers(1, &esfera_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, esfera_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*esfera_NumVertices + sizeof(color3)*esfera_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*esfera_NumVertices, esfera_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*esfera_NumVertices, sizeof(color3)*esfera_NumVertices,
                    esfera_colors);
    */

    //create and initialize a vertex buffer object for floor, to be used in display()//////////
    glGenBuffers(1, &n_floor.m_floor_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, n_floor.m_floor_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*n_floor.m_number_vertices + sizeof(color3)*n_floor.m_number_vertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*n_floor.m_number_vertices, n_floor.m_floor_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*n_floor.m_number_vertices, sizeof(color3)*n_floor.m_number_vertices,
                    n_floor.m_floor_colors);


    //create and initialize a vertex buffer object for ejes, to be used in display()//////////
    glGenBuffers(1, &ejes_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, ejes_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*ejes_NumVertices + sizeof(color3)*ejes_NumVertices,
     NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*ejes_NumVertices, ejes_points);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*ejes_NumVertices, sizeof(color3)*ejes_NumVertices,
                    ejes_colors);
    
    //printf("XD1\n");
 // Load shaders and create a shader program (to be used in display())
    program = InitShader("Shaders/vshader42.glsl", "Shaders/fshader42.glsl");
    
    //printf("XD2\n");
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
    mat4  mv = LookAt(eye, at, up);

    /*
    //----- Set Up the Model-View matrix for the sphere -----//      
      mv = LookAt(eye, at, up) * Translate(deplazamiento[0],deplazamiento[1],deplazamiento[2])
              * Rotate(giro,vecGiro[0],vecGiro[1],vecGiro[2]);

    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    if(sphereFlag==1){ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
    else{ glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); }
    drawObj(esfera_buffer, esfera_NumVertices,primGL_triangles);  // draw the sphere
    */

    /*----- Set Up the Model-View matrix for the floor -----*/
    mv = LookAt(eye, at, up);
    glUniformMatrix4fv(model_view, 1, GL_TRUE, mv); // GL_TRUE: matrix is row-major
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    drawObj(n_floor.m_floor_buffer, n_floor.m_number_vertices, primGL_triangles);  // draw the floor


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
  
  float r=2; // radio de giro
  
  //camara traslacion y giro
  if(flagwasd==1){
    vec4 binormEye=vec4(normalize(cross((at-eye),up))) - vec4(0.0,0.0,0.0,1.0) ;
    vec4 vecNormws=vec4(normalize(at-eye));
    
    eye=(-1)*binormEye*da + vecNormws*dw + init_eye; //cout<<"eye: "<<eye<<endl;
    at= eye + vecNormws*r;
    flagwasd=0;

    cout<<"///////////////"<<endl;
    cout<<"binormEye: "<<binormEye<<endl;
    cout<<"vecNormws: "<<vecNormws<<endl;
    cout<<"up: "<<up<<endl;
    cout<<"eye "<<eye<<endl;
    cout<<"at "<<at<<endl;
  }

  if(flaggh==1){
    if(flagah==1){
      binormEye=vec4(normalize(cross((at-eye),up))) - vec4(0.0,0.0,0.0,1.0) ;
      vec4 vecNormws=vec4(normalize(at-eye));
      
      at=eye - binormEye*r*sin(gh) + vecNormws*r*cos(gh);
      binormEye=vec4(normalize(cross((at-eye),up))) - vec4(0.0,0.0,0.0,1.0) ;

      cout<<"gh: "<<gh<<endl;
      cout<<"gh*3.1415/180.0: "<<gh*3.1415/180.0<<endl;
      cout<<sin(gh)<<" / "<<cos(gh)<<endl;
      if(3.1415<gh){ gh=-3.1415; }
      if(gh<-3.1415){ gh=3.1415; }
      flaggh=0;
      flagah=0;
      }
    if(flagh==1){
      binormEye=vec4(normalize(cross((at-eye),up))) - vec4(0.0,0.0,0.0,1.0) ;
      vec4 vecNormws=vec4(normalize(at-eye));
      
      at=eye + binormEye*r*sin(gh) + vecNormws*r*cos(gh);
      binormEye=vec4(normalize(cross((at-eye),up))) - vec4(0.0,0.0,0.0,1.0) ;
      
      cout<<"gh: "<<gh<<endl;
      cout<<"gh*3.1415/180.0: "<<gh*3.1415/180.0<<endl;
      cout<<sin(gh)<<" / "<<cos(gh)<<endl;
      if(3.1415<gh){ gh=-3.1415; }
      if(gh<-3.1415){ gh=3.1415; }
      flaggh=0;
      flagh=0;
      }
  }

  if(flaggv==1){
    if(flagmA==1){
      vec4 vecNormws=vec4(normalize(at-eye));
      at=eye + vecNormws*r*cos(gv) + up*r*sin(gv);
      
      vecNormws=vec4(normalize(at-eye));
      up=normalize(cross(binormEye,vecNormws));
      flaggv=0;
      flagmA=0;
    }
    if(flagma==1){
      vec4 vecNormws=vec4(normalize(at-eye));
      at=eye + vecNormws*r*cos(gv) - up*r*sin(gv);

      vecNormws=vec4(normalize(at-eye));
      up=cross(binormEye,vecNormws);
      flaggv=0;
      flagma=0;
    }
  }
  
  
  /*if(sphereRollFlag==1 && beginRollFlag==1){
    angle += 0.01;    //YJC: change this value to adjust the cube rotation speed.
    giro += 1.5;
    recorrido += 1.5*0.0174532925; // r*dtheta*(pi/180)=recorrido
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
  }*/
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){
  switch(key) {
    case 033: // Escape Key
    case 'q': case 'Q':
      exit( EXIT_SUCCESS );
      break;

    case 'a': flagwasd=1; da=da+0.1;  break;
	  case 'd': flagwasd=1; da=da-0.1;  break;
    case 'w': flagwasd=1; dw=dw+0.1; break;
	  case 's': flagwasd=1; dw=dw-0.1; break;
        
    case '8': flaggv=1; flagmA=1; break;
	  case '2': flaggv=1; flagma=1; break;
    case '4': flaggh=1; flagah=1; break;
    case '6': flaggh=1; flagh=1; break;
    /*
    case 'a': case 'A': // Toggle between animation and non-animation
      animationFlag = 1 -  animationFlag;
      if (animationFlag == 1) glutIdleFunc(idle);
      else  glutIdleFunc(NULL);
      break;
	   */
    case ' ':  // reset to initial viewer/eye position
	    eye = init_eye;
	    break;
  }	
  glutPostRedisplay();
}

void myMouse(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN){
    //sphereRollFlag=1- sphereRollFlag;
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

void texture(){
  glGenTextures(1, &imagetex); // Generate texture obj name(s)
  //glActiveTexture( GL_TEXTURE0 ); // Set the active texture unit to be 0
  glBindTexture(GL_TEXTURE_2D, imagetex); // Bind the texture to this texture unit
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
  glTexImage2D(GL_TEXTURE_2D, 0, componentsi, wi, hi,
    0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glGenerateMipmap(GL_TEXTURE_2D);

  image = read_texture("texturas/fish_color_map.rgb", &wi, &hi, &componentsi);
  bump = read_texture("texturas/fish_bump_map2.rgb", &w, &h, &components);
}