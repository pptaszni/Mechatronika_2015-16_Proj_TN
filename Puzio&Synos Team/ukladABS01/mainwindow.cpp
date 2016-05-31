#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Wheel.h"
#include "dialog.h"

Wheel test;
int counter = 0;
double speedCar = 30;
double massPerWheel = 350;
double diameterCircle = 0.33;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label_KONSOLA->setStyleSheet("QLabel { color : white; }");

    ui->label_MasaNaKolo->setStyleSheet("QLabel { color : white; }");
    ui->label_MasaNaKoloValue->setStyleSheet("QLabel { color : white; }");
    ui->label_PredkoscSamochodu->setStyleSheet("QLabel { color : white; }");
    ui->label_PredkoscSamochoduValue->setStyleSheet("QLabel { color : white; }");
    ui->label_SrednicaKola->setStyleSheet("QLabel { color : white; }");
    ui->label_SrednicaKolaValue->setStyleSheet("QLabel { color : white; }");
    ui->label->setStyleSheet("QLabel { color : white; }");

    QPixmap ABS("ABS2.jpg");
    ui->label_ABS->setPixmap(ABS);

    //ustawianie domyslnych wartosci danych wejsciowych
    ui->progressBar_MasaNaKolo->setValue(350);
    ui->progressBar_PredkoscSamochodu->setValue(30);
    ui->progressBar_SrednicaKola->setValue(33);

    ui->horizontalSlider_MasaNaKolo->setValue(350);
    ui->horizontalSlider_PredkoscSamochodu->setValue(30);
    ui->horizontalSlider_SrednicaKola->setValue(33);

    ui->label_MasaNaKoloValue->setText("350");
    ui->label_PredkoscSamochoduValue->setText("30");
    ui->label_SrednicaKolaValue->setText("33");

    QPixmap bkgnd("ABS6.jpg");
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_horizontalSlider_PredkoscSamochodu_valueChanged(int value) {
    ui->progressBar_PredkoscSamochodu->setValue(value);
    ui->label_PredkoscSamochoduValue->setText(QString::number(value));
    speedCar = (double)value;
}



void MainWindow::on_horizontalSlider_MasaNaKolo_valueChanged(int value) {
    ui->progressBar_MasaNaKolo->setValue(value);
    ui->label_MasaNaKoloValue->setText(QString::number(value));
    massPerWheel = (double)value;
}



void MainWindow::on_horizontalSlider_SrednicaKola_valueChanged(int value) {
    ui->progressBar_SrednicaKola->setValue(value);
    ui->label_SrednicaKolaValue->setText(QString::number(value));
    double x = (double)value;
    double y = x / 100;
    diameterCircle = y;
}


void MainWindow::inicjalizacja_symulacji() {
    test.results.brakingTorqueData.clear();
    test.results.stopDistanceData.clear();
    test.results.slipData.clear();
    test.results.wheelSpeedData.clear();
    test.results.vehicleSpeedData.clear();

    test.vehicleSpeed = speedCar;
    test.vehicleWeight = massPerWheel;
    test.diameterOfCircle = diameterCircle;
}



void MainWindow::on_pushButton_SYMULUJ_clicked()
{
    //kod glowny
    try{
        MainWindow::inicjalizacja_symulacji();

        test.readDataFriction("dataSplit.txt");
        QString statement; // zmienna do komunikatu ze zakonczono liczenie i mozna odczytac dane
        statement = test.doCalculations(); // wywolanie i przeprowadzenie obliczen

        test.results.saveResult("ABS_SlipQT.csv"); // zapisanie danych w pliku csv do podanej lokalizacji o podanej nazwie

        ui->label_KONSOLA->setText(statement);

        QVector<double> x;
        for (int i = 0 ; i < test.numberOfIterations ; ++i) {
            x.push_back((i));
        }

        //WYKRES PRESKOSCI
        ui->customPlot->addGraph();
        ui->customPlot->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
        ui->customPlot->addGraph();
        ui->customPlot->graph(1)->setPen(QPen(Qt::red));
        ui->customPlot->xAxis2->setVisible(true);
        ui->customPlot->xAxis2->setTickLabels(false);
        ui->customPlot->yAxis2->setVisible(true);
        ui->customPlot->yAxis2->setTickLabels(false);
        ui->customPlot->xAxis->setLabel("próbka");
        ui->customPlot->yAxis->setLabel("prędkość pojazdu [m/s]");
        ui->customPlot->graph(0)->setData(x, test.results.vehicleSpeedData);
        ui->customPlot->graph(1)->setData(x, test.results.wheelSpeedData);
        ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->customPlot->rescaleAxes();

        ui->customPlot->replot();

        //WYKRES POSLIZGU
        ui->customPlot2->addGraph();
        ui->customPlot2->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot2->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
        ui->customPlot2->xAxis2->setVisible(true);
        ui->customPlot2->xAxis2->setTickLabels(false);
        ui->customPlot2->yAxis2->setVisible(true);
        ui->customPlot2->yAxis2->setTickLabels(false);
        ui->customPlot2->xAxis->setLabel("próbka");
        ui->customPlot2->yAxis->setLabel("wartość poślizgu");
        ui->customPlot2->graph(0)->setData(x, test.results.slipData);
        ui->customPlot2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->customPlot2->rescaleAxes();
        ui->customPlot2->replot();

        //WYKRES DROGI
        ui->customPlot3->addGraph();
        ui->customPlot3->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot3->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
        ui->customPlot3->xAxis2->setVisible(true);
        ui->customPlot3->xAxis2->setTickLabels(false);
        ui->customPlot3->yAxis2->setVisible(true);
        ui->customPlot3->yAxis2->setTickLabels(false);
        ui->customPlot3->xAxis->setLabel("próbka");
        ui->customPlot3->yAxis->setLabel("droga pokonana przez pojazd [m]");
        ui->customPlot3->graph(0)->setData(x, test.results.stopDistanceData);
        ui->customPlot3->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->customPlot3->rescaleAxes();
        ui->customPlot3->replot();

        //WYKRES MOMENTU
        ui->customPlot4->addGraph();
        ui->customPlot4->graph(0)->setPen(QPen(Qt::blue));
        ui->customPlot4->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
        ui->customPlot4->xAxis2->setVisible(true);
        ui->customPlot4->xAxis2->setTickLabels(false);
        ui->customPlot4->yAxis2->setVisible(true);
        ui->customPlot4->yAxis2->setTickLabels(false);
        ui->customPlot4->xAxis->setLabel("próbka");
        ui->customPlot4->yAxis->setLabel("wartość momentu hamującego [Nm]");
        ui->customPlot4->graph(0)->setData(x, test.results.brakingTorqueData);
        ui->customPlot4->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
        ui->customPlot4->rescaleAxes();
        ui->customPlot4->replot();

        counter++;
    } catch(QString message) {
        ui->label_KONSOLA->setText(message);
    }
}

void MainWindow::on_actionO_programie_triggered() {
    QApplication::aboutQt();
}

void MainWindow::on_actionJak_korzysta_triggered() {
    Dialog*o = new Dialog();
    o->show();
}
