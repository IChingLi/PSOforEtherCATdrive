// PSO.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include "PSOAlgorithm.h"

#include <windows.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>
#include <ctime>

//ICLi
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

int servogain[5]; //Pn100,Pn101,Pn102,Pn401,cost
int servocost;
//int ReadData;

void ReadData(string filenameTheBest, string filenameCost) {
	// File pointer 
	fstream finG, finC;

	// Open an existing file 
	finG.open(filenameTheBest, ios::in);

	//ifstream inFile("Data.csv", ios::in);
	if (!finG)
	{
		cout << "開啟檔案失敗！" << endl;
		exit(1);
	}
	int i = 0;
	string line;
	string field;
	while (getline(finG, line)) //getline(inFile, line)表示按行讀取CSV檔案中的資料
	{
		string field;
		istringstream sin(line); //將整行字串line讀入到字串流sin中
		if (i == 0) {
		for(int n = 0; n < 5; n++){
			getline(sin, field, ','); //將字串流sin中的字元讀入到field字串中，以逗號為分隔符 
			cout << field.c_str() <<endl;
			servogain[n] = atoi(field.c_str());	//將剛剛讀取的字串轉換成int
		}
		}
		i++;
	}
	finG.close();

	//finC.open(filenameCost, ios::in);

	//if (!finC)
	//{
	//	cout << "開啟檔案失敗！" << endl;
	//	exit(1);
	//}
	//while (getline(finC, line)) //getline(inFile, line)表示按行讀取CSV檔案中的資料
	//{
	//	istringstream sin(line); //將整行字串line讀入到字串流sin中
	//	getline(sin, field, ','); //將字串流sin中的字元讀入到field字串中，以逗號為分隔符 
	//	cout << field.c_str() <<endl;
	//	servocost = atoi(field.c_str());	//將剛剛讀取的字串轉換成int
	//}
	//finC.close();

	cout << "共讀取了：" << i << "行" << endl;
	//cout << ServoGain[0] << "," << ServoGain[1] << "," << ServoGain[2] << "," << ServoGain[3] << "," << ServoCost << endl;
	cout << "讀取資料完成" << endl;
}

int main()
{	
	psoParam input;
	psoSol output;
	string fileBest = "EvolData.csv";
	string fileData = "PartData.csv";
	//string filenameGain = "InputData.csv";	//ICLi
	string filenameCost = "Cost.csv";	//ICLi
	string filenameTheBest = "TheBest.csv";//Roxas

	/*
	Pn100: speed loop gain (default:400)
	Pn101: speed loop integral time constant (default:2000)
	Pn102: position loop gain (default:400)
	Pn401: first dtage first torque reference filter time constant (default:100)
	*/
	ReadData(filenameTheBest, filenameCost);
	
	input.Pn100 = servogain[0];
	input.Pn101 = servogain[1];
	input.Pn102 = servogain[2];
	input.Pn401 = servogain[3];
	input.InitCost = servogain[4];
	
	// PSO parameters
	input.Pn100min = 10;
	input.Pn101min = 15;
	input.Pn102min = 10;
	input.Pn401min = 0;

	input.Pn100max = 20000;
	input.Pn101max = 51200;
	input.Pn102max = 20000;
	input.Pn401max = 65535;

	input.MaxIt =1;
	input.nPop = 20;

	PSO pso(input);
	//pso.PreadData(filenameTheBest, filenameCost); //ICLi
	output = pso.run();
	pso.saveData(fileBest, fileData, filenameTheBest);


	cout << "Routine finished..." << endl;
	system("PAUSE");
}

