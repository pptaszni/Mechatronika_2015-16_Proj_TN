#ifndef WHEEL_H
#define WHEEL_H

#include <math.h>
#include <Results.h>
#include <QString>
#include <cstdlib>
#include <QTextStream>
#include <QApplication>

class Wheel
{
public: 
    double vehicleSpeed = 15;
    double vehicleWeight = 350;
    double diameterOfCircle = 0.33;
    double torqueBraking = 35;
    Results results;
    Wheel();
    ~Wheel();
    QString doCalculations();
    void readDataFriction(QString location);
    int numberOfIterations = 0;

private:
    QVector<double> tableFrictionData;
    double samplingFrequency = 0.01;
    double gravitation = 9.81; // m/s^2
    double actSlip = 0;
    double momentOfInertia = 1.33;
    double vehicleAcceleration;
    double wheelAcceleration;
    double wheelSpeed;
    double tireTorque = 0;
    double distanceToStop;
    int sign(double speedWheel);
    void calculationTorqueAcceleration();
    double integrationWheelSpeed();
    double integrationVehicleSpeed();
    void calculationSlip();
    double choiceFriction(double slip);
    void kutrapaliFunction();
    void bangBang();
    double absControl();
    void setInitialParameters();
};

#endif // WHEEL_H
