#include <windows.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "PSOAlgorithm.h"
//#include "GantryControl.h"
//#include "ErrorProcessing.h"

//ICLi0821
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

// Random numbers function
float myRand() {
	srand(rand() ^ time(NULL));
	return float(rand()) / RAND_MAX;
}

// Objective function
double PSO::sphere(float x[4]) {
	// Testing objective function
	return sqrt(pow(x[0], 2) + pow(x[1], 2) + pow(x[2], 2) + pow(x[3], 2)); //ICLi
	//return PSO.ServoCost;
	// Real objective function 
	// homeCmd();
	// runCmd(x[0], x[1], x[2]);

	//return FixAverage(x);
}
//int PSO::sphere(){
//	fstream finC;
//	finC.open("Cost.csv", ios::in);
//	int i = 0;
//	string line;
//	string field;
//	int servocost;
//
//	if (!finC)
//	{
//		cout << "開啟檔案失敗！" << endl;
//		exit(1);
//	}
//	while (getline(finC, line)) //getline(inFile, line)表示按行讀取CSV檔案中的資料
//	{
//		istringstream sin(line); //將整行字串line讀入到字串流sin中
//		getline(sin, field, ','); //將字串流sin中的字元讀入到field字串中，以逗號為分隔符 
//		cout << field.c_str() <<endl;
//		servocost = atoi(field.c_str());	//將剛剛讀取的字串轉換成int
//	}
//	finC.close();
//	return servocost;
//}

