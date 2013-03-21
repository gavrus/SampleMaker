#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <QLineEdit>

class Counter : public QLineEdit
{
    Q_OBJECT
public:
    explicit Counter(QWidget *parent = 0);
    virtual void focusInEvent(QFocusEvent *);
    virtual void focusOutEvent(QFocusEvent *);

    QString val;
};

#endif // COUNTER_HPP
