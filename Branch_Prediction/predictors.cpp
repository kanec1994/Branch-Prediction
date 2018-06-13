#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

const int MAX_TABLE = 2048;

struct branch{
        unsigned long long ctr;
        string behavior;
};

void alwaysT(vector<branch> &results, ofstream &fout);
void alwaysNT(vector<branch> &results, ofstream &fout);
void singleBimodal(vector<branch> &results, ofstream &fout);
void twoBitBimodal(vector<branch> &results, ofstream &fout);
void gShare(vector<branch> &results, ofstream &fout);
void tournament(vector<branch> &results, ofstream &fout);

int main(int argc, char* argv[]){
	unsigned long long addr;
	string behavior;
	vector<branch> results;
	string input;
	string output;
	if(argc >= 2){
	      input = argv[1];
	      output = argv[2];
	}
	
	ifstream infile(input);
	while(infile >> hex >> addr >> behavior){
	        struct branch temp;
		temp.ctr = addr;
		temp.behavior = behavior;
	        results.push_back(temp);
	}
	infile.close();

	ofstream fout(output);
	alwaysT(results, fout);
	alwaysNT(results, fout);
	singleBimodal(results, fout);
	twoBitBimodal(results, fout);
	gShare(results, fout);
	tournament(results, fout);
	fout.close();
	return 0;
}

void alwaysT(vector<branch> &results, ofstream &fout){
	int correct = 0;
	int total = 0;	
	for(unsigned int i=0; i<results.size(); i++){
		if(results.at(i).behavior == "T"){
			correct++;
			total++;
		}
		else{
			total++;
		}
	}
	fout << correct << "," << total << ";" << endl;
}

void alwaysNT(vector<branch> &results, ofstream &fout){
	int correct = 0;
	int total = 0;	
	for(unsigned int i=0; i<results.size(); i++){
		if(results.at(i).behavior == "NT"){
			correct++;
			total++;
		}
		else{
			total++;
		}
	}
	fout << correct << "," << total << ";" << endl;
}  

void singleBimodal(vector<branch> &results, ofstream &fout){
        int correct = 0;
	int total = 0;
	int index = 0;
	int mod = 16;

	while(mod <= MAX_TABLE){
	        vector<bool> singleBit;
		for(int i=0; i<mod; i++){
		        singleBit.push_back(true);
		}
		for(unsigned int i=0; i<results.size(); i++){
		        index = results.at(i).ctr % mod;
			if(singleBit.at(index) == true && results.at(i).behavior == "T"){
			        correct++;
				total++;
			}
			else if(singleBit.at(index) == false && results.at(i).behavior == "NT"){
			        correct++;
				total++;
			}
			else{
			        singleBit.at(index) = !singleBit.at(index);
				total++;
			}
		}
		fout << correct << "," << total << ";";
		if(mod != MAX_TABLE){
		        fout << " ";
		}
		correct = 0;
		total = 0;
		if(mod == 32){
		        mod *= 4;
		}
		else{
		        mod *= 2;
		}
	}
	fout << endl;
}

void twoBitBimodal(vector<branch> &results, ofstream &fout){
        int correct = 0;
	int total = 0;
	int index = 0;
	int mod = 16;

	while(mod <= MAX_TABLE){
	        vector<short> twoBit;
		for(int i=0; i<mod; i++){
		        twoBit.push_back(3);
		}
		for(unsigned int i=0; i<results.size(); i++){
 		        index = results.at(i).ctr % mod;
			if(twoBit.at(index) >= 2 && results.at(i).behavior == "T"){
			        if(twoBit.at(index) != 3) twoBit.at(index)++;
			        correct++;
				total++;
			}
			else if(twoBit.at(index) <= 1 && results.at(i).behavior == "NT"){
			        if(twoBit.at(index) != 0) twoBit.at(index)--;
			        correct++;
				total++;
			}
			else{
			        if(twoBit.at(index) <= 1) twoBit.at(index)++;
				else twoBit.at(index)--;
				total++;
			}
                }
                fout << correct << "," << total << ";";
		if(mod < MAX_TABLE){
		        fout << " ";
		}
                correct = 0;
                total = 0;
                if(mod == 32){
		        mod *= 4;
                }
                else{
		        mod *= 2;
                }
	}
	fout << endl;
}

void gShare(vector<branch> &results, ofstream &fout){
        int gShareLength = 3;
	int mask = 7;
        int correct = 0;
	int total = 0;
	int index = 0;
	long long globalHist = 0;
	int mod = 2048;

	while(gShareLength <= 11){
		  vector<unsigned int> gshareTable;
		  for(int i=0; i<mod; i++){
			  gshareTable.push_back(3);
		  }
		  for(unsigned int i=0; i<results.size(); i++){
		          int index = (results.at(i).ctr % mod) ^ globalHist;
			  if(gshareTable.at(index) >= 2 && results.at(i).behavior == "T"){
				  if(gshareTable.at(index) != 3) gshareTable.at(index)++;
				  correct++;
				  total++;
			  }
			  else if(gshareTable.at(index) <= 1 && results.at(i).behavior == "NT"){
				  if(gshareTable.at(index) != 0) gshareTable.at(index)--;
				  correct++;
				  total++;
			  }
			  else{
				  if(gshareTable.at(index) <= 1){
					  gshareTable.at(index)++;
				  }
				  else{
					  gshareTable.at(index)--;
				  }
				  total++;
			  }
			  globalHist = globalHist<<1;
			  if(results.at(i).behavior == "T"){
			          globalHist += 1;
			  }
			  globalHist = globalHist & mask;
		  }
		  fout << correct << "," << total << ";";
		  if(gShareLength < 11){
		          fout << " ";
		  }
		  correct = 0;
		  total = 0;
		  globalHist = 0;
		  mask = (2*mask)+1;
		  gShareLength++;
	}
	fout << endl;
}