// PSO Main loop
psoSol PSO::mainLoop() {
	for (int it = 0; it < maxIt; it++) {
		cout << "\tGeneration: " << it << endl;
		for (int i = 0; i < nPop; i++) {

			// Update velocity
			swarm[i].Velocity[0] = w * swarm[i].Velocity[0] + c1 * myRand() * (swarm[i].pBest.Position[0] - swarm[i].Position[0])
				+ c2 * myRand() * (gBest.Position[0] - swarm[i].Position[0]);
			swarm[i].Velocity[1] = w * swarm[i].Velocity[1] + c1 * myRand() * (swarm[i].pBest.Position[1] - swarm[i].Position[1])
				+ c2 * myRand() * (gBest.Position[1] - swarm[i].Position[1]);
			swarm[i].Velocity[2] = w * swarm[i].Velocity[2] + c1 * myRand() * (swarm[i].pBest.Position[2] - swarm[i].Position[2])
				+ c2 * myRand() * (gBest.Position[2] - swarm[i].Position[2]);
			swarm[i].Velocity[3] = w * swarm[i].Velocity[3] + c1 * myRand() * (swarm[i].pBest.Position[3] - swarm[i].Position[3])
				+ c2 * myRand() * (gBest.Position[3] - swarm[i].Position[3]);

			// Apply velocity limits
			swarm[i].Velocity[0] = max(swarm[i].Velocity[0], Pn100Vmin);
			swarm[i].Velocity[0] = min(swarm[i].Velocity[0], Pn100Vmax);
			swarm[i].Velocity[1] = max(swarm[i].Velocity[1], Pn101Vmin);
			swarm[i].Velocity[1] = min(swarm[i].Velocity[1], Pn101Vmax);
			swarm[i].Velocity[2] = max(swarm[i].Velocity[2], Pn102Vmin);
			swarm[i].Velocity[2] = min(swarm[i].Velocity[2], Pn102Vmax);
			swarm[i].Velocity[3] = max(swarm[i].Velocity[2], Pn401Vmin);
			swarm[i].Velocity[3] = min(swarm[i].Velocity[2], Pn401Vmax);

			// Update position
			swarm[i].Position[0] = swarm[i].Position[0] + swarm[i].Velocity[0];
			swarm[i].Position[1] = swarm[i].Position[1] + swarm[i].Velocity[1];
			swarm[i].Position[2] = swarm[i].Position[2] + swarm[i].Velocity[2];
			swarm[i].Position[3] = swarm[i].Position[3] + swarm[i].Velocity[3];

			// Apply position limits
			swarm[i].Position[0] = max(swarm[i].Position[0], Pn100min);
			swarm[i].Position[0] = min(swarm[i].Position[0], Pn100max);
			swarm[i].Position[1] = max(swarm[i].Position[1], Pn101min);
			swarm[i].Position[1] = min(swarm[i].Position[1], Pn101max);
			swarm[i].Position[2] = max(swarm[i].Position[2], Pn102min);
			swarm[i].Position[2] = min(swarm[i].Position[2], Pn102max);
			swarm[i].Position[3] = max(swarm[i].Position[3], Pn401min);
			swarm[i].Position[3] = min(swarm[i].Position[3], Pn401max);

			// Evaluation
			cout << "\t\tEvaluating particle # " << i << ". Please wait" << endl;
			cout << "\t\tServo Gain Parameters. Pn100: " << swarm[i].Position[0] << 
				", Pn101: " << swarm[i].Position[1] << ", Pn102: " << swarm[i].Position[2] <<", Pn401: " << swarm[i].Position[3] << endl; //ICLi
			swarm[i].Cost = sphere(swarm[i].Position);
			//swarm[i].Cost = sphere();
			
			cout << "\t\tCost: " << swarm[i].Cost << endl;

			// Update personal best
			if (swarm[i].Cost < swarm[i].pBest.Cost) {
				swarm[i].pBest.Position[0] = swarm[i].Position[0];
				swarm[i].pBest.Position[1] = swarm[i].Position[1];
				swarm[i].pBest.Position[2] = swarm[i].Position[2];
				swarm[i].pBest.Position[3] = swarm[i].Position[3];
				swarm[i].pBest.Cost = swarm[i].Cost;

				// Update global best
				if (swarm[i].pBest.Cost < gBest.Cost) {
					cout << "PB: " << swarm[i].pBest.Cost << " - GB: " << gBest.Cost;
					gBest = swarm[i].pBest;
				}
			}

			// Save data
			PSOData[0][cont] = it + 1; // # generation
			PSOData[1][cont] = i + 1; // # particle
			PSOData[2][cont] = swarm[i].Position[0];
			PSOData[3][cont] = swarm[i].Position[1];
			PSOData[4][cont] = swarm[i].Position[2];
			PSOData[5][cont] = swarm[i].Position[3]; //ICLi
			PSOData[6][cont] = swarm[i].Cost;
			PSOData[7][cont] = swarm[i].Velocity[0];
			PSOData[8][cont] = swarm[i].Velocity[1];
			PSOData[9][cont] = swarm[i].Velocity[2];
			PSOData[10][cont] = swarm[i].Velocity[3]; //ICLi
			PSOData[11][cont] = swarm[i].pBest.Position[0];
			PSOData[12][cont] = swarm[i].pBest.Position[1];
			PSOData[13][cont] = swarm[i].pBest.Position[2];
			PSOData[14][cont] = swarm[i].pBest.Position[3]; //ICLi
			PSOData[15][cont] = swarm[i].pBest.Cost;
			PSOData[16][cont] = gBest.Position[0];
			PSOData[17][cont] = gBest.Position[1];
			PSOData[18][cont] = gBest.Position[2];
			PSOData[19][cont] = gBest.Position[3]; //ICLi
			PSOData[20][cont] = gBest.Cost;
			cont++;
		}

		w = w * wdamp;
		bSol.push_back(gBest);
		cout << "\tGeneration: " << it << ": Best Cost = " << bSol[it+1].Cost << endl;
		cout << "\tBest Parameters. Pn100: " << bSol[it+1].Position[0] << " Pn101: " << bSol[it+1].Position[1] <<
			" Pn102: " << bSol[it+1].Position[2] << " Pn401: " << bSol[it+1].Position[3] << endl; //ICLi
		cout << "------------------------------" << endl;

		//getchar();
	}

	//closeCard();

	return gBest;
}

