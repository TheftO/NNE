#include "qcnnetwork.h"

QCNNetwork::QCNNetwork(QObject *parent)
    :QGraphicsScene(parent)
{
    lastRightClickCoordinates = QPointF(0, 0);
    addLayerWindow = NULL;
    setItemIndexMethod(NoIndex);

    contextMenu = new QMenu();
    addLayer = new QAction(tr("Add layer"), this);
    firstLayerAction = new QAction(tr("Set input layer"), this);
    lastLayerAction = new QAction(tr("Set output layer"), this);
    contextMenu->addAction(addLayer);

    connect(addLayer, SIGNAL(triggered()),
            this, SLOT(openAddLayer()));
    connect(this, SIGNAL(selectionChanged()),
            this, SLOT(sendSelected()));
}

QCNNetwork::~QCNNetwork()
{
    if(addLayerWindow != NULL)
            addLayerWindow->hide();
    delete addLayerWindow;
    delete lastLayer;
    delete firstLayer;
    delete addLayer;
    delete contextMenu;
}

void QCNNetwork::saveNet(QString file)
{
    std::ofstream out;
    QByteArray filearr = file.toLocal8Bit();
    out.open(filearr.data(), std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
    if(!out.is_open())
    {
        QMessageBox warning;
        warning.setText(tr("Ошибка записи!"));
        warning.exec();
        return;
    }
    QString studyf(studyFunc);
    QByteArray arr = studyf.toLocal8Bit();
    char *ch = arr.data(); //сохраняем функцию обучения
    out.write(ch, /*sizeof(ch)*/ arr.size());
    out.write("\n", 1);
    int si = qcLayers.size();
    out.write((char *)&si, sizeof(int));       //количество слоев
    for(int i = 0; i < qcLayers.size(); i++)
    {
        int type = qcLayers.at(i)->getLayerType();
        out.write((char *)&type, sizeof(int)); //сохраняем тип слоя
        QString sumf = qcLayers.at(i)->getSumFunc();
        arr = sumf.toLocal8Bit();
        ch = arr.data();    //сохраняем функции суммирования
        out.write(ch, arr.size());
        out.write("\n", 1);
        QString actf = qcLayers.at(i)->getActivFunc();
        arr = actf.toLocal8Bit();
        ch = arr.data();        //сохраняем функции активации
        out.write(ch, arr.size());
        out.write("\n", 1);
        out.close();
        qcLayers.at(i)->getLayer()->saveWeights(filearr.data()); //сохраняем веса
        out.open(filearr.data(), std::ios_base::out | std::ios_base::app | std::ios_base::binary);
        qreal x = qcLayers.at(i)->scenePos().x();
        out.write((char *)&x, sizeof(qreal));  //сохраняем координаты
        qreal y = qcLayers.at(i)->scenePos().y();
        out.write((char *)&y, sizeof(qreal));
    }
    out.close();
}

void QCNNetwork::loadNet(QString file)  //надо доделать
{
    for(int i = 0; i < items().size(); i++)
    {
        QGraphicsItem *item = items().at(i);
        removeItem(item);
        delete item;
    }
    qcLayers.clear();
    std::ifstream in;
    QByteArray filearr = file.toLocal8Bit();
    in.open(filearr.data(), std::ios_base::in | std::ios_base::binary);
    if(!in.is_open())
        return;
    qDebug("Start loading");
    char *stud = new char[150];
    QString studf;             //Получаем название файла с функцией обучения
    in.getline(stud, 150, '\n');
    studf.append(stud);
    QByteArray studFarr = studf.toLocal8Bit();
    setStudyFunc(studFarr.data(), studFarr.size());
    qDebug(("Set study func: " + studf).toLocal8Bit().data());
    int size = 0;
    in.read((char *)&size, sizeof(int));
    QVector<int> indxs;
    for(int i = 0; i < size; i++)
    {
        int layerType;
        in.read((char *)&layerType, sizeof(int));
        qDebug((QString::number(i) + ": Get layer type: " + QString::number(layerType)).toLocal8Bit().data());
        QString sumf;               //Получаем название файла с функцией суммрования
        char *sum = new char[150];
        in.getline(sum, 150, '\n');
        sumf.append(sum);
        qDebug((QString::number(i) + ": Get sum func: " + sumf).toLocal8Bit().data());
        QString actf;                //Получаем название файла с функцией активации
        char *act = new char[150];
        in.getline(act, 150, '\n');
        actf.append(act);
        qDebug((QString::number(i) + ": Get active func: " + actf).toLocal8Bit().data());
        int layerId = 0;
        in.read((char *)&layerId, sizeof(int));
        qDebug((QString::number(i) + ": Get layer id: " + QString::number(layerType)).toLocal8Bit().data());
        unsigned int neuronCount = 0;
        in.read((char *)&neuronCount, sizeof(unsigned int));
        qDebug((QString::number(i) + ": Get neuron count: " + QString::number(neuronCount)).toLocal8Bit().data());
        QByteArray sumfarr = sumf.toLocal8Bit();
        QByteArray actfarr = actf.toLocal8Bit();
        CLayer *layer = CNNetwork::addLayer(neuronCount, sumfarr.data(), sumfarr.size(), actfarr.data() ,actfarr.size());
        layer->setId(layerId);
        for(unsigned int j = 0; j < neuronCount; j++)
        {
            indxs.append(in.tellg());  //Мы сюда вернемся чтобы создать связи между нейронами
            int idTo = 0;
            in.read((char *)&idTo, sizeof(int));
            qDebug((QString::number(i) + ":" + QString::number(j) + ": Get id to: " + QString::number(idTo)).toLocal8Bit().data());
            unsigned int weightSize = 0;
            in.read((char *)&weightSize, sizeof(unsigned int));
            qDebug((QString::number(i) + ":" + QString::number(j) + ": Get weights size: " + QString::number(weightSize)).toLocal8Bit().data());
            for(int k = 0; k < weightSize; k++)
            {
                int idFrom = 0;
                in.read((char *)&idFrom, sizeof(int));
                qDebug((QString::number(i) + ":" + QString::number(j) + ":" + QString::number(k) + ": Get id from: " + QString::number(idFrom)).toLocal8Bit().data());
                double weight = 0;
                in.read((char *)&weight, sizeof(double));
                qDebug((QString::number(i) + ":" + QString::number(j) + ":" + QString::number(k) + ": Get weight: " + QString::number(weight)).toLocal8Bit().data());
                layer->getNeurons()[j]->setId(idTo);
                layer->getNeurons()[j]->setWeight(idFrom, weight);
            }
        }
        QCLayer *qlayer = new QCLayer(layer, 0, this);
        if(layerType != QCLayer::None)
        {
            if(layerType == QCLayer::First)
                setFirstLayer(qlayer);
            else
                setLastLayer(qlayer);
        }
        qreal x = 0, y = 0;
        in.read((char *)&x, sizeof(qreal));
        in.read((char *)&y, sizeof(qreal));
        qDebug((QString::number(i) + ": x = " + QString::number(x) + ": y = " + QString::number(y)).toLocal8Bit().data());
        qlayer->setPos(x, y);
        qcLayers.push_back(qlayer);
    }
    for(int i = 0; i < qcLayers.size(); i++)
    {
        in.seekg(indxs[i]);
        qDebug((QString::number(i) + ": goto index " + QString::number(indxs[i])).toLocal8Bit().data());
        for(int j = 0; j < qcLayers[i]->getNeurons().size(); j++)
        {
            int idTo = 0;
            in.read((char *)&idTo, sizeof(int));
            qDebug((QString::number(i) + ":" + QString::number(j) + ": Get id to: " + QString::number(idTo)).toLocal8Bit().data());
            QCNeuron *neuronTo = NULL;
            //bool finded = false;
            unsigned int weightSize = 0;
            in.read((char *)&weightSize, sizeof(unsigned int));
            qDebug((QString::number(i) + ":" + QString::number(j) + ": Get weight size: " + QString::number(weightSize)).toLocal8Bit().data());
            for(int k = 0; k < weightSize; k++)
            {
                int idFrom = 0;
                in.read((char *)&idFrom, sizeof(int));
                qDebug((QString::number(i) + ":" + QString::number(j) + ":" + QString::number(k) + ": Get id from: " + QString::number(idFrom)).toLocal8Bit().data());
                double weight = 0;
                in.read((char *)&weight, sizeof(double));
                qDebug((QString::number(i) + ":" + QString::number(j) + ":" + QString::number(k) + ": Get weight: " + QString::number(weight)).toLocal8Bit().data());
                for(int m = 0; m < qcLayers.size(); m++)
                {
                    for(int n = 0; n < qcLayers[m]->getNeurons().size(); n++)
                        if(qcLayers[m]->getNeurons()[n]->getId() == idTo)
                        {
                            qDebug((QString::number(i) + ":" + QString::number(j) + "Finded").toLocal8Bit().data());
                            neuronTo = qcLayers[m]->getNeurons()[n];
                            break;
                        }
                }
                if(neuronTo != NULL)
                    for(int m = 0; m < qcLayers.size(); m++)
                    {
                        for(int n = 0; n < qcLayers[m]->getNeurons().size(); n++)
                            if(qcLayers[m]->getNeurons()[n]->getId() == idFrom)
                                qcLayers[m]->getNeurons()[n]->connectWithNeuron(neuronTo);
                    }
            }
        }
    }
    in.close();
    qDebug("End loading");
}

void QCNNetwork::deleteLayer(QCLayer *layer)
{
    if(layer == NULL)
        return;
    if(layer->getLayerType() == QCLayer::First)
        setFirstLayer(NULL);
    else if(layer->getLayerType() == QCLayer::Last)
        setLastLayer(NULL);
    for(int i = 0; i < layer->getNeurons().size(); i++)
        layer->delNeuron(layer->getNeurons().at(i));
    qcLayers.remove(qcLayers.indexOf(layer));
    removeItem(layer);
    delete layer;
}

void QCNNetwork::deleteNeuron(QCNeuron *neuron)
{
    if(neuron == NULL)
        return;
    QCLayer *layer = qgraphicsitem_cast<QCLayer *>(neuron->parentItem());
    layer->delNeuron(neuron);
    if(layer->getNeurons().size() <= 0)
        deleteLayer(layer);
}

void QCNNetwork::setFirstLayer(QCLayer *layer)
{
    for(int i = 0; i < qcLayers.size(); i++)
    {
        if(qcLayers.at(i)->getLayerType() == QCLayer::First)
            qcLayers.at(i)->setLayerType(QCLayer::None);
    }
    if(layer != NULL)
    {
        CNNetwork::setFirstLayer(layer->getLayer());
        layer->setLayerType(QCLayer::First);
    }else
        CNNetwork::setFirstLayer(NULL);
}

void QCNNetwork::setLastLayer(QCLayer *layer)
{
    for(int i = 0; i < qcLayers.size(); i++)
    {
        if(qcLayers.at(i)->getLayerType() == QCLayer::Last)
            qcLayers.at(i)->setLayerType(QCLayer::None);
    }
    if(layer != NULL)
    {
        CNNetwork::setLastLayer(layer->getLayer());
        layer->setLayerType(QCLayer::Last);
    }else
        CNNetwork::setFirstLayer(NULL);
}

void QCNNetwork::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton)

        lastRightClickCoordinates = mouseEvent->scenePos();

    if(mouseEvent->modifiers() == Qt::SHIFT)

        views().first()->setDragMode(QGraphicsView::RubberBandDrag);

    else if(mouseEvent->button() == Qt::LeftButton)

        views().first()->setDragMode(QGraphicsView::ScrollHandDrag);

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void QCNNetwork::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton)

        views().first()->setDragMode(QGraphicsView::NoDrag);

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void QCNNetwork::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(event->modifiers() == Qt::CTRL)
    {
        views().first()->setResizeAnchor(QGraphicsView::AnchorUnderMouse);
        views().first()->verticalScrollBar();
        qreal scaleFactor = pow((double)2, event->delta() / 240.0);
        qreal factor = views().first()->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
        if(factor < 0.2 || factor > 20)
            return;
        views().first()->scale(scaleFactor, scaleFactor);
    }
}

