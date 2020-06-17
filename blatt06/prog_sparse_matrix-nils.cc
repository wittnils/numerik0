#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "/home/nilsw/numerik0-1/hdnum/hdnum.hh"    // hdnum header
#include <tgmath.h>   //for pow function
#include <ctime>      //for clock class
#include <chrono>
#include <algorithm>
#include <fstream>

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
      if(value == 0){
        throw std::invalid_argument("Matrixeintrag == 0 nicht erlaubt!");
      }else{
        bool already = 0; 
        for(int l = 0; l < entries.size(); ++l){
          if(entries[l].i == i and entries[l].j == j){
            already = 1; 
            entries[l].value = value;  
          }
        }
        if(already == 0){
          entries.resize(entries.size()+1); 
          entries[entries.size()-1].i = i;
          entries[entries.size()-1].j = j;
          entries[entries.size()-1].value = value;
        }
      }
    }

    template<typename V>
    void mv (Vector<V>& y, const Vector<V>& x) {
    // Wir bestimmen die Dimension der Domain von A,
    // indem wir das maximale i ermitteln   
      int domainDimension = 0; 
      int targetDimension = 0; 
      for(int k = 0; k < entries.size(); ++k){
        if(entries[k].i+1 > domainDimension){
          domainDimension = entries[k].i + 1;
        }
        if(entries[k].j+1 > targetDimension){
          targetDimension = entries[k].j + 1;
        }
      } 
      if(x.size() != domainDimension){
        throw std::invalid_argument("Invalid dimenison of vector x");
      }else{
        // Wir berechnen das Matrix-Vektorprodukt
        y.resize(targetDimension, 0);
        for(int k = 0; k < entries.size(); ++k){
          y[entries[k].i] += entries[k].value*x[entries[k].j]; 
        }
      } 
    }

  private:
    std::vector<MatrixEntry> entries;
  };

}

int main ()
{
  //(c) 
  std::ofstream outfile;
  outfile.open("data.dat", std::ios_base::app);
  const int n = 14;
  const int N = pow(2,n);
  hdnum::DenseMatrix<float> dense(N,N,0.0);
  hdnum::SparseMatrix<float> sparse;

  //initialize Matrix
  /*
  [2][2][2] [0]..[0]
  [0][2][2][2] [0]..[0]
  ....
  ...............[0][2][2][2]
  */
  int j=0;
  for ( int i=0; i<dense.rowsize (); ++i){
    int k =0;
    while (k< 3) {
      dense[i][j] = 2;
      sparse.AddEntry(i,j,2);
      k++;
    }
   j++;
  }

  //vector x:
  /*
  [1]
  ...
  [1]
  */
  hdnum::Vector<float> x(N,1);
  //and results:
  hdnum::Vector<float> result_sparse(N,0);
  hdnum::Vector<float> result_dense(N,0);
  ////////////////////////////////////////////////////////////////////////
  //chrono version
  // https://stackoverflow.com/questions/25836511/how-to-use-chrono-to-determine-runtime 
  //////////////////////////////////////////////////////////////////////////

  auto begin_2 = std::chrono::high_resolution_clock::now();
  //multiply
  dense.mv(result_dense,x);
  auto diff_2 = std::chrono::high_resolution_clock::now() - begin_2;
  auto t2 = std::chrono::duration_cast<std::chrono::microseconds>(diff_2);

  auto begin = std::chrono::high_resolution_clock::now();
  //multiply
  sparse.mv(result_sparse,x);
  auto diff = std::chrono::high_resolution_clock::now() - begin;
  auto t1 = std::chrono::duration_cast<std::chrono::microseconds>(diff);
  outfile << N << " " << t1.count() << " " << t2.count() << std::endl;
  
  return 0; 
}
