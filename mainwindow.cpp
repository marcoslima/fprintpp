#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fprintpp.h"
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Find devices:
    findDevices();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnScan_clicked()
{

}

bool MainWindow::findDevices()
{
    _devs = CFPrint::instance().discoverDevices();

    ui->cbDevices->clear();
    size_t nSize = _devs->size();
    for(size_t i = 0; i < nSize; i++)
    {
        CFpDriver drv = _devs->get_driver(i);
        ui->cbDevices->addItem(drv.getFullName().c_str(), QVariant(drv.getId()));
    }

    return true;

}

void MainWindow::on_cbDevices_currentIndexChanged(int index)
{
    (void)index;

    uint16_t curId = ui->cbDevices->currentData().toUInt();
    size_t nSize = _devs->size();
    for(size_t i = 0; i < nSize; i++)
    {
        if(_devs->get_driver(i).getId() == curId)
        {
            CFpDriver drv = _devs->get_driver(i);
            ui->lblFullName->setText(drv.getFullName().c_str());
            ui->lblName->setText(drv.getName().c_str());
            ui->lblId->setText(QString("%1").arg(drv.getId()));
            break;
        }
    }
}
