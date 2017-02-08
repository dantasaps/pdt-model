#include "../inc/pptmodel.h"
#include <stdio.h>

int main(int argc, char const *argv[]){



	cout << "Lendo " << argv[1]<< endl;
	DadosInstancia inst(argv[1]);
	inst.carregarDados();
	ModeloPLI modelo(inst);
	try{
		modelo.resolver(atoi(argv[2]));
	}catch(...){
		cout << " Não encontrou solução [ótima] para a instância " << endl;
	}
	cout << endl << endl ;

	return 0;
}
