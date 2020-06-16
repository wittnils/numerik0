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
  hdnum::SparseMatrix<float> sparse;
  //sparse.AddEntry(1,1,0);
  //should terminate
  sparse.AddEntry(0,1,2);
  sparse.AddEntry(0,0,4);
  sparse.AddEntry(1,0,4);
  sparse.AddEntry(1,2,3);
  sparse.AddEntry(2,1,1);
  /*Matrix der Form:
  [4][2][0]
  [4][0][3]
  [0][1][0]
  */

  hdnum::DenseMatrix<float> dense(3,3);
  dense[0][0]= 4;
  dense[0][1]=2;
  dense[1][0]=4;
  dense[1][2] = 3;
  dense[2][1]=1;
  
  //sparse.AddEntry(5,8,10);
  hdnum::Vector<float> vec(3,1);
  vec[1] = 2;
  /*vec der Form:
  [1]
  [2]
  [1]
  */

  hdnum::Vector<float> result_sparse(3,0);
  hdnum::Vector<float> result_dense(3,0);

  //multiply both
  /*
  [4][2][0]   [1]
  [4][0][3] * [2]
  [0][1][0]   [1]
  */
  sparse.mv(result_sparse,vec);
  dense.mv(result_dense,vec);
  
  //outstream
  std::cout<< "result dense: " << result_dense << std::endl;
  std::cout<< "result sparse: " << result_sparse << std::endl;
  //passed

  //check equal result:
  if (result_sparse == result_dense){
    std::cout << "Operation was successful!" << std::endl;
  }
  else{
    std::cout << "result was not correct" << std::endl;
  }
  

  
  //plotten nils mach mal
}
