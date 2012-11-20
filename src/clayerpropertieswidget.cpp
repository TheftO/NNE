#include "clayerpropertieswidget.h"

CLayerPropertiesWidget::CLayerPropertiesWidget(QCLayer *layer, QWidget *parent) :
    QGroupBox("Layer", parent)
{
    this->layer = layer;

    QVBoxLayout *main_lo = new QVBoxLayout(this);
    QLabel *layer_id = new QLabel("ID: " + QString::number(layer->getId()), this);
    QLabel *neuron_count = new QLabel(tr("Количество нейронов: <br>") + QString::number(layer->getNeurons().size()));
    QLabel *nextLayer = new QLabel();
    if(layer->getNextLayer() != NULL)
        nextLayer->setText(tr("Следующий слой: ") + QString::number(layer->getNextLayer()->getId()));
    else
        nextLayer->setText(tr("Следующий слой: нет"));
    QLabel *sumFuncLabel = new QLabel(tr("Функция суммирования"), this);
    QLabel *activFuncLabel = new QLabel(tr("Функция активации"), this);
    QPushButton *del = new QPushButton(tr("Удалить"), this);

    QDir *sumDir = new QDir();
    QDir *activeDir = new QDir();
    sumDir->cd("sum");
    activeDir->cd("activate");
    QStringList sumList = sumDir->entryList(QStringList() << "*.lua", QDir::Files);
    QStringList activList = activeDir->entryList(QStringList() << "*.lua", QDir::Files);
    sumFunc = new QComboBox(this);
    for(int i = 0; i < sumList.size(); i++)
    {
        sumFunc->addItem(sumList.at(i), sumDir->filePath(sumList.at(i)));
    }
    sumFunc->setCurrentIndex(sumFunc->findData(layer->getSumFunc()));
    activFunc = new QComboBox(this);
    for(int i = 0; i < activList.size(); i++)
    {
        activFunc->addItem(activList.at(i), activeDir->filePath(activList.at(i)));
    }
    activFunc->setCurrentIndex(activFunc->findData(layer->getActivFunc()));
    connect(sumFunc, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setSumFunc(int)));
    connect(activFunc, SIGNAL(currentIndexChanged(int)),
            this, SLOT(setActivFunc(int)));
    connect(del, SIGNAL(released()),
            this, SLOT(deleteLayer()));

    main_lo->addWidget(layer_id);
    main_lo->addWidget(nextLayer);
    main_lo->addWidget(neuron_count);
    main_lo->addWidget(sumFuncLabel);
    main_lo->addWidget(sumFunc);
    main_lo->addWidget(activFuncLabel);
    main_lo->addWidget(activFunc);
    main_lo->addWidget(del);
    setLayout(main_lo);
}

CLayerPropertiesWidget::~CLayerPropertiesWidget()
{
    delete activFunc;
    delete sumFunc;
}

void CLayerPropertiesWidget::setSumFunc(int indx)
{
    layer->setSumFunc(sumFunc->itemData(indx).toString());
}

void CLayerPropertiesWidget::setActivFunc(int indx)
{
    layer->setActivFunc(activFunc->itemData(indx).toString());
}

void CLayerPropertiesWidget::deleteLayer()
{
    QCNNetwork *network;
    if((network = dynamic_cast<QCNNetwork *>(layer->scene())) != NULL)
    {
        network->deleteLayer(layer);
        close();
    }
}
