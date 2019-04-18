#include "WST.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <iomanip>
//Autor: Karol Br¹galski 235533
WST::WST(std::string nazwa)
{
	std::cout << nazwa;
	std::string a;
	std::ifstream wejscie;
	wejscie.open(nazwa);
	
	std::string firstline;
	wejscie >> firstline;
	wejscie >> numberoftasks;
	tasktime = new int[numberoftasks];
	penaltyfactor = new int[numberoftasks];
	expectedfinishtime = new int[numberoftasks];
	for (int i = 0; i < numberoftasks; i++)
	{
		wejscie >> tasktime[i];
		wejscie >> penaltyfactor[i];
		wejscie >> expectedfinishtime[i];
	}
	wiTiResult = new int*[pow(2, numberoftasks)];
	for (int i = 0; i < pow(2, numberoftasks); i++)
	{
		
		wiTiResult[i] = new int[numberoftasks];
	}
	wiTi = new int[pow(2, numberoftasks)];
	result = new int[numberoftasks];
	starttime = new int[numberoftasks];
	tempresult = new int[numberoftasks];
	wejscie.close();
	
}
void WST::WypiszDane()
{
	std::cout << "NR. Czas wykonania; Wspolczynnik kary; Termin zakonczenia " << std::endl;
	for (int i = 0; i < numberoftasks; i++)
	{
	
		std::cout << std::setw(3) << i+1 << " " << std::setw(10) << tasktime[i] << "     " << std::setw(10) << penaltyfactor[i] << "       " << std::setw(10) << expectedfinishtime[i] << std::endl;
	}
}

void WST::SolveBruteForce()
{
	for (int i = 0; i <numberoftasks; i++)
	{
		tempresult[i] = i;
	}
	int time = 0;
	int temppenalty;
	for (int k = 0; k < numberoftasks; k++)
	{
		result[k] = tempresult[k];
	}
	for (int i = 0; i <numberoftasks; i++)
	{
		int it = tempresult[i];
		starttime[it] = time;
		time += tasktime[it];
	}
	
	resultpenalty = CountPenaltySum();

	while (std::next_permutation(&tempresult[0], &tempresult[numberoftasks]))
	{

		
		time = 0;
		for (int i = 0; i <numberoftasks; i++)
		{
			int it = tempresult[i];
			starttime[it] = time;
			time += tasktime[it];
		}
		temppenalty = CountPenaltySum();
		
		if (temppenalty < resultpenalty)
		{
			
			resultpenalty = temppenalty;
			for (int k = 0; k < numberoftasks; k++)
			{
				result[k] = tempresult[k];
			}
		

		}
	}

}

int WST::CountPenaltySum() 
{
	int Pensum=0;
	for (int i = 0; i < numberoftasks; i++)
	{
		int it = tempresult[i];
		int delay = starttime[it] + tasktime[it] - expectedfinishtime[it];
		if (delay>0)
		{
			Pensum += delay*penaltyfactor[it];

		}
	}
	return Pensum;
}
void WST::PrintResultBrute()
{
	for (int i = 0; i < numberoftasks; i++)
	{
		std::cout << " PATH :" << result[i] +1 << std::endl;

	}
	std::cout << "Kara:  :" << resultpenalty;
}

