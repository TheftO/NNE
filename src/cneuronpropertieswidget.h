#ifndef CNEURONPROPERTIESWIDGET_H
#define CNEURONPROPERTIESWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include "qcneuron.h"
#include "qcnnetwork.h"

class CNeuronPropertiesWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit CNeuronPropertiesWidget(QCNeuron *neuron, QWidget *parent = 0);
    ~CNeuronPropertiesWidget();
private:
    QCNeuron *neuron;
signals:
    
public slots:
    void deleteNeuron();
};

#endif // CNEURONPROPERTIESWIDGET_H
