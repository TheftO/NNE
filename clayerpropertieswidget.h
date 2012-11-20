#ifndef CLAYERPROPERTIESWIDGET_H
#define CLAYERPROPERTIESWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QDir>
#include "qclayer.h"
#include "qcnnetwork.h"

class CLayerPropertiesWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit CLayerPropertiesWidget(QCLayer *layer, QWidget *parent = 0);
    ~CLayerPropertiesWidget();
private:
    QCLayer *layer;
    QComboBox *sumFunc;
    QComboBox *activFunc;
signals:
    
public slots:
    void setSumFunc(int indx);
    void setActivFunc(int indx);
    void deleteLayer();
};

#endif // CLAYERPROPERTIESWIDGET_H