void tournament(vector<branch> &results, ofstream &fout){
        int correct = 0;
	int total = 0;
	int mask = 2047;
	long long globalHist = 0;
	int mod = 2048;
	vector<unsigned int> twoBit;
	vector<unsigned int> gshareTable;
	vector<unsigned int> selector;

	for(int i=0; i<mod; i++){
	        twoBit.push_back(3);
		gshareTable.push_back(3);
		selector.push_back(0);
	}
	for(unsigned int i=0; i<results.size(); i++){
	        int bimodIndex = results.at(i).ctr % mod;
	        int gshareIndex = (results.at(i).ctr % mod) ^ globalHist;
		int gshareCorrect = 0;
		int bimodalCorrect = 0;
		if(selector.at(bimodIndex) <= 1){
		        if(gshareTable.at(gshareIndex) >= 2 && results.at(i).behavior == "T"){
			        if(gshareTable.at(gshareIndex) != 3) gshareTable.at(gshareIndex)++;
				gshareCorrect = 1;
				correct++;
				total++;
			}
			else if(gshareTable.at(gshareIndex) <= 1 && results.at(i).behavior == "NT"){
			        if(gshareTable.at(gshareIndex) != 0) gshareTable.at(gshareIndex)--;
				gshareCorrect = 1;
				correct++;
				total++;
			}
			else{
			        if(gshareTable.at(gshareIndex) <= 1) gshareTable.at(gshareIndex)++;
				else gshareTable.at(gshareIndex)--;
				total++;
			}
			globalHist = globalHist<<1;
			if(results.at(i).behavior == "T"){
			        globalHist += 1;
			}
			globalHist = globalHist & mask;
			if(twoBit.at(bimodIndex) >= 2 && results.at(i).behavior == "T"){
			        if(twoBit.at(bimodIndex) != 3) twoBit.at(bimodIndex)++;   
				bimodalCorrect = 1;
			}
			else if(twoBit.at(bimodIndex) <= 1 && results.at(i).behavior == "NT"){
			        if(twoBit.at(bimodIndex) != 0) twoBit.at(bimodIndex)--;
				bimodalCorrect = 1;
			}
			else{
			        if(twoBit.at(bimodIndex) <= 1) twoBit.at(bimodIndex)++;
				else twoBit.at(bimodIndex)--;
			}
			if(gshareCorrect && !bimodalCorrect){
			        if(selector.at(bimodIndex) != 0) selector.at(bimodIndex)--;
			}
			else if(!gshareCorrect && bimodalCorrect){
			        selector.at(bimodIndex)++;
			}
	        }
		else{
		        if(twoBit.at(bimodIndex) >= 2 && results.at(i).behavior == "T"){
		                if(twoBit.at(bimodIndex) != 3) twoBit.at(bimodIndex)++;
				bimodalCorrect = 1;
				correct++;
				total++;
			}
			else if(twoBit.at(bimodIndex) <= 1 && results.at(i).behavior == "NT"){
			        if(twoBit.at(bimodIndex) != 0) twoBit.at(bimodIndex)--;
				bimodalCorrect = 1;
				correct++;
				total++;
			}
			else{
			        if(twoBit.at(bimodIndex) <= 1) twoBit.at(bimodIndex)++;
				else twoBit.at(bimodIndex)--;
				total++;
			}
			if(gshareTable.at(gshareIndex) >= 2 && results.at(i).behavior == "T"){
			        if(gshareTable.at(gshareIndex) != 3) gshareTable.at(gshareIndex)++;
				gshareCorrect = 1;
			}
                        else if(gshareTable.at(gshareIndex) <= 1 && results.at(i).behavior == "NT"){
			        if(gshareTable.at(gshareIndex) != 0) gshareTable.at(gshareIndex)--;
				gshareCorrect = 1;
			}
                        else{
			        if(gshareTable.at(gshareIndex) <= 1) gshareTable.at(gshareIndex)++;
				else gshareTable.at(gshareIndex)--;
			}
			globalHist = globalHist<<1;
			if(results.at(i).behavior == "T"){
			        globalHist += 1;
			}
			globalHist = globalHist & mask;
			if(!gshareCorrect && bimodalCorrect){
			        if(selector.at(bimodIndex) != 3) selector.at(bimodIndex)++;
                        }
                        else if(gshareCorrect && !bimodalCorrect){
			        selector.at(bimodIndex)--;
                        }
		}
	}
	fout << correct << "," << total << ";" << endl;
}
