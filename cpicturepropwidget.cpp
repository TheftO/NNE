#include "cpicturepropwidget.h"


CPicturePropWidget::CPicturePropWidget(QWidget *parent) :
    QWidget(parent)
{
    QBoxLayout *main_lo = new QBoxLayout(QBoxLayout::TopToBottom, this);
    pic = new QLabel();
    size = new QLabel();
    file = new QLabel();
    main_lo->addWidget(pic);
    main_lo->addWidget(size);
    main_lo->addWidget(file);
    setLayout(main_lo);
}

CPicturePropWidget::CPicturePropWidget(QString filename, QWidget *parent) :
    QWidget(parent)
{
    QBoxLayout *main_lo = new QBoxLayout(QBoxLayout::TopToBottom, this);
    pic = new QLabel();
    size = new QLabel();
    file = new QLabel();
    if(!filename.isEmpty())
        changePic(filename);
    main_lo->addWidget(pic);
    main_lo->addWidget(size);
    main_lo->addWidget(file);
    setLayout(main_lo);
}

CPicturePropWidget::~CPicturePropWidget()
{
    delete file;
    delete size;
    delete pic;
}

void CPicturePropWidget::changePic(QString filename)
{
    QPixmap pM(filename);
    pic->setPixmap(pM);
    size->setText(tr("Размер: ") + QString::number(pic->pixmap()->width()) + 'x' + QString::number(pic->pixmap()->height()));
    file->setText(tr("Файл: ") + filename);
}
