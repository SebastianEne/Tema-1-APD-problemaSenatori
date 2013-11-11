#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <omp.h>

using namespace std;


int n, nr_saptamani, parcurse , culori_ramase;
bool vazut[100];
bool disparute[100];
int dist[100];
int nr_vect_colors[100];
int nr_colors;

int max();

int main(int argv, char **argc)	{
  int **color_matrix_1, **color_matrix_2, **aux;
  int i, j, i_prim, j_prim, radius, day;
  int limN, limS, limE, limV, N, E, S,V;
  int contor;
   
  char *fisin, *fisout;
  ifstream fr;
  ofstream fw;
  bool ok = true;
												// Aici incepe codul:
  nr_saptamani = atoi(argc[1]);					//
  fisin = argc[2];								// obtin parametrii cmd
  fisout = argc[3];								//
  
  fr.open(fisin, ios::in);						// Initializez structuri
  fw.open(fisout, ios::out);					// fisiere
  if(!fr.is_open())
    {											//  Verific deschidere
      cout<<"Nu pot deschide fisierul de citire"<<endl; 
      return -1;								//  caz eroare
    }    

  fr>>n;										//  Citesc dimensiune
  fr>>nr_colors;								//  Citesc nr_saptamani

  color_matrix_1 =  new int*[n];				//  Alocare memorie
  color_matrix_2 =  new int*[n];				//  matrice culoare_1 
  
  // Initializare:
  for(i=0; i<nr_colors; i++) 
    {
		vazut[i] = false;
		dist[i] = 0;
		nr_vect_colors[i] = 0;
		disparute[i] = false;
	}
  
  for(i=0; i<n; i++)							//  matrice culoare_2
    {											//
      color_matrix_1[i] = new int[n];			//
      color_matrix_2[i] = new int[n];			//
    }											//
  
  for(i=0; i<n; i++)							// Citesc din fisier
    for(j=0; j<n; j++)							// elementele
	{											//
	color_matrix_2[i][j] = -1;					// initializez buffer 
	fr>>color_matrix_1[i][j];					// matricei culoare
    }
  
  i_prim = 0;
  j_prim = 0;
  aux = color_matrix_2;
  culori_ramase = nr_colors;
  
  	for(day=0; day<nr_saptamani; day++) {
	  	
	  // Copiez adresa buffer-ului din aux
	  // in matricea de culori 2
	  // initial ca sa nu afecteze
	  // functionarea progr
	  // aux are aceeasi adresa cu buffer-ul 2
	  color_matrix_2 = aux;		
	  aux = color_matrix_1;	
	  
	#pragma omp schedule(dynamic) parallel  for private(contor,j,i_prim,j_prim, ok,radius,N,V,E,S,limN,limS,limE,limV,parcurse,vazut,dist,culori_ramase) 							// DISPARUTE ARE ALTA VALOARE PT FIECARE THREAD IN PARTE SI RAMANE ACLEASI			
	  for(i=0; i<n; i++)
		for(j=0; j<n; j++)
		  {
			  
			radius = 1;																				// Copiez adresa buffer_1 in aux																//
			while(ok == true) 
			{
				N = i - radius;																		// Partea de N:
				V = j - radius;
				E = j + radius;
				S = i + radius;
				
				if(N < 0) limN = 0;
					 else limN=N;
				if(V < 0) limV = 0;
					 else limV=V;
				if(E > n - 1) limE = n - 1;
					 else limE=E;
				if(S > n - 1) limS = n - 1;
					 else limS=S;


				if(N>=0)
				for(j_prim = limV; j_prim <= limE; j_prim++) 
				
					if((vazut[color_matrix_1[limN][j_prim]] == false) && (disparute[color_matrix_1[limN][j_prim]] == false)) {	
						vazut[color_matrix_1[limN][j_prim]] = true;
						dist[color_matrix_1[limN][j_prim]] = radius;
						parcurse++;
					}	
				
														
				if(E < n) 		
				for(i_prim = limN; i_prim <= limS; i_prim++) 																										
					if((vazut[color_matrix_1[i_prim][limE]] == false) && (disparute[color_matrix_1[i_prim][limE]] == false)) {
						vazut[color_matrix_1[i_prim][limE]] = true;
						dist[color_matrix_1[i_prim][limE]] = radius;
						parcurse++;
					}	
				
				
				if(S < n)		
				for(j_prim = limE; j_prim >= limV; j_prim--) 			
					if((vazut[color_matrix_1[limS][j_prim]] == false) && (disparute[color_matrix_1[limS][j_prim]] == false)) {
						vazut[color_matrix_1[limS][j_prim]] = true;
						dist[color_matrix_1[limS][j_prim]] = radius;
						parcurse++;
					}
				
				if(V >=0)
				for(i_prim = limS; i_prim >= limN; i_prim--) 		
					if((vazut[color_matrix_1[i_prim][limV]] == false) && (disparute[color_matrix_1[i_prim][limV]] == false)) {
						vazut[color_matrix_1[i_prim][limV]] = true;
						dist[color_matrix_1[i_prim][limV]] = radius;
						parcurse++;
					}	
																								
				#pragma omp critical
				{				
				if(culori_ramase == parcurse) {										// Verific daca  
					int q;
					int max = -1, pos = 0;
		
					for(q=0; q < nr_colors; q++) 
						if(dist[q] > max) {
							max = dist[q];
							pos = q;
						}
					color_matrix_2[i][j] = pos;									    // tabelul de culori	
					nr_vect_colors[color_matrix_2[i][j]]++;							//							
					ok = false;														// e full: ( Incrementez indicele de culoare din tabela)	
				}																	//
				else {																// Altfel:														//
					radius++;														// cresc raza de cautare cu o unitate
					
					if(radius > n) {			
					int q;
					int max = -1, pos = 0;
		
					for(q=0; q < nr_colors; q++) 
						if(dist[q] > max) {
							max = dist[q];
							pos = q;
						}						
						color_matrix_2[i][j] = pos;		    					//??? intorc prima culoare daca e singurul element?			
						nr_vect_colors[color_matrix_2[i][j]]++;						//	Verific daca raza a depasit dimensiunea
						ok = false;	
						}															//  tabelei:
				}																	//
			  }																		//
			}
					
			for(int q=0; q<nr_colors; q++) 
			{
				vazut[q] = false;
				dist[q] = 0;
			}	
			  ok = true;	 		  		
	  } 
          
          for(contor=0; contor< nr_colors; contor++) {						    // Scrierea in fisier
		     fw<<nr_vect_colors[contor]<<" ";									//
		     
		     if(nr_vect_colors[contor] == 0) {
					disparute[contor] = true;
					culori_ramase--;				
			 }
			 
			 nr_vect_colors[contor] = 0;										//
			 dist[contor] = 0;													//
		  }																		//
		  fw<<endl;															    //
		  color_matrix_1 = color_matrix_2; 										//	Interschimbare buffere de memorie				 
	}
	
  // Afisare matrice:	
  for(i=0; i<n; i++)
    for(j=0; j<n; j++) {
		if(j == n - 1) fw<<color_matrix_2[i][j]<<endl;
		else 		   fw<<color_matrix_1[i][j]<<" ";
    }
    
  // Scot din memorie matricile:
  // delete color_matrix_1;
   //delete color_matrix_2;
  
  // Inchid fisierele:  
  fr.close();
  fw.close();
	  
  return 0;
}

// Returneaza maximul culorii:
int max() {
	int q;
	int max = -1, pos;
	
	for(q=0; q < nr_colors; q++) {
		if(dist[q] > max) {
			max = dist[q];
			pos = q;
		}
	}
	
	return pos;	 
}

