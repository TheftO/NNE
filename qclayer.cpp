#include "qclayer.h"

QCLayer::QCLayer(CLayer *layer, QGraphicsItem *parentItem, QGraphicsScene *scene)
    : QGraphicsRectItem(parentItem, scene)
{
    layerWidth = 20;
    layerType = None;
    polygonItem = NULL;
    this->layer = layer;
    nextQLayer = NULL;
    addW = NULL;

    contextMenu = new QMenu();
    connect = new QAction(QObject::tr("Соединить"), scene);
    addNeurons = new QAction(QObject::tr("Добавить нейроны"), scene);
    contextMenu->addAction(connect);
    contextMenu->addAction(addNeurons);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);

    for(unsigned int i = 0; i < layer->getNeurons().size(); i++)
    {
        QCNeuron *newQCNeuron = new QCNeuron(layer->getNeurons().at(i), this, scene);
        newQCNeuron->setRect(- layerWidth / 2, - layerWidth / 2, layerWidth - 2, layerWidth - 2);
        qneurons.push_back(newQCNeuron);
    }
}

QCLayer::~QCLayer()
{
    for(int i = 0; i < qneurons.size(); i++)
        delNeuron(qneurons.at(i));
    delete addW;
    delete contextMenu;
    delete layer;
}

void QCLayer::connectWithLayer(QCLayer *layer)
{
    if(layer != NULL)
        for(int i = 0; i < qneurons.size(); i++)
            for(int j = 0; j < layer->getNeurons().size(); j++)
                qneurons.at(i)->connectWithNeuron(layer->getNeurons().at(j));
}

void QCLayer::connectWithNeuron(QCNeuron *neuron)
{
    if(layer != NULL)
        for(int i = 0; i < qneurons.size(); i++)
            qneurons.at(i)->connectWithNeuron(neuron);
}

void QCLayer::setNextLayer(QCLayer *layer)
{
        nextQLayer = layer;
        if(layer != NULL)
            this->layer->setNextLayer(layer->getLayer());
        else
            this->layer->setNextLayer(NULL);
}

void QCLayer::addNeuron(int count)
{
    unsigned int i = layer->getNeurons().size();
    layer->addNeuron(count);
    for(; i < layer->getNeurons().size(); i++)
    {
        QCNeuron *newQCNeuron = new QCNeuron(layer->getNeurons().at(i), this, scene());
        newQCNeuron->setRect(- layerWidth / 2, - layerWidth / 2, layerWidth - 2, layerWidth - 2);
        qneurons.push_back(newQCNeuron);
    }
}

void QCLayer::delNeuron(QCNeuron *neuron)
{
    if(neuron != NULL)
    {
        qneurons.remove(qneurons.indexOf(neuron));
        if(scene() != NULL)
            scene()->removeItem(neuron);
        delete neuron;
    }
}

void QCLayer::setSumFunc(QString file)
{
    if(!file.isEmpty())
    {
        QByteArray filearr = file.toLocal8Bit();
        layer->setSummFunc(filearr.data(), filearr.size());
    }
}

QString QCLayer::getSumFunc()
{
    QString str(layer->getSummFunc());
    return str;
}

void QCLayer::setActivFunc(QString file)
{
    if(!file.isEmpty())
    {
        QByteArray filearr = file.toLocal8Bit();
        layer->setActivFunc(filearr.data(), file.size());
    }
}

QString QCLayer::getActivFunc()
{
    QString str(layer->getActivFunc());
    return str;
}

void QCLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(scene()->selectedItems().size() <= 0)
        connect->setDisabled(true);
    QAction *currentAction = contextMenu->exec(event->screenPos());
    if(currentAction == connect)
    {
        for(int i = 0; i < scene()->selectedItems().size(); i++)
        {
            QCNeuron *selectedNeuron;
            if(scene()->selectedItems().at(i)->type() == QCNeuron::Type)
            {
                selectedNeuron = qgraphicsitem_cast<QCNeuron *>(scene()->selectedItems().at(i));
                if(selectedNeuron != NULL)
                    for(int j = 0; j < qneurons.size(); j++)
                        selectedNeuron->connectWithNeuron(qneurons.at(j));
            }
            QCLayer *selectedLayer;
            if(scene()->selectedItems().at(i)->type() == QCLayer::type())
            {
                selectedLayer = qgraphicsitem_cast<QCLayer *>(scene()->selectedItems().at(i));
                if(selectedLayer != NULL)
                    selectedLayer->connectWithLayer(this);
            }
        }
    }
    else if(currentAction == addNeurons)
    {
        delete addW;
        addW = new CNeuronAddWidget(this, &*(scene()->views().first()));
        addW->setWindowModality(Qt::WindowModal);
        addW->show();
    }
    QGraphicsRectItem::contextMenuEvent(event);
}

void QCLayer::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    setBrush(Qt::white);
    setRect(0, 0, layerWidth, qneurons.size() * (2 + layerWidth));
    lastNeuronY = layerWidth / 2;
    for(int i = 0; i < qneurons.size(); i++ )
    {
        qneurons.at(i)->setPos(layerWidth / 2 + 1, lastNeuronY + 2);
        lastNeuronY += (layerWidth + 2);
    }
    if(layerType != None)
    {
        qreal offset = 0;
        if(layerType == First)
            offset = -5;
        else if(layerType == Last)
            offset = 15 + layerWidth;
        if(polygonItem == NULL)
        {
            QPointF p1, p2, p3;
            QPolygonF arrowHead;
            p1 = QPointF(0, 0);
            p2 = QPointF(- 10, 5);
            p3 = QPointF(- 10, -5);
            arrowHead<<p1<<p2<<p3;
            polygonItem = new QGraphicsPolygonItem(arrowHead, this, scene());
        }
        polygonItem->setPos(offset, 0);
        polygonItem->setBrush(Qt::black);
    }
    QGraphicsRectItem::paint(painter, option, widget);
}
