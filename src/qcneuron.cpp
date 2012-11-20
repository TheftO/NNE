#include "qcneuron.h"

QCNeuron::QCNeuron(CNeuron *neuron, QGraphicsItem *parentItem, QGraphicsScene *scene)
    : QGraphicsEllipseItem(parentItem, scene)
{
    this->neuron = neuron;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    contextMenu = new QMenu();
    connect = new QAction(QObject::tr("Соединить"), contextMenu);
    delNeuron = new QAction(QObject::tr("Удалить"), contextMenu);
    contextMenu->addAction(connect);
    contextMenu->addAction(delNeuron);
    QBrush brush(Qt::cyan);
    QPen pen(Qt::NoPen);
    setBrush(brush);
    setPen(pen);
    setToolTip(QString::number(neuron->getId()));
}

QCNeuron::~QCNeuron()
{
    for(int i = 0; i < arrows.size(); i++)
    {
        CConnection *arr = arrows.at(i);
        if(arr->getIdTo() != arr->getIdFrom())
        {
            arr->deleteConnection();
            i--;
        }
    }
    delete delNeuron;
    delete connect;
    delete contextMenu;
    delete neuron;
}

void QCNeuron::connectWithNeuron(QCNeuron *neuron)
{
    if(neuron != NULL)
    {
        this->neuron->connectWithNeuron(neuron->getNeuron());
        for(int i = 0; i < arrows.size(); i++)
        {
            if(arrows.at(i)->getSecondNeuron() == neuron)
                return;
        }
        CConnection *arrow = new CConnection(this, neuron, this, scene());
        addConnection(arrow);
        neuron->addConnection(arrow);
        if(parentItem()->type() == QCLayer::Type && parentItem() != neuron->parentItem())
        {
            if(qgraphicsitem_cast<QCLayer *>(neuron->parentItem())->getNextLayer() != qgraphicsitem_cast<QCLayer *>(parentItem()))
                qgraphicsitem_cast<QCLayer *>(parentItem())->setNextLayer(qgraphicsitem_cast<QCLayer *>(neuron->parentItem()));
        }
    }
}

void QCNeuron::disconnectNeuron(QCNeuron *neuron)
{
    QCLayer *parentLayer = dynamic_cast<QCLayer *>(parentItem());
    this->neuron->disconnectNeuron(neuron->getNeuron());
    for(int i = 0; i < arrows.size(); i++)
        if(arrows.at(i)->getIdTo() == neuron->getId())
        {
            CConnection *arr = arrows.at(i);
            arrows.removeAt(i);
            neuron->delConnection(arr);
            delete arr;
            i--;
        }
    if(parentLayer == NULL)
    {
        return;
    }
    QMap<QCLayer *, unsigned int> candidates;
    unsigned int nextConnectionCount = 0;
    QVector<QCNeuron *> neurons = parentLayer->getNeurons();
    for(int i = 0; i < neurons.size(); i++)
    {
        QList<CConnection *> connections = neurons.at(i)->getConnections();
        for(int j = 0; j < connections.size(); j++)
        {
            QCLayer *otherLayer = dynamic_cast<QCLayer *>(connections.at(j)->getSecondNeuron()->parentItem());
            if(otherLayer != NULL)
            {
                if(otherLayer == parentLayer->getNextLayer())
                {
                    nextConnectionCount++;
                }
                else
                {
                    if(candidates.contains(otherLayer))
                        candidates[otherLayer]++;
                    else
                        candidates[otherLayer] = 1;
                }
            }
        }
    }
    if(nextConnectionCount == 0)
    {
        parentLayer->setNextLayer(NULL);
        if(candidates.size() > 0)
        {
            QMap<QCLayer *, unsigned int>::const_iterator i = candidates.begin();
            QMap<QCLayer *, unsigned int>::const_iterator winner = i;
            while(i != candidates.constEnd())
            {
                if(i.value() > winner.value())
                    winner = i;
                ++i;
            }
            parentLayer->setNextLayer(winner.key());
        }
    }
}

CNeuron *QCNeuron::getNeuron()
{
    return neuron;
}

void QCNeuron::setId(int id)
{
    neuron->setId(id);
}

int QCNeuron::getId()
{
    return neuron->getId();
}

void QCNeuron::setWeight(int id_sender, double value)
{
    neuron->setWeight(id_sender, value);
}

void QCNeuron::addConnection(CConnection *connection)
{
    arrows.append(connection);
}

void QCNeuron::delConnection(CConnection *connection)
{
    arrows.removeAll(connection);
}

void QCNeuron::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(scene()->selectedItems().size() <= 0)
        connect->setDisabled(true);
    QAction *currentAction = contextMenu->exec(event->screenPos());
    if(currentAction == connect)
    {
        for(int i = 0; i < scene()->selectedItems().size(); i++)
        {
            QCNeuron *selectedNeuron;
            if(scene()->selectedItems().at(i)->type() == QCNeuron::type())
            {
                selectedNeuron = qgraphicsitem_cast<QCNeuron *>(scene()->selectedItems().at(i));
                selectedNeuron->connectWithNeuron(this);
            }
            QCLayer *selectedLayer;
            if(scene()->selectedItems().at(i)->type() == QCLayer::Type)
            {
                selectedLayer = qgraphicsitem_cast<QCLayer *>(scene()->selectedItems().at(i));
                selectedLayer->connectWithNeuron(this);
            }
        }
    }
    QGraphicsEllipseItem::contextMenuEvent(event);
}

void QCNeuron::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if(isSelected())
        for(int i = 0; i < arrows.size(); i++)
        {
            arrows.at(i)->setColor(QColor(Qt::cyan));
        }
    else
        for(int i = 0; i < arrows.size(); i++)
        {
            arrows.at(i)->setColor(QColor(Qt::black));
        }
    QGraphicsEllipseItem::paint(painter, option, widget);
}
