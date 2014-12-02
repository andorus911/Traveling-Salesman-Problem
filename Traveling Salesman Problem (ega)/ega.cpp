#include "ega.h"
#include <string>
#include <iostream>
#include <random>
#include <time.h>
using namespace std;

EGA::EGA(vector< vector<int> > rOriginalAdaptation, int rNu)
{
	srand(time(NULL));

	EGA::mL = rOriginalAdaptation.size();
	EGA::mNu = rNu;
	EGA::min = 0;
	EGA::max = 0;
	EGA::mDeltaMu = 0.0;

	for(int i = 0; i < rNu; i++)
	{
		EGA::Parents[i] = RandomCode(mL);
	}

	if(rOriginalAdaptation.size() == rOriginalAdaptation[0].size())
	{
		AdaptationMatrix = rOriginalAdaptation;
	}
	else
	{
		if(rOriginalAdaptation[0].size() == 2)
		{
			for(int i = 0; i < mL; i++)
			{
				for(int j = 0; j < mL; j++)
				{
					if(j < i) 
					{
						EGA::AdaptationMatrix[i][j] = EGA::AdaptationMatrix[j][i];
						continue;
					}
					double a = rOriginalAdaptation[i][0] - rOriginalAdaptation[j][0];
					double b = rOriginalAdaptation[i][1] - rOriginalAdaptation[j][1];
					a = abs(a);
					b = abs(b);
					a *= a;
					b *= b;
					double c = a + b;
					c = sqrt(c);
					AdaptationMatrix[i][j] = (int)c;
				}
			}
		}
		else
		{
			cout << "The object is created incorrectly!\nThe matrix must be square!";
		}
	}
}

vector<int> EGA::RandomCode(int rL)
{
	vector<int> vec;
	bool again;
	random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<int> dist(0, rL - 1); // distribute results between a and b inclusive.
	for(int i = 0; i < rL; i++)
	{
		do
		{
			again = false;
			vec[i] = (int) dist(gen);
			for(int j = 0; j < i; j++)
			{
				if(vec[i] == vec[j]) again = true;
			}
		}
		while(again);
	}
	return vec;
}

vector<vector<int>> EGA::SummArr(vector<vector<int>> rArr1, vector<vector<int>> rArr2)
{
	vector<vector<int>> vec;
	for(int i = 0; i < rArr1.size(); i++)
	{
		vec.push_back(rArr1[i]);
	}
	for(int i = 0; i < rArr2.size(); i++)
	{
		vec.push_back(rArr2[i]);
	}
	return vec;
}

double DeltaMu(vector<vector<int>> rMP, vector<vector<int>> rAdaptationMatrix)
{
	double *pDeltaMu = new double[rMP.size()];
	int m1;
	int m2;
	for(int i = 0; i < rMP.size(); i++)
	{
		for(int j = 0; j < rMP[0].size() - 1; j++)
		{
			m1 = rMP[i][j];
			m2 = rMP[i][j + 1];
			pDeltaMu[i] += rAdaptationMatrix[m1][m2];
		}
	}
	double DeltaMuEnd = 0;
	double DMuMIN = pDeltaMu[0];
	double DMuMAX = 0;
	for(int i = 0; i < rMP.size(); i++)
	{
		if(pDeltaMu[i] > DMuMAX) DMuMAX = pDeltaMu[i];
		if(pDeltaMu[i] < DMuMIN) DMuMIN = pDeltaMu[i];
	}
	DeltaMuEnd = (DMuMAX - DMuMIN) / 2;
	return DeltaMuEnd;
}

int EGA::Mu(vector<vector<int>> rAdaptationMatrix, vector<int> rVec)
{
	int mu = 0;
	for(int i = 0; i < rVec.size(); i++)
	{
		mu += rAdaptationMatrix[rVec[i]][rVec[i + 1]];
	}
	return mu;
}

