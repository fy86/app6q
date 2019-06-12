#ifndef EDITORIDSTR_H
#define EDITORIDSTR_H

#include <QObject>

class editorIDstr : public QObject
{
    Q_OBJECT
public:
    explicit editorIDstr(QObject *parent = 0);

    QString m_IDstr;
    int m_cursor;  // == QString.at(idx)
    void setStr(QString id);
    void setStep();
    void inc();
    void dec();
    void moveRight();
    void moveLeft();
    int getCursor();
    QString getStrFormat();


signals:

public slots:

};

#endif // EDITORIDSTR_H
