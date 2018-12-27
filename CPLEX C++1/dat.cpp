#include <ilcplex/ilocplex.h>
#include <ilconcert/ilotupleset.h>
#include<tuple>
#include <list>
#include <vector>
//#include <ilopl/iloopl.h>
#include <cstdlib> 
ILOSTLBEGIN


void dat(int &d, int&r) {
	srand(time(0)); // tunes the rand() function with the current time 
	ifstream instIN2(to_string(d) + ".IN2");
	ifstream instF(to_string(d) + ".txt");

	// checking that the file exists
	if (!instIN2.is_open()) {
		cerr << "the instance IN2 file does not exists" << endl;
	}

	if (!instF.is_open()) {
		cerr << "the txt file (FORCE) does not exists" << endl;
	}

	int T, k, n;

	instIN2 >> T;
	instIN2 >> k;
	instIN2 >> n;

	vector<int> T_i(n);
	vector<vector<int>> T_ij(r, T_i);

	vector<int> cost(r);

	vector<int> t(n);


	vector<double> F_i(n);
	vector<vector<double>> F_ij(r, F_i);

	vector<double> F(n);


	for (int i = 0; i < n; i++)
	{
		instIN2 >> t[i];
	}
	int p, s;
	char c;
	instIN2 >> p;
	int compt = 0;
	vector <tuple<int, int>> pT;
	while (p != -1 && p != EOF) {

		instIN2 >> c;

		instIN2 >> s;
		pT.push_back(make_tuple(p, s));
		instIN2 >> p;
	}

	for (int i = 0; i < n; i++) { instF >> F[i]; }
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// generate instance
	int wc, w, pt, pi;
	double pc;
	double C = 0.00017;
	//Scholl equip basic cost
	wc = rand() %90+30;
//	cout << "basic cost	" << wc << endl;
	cost[0] = wc;
	for (int j = 0; j < n; j++) {
		for(int i=0;i<r;i++){
		T_ij[i][j] = t[j];
		F_ij[i][j] = F[j];
		}
	}
	// Proportion of task the equipment affect
	//time
	int nn = floor(0.6*n);
	//load
	//pi = rand()%n;
	//cout <<n << "	" << pt << "	" << pi << endl;
	vector<double> Co;
	vector<int> List_t;
	vector<int> List_I;
	int rand_t, rand_i;
	int degt, degi;
	int ind_t, ind_i;
	double m_t = 0; 
	double m_i=0;
	for (int i = 1; i < r; i++) {
		// Proportion of task the equipment affect
		//time
		pt = (rand() %nn)+1;
		//load
		pi = (rand() % nn)+1;
		// Compute time
		for (int h = 0; h < pt; h++) {
			degt = (rand() % 7) + 1;
		
			ind_t = rand() % (n - 1);
			m_t = m_t + degt;
			List_t.push_back(ind_t);

		if ((T_ij[i][ind_t] - degt) < 1) { T_ij[i][ind_t] = T_ij[i][ind_t];}
		else if ((T_ij[i][ind_t]- degt) >= 1) {T_ij[i][ind_t] = T_ij[i][ind_t] - degt;}
		
		}
		// Compute Load 
		for (int h = 0; h < pi; h++) {
			degi = (rand() % 7) + 1;
			
			ind_i = rand() % (n - 1);
			m_i = m_i + degt;
			List_I.push_back(ind_i);

			if ((F_ij[i][ind_t] - degt) < 1) {F_ij[i][ind_t] = F_ij[i][ind_t];}
			else if ((F_ij[i][ind_t] - degt) >= 1) {F_ij[i][ind_t] = F_ij[i][ind_t] - degt;}

		}
		
		
		m_t = floor(m_t / pt);
		m_i = floor(m_i / pt);
		// exp constant for additional cost
		pc = (pt + pi + m_t) / 1000.0;
		//cout << n << "	" << pt << "	" << pi << endl;
		cost[i]=floor(cost[0] + exp(pc*cost[0]));
		//cout << n << "	" << pt << "	" << pi << "	" << m_t << "	" << cost[i] << endl;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Compute I_ij
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < n; j++) {
			F_ij[i][j] = T_ij[0][j] * C*F_ij[i][j];
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	vector<int> Maxtt(n);
	vector<double> MaxII(n);
	int max_t;
	double max_I;
	for (int j = 0; j < n; j++) {
		max_t = T_ij[0][j];
		max_I = F_ij[0][j];
		for (int h = 1; h<r; h++) {
			if (T_ij[h][j] > max_t) { max_t = T_ij[h][j]; }
			if (F_ij[h][j]>max_I) { max_I = F_ij[h][j]; }
		}
		Maxtt[j] = max_t;
		MaxII[j] = max_I;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////// Write .dat file 

	ofstream dout(to_string(d) + "_" + to_string(r) + ".dat");

	dout << T << endl;
	dout << k << endl;
	dout << n << endl;
	dout << r << endl;
	//////////// t
	dout << "[";
	for (int i = 0; i < r; i++) {
		dout << "[";
		for (int j = 0; j < n; j++) {
			dout << T_ij[i][j];
			if (j != n - 1) { dout << ","; }
		}
		if (i == r - 1) { dout << "]]" << endl; }
		else { dout << "]," << endl; }
	}
	//////////////////////////////
	// cost
	dout << "[";
	for (int i = 0; i < r; i++)
	{
		dout << cost[i];
		if (i != r - 1) { dout << ","; }
		if (i == r - 1) { dout << "]" << endl; }
	}
	//////////////////////////////
	//////////// F
	dout << "[";
	for (int i = 0; i < r; i++) {
		dout << "[";
		for (int j = 0; j < n; j++) {
			dout << F_ij[i][j];
			if (j != n - 1) { dout << ","; }
		}
		if (i == r - 1) { dout << "]]" << endl; }
		else { dout << "]," << endl; }
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	// Min t
	dout << "[";
	for (int j = 0; j < n; j++) {
		dout << Maxtt[j];
		if (j != n - 1) { dout << ","; }
		if (j == n - 1) { dout << "]" << endl; }
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	// Min F
	dout << "[";
	for (int j = 0; j < n; j++) {
		dout << MaxII[j];
		if (j != n - 1) { dout << ","; }
		if (j == n - 1) { dout << "]" << endl; }
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	/////////////// Precedences
	for (int i = 0; i < pT.size(); i++)
	{
		dout << get<0>(pT[i]) << "," << get<1>(pT[i]) << endl;
	}
	dout << -1 << "," << -1;

}
