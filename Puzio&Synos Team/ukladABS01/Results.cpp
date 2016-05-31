#include <Results.h>

using namespace std;

Results::Results() {
}
Results::~Results() {
    brakingTorqueData.clear();
    stopDistanceData.clear();
    slipData.clear();
    wheelSpeedData.clear();
    vehicleSpeedData.clear();
}

void Results::slipDataSave(double slip) {
    slipData.push_back(slip);
}

void Results::distanceSave(double distance) {
    stopDistanceData.push_back(distance);
}

void Results::torqueBrakingSave(double brakingTorque) {
    brakingTorqueData.push_back(brakingTorque);
}

void Results::vehicleSpeedSave(double vehicleSpeed) {
    vehicleSpeedData.push_back(vehicleSpeed);
}

void Results::wheelSpeedSave(double wheelSpeed) {
    wheelSpeedData.push_back(wheelSpeed);
}

void Results::saveResult(QString location) {
    fstream plik;
    plik.open(location.toStdString(), ios::out);
    if (plik.good() == true) {
        for (int i = 0 ; i<slipData.size() ; i++) {
            plik << slipData[i] << " , " << wheelSpeedData[i] << " , " << vehicleSpeedData[i]  <<" , " << brakingTorqueData[i]  << " , " <<  stopDistanceData[i] <<"\n" ;
        }
        plik.close();
    }
}
