#ifndef RESULTS_H
#define RESULTS_H

#include <QVector>
#include <fstream>

class Results
{
public:
    Results();
    ~Results();
    void saveResult(QString location);
    void distanceSave(double distance);
    void torqueBrakingSave(double brakingTorque);
    void slipDataSave(double slip);
    void wheelSpeedSave(double wheelSpeed);
    void vehicleSpeedSave(double vehicleSpeed);
    QVector<double> brakingTorqueData;
    QVector<double> stopDistanceData;
    QVector<double> slipData;
    QVector<double> wheelSpeedData;
    QVector<double> vehicleSpeedData;
};

#endif // RESULTS_H
