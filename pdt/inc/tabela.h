#ifndef LOADFILE_H
#define LOADFILE_H

#include <iostream>
#include <vector>
#include "util.h"

using namespace std;

class LinhaTabela{
public:
	string tipoDia;
	int indiceEscala;
	int codTabela;
	int codTipoHorario;	
	int tamanho;
	int codParada;
	Hora HI;
	Hora HF;
	Data DI;
	Data DF;

	LinhaTabela(){}
	~LinhaTabela(){}

};

#endif