#ifndef CCONNECTIONPROPERTIESWIDGET_H
#define CCONNECTIONPROPERTIESWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include "cconnection.h"

class CConnectionPropertiesWidget : public QGroupBox
{
    Q_OBJECT
public:
    explicit CConnectionPropertiesWidget(CConnection *connection, QWidget *parent = 0);
    ~CConnectionPropertiesWidget();
private:
    CConnection *connection;
signals:
    
public slots:
    void deleteConnection();
    void setWeight(QString weight);
};

#endif // CCONNECTIONPROPERTIESWIDGET_H
