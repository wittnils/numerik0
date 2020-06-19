#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "hdnum.hh"    // hdnum header

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




}
