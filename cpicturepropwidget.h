#ifndef CPICTUREPROPWIDGET_H
#define CPICTUREPROPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QBoxLayout>
#include <QLabel>

class CPicturePropWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CPicturePropWidget(QWidget *parent = 0);
    explicit CPicturePropWidget(QString filename, QWidget *parent = 0);
    ~CPicturePropWidget();
private:
    QLabel *pic;
    QLabel *size;
    QLabel *file;
signals:
    
public slots:
    void changePic(QString filename);
};

#endif // CPICTUREPROPWIDGET_H
