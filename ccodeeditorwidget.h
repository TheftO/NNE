#ifndef CCODEEDITORWIDGET_H
#define CCODEEDITORWIDGET_H

#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QBoxLayout>
#include <QDir>
#include <QFileDialog>
#include "ccodeeditor.h"
#include "ccodeeditortabs.h"

class CCodeEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CCodeEditorWidget(QWidget *parent = 0);
    CCodeEditorWidget(QStringList fileNames, QWidget *parent = 0);
    ~CCodeEditorWidget();
    void saveIntoFile(QString file);
    void openFile(QString file);
    void openInNewTab(QString file);
private:
    CCodeEditorTabs *codeEditors;
    //int untitledIndx;
    //QString file;
    QMenuBar* menuBar;
    QAction *newFile;
    QAction *saveFile;
    QAction *saveAs;
    QAction *loadFile;
    QAction *loadInNewTab;
    QAction *exit;
    void setupMenu();
    int saveQuestion();
signals:
    void acceptChanges(QWidget *wid);
public slots:
    void newFileSlot();
    void saveSlot();
    void saveAsSlot();
    void loadSlot();
    void loadInNewSlot();
    void exitSlot();
    void exitSlot(int index);
};

#endif // CCODEEDITORWIDGET_H
