#pragma once
#include <vector>


class EGA
{
	vector<vector<int>> AdaptationMatrix;
	vector<vector<int>> Parents;
	vector<vector<int>> Children;
	vector<vector<int>> Mutants;
	int mNu;
	int mL;
	double mDeltaMu;

	vector<int> RandomCode(int L);
	vector<vector<int>> SummArr(vector<vector<int>> Arr1, vector<vector<int>> Arr2);
	double DeltaMu(vector<vector<int>> mp, vector<vector<int>> AdaptationMatrix);
	int Mu(vector<vector<int>> AdaptationMatrix, vector<int> Arr);
	vector<int> MutOneDot(int L);
	vector<int> MutTwoDot(int L);
	vector<int> RandomMutant();
	vector<int> CopyCycle(vector<int> Sp, int k, vector<int> C, vector<int> Sc);
	vector<int> CrossCycle(vector<int> Sp1, vector<int> Sp2);
	bool AssociatePlus(double Mu1, double Mu2, double DeltaMu);
	bool AssociateMinus(double Mu1, double Mu2, double DeltaMu);
	vector<vector<int>> NotVozr(vector<vector<int>> Reprod);
	vector<vector<int>> LineRangeSel(vector<vector<int>> Reprod, int Nu);
	vector<vector<int>> BetaTournSel(vector<vector<int>> Reprod, int Nu);

protected:
	void Crossing(int CrossoverType);
	void Mutation(int MutationType);
	void Selection(int SelectingType);
	string TakeAnswer();
	
public:
	int sum;
	static int min;// = 0;
	static int max;// = 0;

	EGA();
	~EGA();
	EGA(vector< vector<int> > OriginalAdaptation, int Nu);

};