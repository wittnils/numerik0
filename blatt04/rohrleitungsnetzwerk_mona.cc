// Kompilieren mit:
//
// g++ -I../hdnum/ -o rohrleitungsnetzwerk rohrleitungsnetzwerk.cc
//
// Das setzt voraus, dass ihr Programm in einem Ordner parallel zur
// entpackten HDNum Bibliothek liegt.

#include <iostream>
#include <cstdlib>
#include "hdnum.hh"
#include<tgmath.h>
#include<cmath>


// Funktion zum Aufstellen der Matrix
template<class NumberType>
void flussMatrix( hdnum::DenseMatrix<NumberType> &A )
{
  int M( A.rowsize() );
  int N( A.colsize() );
  if(M!=N)
    HDNUM_ERROR("Matrix muss quadratisch sein!");
  
  //wir erstellen die mat B um dann A=B^tB zu erstellen
  //wir erkennen, dass durch die Erstellung des LGS die Anzahl der Knoten in einer Zeile folgendes beträgt:
  //schließlich ist M = Kontenanzahl -1
  int vertikal = sqrt(M+1);
  int kantenzahl = 2*(vertikal*(vertikal-1));
  hdnum::DenseMatrix<NumberType> B (kantenzahl,M,0.0);

  //wir beginnen mit dem ersten Teil
  B[0][0]=1;
  int j = 0;
  for (int i =1; i < B.rowsize()/2; i++){
      B[i][j]= -1;
      B[i][j+1]= 1;
      j++;
  }

  //konstruiere nun den zweiten Teil
B[B.rowsize()/2][vertikal-1] = 1;
int k = 0;
for(int i = B.rowsize()/2 +1; i<B.rowsize(); i++){
  B[i][k]=-1;
  B[i][k+vertikal-1]=1; 
  k++;
}
std::cout<< "die Matrix B" << std::endl;
std::cout << B << std::endl;
  A.mm(B.transpose(),B);
}


// Funktion zur Berechnung der Frobenius-Norm einer Matrix
template<class NumberType>
NumberType frobeniusNorm(const hdnum::DenseMatrix<NumberType> &A)
{
  // Error checking
  int M(A.rowsize());
  int N(A.colsize());
  if(M!=N)
    HDNUM_ERROR("Matrix muss quadratisch sein!");

  NumberType result=0.0;

 //(b)schon implementiert: Spalten bzw. Zeilensummennorm
  for ( int i=0; i<A.rowsize (); ++i){
      for ( int j=0; j<A.colsize (); ++j){
          //Betrag bestimmen
          result+= abs(A[i][j]);
          //quadrieren
          pow(result,2);
          }
  }
  result = sqrt(result);
  return result;
}


// Funktion zur Berechnung des betragsgrößten Eigenwertes mit Potenzmethode
template<class NumberType>
NumberType maxEigenwert(const hdnum::DenseMatrix<NumberType> &A)
{
  // Error checking
  int M(A.rowsize());
  int N(A.colsize());
  if(M!=N)
    HDNUM_ERROR("Matrix muss quadratisch sein!");

  // TODO Implementieren Sie hier die Potenzmethode

  // Berechnung der EW:
  hdnum::Vector<NumberType> r_0; // wie r_0 aussieht naja ähm

  //hier sollte man vlt noch überprüfen ob Ar_0 \neq 0 ist
  hdnum::Vector<NumberType> var = r_0;

  //schleife 1000 mal durchlaufen, weil wir Grenzwert betr. kA, wie lange das dauern soll^^
  for(int i=0; i<1000; i++){
    A.mv(var,var); // var = A*var
    var = var/ norm(var); // teile durch euklidische Norm
  }

  //Berechnung des EV:
  hdnum::Vector<NumberType> y;
  hdnum::Vector<NumberType> lambda;
  A.mv(y,var); //y=A*var, wobei var nun unser EW ist
  lambda = (var*y)/(var*var); //Berechnung der euklidischen Skalarprodukte

  return var; //max EW zurückgeben, da funkt so heißt, vlt könnte man auch ein Tupel zurückgeben mit EV,EW 
}

// Hauptprogramm
int main(int argc, char ** argv)
{

  // Anzahl der Knoten
  const int N(3);
  std::cout << "Knotenanzahl N: " << N << std::endl;

  // Größe der Matrix
  const int n(N*N-1);

  // Datentyp für die Matrix
  typedef double REAL;

  // Matrix initialisieren
  hdnum::DenseMatrix<REAL> A(n,n);

  // Pretty-printing einmal setzen für alle Matrizen
  A.scientific(false);
  A.width(15);
  A.precision(3);

  flussMatrix(A);
  if (N<=4)
    std::cout << A << std::endl;

  // Bei Schwierigkeiten mit Teilaufgabe a) können Sie Teilaufgaben b)
  // und c) mit folgender Matrix testen
  int size_b = 4;
  hdnum::DenseMatrix<REAL> B(size_b,size_b);
  for (std::size_t i=0; i<size_b; ++i)
  {
    for (std::size_t j=0; j<size_b; ++j)
    {
      B[i][j] = i+j;
    }
  }

  flussMatrix(A);
  std::cout << A << std::endl; 
  std::cout << "Die Frobeniusnorm beträgt: " << frobeniusNorm(A) << std::endl; 

  return 0;
}
