#include "mainwindow.h"
#include "savedialog.h"
#include <ui_savedialog.h>
#include "ui_mainwindow.h"
#include <QtCore>
#include <QPainter>
#include <QtXml/QtXml>
#include <QMessageBox>
#include <QMenu>
#include <QXmlReader>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralWidget->setMinimumWidth(1200);
    ui->centralWidget->setMinimumHeight(600);
    int weight=ui->centralWidget->width();
    int height=ui->centralWidget->height();

    for(int x=0;x+50<=weight;x+=50){

        for (int y=0;y+50<=height;y+=50) {

            QRect rectangle(x,y,50,50);

            Layout layout;
            layout.rect = rectangle;
            layout.color = QColor(255,255,255);
            layout.xstart=x;
            layout.ystart=y;
            layout.xfinish=x+50;
            layout.yfinish=y+50;

            m_Layouts.push_back(layout);

        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    appQuit();
}

int x1,x2,ybir,yiki;
bool presshavepoint=false;
SaveDialog *savedialog;

void MainWindow::ShowContextMenu(const QPoint &pos)
{
   QMenu contextMenu(tr("Context menu"), this);

   QAction action1("SAVE", this);
   connect(&action1, SIGNAL(triggered()),this,SLOT(showInfoDisplay()));
   contextMenu.addAction(&action1);

   QAction action2("UNDO",this);
   connect(&action2,SIGNAL(triggered()),this,SLOT(undo()));
   contextMenu.addAction(&action2);



   QAction action3("CLEAR",this);
   connect(&action3,SIGNAL(triggered()),this,SLOT(clearForm()));
   contextMenu.addAction(&action3);

   QAction action4("EXIT",this);
   connect(&action4,SIGNAL(triggered()),this,SLOT(appQuit()));
   contextMenu.addAction(&action4);

   contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::undo()
{
    if(createdLayout.size()>0){

        int size=createdLayout.size()-1;

        for (int i=0;i<m_Layouts.size();i++) {

            if(m_Layouts[i].xstart>=createdLayout[size].xstart &&
                    m_Layouts[i].xfinish<=createdLayout[size].xfinish &&
                    m_Layouts[i].ystart>=createdLayout[size].ystart &&
                    m_Layouts[i].yfinish<=createdLayout[size].yfinish){

                m_Layouts[i].dolu=false;
            }
        }

        createdLayout.pop_back(); //delete from createdlayout
        update();
    }
    else {
        QMessageBox::information(this,"INFORMATION","FORM IS EMPTY");
    }
}

void MainWindow::showInfoDisplay()
{
    if(createdLayout.size()>0){
        savedialog=new SaveDialog();
        savedialog->maindialog=this;
        savedialog->show();
    }
    else{
        QMessageBox::information(this,"ERROR","THERE ISN'T ANY LAYOUT");
    }
}
bool ctrlactive=false;

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key()==16777249){ //press ctrl
        ctrlactive=true;

    }
    else if (e->key()==90) { //press z
        if(ctrlactive){
            undo();
        }
    }
    else if (e->key()==83) { //press s
        if(ctrlactive){
            showInfoDisplay();
        }
    }
    else if (e->key()==67) { //press c
        if(ctrlactive){
            clearForm();
        }
    }
    else if (e->key()==69) { //press e
        if(ctrlactive){
            appQuit();
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(e->key()==16777249){ //release ctrl
        ctrlactive=false;
    }
}

void MainWindow::appQuit()
{
    QApplication::quit();
}






void MainWindow::clearForm()
{
    if(createdLayout.size()>0){

        createdLayout.clear();

        for (int i=0;i<m_Layouts.size();i++){

            m_Layouts[i].dolu=false;
        }
        update();
    }
    else {
        QMessageBox::information(this,"INFORMATION","AREA IS ALREADY EMPTY !!!");
    }
}

bool MainWindow::areaControl(int xstart,int xfinish,int ystart,int yfinish)
{
    for (int i=0;i<m_Layouts.size();i++) {

        if(m_Layouts[i].xstart>=xstart &&
     m_Layouts[i].xfinish<=xfinish &&
                 m_Layouts[i].ystart>=ystart &&
    m_Layouts[i].yfinish<=yfinish){

            if(m_Layouts[i].dolu==true)
                return false;
        }
    }
    return true;
}

void MainWindow::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(3);
    painter.setPen(pen);

    for(int i=0;i<m_Layouts.size();i++){
        painter.drawRect(m_Layouts[i].rect);
        painter.fillRect(m_Layouts[i].rect,m_Layouts[i].color);
    }


    for (int i=0;i<createdLayout.size();i++) {
        painter.drawRect(createdLayout[i].rect);
        painter.fillRect(createdLayout[i].rect,createdLayout[i].color);
    }

    if(templayout.isActive){
        painter.drawRect(templayout.rect);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton){
        ShowContextMenu(event->pos());
    }

    if(event->button() == Qt::LeftButton){

        templayout.isActive=true;
        templayout.xstart=event->x();
        templayout.ystart=event->y();

        if(point_control(event->x(),event->y())){
            presshavepoint=false;

                for (int i=0;i<m_Layouts.size();i++) {

                    if(m_Layouts[i].xstart<event->x() &&
                            m_Layouts[i].xfinish>event->x() &&
                             m_Layouts[i].ystart<event->y() &&
                            m_Layouts[i].yfinish>event->y()){

                        x1=m_Layouts[i].xstart;
                        ybir=m_Layouts[i].ystart;
                    }
                }
        }
        else {
            presshavepoint=true;
        }
    }
}





void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        templayout.isActive=false;

        if(presshavepoint==false){
            if(point_control(event->x(),event->y())){
                for (int i=0;i<m_Layouts.size();i++) {

                    if(m_Layouts[i].xstart<event->x() &&
            m_Layouts[i].xfinish>event->x() &&
                       m_Layouts[i].ystart<event->y() &&
            m_Layouts[i].yfinish>event->y()){

                        x2=m_Layouts[i].xfinish;
                        yiki=m_Layouts[i].yfinish;

                        //swapping
                        if(x1>=x2){
                            int temp=x2;
                            x2=x1+50;
                            x1=temp-50;
                        }
                        if(ybir>=yiki){
                            int temp=yiki;
                            yiki=ybir+50;
                            ybir=temp-50;
                        }

                        if(areaControl(x1,x2,ybir,yiki)){
                            for (int j=0;j<m_Layouts.size();j++) {
                                if(m_Layouts[j].xstart>=x1 &&
                        m_Layouts[j].xfinish<=x2 &&
                                   m_Layouts[j].ystart>=ybir &&
            m_Layouts[j].yfinish<=yiki){

                                      m_Layouts[j].dolu=true;
                                }
                            }

                            QRect rectangle(x1,ybir,(x2-x1),(yiki-ybir));

                            Layout layout;
                            layout.rect = rectangle;
                            layout.color = QColor(rand() % 255,rand() % 255,rand() % 255);
                            layout.xstart=x1;
                            layout.ystart=ybir;
                            layout.xfinish=x2;
                            layout.yfinish=yiki;
                            createdLayout.push_back(layout);
                            update(); //for paintevent running
                        }
                        else {
                            QMessageBox::information(this,"ERROR","AREA IS NOT EMPTY !!!");
                        }
                    }
                }
            }
            else {
                QMessageBox::information(this,"ERROR","LAST COORD IS FULL !!!");
            }
        }
        else {
            QMessageBox::information(this,"ERROR","FIRST COORD IS FULL !!!");
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(templayout.isActive){

        QRect rectangle(templayout.xstart,templayout.ystart,
             (event->x()-templayout.xstart),(event->y()-templayout.ystart));
        templayout.rect=rectangle;
        update();
    }
}

bool MainWindow::point_control(int x, int y)
{
    for (int i=0;i<m_Layouts.size();i++) {
        if(m_Layouts[i].xstart<x && m_Layouts[i].xfinish>x &&
                 m_Layouts[i].ystart<y &&  m_Layouts[i].yfinish>y){
            if (m_Layouts[i].dolu==false) {
                return true;
            }
            else {
                return false;
            }
        }
     }
}

void MainWindow::XML_Save(QString name,QString shortcut,int ontoolbar)
{
    QFile file("C:/Qt/examples/infodif/MyXML.xml");
    QDomDocument document;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        document.setContent(&file);
        file.close();
    }

    QDomElement xmlroot=document.firstChildElement();
    document.appendChild(xmlroot);

    //QDomElement root=document.createElement("Layouts"); //for first running
    //document.appendChild(root);                         //first element must be "layouts"

    QDomElement layout=document.createElement("Layout");
    layout.setAttribute("name",name);
    layout.setAttribute("short_cut",shortcut);
    layout.setAttribute("on_tool_bar",ontoolbar);
    xmlroot.appendChild(layout);

    for (int i = 0; i < createdLayout.size(); i++) {
        QDomElement position=document.createElement("Position");
        position.setAttribute("number",QString::number(i));
        position.setAttribute("row",createdLayout[i].ystart);
        position.setAttribute("column",createdLayout[i].xstart);
        position.setAttribute("row_span",(createdLayout[i].xfinish-createdLayout[i].xstart));
        position.setAttribute("column_span",(createdLayout[i].yfinish-createdLayout[i].ystart));
        layout.appendChild(position);
    }

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::information(this,"ERROR","FILE DIDN'T OPEN !");
    }
    else {
        QTextStream stream(&file);
        stream<<document.toString();
        file.close();
        QMessageBox::information(this,"INFORMATION","SUCCESFULLY");
    }
}
