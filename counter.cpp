#include "counter.hpp"

Counter::Counter(QWidget *parent) :
    QLineEdit(parent)
{
}

void Counter::focusInEvent(QFocusEvent *)
{
    val = text();
    setText("");
    home(true);
}

void Counter::focusOutEvent(QFocusEvent *)
{
    setText(val);
}
