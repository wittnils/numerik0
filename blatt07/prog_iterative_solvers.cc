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
  template<typename V>
  Vector<V> jacobi(DenseMatrix<V>& A, Vector<V>& x_0, Vector<V>& b, V epsilon, int NumberIterations){
    int n = A.colsize(); 
    // Berechne den Defekt
    Vector<V> d_0(n, 0.0); // initialer Defekt
    Vector<V> d_k(n, 0.0); // Durchlaufender Defekt
    Vector<V> x_k(n,0.0);  // k-te Iterierte
    Vector<V> Ax_k(n,0.0); // Zwischenspeichervariablen
    Vector<V> v(n,0.0); 
    // Für k=0 ist x_k = x_0
    x_k = x_0;
    // Bestimme initialen Defekt
    A.mv(Ax_k, x_0); // Ax_k = A*x_0 = A*x_k
    d_0 = b-Ax_k; // d_0 = b - Ax_k
    d_k = d_0; // k=0 => d_k = d_0
    int i = 0; 
    while(norm(d_k) > epsilon*norm(d_0) && i<NumberIterations){
      /*3. Löse W v = d. Hier ist W = diag(a_11,...,a_nn), also W^{-1} = diag(1./a_11,...,1./a_nn), damit ist
      */
      v = d_k;  
      for(int k = 0; k < n; ++k){
        v[k] *= 1./A[k][k];
      }
      // 4. x = x + v
      x_k = x_k + v; 
      // d = d-Av
      A.mv(Ax_k,v);
      d_k = d_k - Ax_k;
      ++i;
    }
    return x_k;
  }
  template<typename V>
  Vector<V> gauss_seidel(DenseMatrix<V>& A, Vector<V>& x_0, Vector<V>& b, V epsilon, int NumberIterations){
     int n = A.colsize(); 
    // Berechne den Defekt
    Vector<V> d_0(n, 0.0); // initialer Defekt
    Vector<V> d_k(n, 0.0); // Durchlaufender Defekt
    Vector<V> x_k(n,0.0);  // k-te Iterierte
    Vector<V> Ax_k(n,0.0); // Zwischenspeichervariablen
    Vector<V> v(n,0.0); 
    DenseMatrix<V> W_i(n,n,0.0);
    // Für k=0 ist x_k = x_0
    x_k = x_0;
    // Bestimme initialen Defekt
    A.mv(Ax_k, x_0); // Ax_k = A*x_0 = A*x_k
    d_0 = b-Ax_k; // d_0 = b - Ax_k
    d_k = d_0; // k=0 => d_k = d_0
    int i = 0;
    // Stelle W^{-1} auf, W^{-1} ist ebenfalls untere Dreiecksmatrix
    for(int i = 0; i<n; ++i){
      for(int j = 0; j <= i; ++j){ //Elemente über der Hauptidag. sind 0
        if(j==i){
          // Wegen W, W^{-1} untere Dreiecksmatrix 
          W_i[i][j] = 1./A[i][j];
        }else{
          // Wir haben insg., dass (L+D)*W^{-1} = I nach Konstr. 
          // Wir bauen W_i schritweise auf und wir konstruieren hier das Element W_i[i][j]. Alle Zeilen darüber
          // wurden bereits konstruiert und es gilt für j > i dass W[i][j]=0 und dasselbe gilt für A. 
          // Wir suchen nun ein W[i][j], s.d. i-te Zeile von A * j-te Spalte von W_i = 0. Bis zum i-1-ten 
          // Element kennen wir die j-te Spalte von W_i bereits und für alle Elemente darunter ist i-te Zeile von A
          // *j-te Spalte von W_i = 0, denn dort ist die i-te zeile von A =0. Da überhalb der HD W_i = 0 ist, gehen wir
          // ab der HD los und bilde das Produkt aus i-ter Zeile von A und j-ter Spalte von W_i bis zu unserem Element
          // Das wir mit a_ii mult., damit erhalten wir die Bedinung, die hier realisiert wird
          for(int k = j; k < i-1; ++k){
            W_i[i][j] -= A[i][k]*W_i[j][k];
          }
          // und wir teilen durch das HD-Element
          W_i[i][j] *= 1./A[i][i];
        }
      }
    }
    while(norm(d_k) > epsilon*norm(d_0) && i<NumberIterations){
      // 3. Löse W v = d. Hier ist W = L+D
      W_i.mv(v,d_k);
      // 4. x = x + v
      x_k = x_k + v; 
      // d = d-Av
      A.mv(Ax_k,v);
      d_k = d_k - Ax_k;
      ++i;
    }
    return x_k;
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



  
  // Lösen Sie nun A*x=b iterativ

  //Wir können anhand der gegebenen Matrix die Überschätzung aus Satz 14.2 aus den Gerschgorin Kreisen die Eigenwerte abschätzen 
  // \lvert -2 - \xi \rvert \le 2 \implies A hat die größten EW 4 
  
  // Richardson-Verfahren. B = 1/\maxEig \cdot I, aus Satz 14.1 erhalten wir, dass für alle \maxEig mit \maxEig \le \lambda_{max}^{-1}(BA)
  // Also wegen B=I hier der Alogirthmus konvergiert für alle \maxEig mit \maxEig \le \lambda_{max}^{-1}(A). Aus den Gerschgorin-Kreisen 
  // erhalten wir die Abschätzung (s. Matrixstruktur), dass \vert \lambda \vert \le 4. Offensichtlich ist A negativ definit und dann ist
  // Ist setzen wir maxEig = -4
 double maxEig = -4.; 
 int NumberIterations = 200;

 hdnum:: Vector<double> resultGS(N, 0.0);
 hdnum:: Vector<double> resultJac(N, 0.0);
 hdnum:: Vector<double> resultRichardson(N, 0.0);
 
 resultRichardson = hdnum::richardsonIteration(A, x,  b, pow(10,-4), maxEig, NumberIterations);
 resultJac = hdnum::jacobi(A, x,  b, pow(10,-4),NumberIterations);
 resultGS = hdnum::gauss_seidel(A, x,  b, pow(10,-4),NumberIterations);
 std::cout << resultGS << std::endl;
 std::cout << resultJac << std::endl;
 std::cout << resultRichardson << std::endl;
 

}
