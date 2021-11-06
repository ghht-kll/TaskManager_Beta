#ifndef CPULOAD_H
#define CPULOAD_H

#include <QObject>
#include <QFile>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QThread>
#include "Path.h"

class CpuLoad : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    explicit CpuLoad(QObject *parent = nullptr);
    bool running() const;
    void setRunning(bool newRunning);

    QVector<unsigned int> getCpuLoad();
    unsigned int getCoresCount() const;
    ~CpuLoad();
    bool m_running;
signals:
    void runningChanged();
    void finished();
    void sendCpuLoad(QVector<unsigned int> vec);

public slots:
    void run();

private:
    void cpuinfoRead();
    void coresCount();
    void readCpuStats(QVector<QVector<unsigned int>> &vec);

private:
    unsigned int cores;
    QFile file;
    QString cpuinfoStr;
    QVector<QVector<unsigned int>> rawDaraFirst;
    QVector<QVector<unsigned int>> rawDaraSecond;
    QVector<unsigned int> cpuLoad;
    QString stat = "/proc/stat";
    QString cpuinfo = "/proc/cpuinfo";
    QString hostName = "/proc/sys/kernel/hostname";
};

#endif // CPULOAD_H