void WST::SolveDynamic()
{	
	int taskbit=0;
	// obliczenie witi pojedynczych zadan (001,010 itp)
	for (int i = 0; i < numberoftasks; i++)
	{
		wiTi[(int)pow(2, i)] = 0;
		int delay=tasktime[i]-expectedfinishtime[i];
		if (delay > 0)
		{
			wiTi[(int)pow(2,i)] = delay*penaltyfactor[i];
		}
		wiTiResult[(int)pow(2, i)][0] = i;		
	} 
	
	//obliczanie witi kolejnych zadan 
	taskbit=3;
	while (taskbit < pow(2, numberoftasks))
	{				
		int ones=0;
		int tempTaskBit=taskbit;
		int bitTime = 0;
		int it=0;
		//obliczanie czasu i zliczanie jedynek
		while (tempTaskBit > 0)
		{
			if (tempTaskBit %2== 1)
			{				
				bitTime += tasktime[it];
				ones++;
			}
			it++;
			tempTaskBit >>= 1;
		}
		tempTaskBit = taskbit;
		it = 0;
		int it2 = 0;
		int *tempwiti= new int[ones];
		while (tempTaskBit > 0)
		{
			if (tempTaskBit % 2 == 1)
			{
				int delay2 = 0;
				delay2 = bitTime - expectedfinishtime[it];
				tempwiti[it2] = 0;
				
				int negtaskbit = taskbit - pow(2, it); //zapis bitowy z zanegowanym zadaniem ktore wstawiamy na koniec 
				if (delay2 > 0)
				{					
					tempwiti[it2] = delay2*penaltyfactor[it];						
				}		
				
				tempwiti[it2] += wiTi[negtaskbit];
				it2++;
			}
			it++;
			tempTaskBit >>= 1;
		}
		//wybranie zadania najbardzie optymalnego do wstawienia na koniec
		int min = tempwiti[0];
		int index = 0;
		for (int k = 0; k < ones; k++)
		{
			if (tempwiti[k] < min)
			{
				min = tempwiti[k];
				index = k;				
			}
		}

		tempTaskBit = taskbit;
		int it3 = 0;		

		//zapisanie najlepszej sekwencji dla danych zadan(np dla 1101- 032)
		while ((tempTaskBit>0)&&(index>=0)) 
		{			
			if (tempTaskBit%2==1)
			{
				
				if (index == 0)
				{
					for (int o = 0; o < ones-1; o++)
					{
						wiTiResult[taskbit][o] = wiTiResult[(int)(taskbit - pow(2, it3))][o];
						
					}
					wiTiResult[taskbit][ones - 1] = it3;									
				}
				index--;
			}
			tempTaskBit >>= 1; 
			it3++;
		}

		wiTi[taskbit]=min;
		taskbit++;
		//jezeli pojedyncze zadanie (010 itp) przejdz do nastepnego
		if (ceil(log2(taskbit)) == floor(log2(taskbit)))
		{
			taskbit++;
		}
		delete[] tempwiti;		
	}
}
void WST::PrintResultDynamic()
{
	for (int i = 0; i < numberoftasks; i++)
	{
		std::cout << " PATH :" << wiTiResult[(int)pow(2, numberoftasks) - 1][i] + 1 << std::endl;

	}
	std::cout << "Kara:  :" << wiTi[(int)pow(2, numberoftasks) - 1];
}

int WST::CountPenaltySumBB()
{
	int currtime = 0;
	int Pensum = 0;
	for (int i = 0; i < numberoftasks; i++)
	{
		int delay = currtime + tasktime[i] - expectedfinishtime[i];
		currtime += tasktime[i];
		if (delay>0)
		{
			Pensum += delay*penaltyfactor[i];		
		}
	}
	return Pensum;
}

