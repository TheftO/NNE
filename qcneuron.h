#ifndef QCNEURON_H
#define QCNEURON_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsEllipseItem>
#include <QMenu>
#include "cneuron.h"
#include "cconnection.h"
#include "qclayer.h"

class CConnection;
class QCLayer;

class QCNeuron : public QGraphicsEllipseItem
{
public:
    enum {Type = UserType + 10};
    explicit QCNeuron(CNeuron *neuron, QGraphicsItem *parentItem = 0, QGraphicsScene *scene = 0);
    ~QCNeuron();
    void connectWithNeuron(QCNeuron *neuron);
    void disconnectNeuron(QCNeuron *neuron);
    CNeuron *getNeuron();
    int type() const
    {
        return Type;
    }
    void setId(int id);
    int getId();
    double getOutput()
    {
        return neuron->getOutput();
    }
    void setWeight(int id_sender, double value);
    void addConnection(CConnection *connection);
    void delConnection(CConnection *connection);
    QList<CConnection *> getConnections()
    {
        return arrows;
    }

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    CNeuron *neuron;
    QMenu *contextMenu;
    QAction *connect;
    QAction *delNeuron;
    QList<CConnection *> arrows;
//public slots:

};

#endif // QCNEURON_H
