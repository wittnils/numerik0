#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "hdnum.hh"    // hdnum header
#include <tgmath.h> 
#include <chrono>
#include <fstream>
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

    Vector<MatrixEntry>& getEntries(){
      Vector<MatrixEntry> *target = new Vector<MatrixEntry>; 
      for(int i = 0; i<entries.size; ++i){
        target.push_back(entries[i]); 
      }
    }

    void print(){
      for(int i = 0; i < entries.size(); ++i){
        std::cout << entries[i].value << std::endl;
      } 
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
  template<typename V>
  Vector<V> richardsonIteration(DenseMatrix<V>& A, Vector<V>& x_0, Vector<V>& b, V epsilon, V maxEig, int NumberIterations){
    int n = A.colsize();
    // Berechne den Defekt
    Vector<V> d_0(n, 0.0);
    Vector<V> d_k(n, 0.0);
    Vector<V> x_k(n,0.0);
    Vector<V> Ax_k(n,0.0);
    Vector<V> v(n,0.0); 
    x_k = x_0;
    // Bestimme initialen Defekt
    A.mv(Ax_k, x_0); // Ax_k = A*x_0 = A*x_k
    d_0 = b-Ax_k; // d_0 = b - Ax_k
    d_k = d_0; 

    int i = 0;
    while(norm(d_k) > epsilon*norm(d_0) && i<NumberIterations){
      // 3. Löse W v = d, hier ist W^{-1} = 1/\omega I für ein geeignetes omega. hier omega = maxEig
      v = d_k; 
      v *= 1./maxEig; 
      // 4. x = x + v
      x_k = x_k + v; 
      // d = d-Av
      A.mv(Ax_k,v);
      d_k = d_k - Ax_k;
      ++i;
    }
    return x_k; 
  }
  
  template< typename V >
  Vector<V> jacobi (DenseMatrix<V>& A,Vector<V>& x_0, Vector<V>& b, V epsilon, int NumberIterations ){
    int n = A.colsize();
    // Berechne den Defekt
    Vector<V> d_0(n, 0.0);
    Vector<V> d_k(n, 0.0);
    Vector<V> x_k(n,0.0);
    Vector<V> Ax_k(n,0.0);
    Vector<V> v(n,0.0); 
    DenseMatrix<V> W_inverse (n,n,0.0);
    x_k = x_0;
    // Bestimme initialen Defekt
    A.mv(Ax_k, x_0); // Ax_k = A*x_0 = A*x_k
    d_0 = b-Ax_k; // d_0 = b - Ax_k
    d_k = d_0; 
    int i = 0;
    
    //Bestimme inverse Diagonalmatrix:
    for (int k = 0; k < n; k++){
        W_inverse[k][k] = 1. /A[k][k];
    }

    while ( norm(d_k) > epsilon * norm(d_0) && i < NumberIterations){
        // v = W^{-1}* d
        W_inverse.mv(v, d_k);
        // x = x+v
        x_k += v;
        // d_k = d_k -Av
        A.mv(Ax_k, v);
        d_k -= Ax_k;
        i++;
    }
    return x_k;
  }

  template <typename V>
  Vector<V> Gauss_Seidel (DenseMatrix<V>& A,Vector<V>& x_0, Vector<V>& b, V epsilon, int NumberIterations){
    int n = A.colsize();
    // Berechne den Defekt
    Vector<V> d_0(n, 0.0);
    Vector<V> d_k(n, 0.0);
    Vector<V> x_k(n,0.0);
    Vector<V> Ax_k(n,0.0);
    Vector<V> v(n,0.0); 
    x_k = x_0;
    // Bestimme initialen Defekt
    A.mv(Ax_k, x_0); // Ax_k = A*x_0 = A*x_k
    d_0 = b-Ax_k; // d_0 = b - Ax_k
    d_k = d_0; 
    int i = 0;
    
    while (norm(d_k) > norm(d_0)* epsilon && i < NumberIterations){
        for(int i=0; i<n; ++i){
        v[i] = d_k[i];
        for(int m = 0; m < i; ++m){
          v[i] -= A[i][m]*v[m];
        }
        v[i] *= 1./A[i][i];  
      }
      x_k = x_k+v;
      A.mv(Ax_k,x_k);
      d_k = b - Ax_k;
      i++;
    }
    return x_k;
  }
  
}

int main ()
{
  int n = 4;
  int N = pow(2,n);
  
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
  hdnum::Vector<double> LinSolve_x(N, 0.0);
  hdnum::Vector<double> b(N, 1.0);

  // Erstelle Sparse-Version von A
  hdnum::SparseMatrix<double> A_sparse; 
  for(int i = 0; i < N ; ++i){
    for(int j = 0; j < N; ++j){
      A_sparse.AddEntry(i,j,A[i][j]); 
    }
  }
  //A_sparse.print();  
   
  // Lösen Sie nun A*x=b iterativ

  //Wir können anhand der gegebenen Matrix die Überschätzung aus Satz 14.2 aus den Gerschgorin Kreisen die Eigenwerte abschätzen 
  // \lvert -2 - \xi \rvert \le 2 \implies A hat die größten EW 4 
  
  // Richardson-Verfahren. B = 1/\maxEig \cdot I, aus Satz 14.1 erhalten wir, dass für alle \maxEig mit \maxEig \le \lambda_{max}^{-1}(BA)
  // Also wegen B=I hier der Alogirthmus konvergiert für alle \maxEig mit \maxEig \le \lambda_{max}^{-1}(A). Aus den Gerschgorin-Kreisen 
  // erhalten wir die Abschätzung (s. Matrixstruktur), dass \vert \lambda \vert \le 4. Offensichtlich ist A negativ definit und dann ist
  // Ist setzen wir maxEig = -4

 double maxEig = -4.; 
 int NumberIterations = 100000;

 //A ist irreduzibel diagonaldominant, daher konvergieren das Jacobi und Gauß-Seidel-Verfahren

 
 hdnum:: Vector<double> resultGS(N, 0.0);
 hdnum:: Vector<double> resultJac(N, 0.0);
 hdnum:: Vector<double> resultRichardson(N, 0.0);

 resultRichardson = hdnum::richardsonIteration(A, x,  b, pow(10,-4), maxEig, NumberIterations);
 std::cout << "result Richardson: " << resultRichardson << std::endl;

 resultJac = hdnum::jacobi(A, x,  b, pow(10,-4),NumberIterations);
 std::cout << "result Jacobi: " << resultJac << std::endl;
 
 resultGS = hdnum::Gauss_Seidel(A, x,  b, pow(10,-4),NumberIterations);
 std::cout << "result GS: " << resultGS << std::endl;
 



}
