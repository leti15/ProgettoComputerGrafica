#ifndef COMMONS_H
#define COMMONS_H


#include <iostream>
#include "ShaderMaker.h"
#include "Lib.h"
#include <time.h>

typedef struct {
	GLuint VAO;
	GLuint VBO_G;
	GLuint VBO_C;
	GLuint EBO_indici;
	int nTriangles;
	// Vertici
	vector<vec3> vertici;
	vector<vec3> CP;
	vector<vec4> colors;
	vector<int> indici;
	// Numero vertici
	int nv;
	//Matrice di Modellazione: Traslazione*Rotazione*Scala
	mat4 Model;
	int sceltaVS;
	int sceltaFS;
	string name;
} Figura;

typedef struct {
	float x;
	float y;
}position;


extern bool press_a, press_d, press_w;
extern float accelerazione; // forza di accelerazione data dalla tastiera
extern float velocita;
extern float accelerazione_verticale;
extern float posx; //coordinate sul piano della posizione iniziale della palla
extern float  posy;
extern float  width;
extern float  height;
extern float angolo;
extern float* offsets;
extern float posSole;
extern int nPale;
extern unsigned int programId;
extern mat4 Projection;
extern GLuint MatProj, MatModel;
extern float width, height;
extern position* posNuvole;
extern int tot_nuvole;


#endif // COMMONS_H