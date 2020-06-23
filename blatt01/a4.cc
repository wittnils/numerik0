#include <iostream>
#include <iomanip>
#include <math.h>
#include <limits>

// int main(){
//     double sum_x = 2;
//     double x = 1.; 
//     float sum_y = 2;
//     float y = 1.; 
//     int i = 0; 
//     while(sum_y!=1 or sum_x!=1){
//         if(sum_y != 1){
//             y = pow(2, -i);
//             sum_y = 1+y;
//         }
//         if(sum_x != 0){
//             x = pow(2,-i);
//             sum_x = 1+x;
//         }
//         std::cout << "double x: " << x << "\nfloat y: " <<  y << std::endl;
//         std::cout << "double 1+x: " << std::setprecision(std::numeric_limits<double>::digits10+1) << sum_x << std::endl;
//         std::cout << "float 1+x: "<< std::setprecision(std::numeric_limits<float>::digits10+1) << sum_y << std::endl;  
//         ++i; 
//     } 
//     std::cout << "Kleinste Zahl, die bei Addition zu keiner Nulladition führt: \n" << "double: " << x << "\nfloat: " << y << std::endl; 
//     std::cout << std::setprecision(std::numeric_limits<double>::digits10+26) << 1+1e-16;
// }

#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "hdnum.hh"    // hdnum header
#include <tgmath.h> 
//g++ -I../hdnum/ -o prog-iterative prog_iterative_solvers.cc
namespace hdnum {

  template<typename REAL>
  class SparseMatrix {

    struct MatrixEntry {
      int i;
      int j;
      REAL value;
    };

  public:

    void AddEntry (int i, int j, REAL value) {
      assert(i >= 0);
      assert(j >= 0);
      if (value != .0)
        entries.push_back(MatrixEntry{.i=i, .j=j, .value=value});
    }

    template<typename V>
    void mv (Vector<V>& y, const Vector<V>& x) {

      zero(y);

      for (MatrixEntry& matrix_entry : entries) {
        assert(y.size() > matrix_entry.i);
        assert(x.size() > matrix_entry.j);
        y[matrix_entry.i] += matrix_entry.value * x[matrix_entry.j];
      }
    }

  private:
    std::vector<MatrixEntry> entries;
  };
  // Omega ist schon als Kehrwehrt übergeben  
  template<typename V>
  Vector<V>  richardsonIteration(DenseMatrix<V>& A, Vector<V>& start, Vector<V> b, V reduction, V omega){
    // Initialen Defekt berechnen
    int N = A.colsize(); 
    Vector<V> d_initial(N,0.0); 
    Vector<V> d_algorithm(N,0.0); 
    A.mv(d_initial,start); A.mv(d_algorithm,start); 
    
    d_algorithm = b -d_algorithm;
    d_initial = b - d_initial;

    /* Plan: Zwei Abbruchkriterien
    * Den Defekt messen wir im euklid. Skalarpr. 
    * 1. Defekt um Faktor reduction (=10^{-4}) verringert 2. Iterationsanszahl überschreitet 10^5 
    */
   // eukl (inital ) < = eukl (curr defect * reduction)
   // eukl ( init ) * red >= eukl ( curr )
   // norm(init) == eukl (init)
    int i = 0; 
    while(i < 4 && norm(d_initial) * reduction < norm(d_algorithm) ){
      d_algorithm *= omega;
      std::cout << d_algorithm << std::endl; 
      //laut doku von hdnum geht auch: start.update(omega,d_algorithm) // start = start + omega* d_algorithm
      start = start +  d_algorithm;
      A.mv(d_algorithm,start); 
      d_algorithm = b - d_algorithm;
      ++i; 
      //gut, also, falls du wieder lust hast, können wir gerne weiter coden
      //ah gut, es war eh im getexen dokument, also falscher alarm
      // alles gut, no problemo amiga scheerer
    }
    return start; 
  }
}

int main ()
{
  int N = 16;

  // Testmatrix aufsetzen
  hdnum::DenseMatrix<double> A(N,N,.0);
  for (typename hdnum::DenseMatrix<double>::size_type i=0; i<A.rowsize(); ++i)
  {
    if (i > 0) {
      A[i][i-1] = 1.0;
    }
    if (i + 1 < A.colsize()) {
      A[i][i+1] = 1.0;
    }
    A[i][i] -= 2.0;
  }

  // Rechte Seite und Lösungsvektor
  hdnum::Vector<double> x(N, 0.0);
  hdnum::Vector<double> b(N, 1.0);

  A.scientific(false);
  std::cout << A << std::endl;
  
  // Lösen Sie nun A*x=b iterativ

  //Wir können anhand der gegebenen Matrix die Überschätzung aus Satz 14.2 aus den Gerschgorin Kreisen die Eigenwerte abschätzen 
  // \lvert -2 - \xi \rvert \le 2 \implies A hat die größten EW 4 
  
  // Richardson-Verfahren. B = 1/\omega \cdot I, aus Satz 14.1 erhalten wir, dass für alle \omega mit \omega \le \lambda_{max}^{-1}(BA)
  // Also wegen B=I hier der Alogirthmus konvergiert für alle \omega mit \omega \le \lambda_{max}^{-1}(A). Aus den Gerschgorin-Kreisen 
  // erhalten wir die Abschätzung (s. Matrixstruktur), dass \vert \lambda \vert \le 4, nun ist nach Blatt 6 A positiv definit (und symmetrisch), d.h. 
  // alle EW sind >0, damit wählen wir 1/\omega = 1/4 und das Richardson-Verfahren konvergiert für alle Startwerte. 
  double lambda_gerschgorin = 1./4; 
  //x[0]= 1;
  // start = 
  /*
  [1]
  [0]
  ...
  [0]
  */
 hdnum:: Vector<double> result(N, 0.0);
 result = hdnum::richardsonIteration (A, x,  b, pow(10,-4), lambda_gerschgorin);
 std::cout << result << std::endl;
 

}

