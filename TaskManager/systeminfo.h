#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <QObject>
#include <fstream>
#include <QString>

class SystemInfo : public QObject
{
    Q_OBJECT
public:
    explicit SystemInfo(QObject *parent = nullptr);

signals:

public:
    QString getProcessorName() const;
    QString getHostName() const;

private:
    QString readProcessorName();
    QString readHostName();

private:
    std::ifstream file;
    QString processorName;
    QString hostName;
};

#endif // SYSTEMINFO_H