/*oznaczenia dla B&B: 
*-3 wezel dostepny do wyboru
*-2 wezel odrzucony
*-1 wezel wybrany w poprzednim kroku
*0+ wezel wybrany, nr oznacza kolejnosc
*/
void WST::SolveBB() 
{
	int *starttab = new int[numberoftasks];
	tempresultBB = new int[numberoftasks];
	for (int i = 0; i <numberoftasks; i++)
	{
		tempresultBB[i] = i;
		starttab[i] = -3;
	}
	resultpenaltyBB = CountPenaltySumBB();
	
	BBrec(starttab, 0, 0,0);
}
void WST::SolveBBBF()
{
	int *starttab = new int[numberoftasks];
	tempresultBBBF = new int[numberoftasks];
	for (int i = 0; i <numberoftasks; i++)
	{
		tempresultBBBF[i] = i;
		starttab[i] = -3;
	}
	resultpenaltyBB = CountPenaltySumBB();

	BBrecBF(starttab, 0, 0, 0);
}
void WST::BBrecBF(int *usageTab, int currTime, int currPenalty, int it1)
{
	int *queue = new int[numberoftasks];
	int *witiTabBB = new int[numberoftasks];
	int count = 0;
	//tworzenie kolejki wyboru nastepnych wierzcholkow z danego wezla
	for (int i = 0; i < numberoftasks; i++)
	{
		queue[i] = -1;
		
		if (usageTab[i] ==-3)
		{
			queue[count] = i;
			count++;
			if (currTime + tasktime[i] - expectedfinishtime[i] > 0)
			{
				witiTabBB[i] = currPenalty + (currTime + tasktime[i] - expectedfinishtime[i])*penaltyfactor[i];
			}
			else
			{
				witiTabBB[i] = currPenalty;
			}
		
		}	
		if (usageTab[i] == -1)
		{
			usageTab[i] = it1-1;
		}
	}


	int q = 1;
	while ((queue[q] != -1) &&( q < numberoftasks))
	{
		int j = q;
		while ((j > 0) && (witiTabBB[queue[j - 1]] > witiTabBB[queue[j]]))
		{
			int tmp = queue[j];
			queue[j] = queue[j - 1];
			queue[j - 1] = tmp;
			j--;
		}
		q++;
	}

	for (int k = 0; k < numberoftasks; k++)
	{

		int i;
		if (queue[k] != -1)
		{
			i = queue[k];
		}
		else continue;
		if (witiTabBB[i] >= resultpenaltyBB)
		{
			usageTab[i] = -2;
		}else
		{
			if (usageTab[i] == -3)
			{
				if (it1 == numberoftasks - 1)
				{
					usageTab[i] = it1;
					resultpenaltyBB = witiTabBB[i];
					for (int z = 0; z < numberoftasks; z++)
					{
						tempresultBBBF[z] = usageTab[z];
					}		
				}
				else {
					int *usageTab2 = new int[numberoftasks];
					for (int j = 0; j < numberoftasks; j++)
					{
						usageTab2[j] = usageTab[j];
						
					}
					usageTab2[i] = -1;
					BBrecBF(usageTab2, currTime + tasktime[i], witiTabBB[i], it1 + 1);
				}
			}
		}
	}
	delete[] witiTabBB;
	delete[] usageTab;
	delete[] queue;
}
void WST::BBrec(int *usageTab, int currTime, int currPenalty, int it)
{

	int *witiTabBB = new int[numberoftasks];
	for (int i = 0; i < numberoftasks; i++)
	{
		if (usageTab[i] == -3)
		{
			if ((currTime + tasktime[i] - expectedfinishtime[i]) > 0)
			{
				witiTabBB[i] = currPenalty + (currTime + tasktime[i] - expectedfinishtime[i])*penaltyfactor[i];
				
			}
			else
			{
				witiTabBB[i] = currPenalty;
				
			}
		}
		if (usageTab[i] == -1)
		{
			usageTab[i] =it-1;
		}
	}
	for (int i = 0; i < numberoftasks; i++)
	{
		if (usageTab[i] == -3)
		{
		
			if (witiTabBB[i] >= resultpenaltyBB)
			{
				usageTab[i] = -2;
			}
			else
			{
				if (it == numberoftasks - 1)
				{
					usageTab[i] = it;
					resultpenaltyBB = witiTabBB[i];
					for (int z = 0; z < numberoftasks; z++)
					{
						tempresultBB[z] = usageTab[z];
					}
					
				}
				int *usageTab2 = new int[numberoftasks];
				usageTab2[i] = -1;
				for (int j = 0; j < numberoftasks; j++)
				{
					if (usageTab2[j] != -1)
					{
						usageTab2[j] = usageTab[j];
					}
				}
				BBrec(usageTab2, currTime + tasktime[i], witiTabBB[i], it + 1);
			}
		}
	}
	
	delete[] witiTabBB;
	delete[] usageTab;
}

void WST::PrintResultBBBF()
{
	for (int a = 0; a < numberoftasks; a++)
	{
		int k = 0;
		while ((a != tempresultBBBF[k]) && (k<numberoftasks))
		{
			k++;
		}
		std::cout << " PATH :" <<k + 1 << std::endl;
	}
	std::cout << "Kara:  :" << resultpenaltyBB << std::endl;
}
void WST::PrintResultBB()
{
	for (int a = 0; a < numberoftasks; a++)
	{
		int k = 0;
		while ((a != tempresultBB[k]) && (k<numberoftasks))
		{
			k++;
		}
		std::cout << " PATH :" << k + 1 << std::endl;
	}
	std::cout << "Kara:  :" << resultpenaltyBB << std::endl;
}

WST::~WST()
{
	
	delete[] result;
	delete[] tempresult;
	for (int i = 0; i < pow(2, numberoftasks); i++)
    {
		delete[] wiTiResult[i];
	}
	delete[] wiTiResult;
	delete[] wiTi;
	delete[] tasktime;
	delete[] penaltyfactor;
	delete[] expectedfinishtime ;
	delete[] starttime;
}
