#ifndef CURVEHERMITE_H
#define CURVEHERMITE_H

#include <iostream>
#include <stdio.h>
#include <fstream>
#include "ShaderMaker.h"
#include "Lib.h"
#include "json.hpp"
#include "Commons.h"

using namespace std;

//Curve di hermite
#define PHI0(t)  (2.0*t*t*t-3.0*t*t+1)
#define PHI1(t)  (t*t*t-2.0*t*t+t)
#define PSI0(t)  (-2.0*t*t*t+3.0*t*t)
#define PSI1(t)  (t*t*t-t*t)
#define  PI   3.14159265358979323846

float DX(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig);
float DY(int i, float* t, float Tens, float Bias, float Cont, Figura* Fig);
void InterpolazioneHermite(float* t, Figura* Fig, vec4 color_top, vec4 color_bot);
void costruisci_forma(vec4 color_top, vec4 color_bot, Figura* forma, string percorso, vec3 first_point);

#endif // CURVEHERMITE_H