// Constructor
PSO::PSO(psoParam input) {
	// Initialize Card
	//initCard();

	cont = 0;

	// Constriction coefficients
	int kappa = 1;
	float phi1 = 2.05, phi2 = 2.05, Vctrl = 0.3;
	float phi = phi1 + phi2;
	float chi = 2 * kappa / abs(2 - phi - sqrt(pow(phi, 2) - 4 * phi));

	varSize = 4; //4 parameters for servo drive
	w = chi;
	wdamp = 1;
	c1 = chi * phi1;
	c2 = chi * phi2;

	maxIt = input.MaxIt;
	nPop = input.nPop;
	Pn100max = input.Pn100max;
	Pn100min = input.Pn100min;
	Pn101max = input.Pn101max;
	Pn101min = input.Pn101min;
	Pn102max = input.Pn102max;
	Pn102min = input.Pn102min;
	Pn401max = input.Pn401max;
	Pn401min = input.Pn401min;

	Pn100Vmax = Vctrl * (Pn100max - Pn100min);
	Pn101Vmax = Vctrl * (Pn101max - Pn101min);
	Pn102Vmax = Vctrl * (Pn102max - Pn102min);
	Pn401Vmax = Vctrl * (Pn401max - Pn401min);

	/*PVmax = 10;
	IVmax = 10;
	DVmax = 10;*/

	Pn100Vmin = -Pn100Vmax;	
	Pn101Vmin = -Pn101Vmax;
	Pn102Vmin = -Pn102Vmax;
	Pn401Vmin = -Pn401Vmax;

	gBest.Cost = 9999999999;
	//gBest.Cost = input.ServoCost;
	//gBest.Cost = input.InitCost;
	cout << gBest.Cost << endl;
	cout << "Creating intial population. # Particles to be initialized: " << nPop << endl;
	for (int i = 0; i < nPop; i++) {
		cout << "\tParticle: " << i << endl;
		swarm.push_back(psoPart());
		
		
		// Initialize position
		swarm[i].Position[0] = Pn100min + myRand() * (Pn100max - Pn100min);
		swarm[i].Position[1] = Pn101min + myRand() * (Pn101max - Pn101min);
		swarm[i].Position[2] = Pn102min + myRand() * (Pn102max - Pn102min);
		swarm[i].Position[3] = Pn401min + myRand() * (Pn401max - Pn401min);


		
		// Initialize position
	/*	if(i == 0){
			swarm[i].Position[0] = ServoGain[0];
			swarm[i].Position[1] = ServoGain[1];
			swarm[i].Position[2] = ServoGain[2];
			swarm[i].Position[3] = ServoGain[3];
		}*/

		
		//Initialize position
		if(i == 0){
		swarm[i].Position[0] = input.Pn100;
		swarm[i].Position[1] = input.Pn101;
		swarm[i].Position[2] = input.Pn102;
		swarm[i].Position[3] = input.Pn401;
		}

		swarm[i].Velocity[0] = 0;
		swarm[i].Velocity[1] = 0;
		swarm[i].Velocity[2] = 0;
		swarm[i].Velocity[3] = 0;

		// Evaluation
		cout << "\t\tEvaluating particle # " << i << ". Please wait" << endl;
		cout << "\t\tServo Gain Parameters. Pn100: " << swarm[i].Position[0] <<
			", Pn101: " << swarm[i].Position[1] << ", Pn102: " << swarm[i].Position[2] << " Pn401: " << swarm[i].Position[3] << endl;
		swarm[i].Cost = sphere(swarm[i].Position);
		//swarm[i].Cost = sphere();
		cout << "\t\tCost: " << swarm[i].Cost << endl;

		// Update Personal Best
		swarm[i].pBest.Position[0] = swarm[i].Position[0];
		swarm[i].pBest.Position[1] = swarm[i].Position[1];
		swarm[i].pBest.Position[2] = swarm[i].Position[2];
		swarm[i].pBest.Position[3] = swarm[i].Position[3];

		swarm[i].pBest.Cost = swarm[i].Cost;
		//if(i == 0)//Roxas
		//	swarm[i].pBest.Cost = input.InitCost;

		// Update Global Best
		if (swarm[i].pBest.Cost < gBest.Cost) {
			gBest.Position[0] = swarm[i].pBest.Position[0];
			gBest.Position[1] = swarm[i].pBest.Position[1];
			gBest.Position[2] = swarm[i].pBest.Position[2];
			gBest.Position[3] = swarm[i].pBest.Position[3];
			gBest.Cost = swarm[i].pBest.Cost;
		}

		// Save data
		PSOData[0][cont] = 0; // # generation
		PSOData[1][cont] = i + 1; // # particle
		PSOData[2][cont] = swarm[i].Position[0];
		PSOData[3][cont] = swarm[i].Position[1];
		PSOData[4][cont] = swarm[i].Position[2];
		PSOData[5][cont] = swarm[i].Position[3]; //ICLi
		PSOData[6][cont] = swarm[i].Cost;
		PSOData[7][cont] = swarm[i].Velocity[0];
		PSOData[8][cont] = swarm[i].Velocity[1];
		PSOData[9][cont] = swarm[i].Velocity[2];
		PSOData[10][cont] = swarm[i].Velocity[3]; //ICLi
		PSOData[11][cont] = swarm[i].pBest.Position[0];
		PSOData[12][cont] = swarm[i].pBest.Position[1];
		PSOData[13][cont] = swarm[i].pBest.Position[2];
		PSOData[14][cont] = swarm[i].pBest.Position[3]; //ICLi
		PSOData[15][cont] = swarm[i].pBest.Cost;
		PSOData[16][cont] = gBest.Position[0];
		PSOData[17][cont] = gBest.Position[1];
		PSOData[18][cont] = gBest.Position[2];
		PSOData[19][cont] = gBest.Position[3]; //ICLi
		PSOData[20][cont] = gBest.Cost;
		cont++;

	}
			
	bSol.push_back(gBest);
	cout << "\tGeneration: " << "0" << ": Best Cost = " << bSol[0].Cost << endl;
	cout << "\tBest Parameters. Pn100: " << bSol[0].Position[0] << " Pn101: " << bSol[0].Position[1] <<
		" Pn102: " << bSol[0].Position[2] << " Pn401: " << bSol[0].Position[3] << endl; //ICLi
	cout << "------------------------------" << endl;
			
}

