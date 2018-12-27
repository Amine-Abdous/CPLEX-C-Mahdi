
#include <ilcplex/ilocplex.h>
#include <ilconcert/ilotupleset.h>
#include<tuple>
#include <list>
#include <vector>
#include <ilopl/iloopl.h>

ILOSTLBEGIN


void dat2(int &d) {

	ifstream instIN2(to_string(d) + ".IN2");
	//ifstream instF(to_string(d) + ".txt");

	// checking that the file exists
	



	int T, k, n;

	instIN2 >> T;
	instIN2 >> k;
	instIN2 >> n;

	vector<int> t(n);
	vector <tuple<int, int>> pT(5 * n);
	vector<double> F(n);


	for (int i = 0; i < n; i++)
	{
		instIN2 >> t[i];
	}
	int p, s;
	char c;
	instIN2 >> p;
	int compt = 0;
	while (p != -1 && p != EOF) {
		instIN2 >> c;
		instIN2 >> s;
		pT[compt] = make_tuple(p, s);
		instIN2 >> p;
		compt++;
	}
	pT.resize(compt);



	//////////////////////////////////////////////////////// Write .dat file 
	string base(".dat");
	stringstream ss;
	ss << d << base;
	ofstream dout(ss.str());

	dout << T << endl;
	dout << k << endl;
	dout << n << endl;

	//////////// t
	dout << "[";
	for (int i = 0; i < n - 1; i++)
	{
		dout << t[i] << ",";
	}
	dout << t[n - 1] << "]" << endl;

	/////////////// Precedences
	for (int i = 0; i < pT.size(); i++)
	{
		dout << get<0>(pT[i]) << "," << get<1>(pT[i]) << endl;
	}
	dout << -1 << "," << -1;

}
