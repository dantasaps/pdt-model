#include "../inc/pptmodel.h"

ModeloPLI::ModeloPLI(DadosInstancia dados) : dados(dados){
	local = 0;
	total = 0;
}

int ModeloPLI::DC(int e){
	return e%7;
}

int ModeloPLI::SC(int e){
	return e/7;
}

void ModeloPLI::contar(int n){
	cout << n << " | " << local << endl;
	total += local;
	local = 0;
}

void ModeloPLI::resolver(int init){
	// Facilitando o acesso aos limites
	int S = dados.S;
	int D = 7;
	int E = S * D;
	int M = dados.M;

	int uInf = -dados.QMAHPS;
	int uSup = dados.QHED * dados.QMDSF;

	IloEnv env;
	try {
		IloModel model(env);
		// Variáveis de Decisão
		// x_msdtf = indica se a tarefa t, da tabela t, 
		//  do dia d da semana s foi atribuida ao motorista m
		IloArray< IloArray< IloArray< IloArray< IloIntVarArray > > > > x(env, M);

		// y_msd = indica se no dia d da semana s o motorista m trabalhou
		IloArray< IloArray< IloIntVarArray > > y(env, M);

		// z_m = indica se o motorista foi usado na escala
		IloIntVarArray z(env, M, 0, 1);

		// r_msdtf = indica se o motorista m encerroa a jornada na tarefa f 
		//  da tabela t do dia d da semana s
		IloArray< IloArray< IloArray< IloArray< IloIntVarArray > > > > r(env, M);

		// u_ms = indica a quantidade, em minutos, trabalhada a mais ou
		// a menos em relação QHJS
		IloArray< IloIntVarArray > u(env, M);

		// v_ms = indica a quantidade de horas a ser paga ao motorista m na semana s
		IloArray< IloIntVarArray > v(env, M);

		// Dimensionando os vetores
		for (int m = 0; m < M; m++ ){
			x[m] = IloArray< IloArray< IloArray< IloIntVarArray > > >(env, S);
			r[m] = IloArray< IloArray< IloArray< IloIntVarArray > > >(env, S);
			y[m] = IloArray< IloIntVarArray > (env, S);
			u[m] = IloIntVarArray(env, S, uInf, uSup);
			v[m] = IloIntVarArray(env, S, 0, uSup);
			for (int s = 0; s < S; s++){
				x[m][s] = IloArray< IloArray< IloIntVarArray > >(env, D);
				r[m][s] = IloArray< IloArray< IloIntVarArray > >(env, D);
				y[m][s] = IloIntVarArray(env, D, 0, 1);
				for (int d = 0; d < D; d++){
					int Td = dados.Td(d);
					x[m][s][d] = IloArray< IloIntVarArray >(env, Td);
					r[m][s][d] = IloArray< IloIntVarArray >(env, Td);
					for (int t = 0; t < Td; t++){
						int Ft = dados.Ft(d, t);
						x[m][s][d][t] = IloIntVarArray(env, Ft, 0, 1);
						r[m][s][d][t] = IloIntVarArray(env, Ft+1, 0, 1);
					}
				}
			}
		}	

		// Função Objetivo
		// (1) Minimizar a quantidade de motoristas utilizados na escala
		IloExpr obj(env);
		for (int m = 0; m < M; m++){
			obj += z[m];
		}
		model.add(IloMinimize(env, obj));
		obj.end();

		// (2) Todo horário deve ser atendido por um único motorista (SSP)
		for(int s = 0; s < S; s++){
			for (int d = 0; d < D; d++){
				for (int t = 0; t < dados.Td(d); t++){
					for (int f = 0; f < dados.Ft(d,t); f++){
						IloExpr expr(env);
						for (int m = 0; m < M; m++){
							expr += x[m][s][d][t][f];
						}
						model.add(expr == 1);
						local++;
						expr.end();
					}
				}

			}
		}
		contar(2);

		// (3) Identificando se um motorista trabalhou num certo dia 
		for (int m = 0; m < M; m++){
			for(int s = 0; s < S; s++){
				for (int d = 0; d < D; d++){
					for (int t = 0; t < dados.Td(d); t++){
						for (int f = 0; f < dados.Ft(d,t); f++){
							model.add(y[m][s][d] >= x[m][s][d][t][f]);
							local++;
						}
					}
				}
			}
		}
		contar(3);

		// (4) Contabilização do motorista 
		for (int m = 0; m < M; m++){
			for(int s = 0; s < S; s++){
				for (int d = 0; d < D; d++){
					model.add(z[m] >= y[m][s][d]);
					local++;
				}
			}
		}
		contar(4);

		// (10) Mínimo de horas da jornada de trabalho 
		for (int m = 0; m < M; m++){
			for(int s = 0; s < S; s++){
				for (int d = 0; d < D; d++){
					IloExpr expr(env);
					for (int t = 0; t < dados.Td(d); t++){
						for (int f = 0; f < dados.Ft(d,t); f++){
							expr+= dados.T[s][d][t][f].tamanho * x[m][s][d][t][f];
						}
					}
					model.add(expr >= dados.QMIHTT*y[m][s][d]);
					local++;
					expr.end();
				}
			}
		}
		contar(10);

		// (11) Máximo de horas na jornada diária 
		for (int m = 0; m < M; m++){
			for(int s = 0; s < S; s++){
				for (int d = 0; d < D; d++){
					IloExpr expr(env);
					for (int t = 0; t < dados.Td(d); t++){
						for (int f = 0; f < dados.Ft(d,t); f++){
							expr+= dados.T[s][d][t][f].tamanho * x[m][s][d][t][f];
						}
					}
					model.add(expr <= (dados.QMAHTT + dados.QHED));
					local++;
					expr.end();
				}
			}
		}
		contar(11);

		cout << "\nAdicionadas " << total << " restrições\n" << endl;
		// Resolvendo o Modelo
		IloCplex cplex(model);
        cplex.setParam(IloCplex::Param::TimeLimit,3600);

        // adicionando solução inicial
		if (init){
			ifstream file;
			file.open("ans/"+dados.path);
			IloNumVarArray startVar(env);
			IloNumArray startVal(env);
			for (int m = 0; m < M; m++){
				for (int s = 0; s < S; s++){
					for (int d = 0; d < D; d++){
						for (int t = 0; t < dados.Td(d); t++){
							for (int f = 0; f < dados.Ft(d,t); f++){
								int temp;
								file >> temp;
								startVar.add(x[m][s][d][t][f]);
								startVal.add(temp);
							}
						}
					}
				}
			}
			cplex.addMIPStart(startVar, startVal);
		}

		if ( !cplex.solve() ) {
			env.error() << "Failed to optimize LP." << endl;
			throw(-1);
		}

		// salvando a saida
		// ofstream solution_file;
 	// 	solution_file.open(dados.path, 'w');
 		// if(solution_file.is_open()){

			for (int m = 0; m < M; m++){
				for (int s = 0; s < S; s++){
					for (int d = 0; d < D; d++){
						for (int t = 0; t < dados.Td(d); t++){
							for (int f = 0; f < dados.Ft(d,t); f++){
								int aux = cplex.getValue(x[m][s][d][t][f]);
								cout << aux << " ";
							}
						}
					}
				}
				cout << endl; 
			}
			// solution_file.close();
			// cout << "\n Salvou em sol/" << dados.path << endl;
 		// }


		env.out() << "Solution status = " << cplex.getStatus() << endl;
		env.out() << "Solution value = " << cplex.getObjValue() << endl;
		env.out() << "Tempo = " << cplex.getTime() << endl;



	}catch (IloException& e) {
		cerr << "Concert exception caught: " << e << endl;
	}
	env.end();
}







