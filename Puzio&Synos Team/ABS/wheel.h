#pragma once
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class Wheel
{
   public:
        int nrWheel;
        float radiusWheel;
        float massOnWheel;
        const string dataSplit = "dataSplit";
        const string carSpeed = "carSpeed";
        const string torgueOnWheel = "torgueOnWheel ";
        const string frictionForceOnWheel ="frictionForceOnWheel";
        const string wheelSpeed = "wheelSpeed";
        const string tableFriction = "tableFriction";


        float tableFri[21]; // tablica statycznie wczytywana
        vector <float> wheelSpeedVector;
        vector <float> carSpeedVector;
        vector <float> torqueOnWheelVector;
        vector <float> frictionForceOnWheelVector;
        vector <float> dataSplitVector;
   public:
        Wheel()
         {
         }

        ~Wheel()
        {
            wheelSpeedVector.clear();
            carSpeedVector.clear();
            torqueOnWheelVector.clear();
            frictionForceOnWheelVector.clear();
            dataSplitVector.clear();
        }

};