// PSO Main Function
psoSol PSO::run() {
	cout << "\nStarting Particle-Swarm-based optimization process." << endl;
	cout << "# Generations: " << maxIt << ", # Particles: " << nPop << endl;
	cout << "------------------------------" << endl;

	psoSol out = mainLoop();

	cout << "Process finished: Best cost: " << out.Cost << endl;
	cout << "Best parameters. Pn100: " << out.Position[0] << ", Pn101: " << out.Position[1]
		<< ", Pn102: " << out.Position[2] << " Pn401: " << out.Position[3] << endl; //ICLi

	return out;
}

// Save important data from PSO
int PSO::saveData(string filenameBest, string filenameData, string filenameTheBest) {
	ofstream fileBest, fileData, fileTheBest;
	fileBest.open(filenameBest);
	fileBest << "Pn100,Pn101,Pn102,Pn401,Cost,\n";
	int lim = bSol.size();


	for (int i = 0; i < lim; i++) {
		fileBest << bSol[i].Position[0] << "," << bSol[i].Position[1] 
		<< "," << bSol[i].Position[2] << "," << bSol[i].Position[3] << "," << bSol[i].Cost << "\n"; //ICLi
	}

	fileBest.close();
	fileTheBest.open(filenameTheBest);//Roxas
	/*fileTheBest<< (int)bSol[lim-1].Position[0] << "," << (int)bSol[lim-1].Position[1] 
		<< "," << (int)bSol[lim-1].Position[2] << "," << (int)bSol[lim-1].Position[3]<<endl;*/
	fileTheBest<< (int)gBest.Position[0] << "," << (int)gBest.Position[1] 
		<< "," << (int)gBest.Position[2] << "," << (int)gBest.Position[3] << "," << (int)gBest.Cost <<endl;
	fileTheBest.close();

	fileData.open(filenameData);
	fileData << "it,par,PosPn100,PosPn101,PosPn102,PsoPn401,Cost,VelPn100,VelPn101,VelPn102,VelPn401,PBPosPn100,PBPosPn101,PBPosPn102,PBPosPn401,PBCost,GBPosPn100,GBPosPn101,GBPosPn102,GBPosPn401,GPCost,\n"; //ICLi

	for(int i = 0; i < cont; i++){
		fileData << PSOData[0][i] << "," << PSOData[1][i] << "," << PSOData[2][i] << "," <<
			PSOData[3][i] << "," << PSOData[4][i] << "," << PSOData[5][i] << "," <<
			PSOData[6][i] << "," << PSOData[7][i] << "," << PSOData[8][i] << "," <<
			PSOData[9][i] << "," << PSOData[10][i] << "," << PSOData[11][i] << "," <<
			PSOData[12][i] << "," << PSOData[13][i] << "," << PSOData[14][i] << "," <<
			PSOData[15][i] << "," << PSOData[16][i] << "," << PSOData[17][i] << "," << 
			PSOData[18][i] << "," << PSOData[19][i] << "," << PSOData[20][i] << "," <<",\n"; //ICLi
	}

	fileData.close();

	return 0;
}

