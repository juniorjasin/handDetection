#ifndef PRINCIPAL_H
#define PRINCIPAL_H

#include <QWidget>

namespace Ui {
class principal;
}

class principal : public QWidget
{
    Q_OBJECT

public:
    explicit principal(QWidget *parent = 0);
    ~principal();

private:
    Ui::principal *ui;
};

#endif // PRINCIPAL_H
