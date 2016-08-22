#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fprintpp.h"
#include <iostream>
#include <QMessageBox>
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
    bool bOk;
    size_t nDev = ui->cbDevices->currentData().toUInt(&bOk);
    if(!bOk)
        return;

    CFpDevice dev = _devs->get_device(nDev);
    int res;

    CFpImage imgCap = dev.img_capture(0,res);

    if(0 != res)
    {
        QMessageBox::information(this, "Mensagem", "Erro obtendo imagem");
        return;
    }

    // Padronizamos a imagem:
    CFpImage imgStd(imgCap);
    imgCap.standardize();
    CFpImage imgBin = imgCap.binarize();

    ui->lbCap->setPixmap(QPixmap::fromImage(imgCap));
    ui->lbStd->setPixmap(QPixmap::fromImage(imgStd));
    ui->lbBin->setPixmap(QPixmap::fromImage(imgBin));

}

bool MainWindow::findDevices()
{
    _devs = CFPrint::instance().discoverDevices();

    if(_devs->empty())
    {
        QMessageBox::information(this, "Atenção", "Nenhum dispositivo de leitura biométrica foi encontrado no sistema.");
    }

    ui->cbDevices->clear();
    size_t nSize = _devs->size();
    for(size_t i = 0; i < nSize; i++)
    {
        CFpDriver drv = _devs->get_driver(i);
        ui->cbDevices->addItem(drv.getFullName().c_str(), QVariant( (uint) i) );
    }

    emit on_refresh_device();

    return true;

}

void MainWindow::on_cbDevices_currentIndexChanged(int index)
{
    (void)index;
    emit on_refresh_device();
}

void MainWindow::on_refresh_device()
{
    bool bOk;
    uint16_t i = ui->cbDevices->currentData().toUInt(&bOk);
    if(!bOk)
        return;

    CFpDriver drv = _devs->get_driver(i);
    ui->lblFullName->setText(drv.getFullName().c_str());
    ui->lblName->setText(drv.getName().c_str());
    ui->lblId->setText(QString("%1").arg(drv.getId()));

}
