#include "cconnection.h"
#include <math.h>

const qreal Pi = 3.14;

CConnection::CConnection(QCNeuron *first, QCNeuron *second, QGraphicsItem *parent, QGraphicsScene *scene) :
    QGraphicsLineItem(parent, scene)
{
    firstNeuron = first;
    secondNeuron = second;
    id_from = firstNeuron->getId();
    id_to = secondNeuron->getId();
    weight = secondNeuron->getNeuron()->getWeight(firstNeuron->getId());
    if(weight == 0)
        secondNeuron->getNeuron()->setWeight(firstNeuron->getId(), 1.0);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    contextMenu = new QMenu();
    disconnect = new QAction("Delete", contextMenu);
    contextMenu->addAction(disconnect);
    setToolTip(QString::number(firstNeuron->getId()) + ":" + QString::number(id_to));
}

CConnection::~CConnection()
{
    delete contextMenu;
}

QRectF CConnection::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath CConnection::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void CConnection::setWeight(double w)
{
    secondNeuron->getNeuron()->setWeight(firstNeuron->getId(), w);
}

void CConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (firstNeuron->collidesWithItem(secondNeuron))
        return;
    weight = secondNeuron->getNeuron()->getWeight(id_from);
    QPen myPen = pen();
    myPen.setColor(color);
    qreal arrowSize = 10;
    painter->setPen(myPen);
    painter->setBrush(color);
    QPointF point1, point2;
    if(firstNeuron->scenePos().x() < secondNeuron->scenePos().x())
    {
        point1 = QPointF(- secondNeuron->boundingRect().width() / 2, 0);
        point2 = QPointF(firstNeuron->boundingRect().width() / 2, 0);
    }
    else
    {
        point1 = QPointF(secondNeuron->boundingRect().width() / 2, 0);
        point2 = QPointF(- firstNeuron->boundingRect().width() / 2, 0);
    }
    setLine(QLineF(mapFromItem(secondNeuron, point1), point2));
    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;
    QPointF arrowP1 = line().p1() + QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p1() + QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);
    arrowHead.clear();
    arrowHead << line().p1() << arrowP1 << arrowP2;
    painter->drawLine(line());
    painter->drawPolygon(arrowHead);
    if (isSelected())
    {
        painter->setPen(QPen(color, 1, Qt::DashLine));
        QLineF line = QLineF();
        line.translate(0, 4.0);
        painter->drawLine(line);
        line.translate(0,-8.0);
        painter->drawLine(line);
    }
    QGraphicsLineItem::paint(painter, option, widget);
}

void CConnection::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QAction *currentAction = contextMenu->exec(event->screenPos());
    if(currentAction == disconnect)
        deleteConnection();
    QGraphicsLineItem::contextMenuEvent(event);
}

void CConnection::deleteConnection()
{
    firstNeuron->disconnectNeuron(secondNeuron);
}

bool CConnection::operator ==(CConnection &other)
{
    if(firstNeuron == other.firstNeuron && secondNeuron == other.secondNeuron)
        return true;
    return false;
}

bool CConnection::operator !=(CConnection &other)
{
    return !(operator ==(other));
}
