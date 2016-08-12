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
    CFPrint::instance().discoverDevices(_vec_devices);

    ui->cbDevices->clear();
    for(auto i : _vec_devices)
    {
        ui->cbDevices->addItem(i.getFullName().c_str(), QVariant(i.getId()));
    }

    return true;

}

void MainWindow::on_cbDevices_currentIndexChanged(int index)
{
    (void)index;

    uint16_t curId = ui->cbDevices->currentData().toUInt();
    for(auto d : _vec_devices)
    {
        if(d.getId() == curId)
        {
            ui->lblFullName->setText(d.getFullName().c_str());
            ui->lblName->setText(d.getName().c_str());
            ui->lblId->setText(QString("%1").arg(d.getId()));
            break;
        }
    }
}
