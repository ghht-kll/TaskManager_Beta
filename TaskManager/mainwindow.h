#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QChart>
#include <QChartView>
#include <QLineSeries>
#include <QThread>
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <chart.h>
#include "cpuload.h"
#include "memoryload.h"
#include "systeminfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateMessageMemory(QString stat);

private:
    Chart *createChart(unsigned int lineSeries, QString title);
    void labelStyleSetting();
    void connectSetting();
private:
    Ui::MainWindow *ui;
    Chart *chartCpuLoad;
    Chart *chartMemoryLoad;

    QLabel *processorNameLabel;
    QLabel *hostNameLabel;
    QLabel *memInfoLabel;

    CpuLoad cpuLoad;
    MemoryLoad memoryLoad;
    SystemInfo systemInfo;

    QThread *threadCpu;
    QThread *threadMemory;
};
#endif // MAINWINDOW_H
