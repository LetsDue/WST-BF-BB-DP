#pragma once
#include <string>
//Autor: Karol Br¹galski 235533
class WST 
{
public:
	int numberoftasks;
	int *starttime;
	int *tasktime;
	int *penaltyfactor;
	int *expectedfinishtime;
	
	int *result;
	int *tempresult;
	int resultpenalty;
	int **wiTiResult;
	int *wiTi;

	int *tempresultBB;
	int *tempresultBBBF;
	int resultpenaltyBB;


	WST(std::string nazwa);
	void WypiszDane();
	void SolveBruteForce();
	void PrintResultBrute();
	int CountPenaltySum();
	
	void SolveDynamic();
	void PrintResultDynamic();

	void SolveBB();
	void SolveBBBF();
	void BBrec(int *usageTab, int currTime, int currPenalty, int it);
	void BBrecBF(int *usageTab, int currTime, int currPenalty, int it);
	void PrintResultBBBF();
	void PrintResultBB();
	int CountPenaltySumBB();

	~WST();
};