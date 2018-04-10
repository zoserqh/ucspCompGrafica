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

void readObject(int argc, char* argv[]);
	

void readObject(int argc, char* argv[]){
	char OBJETO[40];
	//esfera
	float **esfera;
	int numT;

	if (argc!=3) {
		printf("Indique el nombre del archivo que almacena el Objeto a leer - Ejemplo: [user@equipo]$ %s -readObj cube.8\n",argv[0]);
		exit(1);
	}
	strcpy(OBJETO,argv[2]);

	string linea;
	ifstream archivo_entrada;
	archivo_entrada.open (OBJETO);
	if (archivo_entrada.fail()) { cout<<"el archivo no se abrio correctamente (1era lectura)"<<endl; }
	
	getline(archivo_entrada, linea);
	//cout<<linea<<endl;
	stringstream ss;
	ss<<linea;
	ss>>numT;
	//cout<<numT<<endl;

	////////////////////reserva de memoria para esfera//////////////////////
	esfera=(float**) malloc(numT*3*sizeof(float *)); 
	for(int i=0; i<numT*3; i++)
		esfera[i]=(float*) malloc(3*sizeof(float));
	////////////////////////////////////////////////////////////////////////

	for(int i=0;i<numT;i++){
		getline(archivo_entrada, linea);
		unsigned numV;
		stringstream ss;
		ss<<linea;
		ss>>numV;
		//cout<<numV<<endl;
		for(int j=0;j<numV;j++){
			getline(archivo_entrada, linea);
			//ss<<linea;
			//cout<<"linea: "<<linea<<endl;
			//cout<<"linea: "<<linea[0]<<linea[1]<<linea[2]<<endl;
			int espacios=2;
			vector<char> s1, s2, s3;
			int k=0;
			while(espacios>=0){
				if(espacios==2 && linea[k]!=' '){ s1.push_back(linea[k]); }
				else if(espacios==1 && linea[k]!=' '){ s2.push_back(linea[k]); }
				else if(espacios==0 && linea[k]!='!'){ s3.push_back(linea[k]); }
				k++;
				if(linea[k-1]==' ' || linea[k-1]=='!'){ espacios--;}
			}
			/*
			for(unsigned l=0;l<s1.size();l++){
				cout<<s1[l];
			} cout<<endl;
			*/
			float x,y,z;
			x=strtof(&s1[0],0);
			y=strtof(&s2[0],0);
			z=strtof(&s3[0],0);
			//cout<<"x: "<<x<<" y: "<<y<<" z: "<<z<<endl;
			esfera[3*i+j][0]=x;
			esfera[3*i+j][1]=y;
			esfera[3*i+j][2]=z;
		}

	}

	//pass from float esfera[][] to vec3 esfera[]
	esfera_NumVertices=3*numT;
	for(int i=0;i<esfera_NumVertices;i++){
		esfera_points[i][0]=esfera[i][0];
		esfera_points[i][1]=esfera[i][1];
		esfera_points[i][2]=esfera[i][2];
	}

	for(int i=0;i<esfera_NumVertices;i++){
		esfera_colors[i][0]=1;
		esfera_colors[i][1]=0.84;
		esfera_colors[i][2]=0;
	}
	/*
	//ver coordenadas de la esfera
	for(int i=0;i<numT*3;i++){
		for(int j=0;j<3;j++){
			cout<<esfera[i][j]<<" ";
		} cout<<endl;
	}

	//ver RGB de la esfera
	for(int i=0;i<numT*3;i++){
		for(int j=0;j<3;j++){
			cout<<esfera_colors[i][j]<<" ";
		} cout<<endl;
	}	
	*/
}