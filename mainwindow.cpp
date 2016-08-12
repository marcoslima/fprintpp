#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fprint.h>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fp_init();

    // Find devices:
    findDevices();

}

MainWindow::~MainWindow()
{
    delete ui;

    fp_exit();
}

void MainWindow::on_btnScan_clicked()
{

}

bool MainWindow::findDevices()
{
    struct fp_dscv_dev** pdevs = fp_discover_devs();
    cout << "pdevs: " << pdevs << endl;
    struct fp_dscv_dev** pCurrent = pdevs;
    while(*pCurrent != nullptr)
    {
        struct fp_driver* pDriver = fp_dscv_dev_get_driver(*pCurrent);
        pCurrent++;

        _vec_devices.push_back(string(fp_driver_get_full_name(pDriver)));
    }
    // Liberamos:
    fp_dscv_devs_free(pdevs);

    ui->cbDevices->clear();
    for(auto i : _vec_devices)
    {
        cout << i << endl;
        ui->cbDevices->addItem(i.c_str());
    }

    return true;

}