vector<int> EGA::MutOneDot(int rL)
{
	random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<int> dist(0, rL - 1); // distribute results between a and b inclusive.
	vector<int> Child = RandomMutant();
	int point = dist(gen);
	int point2 = 0;
	uniform_int_distribution<int> dist(0, 99);
	if(point != 0 && point != (rL - 1))
	{
		if(dist(gen) < 50)
		{
			point2 = point - 1;
		}
		else
		{
			point2 = point + 1;
		}
	}
	if(point == 0)
	{
		point2 = 1;
	}
	if(point == (rL - 1))
	{
		point2 = (rL - 1) - 1;
	}
	int empty = Child[point];
	Child[point] = Child[point2];
	Child[point2] = empty;
	return Child;
}

vector<int> EGA::MutTwoDot(int rL)
{
	random_device rd;   // non-deterministic generator
    mt19937 gen(rd());  // to seed mersenne twister.
    uniform_int_distribution<int> dist(0, rL - 1); // distribute results between a and b inclusive.
	vector<int> Child = RandomMutant();
	int point;
	int point2;
	do {
		point = (int) dist(gen);
		point2 = (int) dist(gen);
	} while(point == point2);
	int empty = Child[point];
	Child[point] = Child[point2];
	Child[point2] = empty;
	return Child;
}

vector<int> EGA::RandomMutant() //изменена структура!!! —ƒ≈Ћј“№ ¬≈–ќя“Ќќ—“№ ћ”“ј÷»» ¬¬ќƒ»ћџћ ѕј–јћ≈“–ќћ
{
	vector<int> Child = EGA::Children[rand() % EGA::Children.size()];
	if(rand() % 100 < 10)
	{
		return Child;
	}
}

vector<int> EGA::CopyCycle(vector<int> Sp, int k, vector<int> C, vector<int> Sc)
{
	for(int i = 0; i < Sp.size(); i++)
		if(C[i] == k)
			Sc[i] = Sp[i];
	return Sc;
}

vector<int> EGA::CrossCycle(vector<int> Sp1, vector<int> Sp2)
{
	vector<int> C;
	int k = 0;
	vector<int> Sc;
	int j;
	bool End;
	do {
		k++;
		do {
			j = (int) rand() % Sp1.size();
		} while (C[j] != 0);
		while (C[j] == 0) {
			C[j] = k;
			int i = 0;
			while (Sp1[i] != Sp2[j])
				i++;
			j = i;
		}
		Sc = CopyCycle(Sp1, k, C, Sc);
		End = true;
		for(int end = 0; end < Sp1.size(); end++) {
			if(C[end] == 0) 
				End = false;
		}
	} while (!End);
	return Sc;
}

bool EGA::AssociatePlus(double rMu1, double rMu2, double DMu)
{
	if(abs(rMu1 - rMu2) <= DMu * 2)
		return true;
	else
		return false;
}

bool EGA::AssociateMinus(double rMu1, double rMu2, double DMu)
{
	if(abs(rMu1 - rMu2) >= DMu * 2)
		return true;
	else
		return false;
}

vector<vector<int>> EGA::NotVozr(vector<vector<int>> rReprod)
{
	vector<vector<int>> R = rReprod;
    for (int j = 0; j < R.size() - 1; j++)
        for (int i = 0; i < R.size() - 1 - j; i++) {
            vector<int> m1 = R[i];
            vector<int> m2 = R[i + 1];
            if (EGA::Mu(EGA::AdaptationMatrix, m1) > EGA::Mu(EGA::AdaptationMatrix, m2)) {
                R[i] = m2;
                R[i + 1] = m1;
            }
        }
    return R;
}

vector<vector<int>> EGA::LineRangeSel(vector<vector<int>> rReprod, int rNu)
{
	double np = 2 - (rand() % 10 / 10);
    double nm = 2 - np;
    rReprod = NotVozr(rReprod);
    vector<vector<int>> R;
    int k = 0;
    for (int i = 0; i < rReprod.size(); i++)
        if (k < rNu) 
            if ((1 - (rand() % 10 / 10)) < (nm + (np - nm) * (i) / (rReprod.size() - 1))) {
                R[k] = rReprod[i];
                k++;
            }
    return R;
}

