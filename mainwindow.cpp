#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)

{
    menuInit();

    QBoxLayout *main_lo = new QBoxLayout(QBoxLayout::TopToBottom, this);
    QBoxLayout *horizontal_lo = new QBoxLayout(QBoxLayout::LeftToRight, this);
    network = new QCNNetwork(this);
    network->setSceneRect(0, 0, 5000, 5000);

    gView = new QGraphicsView(network);
    gView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    gView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    resize(800, 600);
    pictureProp = new CPicturePropWidget();
    prop = new CPropertiesWidget();
    connect(network, SIGNAL(itemsSelected(QList<QGraphicsItem*>)),
            this, SLOT(addPropOf(QList<QGraphicsItem*>)));
    editor = new CCodeEditorWidget();

    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidget(prop);
    scrollArea->setFixedWidth(160);
    scrollArea->setWidgetResizable(true);

    horizontal_lo->addWidget(scrollArea);
    horizontal_lo->addWidget(gView);
    horizontal_lo->addWidget(pictureProp);
    horizontal_lo->setAlignment(pictureProp, Qt::AlignTop);

    main_lo->addLayout(horizontal_lo);
    main_lo->addWidget(editor);

    QWidget *mainWidget = new QWidget(this);
    mainWidget->setLayout(main_lo);
    setCentralWidget(mainWidget);
    setWindowTitle(tr("Редактор нейронных сетей"));
}

void MainWindow::createStudyPack()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Открыть папку"), ".");
    QDir directory(dir);
    QStringList files = directory.entryList(QStringList() << "*.bmp", QDir::Files | QDir::Readable);
    if(files.isEmpty())
        return;
    CStudyPackWidget *widget = new CStudyPackWidget(dir, files);
    widget->setWindowModality(Qt::WindowModal);
    widget->show();
}

void MainWindow::setStudyPack()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Выбор обучающей выборки"), ".", tr("Study pack (*.spk)"));
    studyPack = file;
}

void MainWindow::setStudyFunc(QString file)
{
    if(!file.isEmpty())
    {
        QByteArray filearr = file.toLocal8Bit();
        network->setStudyFunc(filearr.data(), filearr.size());
    }
}

void MainWindow::studyNet()
{
    QStringList files;
    QStringList outputs;
    if(network->getLastLayer() == NULL)
    {
        QMessageBox msg;
        msg.setText(tr("Не был выбран выходной слой нейросети"));
        msg.setInformativeText(tr("Выберите выходной слой."));
        msg.exec();
        return;
    }
    Vector<double> studyOutput(network->getLastLayer()->getNeurons().size(), 0.0);
    if(!studyPack.isEmpty())
    {
        QString line;
        QFile in(studyPack);
        in.open(QIODevice::Text | QIODevice::ReadOnly);
        if(in.isOpen())
        {
            while(!in.atEnd())
            {
                line = in.readLine();
                QStringList parts = line.split('\t');
                if(parts.size() > 1)
                {
                    files.append(parts.first());
                    outputs.append(parts.at(1));
                }
            }
        }
        in.close();
    }
    for(int i = 0; i < files.size(); i++)
    {
        inputFile = files.at(i);
        QByteArray filearr = inputFile.toLocal8Bit();
        network->setInput(loadBMP(filearr.data()));
        QStringList chunks = outputs.at(i).split(' ', QString::SkipEmptyParts);
        for(unsigned int j = 0; j < chunks.size(); j++)
        {
            QStringList mChunks =  chunks.at(j).split(':');
            if(mChunks.size() == 1 && j < studyOutput.size())
                studyOutput[j] = mChunks.first().toDouble();
            else
            {
                int k = mChunks.first().toInt();
                if(k < studyOutput.size() && k >= 0)
                    studyOutput[k] = mChunks.at(1).toDouble();
            }
        }
        for(unsigned int j = 0; j < studyOutput.size(); j++)
        network->setOutput(studyOutput);
        network->study();
    }
}

void MainWindow::loadNet()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Загрузка нейросети"), ".", tr("Neural net (*.nnf)"));
    QByteArray filearr = file.toLocal8Bit();
    network->loadNet(filearr.data());
}

void MainWindow::saveNet()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Сохранение нейросети"), ".", tr("Neural net (*.nnf)"));
    QByteArray filearr = file.toLocal8Bit();
    network->saveNet(filearr.data());
}

void MainWindow::saveWeightTable()
{
    QString file = QFileDialog::getSaveFileName(this, tr("Сохранениеь таблицы весов"), ".", tr("Weights table (*.nwt)"));
    QByteArray filearr = file.toLocal8Bit();
    network->saveWeights(filearr.data());
}

void MainWindow::setInput()
{
    inputFile = QFileDialog::getOpenFileName(this, tr("Выбор входного файла"), ".", tr("Image (*.bmp)"));
    pictureProp->changePic(inputFile);
}

void MainWindow::emulate()
{
    if(inputFile.isEmpty())
    {
        QMessageBox warning;
        warning.setText(tr("Ошибка"));
        warning.setInformativeText(tr("Вы не выбрали входной файл"));
        warning.exec();
    }
    else
    {
        QByteArray filearr = inputFile.toLocal8Bit();
        Vector<double> in = loadBMP(filearr.data());
        network->setInput(in);
        network->emulate();
    }
}

