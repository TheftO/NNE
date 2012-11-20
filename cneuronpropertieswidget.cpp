#include "cneuronpropertieswidget.h"

CNeuronPropertiesWidget::CNeuronPropertiesWidget(QCNeuron *neuron, QWidget *parent) :
    QGroupBox("Neuron", parent)
{
    this->neuron = neuron;
    QVBoxLayout *main_lo = new QVBoxLayout(this);
    QLabel *id = new QLabel("ID: " + QString::number(neuron->getId()));
    QLabel *output = new QLabel(tr("Вывод: ") + QString::number(neuron->getOutput()));
    QPushButton *del = new QPushButton(tr("Удалить"));

    connect(del, SIGNAL(released()),
            this, SLOT(deleteNeuron()));

    main_lo->addWidget(id);
    main_lo->addWidget(output);
    main_lo->addWidget(del);
    setLayout(main_lo);
}

CNeuronPropertiesWidget::~CNeuronPropertiesWidget()
{
}

void CNeuronPropertiesWidget::deleteNeuron()
{
    QCNNetwork *network;
    if((network = dynamic_cast<QCNNetwork *>(neuron->scene())) != NULL)
    {
        network->deleteNeuron(neuron);
        close();
    }
}
