#include "ccodeeditorwidget.h"

CCodeEditorWidget::CCodeEditorWidget(QWidget *parent) : QWidget(parent)
{
    setupMenu();
    codeEditors->setTabsClosable(true);
    codeEditors->setUsesScrollButtons(true);
    codeEditors->newTab();
}

CCodeEditorWidget::CCodeEditorWidget(QStringList fileNames, QWidget *parent) :
    QWidget(parent)
{
    setupMenu();
    codeEditors->setTabsClosable(true);
    codeEditors->setUsesScrollButtons(true);
    for(int i = 0; i < fileNames.size(); i++)
        openInNewTab(fileNames.at(i));
}

CCodeEditorWidget::~CCodeEditorWidget()
{
    delete newFile;
    delete loadFile;
    delete saveFile;
    delete saveAs;
    delete exit;
    delete codeEditors;
    delete menuBar;
}

void CCodeEditorWidget::setupMenu()
{
    QBoxLayout *main_lo = new QBoxLayout(QBoxLayout::TopToBottom, this);
    menuBar = new QMenuBar(this);
    QMenu *fileMenu = new QMenu(tr("Файл"), this);
    newFile = new QAction(tr("Новый файл"), this);
    loadFile = new QAction(tr("Открыть"), this);
    loadInNewTab = new QAction(tr("Открыть в новой вкладке"), this);
    saveFile = new QAction(tr("Сохранить"), this);
    saveAs = new QAction(tr("Сохранить как..."), this);
    exit = new QAction(tr("Закрыть"), this);
    codeEditors = new CCodeEditorTabs();
    connect(newFile, SIGNAL(triggered()),
            this, SLOT(newFileSlot()));
    connect(loadFile, SIGNAL(triggered()),
            this, SLOT(loadSlot()));
    connect(loadInNewTab, SIGNAL(triggered()),
            this, SLOT(loadInNewSlot()));
    connect(saveFile, SIGNAL(triggered()),
            this, SLOT(saveSlot()));
    connect(saveAs, SIGNAL(triggered()),
            this, SLOT(saveAsSlot()));
    connect(exit, SIGNAL(triggered()),
            this, SLOT(exitSlot()));
    connect(codeEditors, SIGNAL(tabCloseRequested(int)),
            this, SLOT(exitSlot(int)));
    connect(this, SIGNAL(acceptChanges(QWidget*)),
            codeEditors, SLOT(changesAccepted(QWidget*)));
    menuBar->addMenu(fileMenu);
    fileMenu->addAction(newFile);
    fileMenu->addAction(loadFile);
    fileMenu->addAction(loadInNewTab);
    fileMenu->addAction(saveFile);
    fileMenu->addAction(saveAs);
    fileMenu->addAction(exit);
    main_lo->addWidget(menuBar);
    main_lo->addWidget(codeEditors);
    setLayout(main_lo);
}

int CCodeEditorWidget::saveQuestion()
{
    QMessageBox msg;
    msg.setText(tr("Сохранить?"));
    msg.setInformativeText(tr("Весь несохраненный прогресс будет утерян, сохранить?"));
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Save);
    return msg.exec();
}

void CCodeEditorWidget::newFileSlot()
{
    codeEditors->newTab();
}

void CCodeEditorWidget::saveSlot()
{
    QString file = dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getFile();
    if(file.isEmpty())
        saveAsSlot();
    else
        saveIntoFile(file);
}

void CCodeEditorWidget::saveAsSlot()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Сохранить как..."), ".", tr("Lua script (*.lua)"));
    saveIntoFile(file);
}

void CCodeEditorWidget::loadSlot()
{
    QString file = dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getFile();
    if(dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getUnsavedChanges())
    {
        int choice = saveQuestion();
        switch(choice)
        {
            case QMessageBox::Cancel:
            {
                return;
                break;
            }
            case QMessageBox::Save:
            {
                if(file.isEmpty())
                {
                    saveAsSlot();
                }else
                {
                    saveSlot();
                }
            }
            case QMessageBox::Discard:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
    file = QFileDialog::getOpenFileName(this, tr("Открыть"), ".", tr("Lua script (*.lua)"));
    openFile(file);
}

void CCodeEditorWidget::loadInNewSlot()
{
    QString file = dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getFile();
    if(dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getUnsavedChanges())
    {
        int choice = saveQuestion();
        switch(choice)
        {
            case QMessageBox::Cancel:
            {
                return;
                break;
            }
            case QMessageBox::Save:
            {
                if(file.isEmpty())
                {
                    saveAsSlot();
                }else
                {
                    saveSlot();
                }
            }
            case QMessageBox::Discard:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
    file = QFileDialog::getOpenFileName(this, tr("Открыть"), ".", tr("Lua script (*.lua)"));
    openInNewTab(file);
}

void CCodeEditorWidget::exitSlot()
{
    exitSlot(codeEditors->currentIndex());
}

void CCodeEditorWidget::exitSlot(int index)
{
    QString file = dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getFile();
    if(dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->getUnsavedChanges())
    {
        int choice = saveQuestion();
        switch(choice)
        {
            case QMessageBox::Cancel:
            {
                return;
                break;
            }
            case QMessageBox::Save:
            {
                if(file.isEmpty())
                {
                    saveAsSlot();
                }else
                {
                    saveSlot();
                }
            }
            case QMessageBox::Discard:
            {
                break;
            }
            default:
            {
                break;
            }
        }
    }
    codeEditors->removeTab(index);
    if(codeEditors->count() < 1)
        codeEditors->newTab();
}

void CCodeEditorWidget::saveIntoFile(QString file)
{
    QFile fileHandler(file);
    if(!fileHandler.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    fileHandler.write(dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->toPlainText().toLocal8Bit());
    fileHandler.close();
    QDir dir;
    codeEditors->setTabText(codeEditors->currentIndex(), dir.relativeFilePath(file));
    emit acceptChanges(codeEditors->currentWidget());
}

void CCodeEditorWidget::openFile(QString file)
{
    dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->clear();
    QFile fileHandler(file);
    if(!fileHandler.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString line;
    while(!fileHandler.atEnd())
        line += fileHandler.readLine();
    fileHandler.close();
    dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->appendPlainText(line);
    QDir dir;
    dynamic_cast<CCodeEditor *>(codeEditors->currentWidget())->setFile(file);
    codeEditors->setTabText(codeEditors->currentIndex(), dir.relativeFilePath(file));
    emit acceptChanges(codeEditors->currentWidget());
}

void CCodeEditorWidget::openInNewTab(QString file)
{
    codeEditors->setCurrentIndex(codeEditors->newTab(file));
    openFile(file);
}
