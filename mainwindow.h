#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "fprintpp.h"
#include <vector>
#include <string>

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
    void on_btnScan_clicked();

    void on_cbDevices_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    vec_fpdevice_t _vec_devices;


private:
    bool findDevices(void);
};

#endif // MAINWINDOW_H
