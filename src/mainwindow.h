#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QGraphicsView>
#include <QSignalMapper>
#include <QRgb>
#include <fstream>
#include "DEFINES.h"
#include "qcnnetwork.h"
#include "cstudypackwidget.h"
#include "cpropertieswidget.h"
#include "cpicturepropwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Vector<double> loadBMP(StdString file);
private:
    QGraphicsView *gView; //View для QCNNetwork
    QCNNetwork *network;
    CCodeEditorWidget *editor;
    CPropertiesWidget *prop;
    CPicturePropWidget *pictureProp;
    QMenu *studyFunc;
    QSignalMapper *signalMapper;
    QActionGroup *stFGroup;
    QString inputFile;
    QString studyPack;
    void menuInit();
private slots:
    void createStudyPack();
    void setStudyPack();
    void setStudyFunc(QString file);
    void studyNet();
    void loadNet();
    void saveNet();
    void saveWeightTable();
    void setInput();
    void emulate();
    void addPropOf(QList<QGraphicsItem *> item);
    void fullStudyFuncList();
};

#endif // MAINWINDOW_H
