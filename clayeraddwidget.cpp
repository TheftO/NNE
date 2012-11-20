#include "clayeraddwidget.h"

CLayerAddWidget::CLayerAddWidget(QWidget *parent) :
    QWidget(parent)
{
    setWindowTitle(tr("Введите праметры нового слоя"));
    QBoxLayout *main_lo = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *sum_lo = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout *activ_lo = new QBoxLayout(QBoxLayout::LeftToRight);
    QLabel *neuronCountLabel = new QLabel(tr("Количество нейронов"), this);
    QLabel *sumFuncLabel = new QLabel(tr("Функция суммрования"), this);
    QLabel *activFuncLabel = new QLabel(tr("Функция активации"), this);
    QDir *sumDir = new QDir();
    QDir *activeDir = new QDir();
    sumDir->cd("sum");
    activeDir->cd("activate");
    QStringList sumList = sumDir->entryList(QStringList() << "*.lua", QDir::Files);
    QStringList activList = activeDir->entryList(QStringList() << "*.lua", QDir::Files);
    neuronCount = new QLineEdit(this);
    neuronCount->setValidator(new QIntValidator(0, 62000));
    sumFunc = new QComboBox(this);
    for(int i = 0; i < sumList.size(); i++)
        sumFunc->addItem(sumList.at(i), sumDir->filePath(sumList.at(i)));
    activFunc = new QComboBox(this);
    for(int i = 0; i < activList.size(); i++)
        activFunc->addItem(activList.at(i), activeDir->filePath(activList.at(i)));
    sum_lo->addWidget(sumFunc);
    activ_lo->addWidget(activFunc);
    QPushButton *ok_b = new QPushButton(tr("Ок"), this);
    connect(ok_b, SIGNAL(released()),
            this, SLOT(sendOk()));
    connect(ok_b, SIGNAL(released()),
            this, SLOT(close()));
    main_lo->addWidget(neuronCountLabel);
    main_lo->addWidget(neuronCount);
    main_lo->addWidget(sumFuncLabel);
    main_lo->addLayout(sum_lo);
    main_lo->addWidget(activFuncLabel);
    main_lo->addLayout(activ_lo);
    main_lo->addWidget(ok_b);
    setLayout(main_lo);
    delete sumDir;
    delete activeDir;;
}

CLayerAddWidget::~CLayerAddWidget()
{
    delete activFunc;
    delete sumFunc;
    delete neuronCount;
}

void CLayerAddWidget::sendOk()
{
    int nc;
    QString sf = sumFunc->itemData(sumFunc->currentIndex()).toString();
    QString af = activFunc->itemData(activFunc->currentIndex()).toString();
    if((nc = neuronCount->text().toInt()) > 0)
        emit ok(nc, sf, af);
}
