#ifndef READONLYLINEEDIT_H
#define READONLYLINEEDIT_H

#include <QLineEdit>

class ReadOnlyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ReadOnlyLineEdit(QWidget *parent = 0);

signals:

public slots:

protected slots:
    void mousePressEvent(QMouseEvent * e);
};

#endif // READONLYLINEEDIT_H
