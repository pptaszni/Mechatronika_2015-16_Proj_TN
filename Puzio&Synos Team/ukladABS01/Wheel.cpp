#include <Wheel.h>

using namespace std;

Wheel::Wheel() { 
}

Wheel::~Wheel() {
    tableFrictionData.clear();
}

int Wheel::sign(double speedWheel) {
    if (speedWheel > 0) return 1;
    else if (speedWheel == 0) return 0;
    else
        return -1;
}

void Wheel::calculationTorqueAcceleration() {
    double Ftx = -(choiceFriction(actSlip)) * vehicleWeight * gravitation;
    double torque = tireTorque - sign(wheelSpeed) * torqueBraking;
    vehicleAcceleration = (Ftx / vehicleWeight);
    wheelAcceleration = torque / momentOfInertia;
}

double Wheel::integrationWheelSpeed() {
    double tempWheelSpeed;
    tempWheelSpeed = wheelSpeed + wheelAcceleration * samplingFrequency;
    wheelSpeed = tempWheelSpeed;
    if (wheelSpeed < 0) wheelSpeed = 0;
    double romega = wheelSpeed * diameterOfCircle;
    return romega;
}

double Wheel::integrationVehicleSpeed() {
    double tempVehicleSpeed;
    tempVehicleSpeed = vehicleSpeed + vehicleAcceleration * samplingFrequency;
    return tempVehicleSpeed;
}

void Wheel::calculationSlip() {
    double tempSpeedVehicle;
    double romega = integrationWheelSpeed();
    vehicleSpeed = integrationVehicleSpeed();
    if (romega > vehicleSpeed) tempSpeedVehicle = romega;
    else tempSpeedVehicle = vehicleSpeed;
    actSlip = (romega - vehicleSpeed) / tempSpeedVehicle;
    if (actSlip > 0) { actSlip = 0; }
    distanceToStop = distanceToStop + vehicleSpeed * samplingFrequency;
    results.wheelSpeedSave(romega);
    results.vehicleSpeedSave(vehicleSpeed);
    results.slipDataSave(actSlip);
    results.distanceSave(distanceToStop);
}

double Wheel::choiceFriction(double slip) {
    slip = -slip;
    if (slip < 0.05) return 0.2;
    if (0.05 <= slip && slip < 0.1) return tableFrictionData[1];
    if (0.1 <= slip && slip < 0.15) return tableFrictionData[2];
    if (0.15 <= slip && slip < 0.2) return tableFrictionData[3];
    if (0.2 <= slip && slip < 0.25) return tableFrictionData[4];
    if (0.25 <= slip && slip < 0.3) return tableFrictionData[5];
    if (0.3 <= slip && slip < 0.35) return tableFrictionData[6];
    if (0.35 <= slip && slip < 0.4) return tableFrictionData[7];
    if (0.4 <= slip && slip < 0.45) return tableFrictionData[8];
    if (0.45 <= slip && slip < 0.5) return tableFrictionData[9];
    if (0.5 <= slip && slip < 0.55) return tableFrictionData[10];
    if (0.55 <= slip && slip < 0.6) return tableFrictionData[11];
    if (0.6 <= slip && slip < 0.65) return tableFrictionData[12];
    if (0.65 <= slip && slip < 0.7) return tableFrictionData[13];
    if (0.7 <= slip && slip < 0.75) return tableFrictionData[14];
    if (0.75 <= slip && slip < 0.8) return tableFrictionData[15];
    if (0.8 <= slip && slip < 0.85) return tableFrictionData[16];
    if (0.85 <= slip && slip < 0.9) return tableFrictionData[17];
    if (0.9 <= slip && slip < 0.95) return tableFrictionData[18];
    if (0.95 <= slip && slip < 1) return tableFrictionData[19];
    else
        return 0.7;
}

void Wheel::readDataFriction(QString location) {
    fstream plik;
    plik.open(location.toStdString(), ios::in | ios::out);
    if ( plik.good() == true ) {
        string temp2;
        while (!plik.eof()) {
            getline(plik , temp2);
            tableFrictionData.push_back(atof(temp2.c_str()));
        }
        plik.close();
    } else {
        throw "Error! You can not play the file!";
    }
}

void Wheel::kutrapaliFunction() {
    double mu = choiceFriction(actSlip);
    double tempTorque;
    tempTorque = fabs(mu * diameterOfCircle * gravitation * vehicleWeight);
    if ((tempTorque - tireTorque) > 18) tireTorque += 18;
    else if ((tempTorque - tireTorque) < 18) tireTorque -= 18;
    else (tireTorque = tempTorque);
}

void Wheel::bangBang() {
    double slipError = absControl();
    if (slipError > 0.017) {
        torqueBraking = torqueBraking - 20;
    } else
        if (slipError < 0.017) {
            torqueBraking = torqueBraking + 20;
        }
    results.torqueBrakingSave(torqueBraking);
}

double Wheel::absControl() {
    double desiredSlip = -0.2;
    return (desiredSlip - actSlip);
}

QString Wheel::doCalculations() {
    if (vehicleSpeed == 0 && vehicleWeight == 0 && diameterOfCircle == 0) {
        return "Nie podano parametrow";
    } else {
        setInitialParameters();
        while (wheelSpeed) {
            calculationTorqueAcceleration();
            calculationSlip();
            kutrapaliFunction();
            bangBang();
            numberOfIterations++;
        }
        return "Obliczenia zostały wykonane!";
    }
}

void Wheel::setInitialParameters() {
    wheelSpeed = (vehicleSpeed / diameterOfCircle);
    distanceToStop = 0;
}