vector<vector<int>> EGA::BetaTournSel(vector<vector<int>> rReprod, int rNu)
{
	vector<vector<int>> R;
    int r;
    int r1;
    int r2;
    int r3;
    for (int i = 0; i < rNu; i++) {
        r1 = (int) rand() % rReprod.size();
        do {
            r2 = (int) rand() % rReprod.size();
            r3 = (int) rand() % rReprod.size();
        } while (r2 == r1 || r3 == r1 || r2 == r3);
        if (Mu(AdaptationMatrix, rReprod[r1]) > Mu(AdaptationMatrix, rReprod[r2]))
            if (Mu(AdaptationMatrix, rReprod[r2]) > Mu(AdaptationMatrix, rReprod[r3]))
                r = r3;
            else r = r2;
        else
            if (Mu(AdaptationMatrix, rReprod[r1]) > Mu(AdaptationMatrix, rReprod[r3]))
                r = r3;
            else r = r1;
        R[i] = rReprod[r];
    }
    return R;
}

void EGA::Crossing(int rType)
{
	mDeltaMu = DeltaMu(Parents, AdaptationMatrix);
	//this.Children = new int[this.nu][this.L];
	for(int i = 0; i < mNu;) {
		int n1 = (int) rand() % mNu;
		int n2;
		
		do {
			n2 = (int) rand() % mNu;
		} while(n1 == n2);
		
		int N1 = Mu(AdaptationMatrix, Parents[n1]);
		int N2 = Mu(AdaptationMatrix, Parents[n2]);
		
		switch(rType) {
		case 1: 
			if(AssociatePlus(N1, N2, mDeltaMu)) {
				Children[i] = CrossCycle(Parents[n1], Parents[n2]);
				i++;
			}
			break;
		case 2:
			if(AssociateMinus(N1, N2, mDeltaMu)) {
				Children[i] = CrossCycle(Parents[n1], Parents[n2]);
				i++;
			}
			break;
		}
	}
	
	/*int k = 0;
	for(int i = 0; i < nu; i++) {
		for(int j = 0; j < nu; j++) {
			if(associatePlus(Mu(AdaptationMatrix, Parents[i]), Mu(AdaptationMatrix, Parents[j]), DeltaMu)) {
                Children[k] = crossCycle(Parents[i], Parents[j]);
                k++;
                Children[k] = crossCycle(Parents[j], Parents[i]);
                k++;
            };
		}
	}*/ //вроде нормально, Ќќ надо выбрать как рожать потомков
}

void EGA::Mutation(int rType)
{ 
	//vector<int> (*pMut)(int);
	switch(rType)
	{
	case 1:
		for(int i = 0; i < mNu; i++)
		{
			Mutants.push_back(MutOneDot(mL));
		}
		break;
	case 2:
		for(int i = 0; i < mNu; i++)
		{
			Mutants.push_back(MutTwoDot(mL));
		}
		break;

	default:
		return;
	}
}

void EGA::Selection(int rType)
{
	vector<vector<int>> R = SummArr(Mutants, Children);
	//можно обойтись без этого массива и сохранить пам€ть
	//возможны проблемы с созданием массива
	R = SummArr(R, Parents);
	
	switch(rType) {
	case 1: R = LineRangeSel(R, mNu);
	break;
	case 2: R = BetaTournSel(R, mNu);
	break;
	default: 
	}
	Parents = R;
}

string EGA::TakeAnswer()
{
	if(min == 0) min = Mu(AdaptationMatrix, Parents[Parents.size() - 1]);
	if(min > Mu(AdaptationMatrix, Parents[Parents.size() - 1])) min = Mu(AdaptationMatrix, Parents[Parents.size() - 1]);
	if(max < Mu(AdaptationMatrix, Parents[Parents.size() - 1])) max = Mu(AdaptationMatrix, Parents[Parents.size() - 1]);
	Parents = NotVozr(Parents);
	string answer = "The best way:";
	for(int y: Parents[Parents.size() - 1]) {
		answer += " " + y;
	}
	answer += ". Mu = " + Mu(AdaptationMatrix, Parents[Parents.size() - 1]);
	answer += ".\n";
	sum = Mu(AdaptationMatrix, Parents[Parents.size() - 1]);
	return answer;
}