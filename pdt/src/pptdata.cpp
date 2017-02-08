#include "../inc/pptdata.h"

DadosInstancia::DadosInstancia(){
	// Quantidade máxima de minutos extras diários permitidas 
	QHED = 120;
	// Quantidade mínima de descanso entre jornadas em minutos
	QMDEJ = 660;
	// Tamanho máximo de um turno de trabalho em minutos
	QMAHTT = 440;
	// Tamanho mínimo de um turno de trabalho em minutos
	QMIHTT = 300;
	// Quantidade máxima de minutos extras para cada semana da escala
	QMHES = 3000;
	// Quantidade máxima de minutos não aproveitadas por motorista 
	// para cada semana da escala
	QMAHPS = 2640;
	// Jornada semanal de trabalho do motorista, em minutos,
	// obrigatoriamente paga pela empresa
	QHJS = 2640;
	

	// Quantidade máxima de dias sem folga
	QMDSF = 6;
	// Quantidade máxima de semanas sem folga no domingo na escala
	QMSSFD = 6;
}

// Dados inicias e default da instancia.
DadosInstancia::DadosInstancia(string path){
	new (this) DadosInstancia() ;
	this->path = path;
}

DadosInstancia::~DadosInstancia(){}

// Retorna um vector de strings, divide a string str utilizando o
// token como ponto de quebra
vector<string> DadosInstancia::split(string str, char token){
	vector<string> vec;
 	string aux = "";
 	for(unsigned int i = 0; i < str.size(); i++){
 		if (str[i] != token){
 			aux += str[i];
 		}else{
 			vec.push_back(aux);
 			aux = "";
 		}
 	}
 	if (aux != ""){
 		vec.push_back(aux);
 	}
 	return vec;
}

// Retorna a quantidade de Tabelas que um dia d possui
int DadosInstancia::Td(int d){
 	switch(d){
 		case 5:
 			return nSAB;
		case 6:
			return nDOM;
		default:
			return nUTL;
	}
}

// Retorna a quantidade de horários de uma tabela t no dia dia d
int DadosInstancia::Ft(int d, int t){
	switch(d){
		case 5:
			return hSAB[t];
		case 6:
			return hDOM[t];
		default:
			return hUTL[t];
	}
}


// Carrega na memória os dados do arquivo fornecido no construtor
// e segue a disposição descrita no arquivo Leiaute.txt
void DadosInstancia::carregarDados(){
	cout << "Caregando dados \n..." << endl;
 	ifstream file;
 	file.open("ins/"+path);
 	string _;
	string::size_type sz;
	// Dados gerais da instância (linha, empresa, posto de controle, ...)
 	file >> empCod >> _;
	getline(file,empNome);
	file >> linhaCod >> _;
	getline(file,linhaNome);
	getline(file, periodo);
	file >> tipoDePDT >> _;
	getline(file, nomePDT);
	file >> _ >> _ >> _ >> codTabela;
	file >> codPControle >> _;
	getline(file, nomePControle);
	file >> tJornada;
	// QMAHTT = tJornada;
	file >> S;
	file >> M;

	// Dados dos Motoristas
	// ultima folga
	for (int i = 0; i < M; i++){
		Motorista motorista;
		file >> motorista.uf;
		m.push_back(motorista);
	}
	// ultima foga de domingo
	for (int i = 0; i < M; i++){
		file >> m[i].ufd;
	}
	// termino da jornada
	for (int i = 0; i < M; i++){
		file >> _;
		m[i].tj = Hora(split(_, ':'));
	}

	// Dados da tabela
	// Quantidades das tabelas em dias úteis, sábados e domingos
	file >> nUTL;
	file >> nSAB;
	file >> nDOM;
	file >> total;

	// Quantidade de horários que cada tabela de cada tipo de dia possui
	for (int i = 0; i < nUTL; i++){
		file >> _;
		hUTL.push_back(stoi(split(_, '-')[2], &sz));
	}
	for (int i = 0; i < nSAB; i++){
		file >> _;
		hSAB.push_back(stoi(split(_, '-')[2], &sz));
	}
	for (int i = 0; i < nDOM; i++){
		file >> _;
		hDOM.push_back(stoi(split(_, '-')[2], &sz));
	}

	// Dados referentes a cada tarefa de cada tabela
	T.resize(S);
	string line;
 	for (int s = 0; s < S; s++){
 		T[s].resize(7);
 		for (int d = 0; d < 7; d++){
 			T[s][d].resize(Td(d));
 			for (int t = 0; t < Td(d); t++){
 				T[s][d][t].resize(Ft(d,t));
 				for (int f = 0; f < Ft(d, t); f++){
 					file >> line;
 					vector<string> vec = split(line, '-');
 					LinhaTabela l;
 					l.tipoDia = vec[0];
 					l.indiceEscala = stoi(vec[1], &sz);
 					l.codTabela = stoi(vec[2], &sz);
 					l.codTipoHorario = stoi(vec[3], &sz);
 					l.tamanho = stoi(vec[4], &sz);
 					l.codParada = stoi(vec[5], &sz);
 					l.DI = Data(split(vec[6], '/'));
 					file >> line;
 					vec = split(line, '-');
 					l.HI = Hora(split(vec[0], ':'));
 					l.DF = Data(split(vec[1], '/'));
 					file >> line;
 					l.HF = Hora(split(line, ':'));
 					T[s][d][t][f] = l;
 				}
 			}
 		}
 	}

 	file.close();
 	cout << "Pronto" << endl;
}
