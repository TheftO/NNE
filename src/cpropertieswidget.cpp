#include "cpropertieswidget.h"

CPropertiesWidget::CPropertiesWidget(QWidget *parent) : /*QScrollArea(parent)*/ QWidget(parent)
{
    main_lo = new QVBoxLayout(this);
    setLayout(main_lo);
    QLabel *title = new QLabel(tr("<b> Свойства </b>"), this);
    title->setTextFormat(Qt::RichText);
    main_lo->addWidget(title);
    title->setMinimumSize(title->sizeHint());
    main_lo->setAlignment(title, Qt::AlignHCenter | Qt::AlignTop);
}


CPropertiesWidget::~CPropertiesWidget()
{
    propertiesList.clear();
    delete main_lo;
}

void CPropertiesWidget::addProp(QWidget *prop)
{
    if(prop == NULL)
        return;
    main_lo->addWidget(prop);
    prop->setMinimumSize(prop->sizeHint());
    main_lo->setAlignment(prop, Qt::AlignTop);
    propertiesList.append(prop);
    prop->setAttribute(Qt::WA_DeleteOnClose);
}

void CPropertiesWidget::clearProp()
{
    for(int i = 0; i < propertiesList.size(); i++)
        propertiesList.at(i)->close();
    propertiesList.clear();
}

