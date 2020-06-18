#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H
#include <mainwindow.h>

#include <QDialog>

namespace Ui {
class SaveDialog;
}

class SaveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SaveDialog(QWidget *parent = nullptr);
    ~SaveDialog();
    MainWindow *maindialog;


private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit2_textEdited(const QString &arg1);

private:
    Ui::SaveDialog *ui;
    void saveData();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
};

#endif // SAVEDIALOG_H
