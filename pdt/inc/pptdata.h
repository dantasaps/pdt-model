#ifndef PPTDATA_H
#define PPTDATA_H

#include <iostream>
#include <fstream>
#include <vector>

#include "motorista.h"
#include "tabela.h"
#include "util.h"

#define UTL 0
#define SAB 1
#define DOM 2

using namespace std;

class DadosInstancia{
public:
	DadosInstancia();
	DadosInstancia(string path);
	~DadosInstancia();

	static vector<string> split(string str, char token);
	void carregarDados();
	int Ft(int d, int t);
	int Td(int d);



	string path;
	string path_asw;

	// Informações da instância
	int empCod;
	string empNome;
	int linhaCod;
	string linhaNome;
	string periodo;
	int tipoDePDT;
	string nomePDT;
	int codTabela;
	int codPControle;
	string nomePControle;
	int tJornada; //em minutos
	int S; //número de semanas
	
	int M; //número de motoristas
	vector< Motorista > m;

	int nUTL; // número de tabelas num dia útil  
	int nSAB; // número de tabelas num sábado
	int nDOM; // número de tabelas num domingo 
	int total; // número total de tabelas
	vector< int > hUTL;
	vector< int > hSAB;
	vector< int > hDOM;
	
	vector< vector< vector< vector <LinhaTabela> > > > T;

	// Informações adicionais de uma instância
 	int QHED;
 	int QMDEJ;
 	int QMDSF;
 	int QMAHTT;
 	int QMIHTT;
 	int QMSSFD;
 	int QMHES;
 	int QMAHPS;
 	int QHJS;

};

#endif