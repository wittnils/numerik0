#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "hdnum.hh"    // hdnum header
#include <tgmath.h>   //for pow function
#include <ctime>      //for clock class
#include <algorithm>
#include <chrono>

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
      if (value == 0){
        throw std::invalid_argument("received value 0");
      }
      else{
        //Stelle ist schon vorhanden?
        for (std::size_t k = 0; k<entries.size(); k++){
              if (i == entries[k].i && j==entries[k].j){
                entries[k].value = value;
                return;
              }  
        }
        //nun ist Stelle nicht vorhanden und wir können einfügen
        //passe größe von entries an
        entries.resize(entries.size()+ 1);
        //füge wert in ensprechender zeile und spalte 
        entries[entries.size()-1].i=i;
        entries[entries.size()-1].j=j;
        entries[entries.size()-1].value = value;
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
  //test for (b)
  hdnum::SparseMatrix<float> test_sparse;
  //sparse.AddEntry(1,1,0);
  //should terminate
  test_sparse.AddEntry(0,1,2);
  test_sparse.AddEntry(0,0,4);
  test_sparse.AddEntry(1,0,4);
  test_sparse.AddEntry(1,2,3);
  test_sparse.AddEntry(2,1,1);
  /*Matrix der Form:
  [4][2][0]
  [4][0][3]
  [0][1][0]
  */
  hdnum::DenseMatrix<float> test_dense(3,3);
  test_dense[0][0]= 4;
  test_dense[0][1]=2;
  test_dense[1][0]=4;
  test_dense[1][2] = 3;
  test_dense[2][1]=1;
  
  hdnum::Vector<float> vec(3,1);
  vec[1] = 2;
  /*vec der Form:
  [1]
  [2]
  [1]
  */

  hdnum::Vector<float> result_test_sparse(3,0);
  hdnum::Vector<float> result_test_dense(3,0);

  //multiply both
  /*
  [4][2][0]   [1]
  [4][0][3] * [2]
  [0][1][0]   [1]
  */
  test_dense.mv(result_test_dense,vec);
  test_sparse.mv(result_test_sparse,vec);
  
  //outstream
  std::cout<< "result test_dense: " << result_test_dense << std::endl;
  std::cout<< "result sparse: " << result_test_sparse << std::endl;
  //passed

  //(b) check equal result:
  if (result_test_sparse == result_test_dense){
    std::cout << "Operation was successful!" << std::endl;
  }
  else{
    std::cout << "result was not correct" << std::endl;
  }

   //(c) wir haben n jeweils verändert und in data.dat eingelesen
  std::ofstream outfile;
  outfile.open("data.dat", std::ios_base::app);
  const int n = 14;
  const int N = pow(2,n);
  hdnum::DenseMatrix<float> dense(N,N,0.0);
  hdnum::SparseMatrix<float> sparse;

  //initialize Matrix
  /*
  [2][0] [0]..[0]
  [0][2] [0]..[0]
  ....
  .........[0][2]
  */
  int j=0;
  for ( int i=0; i<dense.rowsize (); ++i){
   dense[i][j] = 2;
   sparse.AddEntry(i,j,2);
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
