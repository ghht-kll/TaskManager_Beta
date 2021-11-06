#include "memoryload.h"
#include <QDebug>

MemoryLoad::MemoryLoad(QObject *parent) : QObject(parent)
{
    this->file.open("/proc/meminfo");
    this->memTotal = this->getMemTotalMiB();
}

float MemoryLoad::getUsedMemoryMiB()
{
    this->file.seekg(0);
    float val {0};
    std::string line;
    std::getline(file, line);
    std::getline(file, line);

    std::istringstream item(line);
    item >> line; item >> line;
    val = atoi(line.c_str());
    qDebug() << val;

    val = std::atoi(line.c_str());

    return this->memTotal - val / 1024;
}

float MemoryLoad::getMemTotalMiB()
{
    float val {0};
    std::string line;
    std::getline(this->file, line);

    std::istringstream item(line);
    item >> line;
    item >> line;
    val = std::atoi(line.c_str());

    return  val / 1024;
}

bool MemoryLoad::running() const
{
    return m_running;
}

void MemoryLoad::setRunning(bool newRunning)
{
    if (m_running == newRunning)
        return;
    m_running = newRunning;
    emit runningChanged();
}

MemoryLoad::~MemoryLoad()
{
    this->file.close();
}

void MemoryLoad::run()
{
    float avg {0};
    float avgProcent {0};
    while (this->m_running)
    {
        QThread::sleep(1);
        avg = this->getUsedMemoryMiB();
       // qDebug() << avg << " " << avgProcent;
                avgProcent = ((avg * 100) / this->memTotal);
        emit this->sendUsedMemory(avgProcent);
        emit this->status(QString::number(avg / 1024) + " MiB (" + QString::number(avgProcent) + "%) of " + QString::number(this->memTotal / 1024) + " GiB");
    }
    emit this->finished();
}
