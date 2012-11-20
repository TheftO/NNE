#ifndef QCLAYER_H
#define QCLAYER_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsRectItem>
#include <QMenu>
#include <QGraphicsView>
#include "clayer.h"
#include "qcneuron.h"
#include "cneuronaddwidget.h"

class QGraphicsRectItem;
class QCNeuron;
class CNeuronAddWidget;

class QCLayer : public QGraphicsRectItem
{

public:
    enum {Type = UserType + 11};
    enum LayerType { None = 0, First, Last};
    QCLayer(CLayer *layer, QGraphicsItem *parentItem = 0, QGraphicsScene *scene = 0);
    ~QCLayer();
    int type() const
    {
        return Type;
    }
    void connectWithLayer(QCLayer *layer);
    void connectWithNeuron(QCNeuron *neuron);
    void setNextLayer(QCLayer *layer);
    QCLayer *getNextLayer()
    {
        return nextQLayer;
    }

    CLayer *getLayer()
    {
        return layer;
    }
    int getId()
    {
        return layer->getId();
    }
    void setLayerType(LayerType type = None)
    {
        layerType = type;
    }
    int getLayerType()
    {
        return layerType;
    }
    QVector<QCNeuron *> getNeurons()
    {
        return qneurons;
    }
    void addNeuron(int count);
    void delNeuron(QCNeuron *neuron);
    void setSumFunc(QString file);
    QString getSumFunc();
    void setActivFunc(QString file);
    QString getActivFunc();
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    CLayer *layer;
    QCLayer *nextQLayer;
    CNeuronAddWidget *addW;
    QMenu *contextMenu;
    QAction *connect;
    QAction *addNeurons;
    QVector<QCNeuron *> qneurons;
    qreal layerWidth;
    qreal lastNeuronY;
    LayerType layerType;
    QGraphicsPolygonItem *polygonItem;
};

#endif // QCLAYER_H
