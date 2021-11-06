#include "cpuload.h"

CpuLoad::CpuLoad(QObject *parent) : QObject(parent)
{
    this->cpuinfoRead();
    this->coresCount();
    this->rawDaraFirst.resize(this->cores);
    this->rawDaraSecond.resize(this->cores);

    this->file.setFileName(stat);
    this->file.open(QIODevice::ReadOnly);
}

bool CpuLoad::running() const
{
    return m_running;
}

void CpuLoad::setRunning(bool newRunning)
{
    if (m_running == newRunning)
        return;
    m_running = newRunning;
}

QVector<unsigned int> CpuLoad::getCpuLoad()
{
    this->readCpuStats(this->rawDaraFirst);
    QThread::sleep(1);
    this->readCpuStats(this->rawDaraSecond);


    QVector<unsigned int> avg;
    unsigned int sum {0};

    for (size_t i = 0; i < cores; i++)
    {
        for (size_t j = 0; j < 4; j++)
            this->rawDaraSecond[i][j] -= this->rawDaraFirst[i][j];
        for (size_t j = 0; j < 4; j++)
            sum += this->rawDaraSecond[i][j];
        unsigned int res = 100 - (this->rawDaraSecond[i][this->rawDaraSecond[i].size() - 1] * 100 / sum);
        avg.push_back(res);
        sum = 0;
    }
    this->cpuLoad = avg;
    return avg;
}

unsigned int CpuLoad::getCoresCount() const
{
    return this->cores;
}

CpuLoad::~CpuLoad()
{
    this->file.close();
}

void CpuLoad::run()
{
    while (this->m_running)
    {
        emit sendCpuLoad(this->getCpuLoad());
    }
    emit this->finished();
}

void CpuLoad::cpuinfoRead()
{
    QFile cpuinfoFile(cpuinfo);
    cpuinfoFile.open(QIODevice::ReadOnly);
    this->cpuinfoStr = cpuinfoFile.readAll();
    cpuinfoFile.close();
}

void CpuLoad::coresCount()
{
    this->cores = cpuinfoStr.count("processor");
}

void CpuLoad::readCpuStats(QVector<QVector<unsigned int>> &vec)
{
    this->file.seek(0);
    unsigned int val {0};
    this->file.readLine();

    std::string line;

    for (size_t i = 0; i < this->cores; i++)
    {
        line = this->file.readLine();
        std::istringstream item(line);
        item >> line;

        for (size_t j = 0; j < 4; j++)
        {
            vec[i].resize(4);
            item >> val;
            vec[i][j] = val;
        }
    }
}
