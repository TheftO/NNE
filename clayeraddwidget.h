#ifndef CLayerAddWidget_H
#define CLayerAddWidget_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QIntValidator>
#include <QComboBox>
#include <QDir>
#include <QLabel>
#include "DEFINES.h"
#include "ccodeeditorwidget.h"

class CLayerAddWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CLayerAddWidget(QWidget *parent = 0);
    ~CLayerAddWidget();
private:
    QLineEdit *neuronCount;
    QComboBox *sumFunc;
    QComboBox *activFunc;
signals:
    void ok(int neuronCount, QString sum, QString activ);
public slots:
    void sendOk();
};

#endif // CLayerAddWidget_H
