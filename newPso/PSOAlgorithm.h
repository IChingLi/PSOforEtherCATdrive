#pragma once

#include <vector>

using namespace std;

#define RECORD_MAX 1500

struct psoSol {
	float Position[4]; //4 parameters for servo drive
	float Cost;
};

struct psoParam {
	float Pn100min; //Pmin;
	float Pn100max;
	
	float Pn101min; //Imin;
	float Pn101max;
	
	float Pn102min; //Dmin;
	float Pn102max;

	float Pn401min;
	float Pn401max;

	int MaxIt;
	int nPop;

	int Pn100;
	int Pn101;
	int Pn102;
	int Pn401;
	int InitCost;
};

class PSO {
	private:
	///Variables
		// PSO parameters 
		int maxIt, nPop, varSize, cont;
		float w, wdamp, c1, c2;
		float Pn100max, Pn100min, Pn101max, Pn101min, Pn102max, Pn102min, Pn401max, Pn401min;
		float Pn100Vmax, Pn100Vmin, Pn101Vmax, Pn101Vmin, Pn102Vmax, Pn102Vmin, Pn401Vmax, Pn401Vmin;
		vector<psoSol> bSol;
		double PSOData[21][RECORD_MAX]; //PSOData 17->21
		int ServoGain[4]; //Pn100,Pn101,Pn102,Pn401
		int ServoCost;

		struct psoPart {
			float Position[4]; //4 parameters for servo drive
			float Cost;
			float Velocity[4]; //4 parameters for servo drive
			psoSol pBest;
		};

		vector<psoPart> swarm;
		psoSol gBest;

	///Fucntions
		double sphere(float x[4]); //4 parameters for servo drive
		psoSol mainLoop();
		//void readData(string filenameGain, string filenameCost); //ICLi

	public:
	///Functions
		PSO(psoParam input);
		~PSO();
		psoSol run();
		//int sphere();
		int saveData(string filenameBest, string filenameData, string filnemaTheBest);//Roxas
		//void PreadData(string filenameTheBest, string filenameCost); //ICLi
};