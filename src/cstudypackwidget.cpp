#include "cstudypackwidget.h"

CStudyPackWidget::CStudyPackWidget(QString path, QStringList filelist, QWidget *parent) :
    QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle(tr("Создание обучающей выборки"));
    directory = new QDir(path);
    files = filelist;
    files = directory->entryList(QStringList() << "*.bmp", QDir::Files | QDir::Readable);
    if(files.isEmpty())
        close();
    for(int i = 0; i < files.size(); i++)
        outputs.append("");
    QString studyPack;
    if(!directory->entryList(QStringList() << "*.spk", QDir::Files | QDir::Readable).isEmpty())
    {
        for(int i = 0; i < directory->entryList(QStringList() << "*.spk", QDir::Files | QDir::Readable).size(); i++)
            if(directory->entryList(QStringList() << "*.spk", QDir::Files | QDir::Readable).at(i) == (directory->dirName() + "*.spk"))
                studyPack = directory->entryList(QStringList() << "*.spk", QDir::Files | QDir::Readable).at(i);
        if(studyPack.isEmpty())
            studyPack = QStringList(directory->entryList(QStringList() << "*.spk", QDir::Files | QDir::Readable)).first();
    }
    if(!studyPack.isEmpty())
    {
        QString line;
        QFile in(directory->absoluteFilePath(studyPack));
        in.open(QIODevice::Text | QIODevice::ReadOnly);
        if(in.isOpen())
        {
            while(!in.atEnd())
            {
                line = in.readLine();
                QStringList parts = line.split('\t');
                if(files.contains(parts.first()))
                    outputs[files.indexOf(parts.first())] = parts.at(1);
            }
        }
        in.close();
    }
    index = 0;
    picture = new QLabel(this);
    outputLine = new QLineEdit(this);
    updatePic();
    updateLine(-1);
    QPushButton *prev_b = new QPushButton(tr("Предыдущий"));
    QPushButton *next_b = new QPushButton(tr("Следующий"));
    QPushButton *save_b = new QPushButton(tr("Сохранить"));
    QPushButton *close_b = new QPushButton(tr("Закрыть"));
    connect(prev_b, SIGNAL(released()),
            this, SLOT(previous()));
    connect(next_b, SIGNAL(released()),
            this, SLOT(next()));
    connect(save_b, SIGNAL(released()),
            this, SLOT(save()));
    connect(close_b, SIGNAL(released()),
            this, SLOT(close()));
    connect(outputLine, SIGNAL(textEdited(QString)),
            this, SLOT(checkInput()));
    QBoxLayout *main_lo = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *buttons_lo = new QBoxLayout(QBoxLayout::LeftToRight);
    main_lo->addWidget(picture);
    main_lo->setAlignment(picture, Qt::AlignHCenter);
    main_lo->addWidget(outputLine);
    buttons_lo->addWidget(prev_b);
    buttons_lo->addWidget(next_b);
    buttons_lo->addWidget(save_b);
    buttons_lo->addWidget(close_b);
    main_lo->addLayout(buttons_lo);
    setLayout(main_lo);
}

CStudyPackWidget::~CStudyPackWidget()
{
    delete directory;
    delete outputLine;
    delete picture;
}

void CStudyPackWidget::updatePic()
{
    if(!files.isEmpty() && index < files.size() && index >= 0)
    {
        QPixmap pic;
        pic.load(directory->filePath(files.at(index)));
        picture->setPixmap(pic);
    }
}

void CStudyPackWidget::updateLine(int prevIndex)
{
    if(prevIndex < outputs.size() && prevIndex >= 0)
        outputs[prevIndex] = outputLine->text();
    if(index < outputs.size() && index >= 0)
        outputLine->setText(outputs.at(index));
}

/*
int CStudyPackWidget::loadStudyPack(QString file, QStringList *outputs, QStringList *files)
{
    for(int i = 0; i < files.size(); i++)
                    outputs.append("");
    if(!file.isEmpty())
    {
        QString line;
        std::ifstream in;
        in.open((char *)directory->absoluteFilePath(studyPack).data());
        if(in.is_open())
        {
            while(!in.eof())
            {
                char ch;
                do{
                    in>>ch;
                    if(ch != '\t')
                        line += ch;
                    else
                        break;
                }while(true);
                QString file = line;
                do{
                    in>>ch;
                    if(ch != '\n')
                        line += ch;
                    else
                        break;
                }while(true);
                outputs[files.indexOf(file)] = line;
            }
        }
        in.close();
    }
}
*/

void CStudyPackWidget::previous()
{
    int previndex = index;
    if(index-- < 0)
        index = 0;
    updatePic();
    updateLine(previndex);
}

void CStudyPackWidget::next()
{
    int previndex = index;
    if(index++ > files.size())
        index = files.size() - 1;
    updatePic();
    updateLine(previndex);
}

void CStudyPackWidget::save()
{
    updateLine(index);
    QString file(directory->absolutePath() + '/' + directory->dirName());
    file  += ".spk";
    std::ofstream out;
    QByteArray filearr = file.toLocal8Bit();
    out.open(filearr.data());
    if(!out.is_open())
        return;
    for(int i = 0; i < files.size(); i++)
    {
        for(int j = 0; j < files.at(i).size(); j++)
            out<<files.at(i).toLocal8Bit().at(j);
        out<<'\t';
        for(int j = 0; j < outputs.at(i).size(); j++)
            out<<outputs.at(i).toLocal8Bit().at(j);
        out<<'\n';
    }
    out.close();
}

void CStudyPackWidget::checkInput()
{
    QString inputLine = outputLine->text();
    QString outLine;
    outputLine->clear();
    for(int i = 0; i < inputLine.size(); i++)
        if(inputLine.at(i).isDigit() || inputLine.at(i) == ':' || inputLine.at(i) == ' ' || inputLine.at(i) == '.')
            outLine.append(inputLine.at(i));
    outputLine->setText(outLine);
}
