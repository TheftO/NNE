#include "ccodeeditortabs.h"

CCodeEditorTabs::CCodeEditorTabs(QWidget *parent) : QTabWidget(parent)
{
    untitledIndx = 0;
    mapper = new QSignalMapper(this);
    connect(mapper, SIGNAL(mapped(QWidget*)),
            this, SLOT(tabChanged(QWidget*)));
}

CCodeEditorTabs::~CCodeEditorTabs()
{
    delete mapper;
}

int CCodeEditorTabs::newTab(QString file)
{
    CCodeEditor *codeEdit = new CCodeEditor();
    if(file.isEmpty())
        file = tr("Untitled") + QString::number(untitledIndx++);
    int index = addTab(codeEdit, file);
    connect(codeEdit, SIGNAL(textChanged()),
            mapper, SLOT(map()));
    mapper->setMapping(codeEdit, codeEdit);
    return index;
}

void CCodeEditorTabs::mouseDoubleClickEvent(QMouseEvent *event)
{
    newTab();
    QTabWidget::mouseDoubleClickEvent(event);
}

void CCodeEditorTabs::tabChanged(QWidget *wid)
{
    if(tabText(indexOf(wid)).at(tabText(indexOf(wid)).size() - 1) != '*')
        setTabText(indexOf(wid), tabText(indexOf(wid)) + '*');
}

void CCodeEditorTabs::changesAccepted(QWidget *wid)
{
    if(tabText(indexOf(wid)).at(tabText(indexOf(wid)).size() - 1) == '*')
    {
        QString text;
        for(int i = 0; i < tabText(indexOf(wid)).size() - 1; i++)
            text.append(tabText(indexOf(wid)).at(i));
        setTabText(indexOf(wid), text);
    }
    dynamic_cast<CCodeEditor *>(wid)->setUnsavedChanges(false);
}
