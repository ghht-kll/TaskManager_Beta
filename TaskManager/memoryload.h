#ifndef MEMORYLOAD_H
#define MEMORYLOAD_H

#include <QObject>
#include <fstream>
#include <iostream>
#include <sstream>
#include <QThread>

class MemoryLoad : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
public:
    explicit MemoryLoad(QObject *parent = nullptr);
    float getUsedMemoryMiB();
    bool running() const;
    void setRunning(bool newRunning);
    ~MemoryLoad();
    bool m_running;

signals:
    void runningChanged();
    void finished();
    void sendUsedMemory(float memory);
    void status(QString stat);

public slots:
    void run();

private:
    float getMemTotalMiB();

private:
    std::ifstream file;
    float memTotal;
};

#endif // MEMORYLOAD_H
