#include "cconnectionpropertieswidget.h"

CConnectionPropertiesWidget::CConnectionPropertiesWidget(CConnection *connection, QWidget *parent) :
    QGroupBox("Connection", parent)
{
    if(connection != NULL)
        this->connection = connection;
    else close();
    QVBoxLayout *main_lo = new QVBoxLayout(this);
    QLabel *firstN = new QLabel(tr("От: ") + QString::number(connection->getFirstNeuron()->getId()));
    QLabel *lastN = new QLabel(tr("До: ") + QString::number(connection->getIdTo()));
    QLabel *weight_l = new QLabel(tr("Вес: "));
    QLineEdit *weight = new QLineEdit(QString::number(connection->getWeight()));
    weight->setValidator(new QDoubleValidator());
    QPushButton *deleteCon = new QPushButton(tr("Удалить"));
    connect(deleteCon, SIGNAL(released()),
            this, SLOT(deleteConnection()));
    connect(weight, SIGNAL(textEdited(QString)),
               this, SLOT(setWeight(QString)));
    main_lo->addWidget(firstN);
    main_lo->addWidget(lastN);
    main_lo->addWidget(weight_l);
    main_lo->addWidget(weight);
    main_lo->addWidget(deleteCon);
    setLayout(main_lo);
}

CConnectionPropertiesWidget::~CConnectionPropertiesWidget()
{
}

void CConnectionPropertiesWidget::deleteConnection()
{
    connection->deleteConnection();
    close();
}

void CConnectionPropertiesWidget::setWeight(QString weight)
{
    connection->setWeight(weight.toDouble());
}
