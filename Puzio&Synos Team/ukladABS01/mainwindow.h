#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Wheel.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_horizontalSlider_PredkoscSamochodu_valueChanged(int value);

    void on_horizontalSlider_MasaNaKolo_valueChanged(int value);

    void on_horizontalSlider_SrednicaKola_valueChanged(int value);

    void on_pushButton_SYMULUJ_clicked();

    void on_actionO_programie_triggered();

    void on_actionJak_korzysta_triggered();

private:
    Ui::MainWindow *ui;
    void inicjalizacja_symulacji();
};

#endif // MAINWINDOW_H
