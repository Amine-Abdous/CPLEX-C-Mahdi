
#include <ilcplex/ilocplex.h>
#include <ilconcert/ilotupleset.h>
#include<tuple>
#include <list>
#include <time.h> 
#include <algorithm>
#include <ctime>
#include <vector>


ILOSTLBEGIN


void SALBP_Solve(int d) {
	IloEnv env;

	try {

		ofstream f_out("SALBP-1_" + to_string(d) + ".sol");


		// Declare parameters
		IloNumArray t(env);
		IloNum T; // adj takt time
		IloNum takt; // takt time= adj takt time + transfer time
		IloNum k; // number of workstation
		IloNum n; // number of tasks
		IloNumArray Ln(env); // tasks load


							 // Read data file
		ifstream din(to_string(d) + ".dat");
		// checking that the file exists
		if (!din.is_open()) {
			cerr << "the instance IN2 file does not exists" << endl;
		}
		din >> T >> k >> n;
		din >> t;

		///////////// Precedence

		int p, s;
		char c;
		din >> p;
		int compt = 0;
		vector <tuple<int, int>> pT;
		while (p != -1 && p != EOF) {

			din >> c;

			din >> s;
			pT.push_back(make_tuple(p, s));
			din >> p;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Create model
		IloModel mdl(env);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Define variables
		// x_jk
		IloArray<IloBoolVarArray> x_jk(env, n);
		for (int i = 0; i < n; i++) {
			x_jk[i] = IloBoolVarArray(env, k);
		}
		IloBoolVarArray y_k(env, k);

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// State constraints that all tasks should be assigned
		IloExpr Sum_k(env);
		//Sum_k.setName("c1");
		for (int j = 0; j < n; j++) {
			Sum_k.clear(); // clear the current
			for (int m = 0; m < k; m++) {
				Sum_k += x_jk[j][m];
			}
			mdl.add(Sum_k == 1);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// State constraints of takt time
		IloExpr Sum_j(env);
		//Sum.setName("c2");
		for (int m = 0; m < k; m++) {
			Sum_j.clear(); // clear the current

			for (int j = 0; j < n; j++) {
				Sum_j += x_jk[j][m] * t[j];
			}
			mdl.add(Sum_j <= T*y_k[m]);
		}

		// constraints on the number of workstations
		IloExpr Sum_m(env);

		Sum_m.clear(); // clear the current

		for (int m = 0; m < k; m++) {

			Sum_m += y_k[m];
		}
		mdl.add(Sum_m == k);
		/////////////// Precedences constraints (g,h) g is the predecessor of h ////////////////////////////////////////////////////
		int g, h;
		IloExpr Sum_g(env);
		IloExpr Sum_h(env);

		for (int p = 0; p < pT.size(); p++) {
			Sum_g.clear();
			Sum_h.clear();
			g = get<0>(pT[p]);
			h = get<1>(pT[p]);
			for (int m = 0; m < k; m++) {

				Sum_h += ((m + 1)*x_jk[h - 1][m]);
				Sum_g += ((m + 1)*x_jk[g - 1][m]);


			}
			mdl.add(Sum_g <= Sum_h);
		}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Solve and output solutions to a file
		IloCplex cplex(mdl);
		cplex.setOut(env.getNullStream()); // free the console from compilation's journal
		//cplex.exportModel("salbp-fmax.lp");

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		clock_t Start_total_d = clock(); // total time

										 //cplex.setParam(IloCplex::ClockType, 1);
		cplex.setParam(IloCplex::TiLim, 900);

		if (cplex.solve()) {
			clock_t Fin_total_d = clock();
			// total time
			double Time_Total_d = (Fin_total_d - Start_total_d) / (double)CLOCKS_PER_SEC;

			vector<bool> X__k(k);
			vector<vector<bool>> X_jk(n, X__k);
			f_out << "x" << endl;
			for (int j = 0; j < n; j++) {
				for (int m = 0; m < k; m++) {

					X_jk[j][m] = cplex.getValue(x_jk[j][m]);
					f_out << cplex.getValue(x_jk[j][m]) << "	";
				}
				f_out << endl;
			}

			f_out << "y" << endl;
			for (int m = 0; m < k; m++) {
				f_out << cplex.getValue(y_k[m]) << "	";
			}

			f_out << endl;


		
			cout << d << "	" << cplex.getStatus() << "	"<< cplex.getBestObjValue() <<"	" <<  cplex.getCplexTime() << "	" << cplex.getMIPRelativeGap() * 100 << endl;
		}
		else {
			cout << "No solution for SALBP-1" << endl;
			f_out << "No solution for SALBP-1" << endl;
			cplex.printTime();
		}
	}

	// Error handling
	catch (IloException& ex) {
		cerr << "Error: " << ex << endl;
	}
	catch (...) {
		cerr << "Error" << endl;
	}

	// End environment

	env.end();

}

