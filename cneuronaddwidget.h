#ifndef CNEURONADDWIDGET_H
#define CNEURONADDWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include "qclayer.h"

class QCLayer;

class CNeuronAddWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CNeuronAddWidget(QCLayer *layer,QWidget *parent = 0);
    ~CNeuronAddWidget();
private:
    QLineEdit *line;
    QCLayer *layer;
signals:

private slots:
    void addNeurons();
};

#endif // CNEURONADDWIDGET_H
