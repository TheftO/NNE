#ifndef QCNNETWORK_H
#define QCNNETWORK_H

#include <QMenu>
#include <QGraphicsView>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsScene>
#include <QGraphicsSimpleTextItem>
#include <QPointF>
#include <QDebug>
#include <QMessageBox>
#include "cnnetwork.h"
#include "qclayer.h"
#include "clayeraddwidget.h"
#include <math.h>

class QCNNetwork : public QGraphicsScene, public CNNetwork
{
    Q_OBJECT
public:
    QCNNetwork(QObject* parent = 0);
    ~QCNNetwork();
    void saveNet(QString file);
    void loadNet(QString file);
    void deleteLayer(QCLayer *layer);
    void deleteNeuron(QCNeuron *neuron);
    void setFirstLayer(QCLayer *layer);
    void setLastLayer(QCLayer *layer);
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    QMenu *contextMenu;
    QAction *addLayer;
    QAction *firstLayerAction;
    QAction *lastLayerAction;
    QVector<QCLayer *> qcLayers;
    QPointF lastRightClickCoordinates;
    CLayerAddWidget *addLayerWindow;
signals:
    void itemsSelected(QList<QGraphicsItem *> item);
public slots:
    void openAddLayer();
    void addLayerSlot(int count, QString sum, QString activ);
    void sendSelected();
};

#endif // QCNNETWORK_H