// Read data for PSO  //ICLi0820
//int PSO::readData(string filenameTheBest, string filenameCost) {
//	// File pointer 
//	fstream finG, finC;
//
//	// Open an existing file 
//	finG.open(filenameTheBest, ios::in);
//
//	//ifstream inFile("Data.csv", ios::in);
//	if (!finG)
//	{
//		cout << "開啟檔案失敗！" << endl;
//		exit(1);
//	}
//	int i = 0;
//	string line;
//	string field;
//	while (getline(finG, line)) //getline(inFile, line)表示按行讀取CSV檔案中的資料
//	{
//		string field;
//		istringstream sin(line); //將整行字串line讀入到字串流sin中
//		if (i == 0) {
//		for(int n = 0; n < 4; n++){
//			getline(sin, field, ','); //將字串流sin中的字元讀入到field字串中，以逗號為分隔符 
//			cout << field.c_str() <<endl;
//			ServoGain[n] = atoi(field.c_str());	//將剛剛讀取的字串轉換成int
//		}
//		}
//		i++;
//	}
//	finG.close();
//
//	finC.open(filenameCost, ios::in);
//
//	if (!finC)
//	{
//		cout << "開啟檔案失敗！" << endl;
//		exit(1);
//	}
//	while (getline(finC, line)) //getline(inFile, line)表示按行讀取CSV檔案中的資料
//	{
//		istringstream sin(line); //將整行字串line讀入到字串流sin中
//		getline(sin, field, ','); //將字串流sin中的字元讀入到field字串中，以逗號為分隔符 
//		cout << field.c_str() <<endl;
//		ServoCost = atoi(field.c_str());	//將剛剛讀取的字串轉換成int
//	}
//	finC.close();
//
//	cout << "共讀取了：" << i << "行" << endl;
//	cout << ServoGain[0] << "," << ServoGain[1] << "," << ServoGain[2] << "," << ServoGain[3] << "," << ServoCost << endl;
//	cout << "讀取資料完成" << endl;
//}

PSO::~PSO(){}