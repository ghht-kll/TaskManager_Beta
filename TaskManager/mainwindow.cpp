#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->chartCpuLoad = this->createChart(this->cpuLoad.getCoresCount(), "CPU load");
    this->chartMemoryLoad = this->createChart(1, "Memory load");

    this->processorNameLabel = new QLabel("Processor: " + this->systemInfo.getProcessorName());
    this->hostNameLabel = new QLabel("HostName: " + this->systemInfo.getHostName());
    this->memInfoLabel = new QLabel();

    QChartView *chartViewCpu = new QChartView(this->chartCpuLoad);
    QChartView *chartViewMemory = new QChartView(this->chartMemoryLoad);
    chartViewCpu->setRenderHint(QPainter::Antialiasing);
    chartViewMemory->setRenderHint(QPainter::Antialiasing);
    this->ui->gridLayout->addWidget(this->processorNameLabel);
    this->ui->gridLayout->addWidget(this->hostNameLabel);
    this->ui->gridLayout->addWidget(chartViewCpu);
    this->ui->gridLayout->addWidget(chartViewMemory);
    this->ui->gridLayout->addWidget(this->memInfoLabel);

    this->threadCpu = new QThread(this);
    this->threadMemory = new QThread(this);

    this->connectSetting();

    this->cpuLoad.moveToThread(this->threadCpu);
    this->cpuLoad.setRunning(true);
    this->threadCpu->start();

    this->memoryLoad.moveToThread(this->threadMemory);
    this->memoryLoad.setRunning(true);
    this->threadMemory->start();

    this->labelStyleSetting();
}

MainWindow::~MainWindow()
{
    delete this->chartCpuLoad;
    delete this->chartMemoryLoad;
    delete this->processorNameLabel;
    delete this->hostNameLabel;
    this->cpuLoad.m_running = false;
    this->memoryLoad.m_running = false;

   // this->threadCpu->wait();
   // this->threadMemory->wait();

    delete this->threadCpu;
    delete this->threadMemory;

    delete ui;
}

void MainWindow::updateMessageMemory(QString stat)
{
    this->memInfoLabel->setText(stat);
}

Chart *MainWindow::createChart(unsigned int lineSeries, QString title)
{
    Chart *chart = new Chart;
    chart->createLineSeries(lineSeries);
    chart->setTitle(title);
    chart->legend()->hide();
    chart->setAnimationOptions(QChart::NoAnimation);

    return chart;
}

void MainWindow::labelStyleSetting()
{
    QFile file(":/LabelStyle.css");
    file.open(QFile::ReadOnly);

    QString style = file.readAll();

    file.close();

    this->processorNameLabel->setStyleSheet(style);
    this->hostNameLabel->setStyleSheet(style);
    this->memInfoLabel->setStyleSheet(style);
}

void MainWindow::connectSetting()
{
    connect(this, &MainWindow::destroyed, this->threadCpu, &QThread::quit);
    connect(this->threadCpu, &QThread::started, &this->cpuLoad, &CpuLoad::run);
    connect(&this->cpuLoad, &CpuLoad::finished, this->threadCpu, &QThread::terminate);
    connect(&this->cpuLoad, &CpuLoad::sendCpuLoad, this->chartCpuLoad, qOverload<QVector<unsigned int>>(&Chart::updateChart));

    connect(this, &MainWindow::destroyed, this->threadMemory, &QThread::quit);
    connect(this->threadMemory, &QThread::started, &this->memoryLoad, &MemoryLoad::run);
    connect(&this->memoryLoad, &MemoryLoad::finished, this->threadMemory, &QThread::terminate);
    connect(&this->memoryLoad, &MemoryLoad::sendUsedMemory, this->chartMemoryLoad, qOverload<float>(&Chart::updateChart));

    connect(&this->memoryLoad, &MemoryLoad::status, this, &MainWindow::updateMessageMemory);
}

