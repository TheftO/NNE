#ifndef CCODEEDITOR_H
#define CCODEEDITOR_H

#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>

class CCodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit CCodeEditor(QWidget *parent = 0);
    ~CCodeEditor();
    int lineNumberAreaWidth();
    void setFile(QString file)
    {
        this->file = file;
    }
    QString getFile()
    {
        return file;
    }
    void setUnsavedChanges(bool changes)
    {
        unsavedChanges = changes;
    }
    bool getUnsavedChanges()
    {
        return unsavedChanges;
    }
    void lineNumberAreaPaintEvent(QPaintEvent *event);
protected:
    void resizeEvent(QResizeEvent *event);
private:
    QWidget *lineNumberArea;
    QString file;
    bool unsavedChanges;
signals:
    
public slots:

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect & rect, int dy);
    void changed();
};

class CLineNumber : public QWidget
{
    Q_OBJECT
public:
    explicit CLineNumber(CCodeEditor *editor) : QWidget(editor)
    {
        codeEditor = editor;
    }

    QSize sizeHint() const
    {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }
protected:
    void paintEvent(QPaintEvent *event)
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }
private:
    CCodeEditor* codeEditor;
signals:

public slots:

};
#endif // CCODEEDITOR_H
