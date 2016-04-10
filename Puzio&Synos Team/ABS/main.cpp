#include <iostream>
#include "ActionData.h"

using namespace std;

int main()
{
    try
    {
        cout << "Odczyt oraz zapis do ABS \n\n" << endl;
        ActionData operacje;
        operacje.adres = "C:\\Users\\user\\Desktop\\ABS\\"; // ustawienie adresu katalogu gdzie sa pliki!!!!!
        operacje.readData(operacje.kola.tableFriction); // wczytanie statycznej tablicy


        // To ponizej to do destowania slipa czy dziala jak sauron przykazał
        float slip = 0.23;
        cout << operacje.kola.tableFri[4] << endl;
        cout << *operacje.choiceFriction(&slip) << endl;





       // operacje.readData(operacje.kola.dataSplit);
       // operacje.viewData(operacje.kola.dataSplit);






    }catch(const char* err)
    {
      cout << endl << err << endl << endl ;
    }
    //	system("pause");
    return 0;
}
