//============================================================================
// Name        : minisat.cpp
// Author      : Vsk
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <regex.h>
using namespace std;

template<typename T>
std::string to_string(T value)
{
std::stringstream stream;
stream << value;
return stream.str();
}

int main(int argc, char **argv) {
    int rank, size;
    int timecpu, iterations, unrep;
    ofstream out;
    const string MINISATPATH = "./minisatmod/minisatmod";
    const string HCSMPATH = "./NEFindingRep.py";
    const string HCMPATH = "./FindingRep.py";
    timecpu = atoi(argv[1]);
    iterations = atoi(argv[2]);
    unrep = atoi(argv[3]);
    MPI_Init(&argc, &argv);
    //Количество процессов в этом коммуникаторе5.80357e+08
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Ранг процесса
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    string nameM = "minisatout" + to_string(rank);
    string nameSH = "hashout" + to_string(rank);
    string nameHSM = "hashmodout" + to_string(rank);
    string nameHSM1 = "hashmodoutun" + to_string(rank);
    srand(rank*time(NULL));
    string var_decay = "-var-decay=" + to_string(((float)rand()+1)/((float)RAND_MAX+2));
    for (int z=0;z<iterations;z++){
    printf("%d", rank);
    ofstream fileM(nameM);
    ofstream fileSH(nameSH);
    ofstream fileunic("nameunic");
    ofstream fileSHM(nameHSM);
    ofstream fileSHM1(nameHSM1);
    string scmd = MINISATPATH + " " + var_decay + " -cpu-lim=" + to_string(timecpu) + " MD4_implication_vars_16.cnf > " + nameM;
 const char *cmd = scmd.c_str();
 system(cmd);
    string scmd1 = HCSMPATH + " " + nameM + " > " + nameSH;
   const char *cmd1 = scmd1.c_str();
   system(cmd1);
   string scmd3 = HCMPATH + " " + nameM + " > " + nameHSM;
   const char *cmd3 = scmd3.c_str();
   system(cmd3);
   string scmd5 = HCSMPATH + " " + nameHSM + " > " + nameHSM1;
   const char *cmd5 = scmd5.c_str();
   system(cmd5);
   MPI_Barrier(MPI_COMM_WORLD);
   if (rank == 0){
	   string scmd4 = "cat";
	   for (int i = 0; i<size; i++){
		   string cathcm = " hashmodoutun" + to_string(i);
		   scmd4 = scmd4 + cathcm;
	   }
	   scmd4 = scmd4 + " > repeats";
	   const char *cmd4 = scmd4.c_str();
	   system(cmd4);
	   system("./FindingRep.py repeats > repeatsrepeats");
	   system("cat MD4_implication_vars_16.cnf repeatsrepeats > cnfwithrepeats.cnf");
   }

 if (rank == 0){
    	//printf("working");
    	string creation = "cat";
    	for ( int i = 0; i<size; i++) {
    	    string namehere = " hashout" + to_string(i);
    		creation = creation + namehere;
    	}
    	//creation = creation + " > uniccluases";
    	cout << creation << endl;
 const char *cmd2 = creation.c_str();
 FILE * popen_result;
 char buff[BUFSIZ];
 popen_result = popen(cmd2, "r");
 while(fgets(buff, sizeof(buff), popen_result)!=NULL){
	 fileunic<<buff;
 }
pclose(popen_result);
 }
if (rank == 0){
system("./FindingRep.py nameunic > repeatnameunic");
system("cat MD4_implication_vars_16.cnf repeatnameunic > CNFwithUnic.cnf");
remove("freq_file");
remove("stat");
remove("split_stat");
remove("str_len_file");
string namehere8 = "MD4_implication_vars_16_" + to_string(z) + ".cnf";
const char *nh8 = namehere8.c_str();
rename("MD4_implication_vars_16.cnf", nh8);
for ( int i = 0; i<size; i++) {
	string namehere1 = "hashout" + to_string(i);
	string namehere2 = "hashmodout" + to_string(i);
	string namehere7 = "hashmodoutun" + to_string(i);
	string namehere3 = "minisatout" + to_string(i);
	string namehere5 = "repeatsrepeats" + to_string(z);
	string namehere6 = "repeatnameunic" + to_string(z);
	const char *nh1 = namehere1.c_str();
	const char *nh2 = namehere2.c_str();
	const char *nh3 = namehere3.c_str();
	const char *nh5 = namehere5.c_str();
	const char *nh6 = namehere6.c_str();
	const char *nh7 = namehere7.c_str();
	remove(nh1);
	remove(nh2);
	remove(nh7);
	remove(nh3);
	rename("repeatsrepeats", nh5);
	rename("repeatnameunic", nh6);
	    }
remove("repeats");
remove("nameunic");
if (unrep==1){
	rename("cnfwithrepeats.cnf", "MD4_implication_vars_16.cnf");
	string namehere9 = "CNFwithUnic" + to_string(z) + ".cnf";
	const char *nh9 = namehere9.c_str();
	rename("CNFwithUnic.cnf", nh9);
}
if (unrep==0){
	rename("CNFwithUnic.cnf", "MD4_implication_vars_16.cnf");
	string namehere10 = "cnfwithrepeats" + to_string(z) + ".cnf";
	const char *nh10 = namehere10.c_str();
	rename("cnfwithrepeats.cnf", nh10);
}
   }
MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    	return 0;
}
