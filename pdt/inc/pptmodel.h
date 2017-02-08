#ifndef MODELOPLI_H
#define MODELOPLI_H
#include "pptdata.h"
#include <ilcplex/ilocplex.h>

ILOSTLBEGIN

class ModeloPLI{
public:
	int local;
	int total;
	DadosInstancia dados;
	ModeloPLI(DadosInstancia dados);

	void resolver(int init);

	void contar(int n);
	// funções de indexação 
	int DC(int e);
	int SC(int e);
};

#endif