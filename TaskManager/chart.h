#ifndef CHART_H
#define CHART_H

#include <QChart>
#include <QTimer>
#include <QVector>

QT_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_END_NAMESPACE

QT_USE_NAMESPACE

class Chart: public QChart
{
    Q_OBJECT
public:
    Chart(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    virtual ~Chart();

public slots:
    void updateChart(QVector<unsigned int> vec);
    void updateChart(float item);
public:
    void createLineSeries(unsigned int number);

private:
    QVector<QSplineSeries*> series;
    QSplineSeries *oneSeries = nullptr;
    QValueAxis *m_axisX;
    QValueAxis *m_axisY;
    qreal m_x;
    qreal m_y;
};

#endif
