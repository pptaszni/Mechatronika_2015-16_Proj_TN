#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "Wheel.h"

using namespace std;

class ActionData
{
   public:
            string adres;
            Wheel kola;
   public:
        ActionData()
         {
         }

        ~ActionData()
        {
        }

        void viewData(string location)
        {
            if(location == "dataSplit")
            {
                    if(kola.dataSplitVector.empty()==true)
                    {
                        throw "Brak Danych!!! Pusty wektor z dnaymi!!";
                    }else{
                        cout<<"Zawartosc odczytano z : "<<adres<<":"<<endl;
                        for(size_t i=0; i<kola.dataSplitVector.size(); i++)
                        {
                            cout<<kola.dataSplitVector[i]<<"\n"<<endl;
                        }
                    }
            }

            if(location == "wheelSpeed")
            {
                    if(kola.wheelSpeedVector.empty()==true)
                    {
                        throw "Brak Danych!!! Pusty wektor z dnaymi!!";
                    }else{
                        cout<<"Zawartosc odczytano z : "<<adres<<":"<<endl;
                        for(size_t i=0; i<kola.wheelSpeedVector.size(); i++)
                        {
                            cout<<kola.wheelSpeedVector[i]<<"\n"<<endl;
                        }
                    }
            }

            if(location == "carSpeed")
            {
                    if(kola.carSpeedVector.empty()==true)
                    {
                        throw "Brak Danych!!! Pusty wektor z dnaymi!!";
                    }else{
                        cout<<"Zawartosc odczytano z : "<<adres<<":"<<endl;
                        for(size_t i=0; i<kola.carSpeedVector.size(); i++)
                        {
                            cout<<kola.carSpeedVector[i]<<"\n"<<endl;
                        }
                    }
            }

            if(location == "torqueOnWheel")
            {
                    if(kola.torqueOnWheelVector.empty()==true)
                    {
                        throw "Brak Danych!!! Pusty wektor z dnaymi!!";
                    }else{
                        cout<<"Zawartosc odczytano z : "<<adres<<":"<<endl;
                        for(size_t i=0; i<kola.torqueOnWheelVector.size(); i++)
                        {
                            cout<<kola.torqueOnWheelVector[i]<<"\n"<<endl;
                        }
                    }
            }

            if(location == "frictionForceOnWheel")
            {
                    if(kola.frictionForceOnWheelVector.empty()==true)
                    {
                        throw "Brak Danych!!! Pusty wektor z dnaymi!!";
                    }else{
                        cout<<"Zawartosc odczytano z : "<<adres<<":"<<endl;
                        for(size_t i=0; i<kola.frictionForceOnWheelVector.size(); i++)
                        {
                            cout<<kola.frictionForceOnWheelVector[i]<<"\n"<<endl;
                        }
                    }
            }

        }

        void readData(string location)
        {      
            if(location == "dataSplit")
            {
                    fstream plik;
                    plik.open(adres+location+".txt", ios::in | ios::out);
                    if( plik.good() == true )
                    {
                        string temp2;
                        while( !plik.eof() )
                        {
                            getline( plik, temp2);
                            kola.dataSplitVector.push_back(atof(temp2.c_str()));
                        }
                    plik.close();
                    cout << "Wczytano plik !!!\n" << endl;
                    } else throw "Error! Nie udalo otworzyc sie pliku!";
            }

            if(location == "wheelSpeed")
            {
                    fstream plik;
                    plik.open(adres+location+".txt", ios::in | ios::out);
                    if( plik.good() == true )
                    {
                        string temp2;
                        while( !plik.eof() )
                        {
                            getline( plik, temp2);
                            kola.wheelSpeedVector.push_back(atof(temp2.c_str()));
                        }
                    plik.close();
                    cout << "Wczytano plik !!!\n" << endl;
                    } else throw "Error! Nie udalo otworzyc sie pliku!";
            }

            if(location == "carSpeed")
            {
                    fstream plik;
                    plik.open(adres+location+".txt", ios::in | ios::out);
                    if( plik.good() == true )
                    {
                        string temp2;
                        while( !plik.eof() )
                        {
                            getline( plik, temp2);
                            kola.carSpeedVector.push_back(atof(temp2.c_str()));
                        }
                    plik.close();
                    cout << "Wczytano plik !!!\n" << endl;
                    } else throw "Error! Nie udalo otworzyc sie pliku!";
            }

            if(location == "torqueOnWheel")
            {
                    fstream plik;
                    plik.open(adres+location+".txt", ios::in | ios::out);
                    if( plik.good() == true )
                    {
                        string temp2;
                        while( !plik.eof() )
                        {
                            getline( plik, temp2);
                            kola.torqueOnWheelVector.push_back(atof(temp2.c_str()));
                        }
                    plik.close();
                    cout << "Wczytano plik !!!\n" << endl;
                    } else throw "Error! Nie udalo otworzyc sie pliku!";
            }

            if(location == "frictionForceOnWheel")
            {
                    fstream plik;
                    plik.open(adres+location+".txt", ios::in | ios::out);
                    if( plik.good() == true )
                    {
                        string temp2;
                        while( !plik.eof() )
                        {
                            getline( plik, temp2);
                            kola.frictionForceOnWheelVector.push_back(atof(temp2.c_str()));
                        }
                    plik.close();
                    cout << "Wczytano plik !!!\n" << endl;
                    } else throw "Error! Nie udalo otworzyc sie pliku!";
            }
            if(location == "tableFriction")
            {
                    fstream plik;
                    plik.open(adres+location+".txt", ios::in | ios::out);
                    if( plik.good() == true )
                    {
                        string temp2;
                        int i =0;
                        while( !plik.eof() )
                        {
                            getline( plik, temp2);
                            kola.tableFri[i] = atof(temp2.c_str());
                            i++;
                        }
                    plik.close();
                    cout << "Wczytano plik !!!\n" << endl;
                    } else throw "Error! Nie udalo otworzyc sie pliku!";
            }
        }

