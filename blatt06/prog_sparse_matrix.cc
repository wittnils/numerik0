#include <iostream>    // notwendig zur Ausgabe
#include <vector>
#include "hdnum.hh"    // hdnum header
#include <tgmath.h>   //for pow function
#include <ctime>      //for clock class

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
      //array on false
      bool a[entries.size()] = {0};
      
      for (std::size_t l = 0; l<entries.size(); l++){
        //multipliziere nicht null eintrag der l-ten Zeile an j-ter Stelle mit j-tem Eintrag in vektor
        //(lüge) TODO: die size von y wird intern durch die def vektormult angepasst, wir müssen also nichts mehr überprüfen
        //TODO: man müsste noch die size von x anpassen

        //wird aufgerufen, falls Zeile noch nicht verarbeitet
        if (a[entries[l].i]==false){

          y[entries[l].i] = entries[l].value * x[entries[l].j];

          //durchsuche ob andere Einträge ungleich 0 in zeile ex.
          for (std::size_t k = l+1; k<entries.size(); k++){
            if (entries[l].i == entries[k].i){

               y[entries[l].i] += entries[k].value*x[entries[k].j];
               a[entries[l].i]= true;

            }  
          }
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
  test_sparse.mv(result_test_sparse,vec);
  test_dense.mv(result_test_dense,vec);
  
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

  //(c) 
  const int n = 4;
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

  // see the following code in stackoverflow: (falls du auch gucken wolltest :D)
  // https://stackoverflow.com/questions/3220477/how-to-use-clock-in-c
  std::clock_t start;
  double duration;

  start = std::clock();

  //multiply
  dense.mv(result_dense,x);

  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout<<"duration of dense: "<< duration <<'\n';

  start = std::clock();

  //multiply
  sparse.mv(result_sparse,x);

  duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;

  std::cout<<"duration of sparse: "<< duration <<'\n';
  
  //plotten nils mach mal
}
