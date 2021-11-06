#include "chart.h"
#include <QAbstractAxis>
#include <QSplineSeries>
#include <QValueAxis>
#include <QRandomGenerator>
#include <QDebug>

Chart::Chart(QGraphicsItem *parent, Qt::WindowFlags wFlags):
    QChart(QChart::ChartTypeCartesian, parent, wFlags),
    m_axisX(new QValueAxis()),
    m_axisY(new QValueAxis()),
    m_x(100),
    m_y(0)
{
    addAxis(m_axisX,Qt::AlignBottom);
    addAxis(m_axisY,Qt::AlignLeft);
    m_axisX->hide();
    m_axisX->setTickCount(100);
    m_axisX->setRange(0, 100);
    m_axisY->setRange(0, 100);
}

Chart::~Chart()
{

}

void Chart::updateChart(QVector<unsigned int> vec)
{
    qreal x = plotArea().width() / m_axisX->tickCount();
    qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();
    m_x += y;

    for(int i = 0; i < series.size(); i++)
    {
        series[i]->append(m_x, vec[i]);
        if(series[i]->count() >= 110)
            series[i]->remove(0);
    }
    scroll(x, 0);
}

void Chart::updateChart(float item)
{
    qreal x = plotArea().width() / m_axisX->tickCount();
    qreal y = (m_axisX->max() - m_axisX->min()) / m_axisX->tickCount();
    m_x += y;

    this->oneSeries->append(m_x, item);
    if(this->oneSeries->count() >= 110)
        this->oneSeries->remove(0);
    scroll(x, 0);
}

void Chart::createLineSeries(unsigned int number)
{
    if(number == 1)
    {
        this->oneSeries = new QSplineSeries(this);
        QPen pen(Qt::green);
        pen.setWidth(1);
        this->oneSeries->setPen(pen);
        this->oneSeries->append(this->m_x, this->m_y);
        this->addSeries(this->oneSeries);
        this->oneSeries->attachAxis(this->m_axisX);
        this->oneSeries->attachAxis(this->m_axisY);
    }
    else
    {
        for(size_t i = 0; i < number; i++)
        {
            QSplineSeries* ser = new QSplineSeries(this);
            QPen pen(Qt::green);
            pen.setWidth(1);
            ser->setPen(pen);
            ser->append(this->m_x, this->m_y);
            this->addSeries(ser);
            ser->attachAxis(this->m_axisX);
            ser->attachAxis(this->m_axisY);

            this->series.push_back(ser);
        }
    }
}


