#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <fprint.h>
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

private:
    Ui::MainWindow *ui;

    using vec_devices_t = std::vector<std::string>;

    vec_devices_t _vec_devices;

private:
    bool findDevices(void);
};

#endif // MAINWINDOW_H
