#ifndef CCODEEDITORTABS_H
#define CCODEEDITORTABS_H

#include <QTabWidget>
#include <QSignalMapper>
#include "ccodeeditor.h"

class CCodeEditorTabs : public QTabWidget
{
    Q_OBJECT
public:
    explicit CCodeEditorTabs(QWidget *parent = 0);
    ~CCodeEditorTabs();
    int newTab(QString file = QString());
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
private:
    QSignalMapper *mapper;
    int untitledIndx;
signals:
public slots:
    void tabChanged(QWidget *wid);
    void changesAccepted(QWidget *wid);
};

#endif // CCODEEDITORTABS_H
