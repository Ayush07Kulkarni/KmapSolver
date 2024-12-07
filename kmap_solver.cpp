#include <iostream>
#include <vector>
#include <set>
#include <bitset>
#include<map>
#include <algorithm>
#include <sstream>
using namespace std;

class Util{
	/*
	A class that contains utility functions required in the program.
	*/
public:



	string intToBinString(int size, int val){
		string bin;
		bin = bitset<100>(val).to_string();
		bin = bin.substr(100-size);
		return bin;
	}

	int get1s(string x) {
		// returns the number of 1s in a binary string
		int count = 0;
		for (int i = 0; i < x.size(); ++i){
			if (x[i] == '1')
				count++;
		}
		return count;
	}

	bool compare(string a, string b) {
		// checks if two string differ at exactly one location or not
		int count = 0;
		for(int i = 0; i < a.length(); i++) {
			if (a[i]!=b[i])
				count++;
		}

		if(count == 1)
			return true;

		return false;
	}

	string getDiff(string a, string b) {
		// returs a string that replaces the differ location of two strings with '-'
		for(int i = 0; i < a.length(); i++) {
			if (a[i]!=b[i])
				a[i]='-';
		}

		return a;
	}

	bool checkEmpty(vector< vector< string> > table){
		for (int i = 0; i < table.size(); ++i){
			if (table[i].size()!=0) {
				return false;
			}
		}
		return true;
	}

	string binToString(string x){
		// converts binary string to alphabetic variables
		string out = "";
		for (int i = 0; i < x.size(); ++i){
			if (x[i]=='1') {
				char x = 97 + i;
				out += x;
			}
			else if (x[i]=='0') {
				char x = 97 + i;
				out += x;
				out += "'";
			}
		}
		return out;
	}

	bool primeIncludes(string imp, string minTerm){
		// check is a prime implicant satisfies a min term or not
		for (int i = 0; i < imp.size(); ++i){
			if (imp[i]!='-'){
				if(imp[i]!=minTerm[i]){
					return false;
				}
			}
		}
		return true;
	}

	int getVar(set<int> comb, vector<string> primeImp){
		// returns the number of variables in a petrick method combination
		int count = 0;
		set<int> :: iterator itr;
		for (itr = comb.begin(); itr != comb.end(); ++itr){
			int imp = *itr;
			for (int i = 0; i < primeImp[imp].size(); ++i){
				if (primeImp[imp][i]!='-')
					count ++;
			}
		}
		return count;

	}

};

class Tabulation : private Util{
private:
	Util util;
	vector< int > minInt; // min terms in decimal
 	vector< string > minBin; // min terms in binary
    vector<int> dcInt; // dontcare in decimal
    vector<string> dcBin; // dont care in binary
 	int nBits; // no of variables
 	int nMin;  // no of minterms
    int nDontCare; // no of dont care condition
 	vector< vector< string> > table; 
 	vector< string > primeImp;
 	vector< set<int> > functions;

     map<int,int> codeConverter = {
        {0, 0},
        {1, 1},
        {2, 3},
        {3, 2},
        {4, 4},
        {5, 5},
        {6, 7},
        {7, 6},
        {8, 12},
        {9, 13},
        {10, 15},
        {11, 14},
        {12, 8},
        {13, 9},
        {14, 11},
        {15, 10}
    };
 	
 

public:

    vector<int> getGrayCode() 
    {
        vector<int> graycodes;
        for (int i : minInt) 
        {
            graycodes.push_back(codeConverter[i]);
        }
        for (int i: dcInt)
        {
            graycodes.push_back(codeConverter[i]);
        }
        return graycodes;
    }

    vector<vector<int>> getkmap()
    {
        int rows,cols;
        if( nBits == 2)
        {
            rows = 2;
            cols =2;
        }
        else if(nBits == 3)
        {
            rows = 2;
            cols = 4;
        }
        else if (nBits == 4)
        {
            rows = 4;
            cols = 4;
        }
        vector<vector<int>> kmap(rows, vector<int>(cols ,0));
        for( int code : getGrayCode())
        {
            int col = code/nBits;
            kmap[col][code%nBits] = 1;
        }
        
        return kmap;
    }

