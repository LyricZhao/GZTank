#ifndef HTP_H
#define HTP_H

#include <QDialog>

namespace Ui {
class HTP;
}

class HTP : public QDialog
{
    Q_OBJECT

public:
    explicit HTP(QWidget *parent = 0);
    ~HTP();

private:
    Ui::HTP *ui;
};

#endif // HTP_H
