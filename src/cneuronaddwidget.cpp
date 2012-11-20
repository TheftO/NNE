#include "cneuronaddwidget.h"

CNeuronAddWidget::CNeuronAddWidget(QCLayer *layer, QWidget *parent) : QWidget(parent)
{
    this->layer = layer;
    line = new QLineEdit(this);
    line->setValidator(new QIntValidator());
    QPushButton *ok = new QPushButton("Ok", this);
    QPushButton *cancel = new QPushButton("Cancel", this);
    connect(ok, SIGNAL(released()),
            this, SLOT(addNeurons()));
    connect(cancel, SIGNAL(released()),
            this, SLOT(close()));
    QHBoxLayout *main_lo = new QHBoxLayout(this);
    main_lo->addWidget(line);
    main_lo->addWidget(ok);
    main_lo->addWidget(cancel);
    setLayout(main_lo);
}

CNeuronAddWidget::~CNeuronAddWidget()
{
    delete line;
}

void CNeuronAddWidget::addNeurons()
{
    if(line->text().toInt() > 0)
        layer->addNeuron(line->text().toInt());
}