        void writeData(string location)
        {

            if(location == "dataSplit")
            {
                fstream plik;
                plik.open(adres+location+".txt", ios::out);
                if( plik.good() == true )
                {
                    for(size_t i=0 ; i<kola.dataSplitVector.size() ; i++)
                    {
                         plik << kola.dataSplitVector[i] << "\n" ;
                    }
                    plik.close();
                }else throw "Error! Nie udalo zapisac sie pliku!" ;
            }

            if(location == "wheelSpeed")
            {
                fstream plik;
                plik.open(adres+location+".txt", ios::out);
                if( plik.good() == true )
                {
                    for(size_t i=0 ; i<kola.wheelSpeedVector.size() ; i++)
                    {
                         plik << kola.wheelSpeedVector[i] << "\n" ;
                    }
                    plik.close();
                }else throw "Error! Nie udalo zapisac sie pliku!" ;
            }

            if(location == "carSpeed")
            {
                fstream plik;
                plik.open(adres+location+".txt", ios::out);
                if( plik.good() == true )
                {
                    for(size_t i=0 ; i<kola.carSpeedVector.size() ; i++)
                    {
                         plik << kola.carSpeedVector[i] << "\n" ;
                    }
                    plik.close();
                }else throw "Error! Nie udalo zapisac sie pliku!" ;
            }

            if(location == "torqueOnWheel")
            {
                fstream plik;
                plik.open(adres+location+".txt", ios::out);
                if( plik.good() == true )
                {
                    for(size_t i=0 ; i<kola.torqueOnWheelVector.size() ; i++)
                    {
                         plik << kola.torqueOnWheelVector[i] << "\n" ;
                    }
                    plik.close();
                }else throw "Error! Nie udalo zapisac sie pliku!" ;
            }

            if(location == "frictionForceOnWheel")
            {
                fstream plik;
                plik.open(adres+location+".txt", ios::out);
                if( plik.good() == true )
                {
                    for(size_t i=0 ; i<kola.frictionForceOnWheel.size() ; i++)
                    {
                         plik << kola.frictionForceOnWheelVector[i] << "\n" ;
                    }
                    plik.close();
                }else throw "Error! Nie udalo zapisac sie pliku!" ;
            }

        }

        float* choiceFriction(float* slip) //zwraca wzkaznik do elementu o wartosci tego współczynika tarcia
        {
            if (*slip < 0.05) return &kola.tableFri[0];
            if (0.05 <= *slip && *slip < 0.1) return &kola.tableFri[1];
            if (0.1 <= *slip && *slip< 0.15) return &kola.tableFri[2];
            if (0.15 <= *slip && *slip < 0.2) return &kola.tableFri[3];
            if (0.2 <= *slip && *slip < 0.25) return &kola.tableFri[4];
            if (0.25 <= *slip && *slip < 0.3) return &kola.tableFri[5];
            if (0.3 <= *slip && *slip < 0.35) return &kola.tableFri[6];
            if (0.35 <= *slip && *slip < 0.4) return &kola.tableFri[7];
            if (0.4 <= *slip && *slip < 0.45) return &kola.tableFri[8];
            if (0.45 <= *slip && *slip < 0.5) return &kola.tableFri[9];
            if (0.5 <= *slip && *slip < 0.55) return &kola.tableFri[10];
            if (0.55 <= *slip && *slip < 0.6) return &kola.tableFri[11];
            if (0.6 <= *slip && *slip < 0.65) return &kola.tableFri[12];
            if (0.65 <= *slip && *slip < 0.7) return &kola.tableFri[13];
            if (0.7 <= *slip && *slip < 0.75) return &kola.tableFri[14];
            if (0.75 <= *slip && *slip < 0.8) return &kola.tableFri[15];
            if (0.8 <= *slip && *slip < 0.85) return &kola.tableFri[16];
            if (0.85 <= *slip && *slip < 0.9) return &kola.tableFri[17];
            if (0.9 <= *slip && *slip < 0.95) return &kola.tableFri[18];
            if (0.95 <= *slip && *slip < 1) return &kola.tableFri[19];
            else return 0;
        }
};


