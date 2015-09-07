#include "ReadOnlyLineEdit.h"

ReadOnlyLineEdit::ReadOnlyLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    this->setReadOnly(true);
}

void ReadOnlyLineEdit::mousePressEvent(QMouseEvent * e) {
    this->selectAll();
}
