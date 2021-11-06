#include "systeminfo.h"

SystemInfo::SystemInfo(QObject *parent) : QObject(parent)
{
    this->processorName = this->readProcessorName();
    this->hostName = this->readHostName();
}

QString SystemInfo::getProcessorName() const
{
    return this->processorName;
}

QString SystemInfo::getHostName() const
{
    return this->hostName;
}

QString SystemInfo::readProcessorName()
{
    this->file.open("/proc/cpuinfo");

    std::string str;
    for(int i = 0; i < 16; i++)
        this->file >> str;
    std::getline(this->file, str);
    file.close();

    return QString::fromStdString(str);
}

QString SystemInfo::readHostName()
{
    this->file.open("/proc/sys/kernel/hostname");

    std::string str;
    std::getline(file, str);
    file.close();

    return QString::fromStdString(str);
}
