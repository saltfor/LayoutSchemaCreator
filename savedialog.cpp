#include "savedialog.h"
#include "ui_savedialog.h"
#include <mainwindow.h>
#include <QMessageBox>
#include <QDebug>

SaveDialog::SaveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaveDialog)
{
    ui->setupUi(this);
    this->activateWindow();
    this->topLevelWidget();
}

SaveDialog::~SaveDialog()
{
    delete ui;
    this->close();
}

void SaveDialog::on_pushButton_2_clicked()
{
    this->close();
}

void SaveDialog::on_pushButton_clicked()
{
    saveData();
}

QString shortcut;

void SaveDialog::keyPressEvent(QKeyEvent *e){
    if (e->key()==16777251) { //alt
        shortcut="ALT";
    }
    if (e->key()==16777249) { //ctrl
        shortcut="CTRL";
    }
}

void SaveDialog::keyReleaseEvent(QKeyEvent *e){
    if (e->key()==16777251) { //alt
        shortcut="";
    }
    if (e->key()==16777249) { //ctrl
        shortcut="";
    }
}

void SaveDialog::saveData(){
    if(ui->lineEdit1->text().trimmed()!="" &&
            ui->lineEdit2->text().trimmed()!=""){

        if(ui->radioButton->isChecked()){
            maindialog->XML_Save(ui->lineEdit1->text().trimmed(),ui->lineEdit2->text().trimmed(),0);
        }
        else{
            maindialog->XML_Save(ui->lineEdit1->text().trimmed(),ui->lineEdit2->text().trimmed(),1);
        }
        this->close();
        maindialog->clearForm();
    }
    else {
        QMessageBox::information(this,"ERROR","CAN NOT BE EMPTY AREA FOR SAVING");
    }
}

void SaveDialog::on_lineEdit2_textEdited(const QString &arg1)
{
    if (shortcut!="") {
        ui->lineEdit2->setText(shortcut+" + "+arg1.at(arg1.size()-1));
    }
    else {
        ui->lineEdit2->setText("");
    }
}
