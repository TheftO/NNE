#ifndef CStudyPackWidget_H
#define CStudyPackWidget_H

#include <QWidget>
#include <QLineEdit>
#include <QBoxLayout>
#include <QLabel>
#include <QPicture>
#include <QPushButton>
#include <QDir>
#include <fstream>

class CStudyPackWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CStudyPackWidget(QString path, QStringList files, QWidget *parent = 0);
    ~CStudyPackWidget();
    void updatePic();
    void updateLine(int prevIndex);
    //static int loadStudyPack(QString file, QStringList *outputs, QStringList *files = NULL);
private:
    QDir *directory;
    QStringList files;
    QStringList outputs;
    int index;
    QLineEdit *outputLine;
    QLabel *picture;
signals:
    
public slots:
    void previous();
    void next();
    void save();
    void checkInput();
};

#endif // CStudyPackWidget_H
