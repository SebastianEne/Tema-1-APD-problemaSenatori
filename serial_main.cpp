#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;


int n, nr_saptamani;
bool validChoice(int i, int j);

int modulo(int val);
int max(int val_1, int val_2);
bool isInMatrix(int i, int j);

struct ColorRecord {
  int color;
  int radius;
  int contor;
  bool valid;
  bool checked;
};
  
class ColorTable {
  public:
		
	ColorTable(int size, ofstream &write) {
		int i;	
		
		nr_col_added = 0;	
		data = new ColorRecord[size];
		dimensiune = size;
		fw = &write;

		for(i=0; i<size; i++) {
                  data[i].color = i;
                  data[i].contor = 0;
                  data[i].checked = false;
				  data[i].valid = true;
		}    
	}

	~ColorTable() {
		delete []data;
	}
	
	// Incrementez culorile sa stiu cate culori 
	// am la sfarsitul
	// saptamanii in noua matrice;
	void incrementColor(int col) {
		data[col].contor++;
	}


	 // Dupa ce am scris noua matrice
	 // interschimb adresele de memorie
	 // a bufferelor de matrici 
	 // si setez numarul de culori din 
	 // noua saptamana pe 0
	void setColorContorsToZero() {
		for(int i=0; i<dimensiune; i++)
			data[i].contor = 0;
	}

	// Bifez culoarea ca fiind marcata in tabela
	// de culori si memorez raza
	void checkColor(int i, int radius) {
		
		if((data[i].checked == false) && (data[i].valid == true) ) {
		    data[i].checked = true;
		    data[i].radius = radius;
		    nr_col_added++;
		}
	} 
	
	// Debifez toate culorile dupa ce
	// termin de scris un element
	// in noua matrice
	void uncheckAllColors() {
		for(int i=0; i<dimensiune; i++)
			data[i].checked = false;
		nr_col_added = 0;	
	}


	// Verific daca tabela de culori e plina:
	bool isFullTable() {
	    if( dimensiune > nr_col_added )
		return false;
		return true;
	}
	
	
	// Intoarce prima culoare de la
	// distanta cea mai mare din tabela:
	int getColor() {
	    int i = 0, pos = 0;
	    int nMax = -99999;
	    
	    for(i = 0; i<dimensiune; i++) {
			if((data[i].radius > nMax) && (data[i].valid == true)) {
				nMax = data[i].radius;
				pos = i;
			}
	    }
	    
	    return pos;
	}
  
	
	// Vad care culoare lipseste:
	void colorMissingAfterWeek() {		
		for(int i=0; i<dimensiune; i++) {
			if(data[i].contor == 0) {
				data[i].valid = false;
			}
		}
	}
	
	
	// Print the color values:
	void printValue() {
		for(int i=0; i<dimensiune; i++) 
			*fw<<data[i].contor<<" ";
	}
	
  private:
	int nr_col_added, dimensiune;
	ColorRecord *data;
	ofstream *fw;
};


