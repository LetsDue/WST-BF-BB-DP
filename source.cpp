#include <iostream>
#include <windows.h>

#include "WST.h"
//Autor: Karol Br¹galski 235533
using namespace std;
double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
}
double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - CounterStart) / PCFreq;
}


int main()
{
	double czas;
	WST *tes = NULL;
	char ster = 'x';
	while (ster != '0')
	{

		//system("cls");
		cout << endl;
		cout << "1. Wczytaj z pliku." << endl;
		cout << "2. Wyswietlanie aktualnych danych." << endl;
		cout << "3. Rozwiaz metada przekladu calkowitego" << endl;
		cout << "4. Wypisz wynik metody przegladu zupelnego" << endl;
		cout << "5. Rozwiaz metoda programowania dynamicznego" << endl;
		cout << "6. Wypisz wynik metody programowania dynamicznego" << endl;
		cout << "7. B&B" << endl;
		cout << "8. Wypisz wynik B&B" << endl;
		cout << "9. B&B BF" << endl;
		cout << "A. Wypisz wynik B&B BF" << endl;
		cin >> ster;
		string a = "data10.txt";
		switch(ster)
		{
		case '1':
			system("cls");
			std::cout << "Podaj nazwe pliku: ";
			std::cin >> a;
			
			if (tes != NULL) delete tes;
			tes=new WST(a);
			break;
		case '2':
			system("cls");
			if (tes != NULL)
			{
				tes->WypiszDane();
			}
			else
			{
				std::cout << "Brak zaladowanych danych"<<std::endl;
			}
			break;
		case '3':
			system("cls");
			StartCounter();
			tes->SolveBruteForce();
			czas = GetCounter();
			std::cout << "Czas operacji:" << czas << std:: endl;
			break;
		case '4':
			system("cls");
			tes->PrintResultBrute();
			break;
		case '5':
			system("cls");
			StartCounter();
			tes->SolveDynamic();
			czas = GetCounter();
			std::cout << "Czas operacji:" << czas << std::endl;
			break;
		case '6' :
			system("cls");
			tes->PrintResultDynamic();
			break;
		case '7':
			system("cls");
			StartCounter();
			tes->SolveBB();
			czas = GetCounter();
			std::cout << "Czas operacji:" << czas << std::endl;
			break;
		case '8':
			system("cls");
			tes->PrintResultBB();
			break;
		case '9':
			system("cls");
			StartCounter();
			tes->SolveBBBF();
			czas = GetCounter();
			std::cout << "Czas operacji:" << czas << std::endl;
			break;
		case 'A':
			system("cls");
			tes->PrintResultBBBF();
			break;
		}

	}




	return 0;
}