void MainWindow::addPropOf(QList<QGraphicsItem *> items)
{
    prop->clearProp();
    QWidget *wProp = NULL;
    QCLayer *layer = NULL;
    QCNeuron *neuron = NULL;
    CConnection *connection = NULL;
    for(int i = 0; i < items.size(); i++)
        if((layer = dynamic_cast<QCLayer *>(items.at(i))) != NULL)
        {
            wProp = new CLayerPropertiesWidget(layer);
            prop->addProp(wProp);
        }
        else if((neuron = dynamic_cast<QCNeuron *>(items.at(i))) != NULL)
        {
            wProp = new CNeuronPropertiesWidget(neuron);
            prop->addProp(wProp);
        }

        else if((connection = dynamic_cast<CConnection *>(items.at(i))) != NULL)
        {
            wProp = new CConnectionPropertiesWidget(connection);
            prop->addProp(wProp);
        }
}

void MainWindow::fullStudyFuncList()
{
    for(int i = 0; i < stFGroup->actions().size(); i++)
        delete stFGroup->actions()[i];
    stFGroup->actions().clear();
    QDir studyDir;
    studyDir.cd("study");
    QStringList studyList = studyDir.entryList(QStringList() << "*.lua", QDir::Files);
    for(int i = 0; i < studyList.size(); i++)
    {
        QAction *stF = new QAction(studyDir.filePath(studyList.at(i)), this);
        stF->setCheckable(true);
        stFGroup->addAction(stF);
        connect(stF, SIGNAL(triggered()),
                signalMapper, SLOT(map()));
        signalMapper->setMapping(stF, stF->text());
    }
    studyFunc->addActions(stFGroup->actions());
}

MainWindow::~MainWindow()
{
    delete gView;
    delete network;
    delete editor;
    delete prop;
    delete pictureProp;
    delete studyFunc;
    delete stFGroup;
    delete signalMapper;
}

Vector<double> MainWindow::loadBMP(StdString file)
{
    Vector<double> res;
    if(file == NULL)
        return res;
    int bound = 128;
    QImage image(file);
    if(image.isNull())
        return res;
    for(int i = 0; i < image.width(); i++)
    {
        for(int j = 0; j < image.height(); j++)
        {
            QColor color(image.pixel(i, j));
            if((color.red() + color.green() + color.blue())/3 <= bound)
                res.push_back(1.0);
            else
                res.push_back(0.0);
        }
    }
    return res;
}

void MainWindow::menuInit()
{
    QMenu *file = menuBar()->addMenu(tr("&Файл"));
    QMenu *study = menuBar()->addMenu(tr("Обучение"));
    QMenu *net = menuBar()->addMenu(tr("Сеть"));
    QAction *setInput = new QAction(tr("Открыть файл"), this);
    QAction *emulate = new QAction(tr("Запустить эмуляцию"), this);
    QAction *createStudyPack = new QAction(tr("Создать обучающую выборку"), this);
    QAction *setStudyPack = new QAction(tr("Выбрать обучающую выборку"), this);
    QAction *studyNet = new QAction(tr("Обучить нейросеть"), this);
    studyFunc = new QMenu(tr("Алгоритм обучения"));
    QAction *refresh = new QAction(tr("Обновить"), this);
    studyFunc->addAction(refresh);
    studyFunc->addSeparator();

    signalMapper = new QSignalMapper(this);
    stFGroup = new QActionGroup(this);
    fullStudyFuncList();
    connect(signalMapper, SIGNAL(mapped(QString)),
            this, SLOT(setStudyFunc(QString)));
    stFGroup->setExclusive(true);

    QAction *loadNet = new QAction(tr("Загрузить нейросеть"), this);
    //loadNet->setEnabled(false);
    QAction *saveNet = new QAction(tr("Сохранить как..."), this);
    QAction *saveWeightTable = new QAction(tr("Сохранить таблицу весов"), this);
    QAction *exit = new QAction(tr("Выход"), this);

    connect(setInput, SIGNAL(triggered()),
            this, SLOT(setInput()));
    connect(emulate, SIGNAL(triggered()),
            this, SLOT(emulate()));
    connect(createStudyPack, SIGNAL(triggered()),
            this, SLOT(createStudyPack()));
    connect(setStudyPack, SIGNAL(triggered()),
            this, SLOT(setStudyPack()));
    connect(studyNet, SIGNAL(triggered()),
            this, SLOT(studyNet()));
    connect(refresh,SIGNAL(triggered()),
            this, SLOT(fullStudyFuncList()));
    connect(loadNet, SIGNAL(triggered()),
            this, SLOT(loadNet()));
    connect(saveNet, SIGNAL(triggered()),
            this, SLOT(saveNet()));
    connect(saveWeightTable, SIGNAL(triggered()),
            this, SLOT(saveWeightTable()));
    connect(exit, SIGNAL(triggered()),
            this, SLOT(close()));

    file->addAction(loadNet);
    file->addAction(saveNet);
    file->addAction(saveWeightTable);
    file->addAction(exit);
    study->addAction(createStudyPack);
    study->addAction(setStudyPack);
    study->addMenu(studyFunc);
    study->addAction(studyNet);
    net->addAction(setInput);
    net->addAction(emulate);
}
