// TEST de VERIFICARE !!!
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argv, char **argc)	{
  int nr_saptamani, n, **color_matrix;
  int i, j;

  char *fisin, *fisout;
  ifstream fr;
  ofstream fw;

							// Aici incepe codul:
  nr_saptamani = atoi(argc[1]);				//
  fisin = argc[2];					// obtin parametrii cmd
  fisout = argc[3];					//
  
  fr.open(fisin, ios::in);				// Initializez structuri
  fw.open(fisout, ios::out);				// fisiere
  if(!fr.is_open())
    {
      cout<<"Nu pot deschide fisierul de citire"<<endl; //  Verific deschidere
      return -1;					//  caz eroare
    }    

  fr>>n;						//  Citesc dimensiune
  fr>>nr_saptamani;					//  Citesc nr_saptamani

  color_matrix = (int**) new int[n];			//  Alocare memorie 
  for(i=0; i<n; i++)					//  matrice culoare
    {							//
      color_matrix[i] = new int[n];			//
    }							//

  for(i=0; i<n; i++)					// Citesc din fisier
    for(j=0; j<n; j++)					// elementele
	fr>>color_matrix[i][j];				// matricei culoare
  
  
  return 0;
}
