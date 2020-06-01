// Kompilieren mit:
//
// g++ -I../hdnum/ -o rohrleitungsnetzwerk rohrleitungsnetzwerk.cc
//
// Das setzt voraus, dass ihr Programm in einem Ordner parallel zur
// entpackten HDNum Bibliothek liegt.

#include <iostream>
#include <cstdlib>
#include "hdnum/hdnum.hh"


// Funktion zum Aufstellen der Matrix
template<class NumberType>
void flussMatrix( hdnum::DenseMatrix<NumberType> &A )
{
  int M( A.rowsize() );
  int N( A.colsize() );
  if(M!=N)
    HDNUM_ERROR("Matrix muss quadratisch sein!");
  
  // Konstruiere die Matrix B, damit mit A = B^t B die Matrix A berechnet werden kann
  int vertices = int(sqrt(N+1)); 
  int numEdges = 2*(vertices*(vertices-1));
  hdnum::DenseMatrix<NumberType> hilfsmat(numEdges,N,0.0);  
  // Konstruiere erste Hälfte
  int jump = 0;
  for(int i = 0; i<numEdges/2; ++i){
    if((i+jump+1) % vertices == 0 && i!= 0){
      jump += 1;
    }
    hilfsmat[i][jump+i] = 1;
    if(i != 0){
      hilfsmat[i][jump+(i-1)] = -1;
    }
  }
  //Konstruiere zweite Häflte
  for(int i = 0; i < numEdges/2; ++i){
    hilfsmat[i+numEdges/2][(vertices-1)+i] = 1;
    if(i != 0){
      hilfsmat[i+numEdges/2][i-1] = -1;
    }
  } 
  A.mm(hilfsmat.transpose(),hilfsmat);
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
  for(int i = 0; i < M; ++i){
    for(int j = 0; j < M; ++j){
      result+=pow(abs(A[i][j]),2);
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
  
  // 1. Wir finden einen Vektor v!=0, s.d. Av != 0
  // Wir durchlauen dazu die Basisvektoren e1,...,e_n und bilden sie mittels A ab
  // Ist einer der Bilder != 0, so haben wir einen geeigeten Vektor gefunden
  int dim = M; 
  hdnum::Vector<NumberType> bv(dim,0.0), v(dim,0.0);
  bool found = false;  
  for(int i = 0; i<dim && found == false; ++i){
    // Erstelle aus dem i-1 ten BV den i-ten BV
    bv[i] = 1;
    if(i!=0){
      bv[i-1]=0;
    }
    // Berechne das Bild des i-ten BV
    A.mv(v,bv);
    // Ist eine der Komponenten != 0, so setze v auf den i-ten BV
    for(int i = 0; i < dim; ++i){
      if(v[i]!=0){
        v = bv; 
        found = true; 
      }
    }
    if(i==dim-1){
      std::cout << "Die Bilder aller BV sind 0, also ist A die Nullmatrix.";
    }
  }
  // 2. Wir berechnen die Folgeglieder der Iterationsfolge zur Bestimmung
  // eines EV zum betragsmäßig größten EW
  hdnum::Vector<NumberType> r = v; 
  for(int i = 0; i < 500; ++i){
    A.mv(r,v);
    r *= 1./norm(r); 
    A.mv(v,r);
    v *= 1./norm(v);
  } 
  // 3. Mittels des Rayleigh-Quotienten erhalten wir den dazugehörgen EV
  A.mv(r,v);  
  return (r*v)/(v*v); 
}

// Hauptprogramm
int main(int argc, char ** argv)
{

  // Anzahl der Knoten
  const int N(5);
  std::cout << "Knotenanzahl N: " << N << std::endl;

  // Größe der Matrix
  const int n(N*N-1);

  // Datentyp für die Matrix
  typedef double REAL;

  // Matrix initialisieren
  hdnum::DenseMatrix<REAL> A(n,n);

  // Pretty-printing einmal setzen für alle Matrizen
  A.scientific(false);
  A.width(8);
  A.precision(3);

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
  REAL x = maxEigenwert(A);
  std::cout << "Der maximale Eigenwert ist: " << maxEigenwert(A) << std::endl;
  return 0;
}
