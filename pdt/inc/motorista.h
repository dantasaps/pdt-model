#ifndef MOTORISTA_H
#define MOTORISTA_H
#include <iostream>
#include "util.h"

using namespace std;

class Motorista{
public:
	Motorista(){}
	// uf = última folga
	// ufd = última folga de domingo
	// tj = término da jornada no dia anterior ao início da escala
	int uf;
	int ufd;
	Hora tj;

};

#endif