void QCNNetwork::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QGraphicsScene::contextMenuEvent(event);
    contextMenu->exec(event->screenPos());
}

void QCNNetwork::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Delete)
    {
        for(int i = 0; i < selectedItems().size(); i++)
        {
            if(selectedItems().at(i)->type() == CConnection::Type)
            {
                CConnection *connection = qgraphicsitem_cast<CConnection *>(selectedItems().at(i));
                if(connection != NULL)
                {
                    connection->deleteConnection();
                    i--;    //Т.к. мы удаляем элемент списка, все остальные смещаются на 1 влево.
                }          //Мы же не хотим никого пропустить?
            }
            else if(selectedItems().at(i)->type() == QCLayer::Type)
            {
                QCLayer *layer = qgraphicsitem_cast<QCLayer *>(selectedItems().at(i));
                if(layer != NULL)
                {
                    deleteLayer(layer);
                    i--;
                }
            }
            else if(selectedItems().at(i)->type() == QCNeuron::Type)
            {
                QCNeuron *neuron = qgraphicsitem_cast<QCNeuron *>(selectedItems().at(i));
                if(neuron != NULL)
                {
                    deleteNeuron(neuron);
                    i--;
                }
            }
        }
    }
    else if(event->key() == Qt::Key_F)
    {
        for(int i = 0; i < selectedItems().size(); i++)
        {
            if(selectedItems().at(i)->type() == QCLayer::Type)
            {
                QCLayer *l = qgraphicsitem_cast<QCLayer *>(selectedItems().at(i));
                setFirstLayer(l);
                break;
            }
        }
    }
    else if(event->key() == Qt::Key_L)
    {
        for(int i = 0; i < selectedItems().size(); i++)
            if(selectedItems().at(i)->type() == QCLayer::Type)
            {
                QCLayer *l = qgraphicsitem_cast<QCLayer *>(selectedItems().at(i));
                setLastLayer(l);
                break;
            }
    }
    QGraphicsScene::keyPressEvent(event);
}

void QCNNetwork::openAddLayer()
{
    if(addLayerWindow != NULL)
        addLayerWindow->hide();
    delete addLayerWindow;
    addLayerWindow = new CLayerAddWidget();

    connect(addLayerWindow, SIGNAL(ok(int,QString,QString)),
        this, SLOT(addLayerSlot(int,QString,QString)));

    addLayerWindow->setWindowModality(Qt::WindowModal);
    addLayerWindow->show();
}

void QCNNetwork::addLayerSlot(int count, QString sum, QString activ)
{
    if(count <= 0)
        return;
    QByteArray sumarr = sum.toLocal8Bit();
    QByteArray activarr = activ.toLocal8Bit();
    QCLayer *newQCLayer = new QCLayer(CNNetwork::addLayer(count, sumarr.data(), sumarr.size(), activarr.data(), activarr.size()));
    qcLayers.push_back(newQCLayer);
    addItem(newQCLayer);
    newQCLayer->setPos(lastRightClickCoordinates);
}

void QCNNetwork::sendSelected()
{
    emit itemsSelected(selectedItems());
}
