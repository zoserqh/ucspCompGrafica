#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream> //ifstream
#include <sstream> //stringstream
#include <vector>
#include <string>
#include "Angel-yjc.h"
using namespace std;

typedef Angel::vec3  color3;
typedef Angel::vec3  point3;
/*
extern float **esfera; //esfera
extern float **esfera_colors;
extern int numT;
*/

//esfera
extern int esfera_NumVertices;
extern point3 esfera_points[3072];
extern color3 esfera_colors[3072];

class Floor{
	public:
		int 		m_number_vertices;
		GLuint 		m_floor_buffer;
		point3* 	m_floor_points;
		color3*   	m_floor_colors;
		std::string m_name;	
		int m_solid_flag;
		
		Floor(){}
		
		inline double findnoise2(double x,double y){
			int n=(int)x+(int)y*57;
			n=(n<<13)^n;
			int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
			return 1.0-((double)nn/1073741824.0);
		}

		inline double interpolate(double a,double b,double x){
			double ft=x*3.1415927;
			double f=(1.0-cos(ft))*0.5;
			return a*(1.0-f)+b*f;
		}
		double noise(double x,double y){
			double floorx=(double)((int)x);
			double floory=(double)((int)y);
			double s,t,u,v;

			s = findnoise2(floorx+0,floory+0);
			t = findnoise2(floorx+1,floory+0);
			u = findnoise2(floorx+0,floory+1);
			v = findnoise2(floorx+1,floory+1);

			double int1=interpolate(s,t,x-floorx);
			double int2=interpolate(u,v,x-floorx);			
			return interpolate(int1,int2,y-floory);
		}
		/*
		void ReadFloor(){
			int m=100;
			m_number_vertices = m*m*2*3;

			m_floor_points = new point3[m_number_vertices];
			m_floor_colors = new point3[m_number_vertices];

			for(int i=0;i<m_number_vertices;i++){
				m_floor_colors[i] = color3(0.0,0.0,0.0);
			}			
			int it=0;
			for(float x=0;x<m/10;x+=0.1){
				for(float y=0;y<m/10;y+=0.1){
					it++;
					m_floor_points[it*6  ] = point3(x    ,noise(x,y)     	 ,y    );
					m_floor_points[it*6+1] = point3(x+0.1,noise(x+0.1,y) 	 ,y    );
					m_floor_points[it*6+2] = point3(x    ,noise(x,y+0.1) 	 ,y+0.1);

					m_floor_points[it*6+3] = point3(x+0.1,noise(x+0.1,y) 	 ,y    );
					m_floor_points[it*6+4] = point3(x    ,noise(x,y+0.1) 	 ,y+0.1);
					m_floor_points[it*6+5] = point3(x+0.1,noise(x+0.1,y+0.1) ,y+0.1);
				}
			}
		}
		*/

		void ReadFloor(){
			int m0=100, m1=50, m2=25;
			m_number_vertices = 2*(m0*m0+m1*m1*8+m2*m2*16)*2*3; // numero de cuadrados*2*3

			m_floor_points = new point3[m_number_vertices];
			m_floor_colors = new point3[m_number_vertices];

			for(int i=0;i<m_number_vertices;i++){
				m_floor_colors[i] = color3(0.0,0.0,0.0);
			}

			int it=0;
			float paso=0.4;
			for(float x=0;x<500/10;x+=paso){
				for(float y=0;y<500/10;y+=paso){
					it++;
					if(0<=x && x<=10){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(10<=x && x<=20 && 0<=y && y<=10){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(20<=x && x<=30 && 0<=y && y<=10){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(30<=x && x<=40 && 0<=y && y<=10){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(40<=x && x<=50){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(30<=x && x<=40 && 40<=y && y<=50){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(20<=x && x<=30 && 40<=y && y<=50){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
					else if(10<=x && x<=20 && 40<=y && y<=50){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(0.0,1.0,0.0);
					}
				}
			}

			paso=0.2;
			for(float x=10;x<400/10;x+=paso){
				for(float y=10;y<400/10;y+=paso){
					it++;
					if(10<=x && x<=20 && 10<=y && y<=40){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(1.0,0.0,0.0);
					}
					else if(20<=x && x<=30 && 10<=y && y<=20){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(1.0,0.0,0.0);
					}
					else if(30<=x && x<=40 && 10<=y && y<=40){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(1.0,0.0,0.0);
					}
					else if(20<=x && x<=30 && 30<=y && y<=40){
						readFloorInside(it, x, y, paso);
						//m_floor_colors[it] = color3(1.0,0.0,0.0);
					}
				}
			}

			paso=0.1;
			for(float x=20;x<300/10;x+=paso){
				for(float y=20;y<300/10;y+=paso){
					it++;
					//m_floor_colors[it] = color3(0.0,1.0,1.0);
					readFloorInside(it, x, y, paso);
				}
			}

			for(int i=0;i<m_number_vertices;i++){
				m_floor_points[i][0] = m_floor_points[i][0]-25.0;
				m_floor_points[i][2] = m_floor_points[i][2]-25.0;
			}
		}

		void readFloorInside(int it, float x, float y, float paso){
			m_floor_points[it*6  ] = point3(x    ,noise(x,y)    	 ,y    );
			m_floor_points[it*6+1] = point3(x+paso,noise(x+paso,y) 	 ,y    );
			m_floor_points[it*6+2] = point3(x    ,noise(x,y+paso) 	 ,y+paso);

			m_floor_points[it*6+3] = point3(x+paso,noise(x+paso,y) 	 ,y    );
			m_floor_points[it*6+4] = point3(x    ,noise(x,y+paso) 	 ,y+paso);
			m_floor_points[it*6+5] = point3(x+paso,noise(x+paso,y+paso) ,y+paso);
		}

		void InitFloor(){										
			glGenBuffers(1, &m_floor_buffer);		 	
			glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices + sizeof(color3)*m_number_vertices, NULL, GL_STATIC_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point3)*m_number_vertices, m_floor_points);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(point3)*m_number_vertices, sizeof(color3)*m_number_vertices, m_floor_colors);	
		}

		void DrawFloor(GLuint& program,GLuint& model_view,mat4& mv) {

			mv = mv*Translate(0.0,0.0,-8.0) * Scale(1.0,1.0,1.0);
			glUniformMatrix4fv(model_view,1,GL_TRUE,mv);  
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			//glBindBuffer(GL_ARRAY_BUFFER, m_floor_buffer);

			GLuint vPosition = glGetAttribLocation(program, "vPosition");
			glEnableVertexAttribArray(vPosition);
			glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
					  
			GLuint vColor = glGetAttribLocation(program, "vColor");
			glEnableVertexAttribArray(vColor);
			glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(point3) * m_number_vertices) );

			glDrawArrays(GL_TRIANGLES, 0, m_number_vertices); 	

			glDisableVertexAttribArray(vPosition);
			glDisableVertexAttribArray(vColor);

		}


};