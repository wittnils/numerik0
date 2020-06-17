#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "/home/nilsw/numerik0-1/hdnum/hdnum.hh"    // hdnum header

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
        std::cout << entries[k].i << std::endl;
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
  std::cout << "test" << std::endl; 
  hdnum::DenseMatrix<float> test_dense(3,3);
  test_dense[0][0]= 4;
  test_dense[0][1]=2;
  test_dense[1][0]=4;
  test_dense[1][2] = 3;
  test_dense[2][1]=1;
  
  //sparse.AddEntry(5,8,10);
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

  //check equal result:
  if (result_test_sparse == result_test_dense){
    std::cout << "Operation was successful!" << std::endl;
  }
  else{
    std::cout << "result was not correct" << std::endl;
  }
  return 0; 
}