int main(int argv, char **argc)	{
  int **color_matrix_1, **color_matrix_2, **aux;
  int nr_colors;
  int i, j, i_prim, j_prim, radius, day;

  char *fisin, *fisout;
  ifstream fr;
  ofstream fw;
  ColorTable *tabel_culori;
  bool ok;
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
  tabel_culori = new ColorTable(nr_colors, fw);	//  Setez tabel de culori
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

	for(day=0; day<nr_saptamani; day++) {
	  	
	  // Copiez adresa buffer-ului din aux
	  // in matricea de culori 2
	  // initial ca sa nu afecteze
	  // functionarea progr
	  // aux are aceeasi adresa cu buffer-ul 2
	  color_matrix_2 = aux;
	  aux = color_matrix_1;		
	  	
	  for(i=0; i<n; i++)
		for(j=0; j<n; j++)
		  {
		/* Pentru fiecare element in parte:
		 * 	-plec cu raza r = 1;
		 * 	-verific de jur imprejurul elementului astfel:
		 * 		{
			 * 		data fiind pozitia initiala a elementului [i][j] 
			 * 		elementele din jur imprejur sunt:
			 * 		de la (j-r, i-r) pana la (j+r, i-r) pe partea de N
			 * 		de la (j+r, i-r) pana la (j+r, i+r) pe partea de E
			 * 		de la (j+r, i+r) pana la (j-r, i+r) pe partea de S
			 * 		de la (j-r, i+r) pana la (j-r, i-r) pe partea de V
			 * 		}
			 * 	
			 * - verifiv pt fiecare element in parte generat aleator daca e valid;
			 * - daca e valid si se afla in matrice il adaug la componenta "ColorTable"
			 * - verific daca am adauga toate culorile in tabela:
			 * 		- daca le am pe toate: {
			 * 				scot culoarea din tabela cu indicele cel mai mic;
			 * 				o pun in noua matrice + trec la urmatorul element din matrice 
			 * 				}
			 * 	- daca nu le am pe toare cresc radius;
		*/
		radius = 1;
															// Copiez adresa buffer_1 in aux !
																				//
		  while(ok == true) 
			{
			i_prim = i - radius;												// Partea de N:
			for(j_prim = j - radius; j_prim <= j + radius; j_prim++) {			// 
																				//
				if( isInMatrix(i_prim, j_prim) == true) {						//
					tabel_culori->checkColor(color_matrix_1[i_prim][j_prim], radius);																			//
				}																//
			}																	//
			
			
			j_prim = j + radius;												// Partea de E:
			for(i_prim = i - radius; i_prim <= i + radius; i_prim++) {			//
																				//
				if( isInMatrix(i_prim, j_prim) == true) {						//			
					tabel_culori->checkColor(color_matrix_1[i_prim][j_prim], radius);	
				}																//
			}																	//
			
			
			i_prim = i + radius;												// Partea de S:
			for(j_prim = j + radius; j_prim >= j - radius; j_prim--) {			//
																				//
				if( isInMatrix(i_prim, j_prim) == true) {						//
					tabel_culori->checkColor(color_matrix_1[i_prim][j_prim], radius);	
				}																//
			}																	//
			
			
			j_prim = j - radius;												// Partea de V:
			for(i_prim = i + radius; i_prim >= i - radius; i_prim--) {			//
																				//
				if( isInMatrix(i_prim, j_prim) == true) {						//
					tabel_culori->checkColor(color_matrix_1[i_prim][j_prim], radius);	
				}																//
			}																	//
				
				
			if(tabel_culori->isFullTable() == true) {							// Verific daca 
				color_matrix_2[i][j] = tabel_culori->getColor();				// tabelul de culori	
				tabel_culori->incrementColor(color_matrix_2[i][j]);				//							
				ok = false;														// e full: ( Incrementez indicele de culoare din tabela)
			}																	//
			else {																// Altfel:
				radius++;														// cresc raza de cautare cu o unitate
				
				if(radius > n) {	
					color_matrix_2[i][j] = tabel_culori->getColor();		    //??? intorc prima culoare daca e singurul element?	
					tabel_culori->incrementColor(color_matrix_2[i][j]);											//	Verific daca raza a depasit dimensiunea
					ok = false;	
					}															//  tabelei:
			}																	//
		  }																		//
		  tabel_culori->uncheckAllColors();
		  ok = true;		  
	      } 
	  
		  tabel_culori->printValue();											//
		  fw<<endl;																//  
		  color_matrix_1 = color_matrix_2; 										//	Interschimbare buffere de memorie
		  tabel_culori->colorMissingAfterWeek();								//	Setez culorile care au nr total 0 ca fiind disparute	
		  tabel_culori->setColorContorsToZero();	  							//
	}
	
  // Afisare matrice:	
  for(i=0; i<n; i++)
    for(j=0; j<n; j++) {
		if(j == n - 1) fw<<color_matrix_2[i][j]<<endl;
		else 		   fw<<color_matrix_1[i][j]<<" ";
    }
    
  // Scot din memorie matricile:
  // delete color_matrix_1;
  // delete color_matrix_2;
  
  // Inchid fisierele:  
  fr.close();
  fw.close();
	  
  return 0;
}

// Returneaza maximul - Momenta nu o folosim -
int max(int val_1, int val_2) {
  if(val_1 <= val_2)
    return val_2;
    return val_1;	 
}

// Verifica daca elementul cautat se incadreaza
// in matrice sau nu
bool isInMatrix(int i, int j) {
    if((i >= 0) && (i < n) && (j >= 0) && (j < n))
	return true;
	return false;
}

