#ifndef CPROPERTIESWIDGET_H
#define CPROPERTIESWIDGET_H

#include <QWidget>
#include <QScrollArea>
#include "clayerpropertieswidget.h"
#include "cneuronpropertieswidget.h"
#include "cconnectionpropertieswidget.h"
#include "qclayer.h"
#include "qcneuron.h"
#include "cconnection.h"

class CPropertiesWidget : /*public QScrollArea*/ public QWidget
{
    Q_OBJECT
public:
    explicit CPropertiesWidget(QWidget *parent = 0);
    ~CPropertiesWidget();
private:
    QVBoxLayout *main_lo;
    //QWidget *widget;
    QList<QWidget *> propertiesList;

signals:
    
public slots:
    void addProp(QWidget *prop);
    void clearProp();
};

#endif // CPROPERTIESWIDGET_H