    int printKmap()
    {
        for (const auto& row : getkmap()) 
        {
        for (int elem : row) 
        {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
        }
    }


	void initialise(int bits, int minCount, const vector<int> &minTerms, int dontCareCount, const vector<int> &dontCareTerms) {
    	nBits = bits;
    	nMin = minCount;
    	nDontCare = dontCareCount;

    	minInt = minTerms;
    	for (int i = 0; i < nMin; ++i) {
        	minBin.push_back(util.intToBinString(nBits, minInt[i]));
    	}

    	dcInt = dontCareTerms;
    	for (int i = 0; i < nDontCare; ++i) 
		{
        	dcBin.push_back(util.intToBinString(nBits, dcInt[i]));
    	}

    	// Initialize the table
    	table = vector<vector<string>>(nBits + 1);
	}


	void setPrimeImp() {
		set< string > primeImpTemp;
		createTable();
		

		// Combine consecutive terms in the table until its empty
		while (!util.checkEmpty(table)){
			table = combinePairs(table, primeImpTemp);
		}
		
		set<string > :: iterator itr;  // convert set to vector
		 for (itr = primeImpTemp.begin(); itr != primeImpTemp.end(); ++itr){
		 	string x = *itr;
        	primeImp.push_back(x);
    	}

    	

	}

	void minimise() {
		// prepare primeImp chart
		vector<vector<bool>> primeImpChart(primeImp.size(), vector<bool>(nMin, false));

		for (int i = 0; i < primeImp.size(); ++i){
			for (int j = 0; j < nMin; ++j){
				primeImpChart[i][j] = util.primeIncludes(primeImp[i], minBin[j]);
			}
		}

	

		// petric logic
		vector< set<int> > patLogic;
		for (int j = 0; j < nMin; ++j){ // column iteration
			set<int> x;
			for (int i = 0; i < primeImp.size(); ++i){ // row iteration
				if (primeImpChart[i][j] == true) {
					x.insert(i);
				}
			}
			patLogic.push_back(x);
		}
		for (int i = 0; i < patLogic.size(); ++i){
			set<int > :: iterator itr;  // convert set to vector
			for (itr = patLogic[i].begin(); itr != patLogic[i].end(); ++itr){
				int x = *itr;
	        	
	    	}
	    	
		}

		// get all possible combinations
		set< set<int> > posComb;
		set<int> prod;
		getPosComb(patLogic, 0, prod, posComb); // recursively multiply set elements
		int min = 9999;

		
		set< set<int> > :: iterator itr1;
		for (itr1 = posComb.begin(); itr1 != posComb.end(); ++itr1){
			set<int> comb = *itr1;
			if (comb.size() < min){
				min = comb.size();
			}
			set<int > :: iterator itr;  
			for (itr = comb.begin(); itr != comb.end(); ++itr){
				int x = *itr;
	        	
	    	}
	    	
		}

		
		//Combinations with minimum terms
		vector< set<int> > minComb;
		set< set<int> > :: iterator itr;
		for (itr = posComb.begin(); itr != posComb.end(); ++itr){
			set<int> comb = *itr;
			if (comb.size() == min) {
				minComb.push_back(comb);
			}
		}

		//Combinations with minimum variables
		min = 9999;
		for (int i = 0; i < minComb.size(); ++i){
			if(util.getVar(minComb[i], primeImp) < min){
				min = util.getVar(minComb[i], primeImp);
			}
		}

		for (int i = 0; i < minComb.size(); ++i){
			if(util.getVar(minComb[i], primeImp) == min){
				functions.push_back(minComb[i]);
			}
		}
	}

	void displayFunctions() {
		// prints output
		cout << "\n\nThe possible functions are-\n" << endl;
		for (int i = 0; i < functions.size(); ++i){
			set<int> function = functions[i]; 
			set<int> :: iterator itr;
			cout << "Function " << i+1 << ":"<< endl;
			for (itr = function.begin(); itr != function.end(); ++itr){
				int x = *itr;
				cout << util.binToString(primeImp[x]) << " + ";
			}
			
		}
	}

	void getPosComb(vector< set<int> > &patLogic, int level, set<int> prod, set< set<int> > &posComb) {
		// a recursive function to multiple elements of set patLogic and store it in set posComb
		if (level == patLogic.size()){
			set<int> x = prod;
			posComb.insert(x);
			return;
		}
		else{
			set<int > :: iterator itr;  
			for (itr = patLogic[level].begin(); itr != patLogic[level].end(); ++itr){

				int x = *itr;
	        	bool inserted = prod.insert(x).second;
	        	getPosComb(patLogic, level+1, prod, posComb);
	        	if (inserted){
	        		prod.erase(x);
	        	}
	    	}
		}
	}

	vector< vector< string> > combinePairs(vector< vector< string> > table, set<string>& primeImpTemp) {
		vector<vector<bool>> checked(table.size(), vector<bool>( nMin , false));
		vector< vector< string> > newTable(table.size()-1);


		for (int i = 0; i < table.size() -1; ++i){
			for (int j = 0; j < table[i].size(); ++j){
				for (int k = 0; k < table[i+1].size(); k++){
					if (util.compare(table[i][j], table[i+1][k])){
						newTable[i].push_back(util.getDiff(table[i][j], table[i+1][k]));
						checked[i][j] = true;
						checked[i+1][k] = true;
					}
				}
			}
		}

		for (int i = 0; i < table.size(); ++i){
			for (int j = 0; j < table[i].size(); ++j){
				if (!checked[i][j]) {
					primeImpTemp.insert(table[i][j]);
				}
			}
		}

		return newTable;
	}

	void createTable() {
		for (int i = 0; i < nMin; ++i){
			int num1s = util.get1s(minBin[i]);
			table[num1s].push_back(minBin[i]);
		}
        if ( nDontCare != 0)
        {
            for (int i = 0; i < nDontCare ; ++i)
            {
                int num1s = util.get1s(dcBin[i]);
                table[num1s].push_back(dcBin[i]);
            }
        }
		
		
	}

};

int main()  {

	Tabulation tab;

    // Reading input from stdin
    string input;
    getline(cin, input); // Read all inputs as a single line

    stringstream ss(input);
    int nBits, nMin, nDontCare;
    ss >> nBits >> nMin; // First two values: number of variables and min terms count

    vector<int> minTerms(nMin);
    for (int i = 0; i < nMin; ++i) {
        ss >> minTerms[i];
    }

    ss >> nDontCare; // Next value: don't-care terms count
    vector<int> dontCareTerms(nDontCare);
    for (int i = 0; i < nDontCare; ++i) {
        ss >> dontCareTerms[i];
    }

    // Initialize Tabulation with provided input
    tab.initialise(nBits, nMin, minTerms, nDontCare, dontCareTerms);

    // Solve and display results
    tab.printKmap();
    tab.setPrimeImp();
    tab.minimise();
    tab.displayFunctions();

    return 0;
}