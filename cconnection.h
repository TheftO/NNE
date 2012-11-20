#ifndef CCONNECTION_H
#define CCONNECTION_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QLineF>
#include <QKeyEvent>
#include "qcneuron.h"
#include <qstyleoption.h>

class QCNeuron;
class QPainter;
class QGraphicsLineItem;

class CConnection : public QGraphicsLineItem
{
public:
    explicit CConnection(QCNeuron *first, QCNeuron *second, QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
    ~CConnection();
    enum {Type = UserType + 14};
    int type() const
    {
        return Type;
    }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
    {
        this->color = color;
    }
    int getIdTo() const
    {
        return id_to;
    }
    int getIdFrom() const
    {
        return id_from;
    }
    QCNeuron *getFirstNeuron() const
    {
        return firstNeuron;
    }
    QCNeuron *getSecondNeuron() const
    {
        return secondNeuron;
    }
    void setWeight(double w);
    double getWeight()
    {
        return weight;
    }
    void deleteConnection();
    bool operator ==(CConnection &other);
    bool operator !=(CConnection &other);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    QCNeuron *firstNeuron;
    QCNeuron *secondNeuron;
    int id_from;
    int id_to;
    double weight;
    QColor color;
    QPolygonF arrowHead;
    QMenu *contextMenu;
    QAction *disconnect;
};

#endif // CCONNECTION_H
