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
      //Stelle ist schon vorhanden?
      for (std::size_t k = 0; k<entries.size(); k++){
            if (i == entries[k].i && j==entries[k].j){
               entries[k].value = value;
               return;
            }  
      }
      //passe größe von entries an
      entries.resize(entries.size()+ 1);
      //füge wert an ensprechender zeile und spalte an
      entries[entries.size()-1].i=i;
      entries[entries.size()-1].j=j;
      entries[entries.size()-1].value = value;

    }

    template<typename V>
    void mv (Vector<V>& y, const Vector<V>& x) {
      //array on false
      bool a[entries.size()] = {0};
      
      for (std::size_t l = 0; l<entries.size(); l++){
        //multipliziere nicht null eintrag der l-ten Zeile an j-ter Stelle mit j-tem Eintrag in vektor
        //(lüge) TODO: die size von y wird intern durch die def vektormult angepasst, wir müssen also nichts mehr überprüfen
        //TODO: man müsste noch die size von x anpassen
        //std::cout<< entries.size()<< std::endl;
        //std::cout<< entries[l].value<< std::endl;
        std::cout<< a[entries[l].i]<< std::endl;
        std::cout<< "ende"<< std::endl;
        if (a[entries[l].i]==false){
          y[entries[l].i] = entries[l].value * x[entries[l].j];
          //std::cout<< y[entries[l].i]<< std::endl;
          //durchsuche ob andere Einträge ungleich 0 in zeile ex.
          for (std::size_t k = l+1; k<entries.size(); k++){
            if (entries[l].i == entries[k].i){
              std::cout << "hi"<< std::endl;
              //std::cout<< a[entries[l].i]<< std::endl;
               y[entries[l].i] += entries[k].value*x[entries[k].j];
               a[entries[l].i]= true;
              //std::cout <<entries[k].value << std::endl; 
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
  //test
  hdnum::SparseMatrix<float> sparse;
  sparse.AddEntry(0,0,1);
  sparse.AddEntry(0,1,2);
  sparse.AddEntry(0,0,4);
  sparse.AddEntry(1,0,4);

  hdnum::DenseMatrix<float> dense(2,2);
  dense[0][0]= 4;
  dense[0][1]=2;
  dense[1][0]=4;
  
  //sparse.AddEntry(5,8,10);
  hdnum::Vector<float> vec(2,1);
  hdnum::Vector<float> result_sparse(2,0);
  hdnum::Vector<float> reslut_dense(2,0);

  //multiply both
  sparse.mv(result_sparse,vec);
  dense.mv(reslut_dense,vec);
  
  //outstream
  std::cout<< "result dense: " << reslut_dense << std::endl;
  std::cout<< "result sparse: " << result_sparse << std::endl;
  //passed
  
  //plotten nils mach mal
}
