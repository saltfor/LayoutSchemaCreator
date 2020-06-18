#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <vector>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QLineEdit>
#include <QRadioButton>
namespace Ui {
class MainWindow;
}



struct Layout
{
    QRect rect;
    QColor color;
    int xstart,ystart,xfinish,yfinish;
    bool dolu=false;
};
struct TempLayout{
    int xstart,ystart,xfinish,yfinish;
    QRect rect;
    bool isActive;
    QColor color;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    virtual void paintEvent(QPaintEvent *event);
    bool point_control(int x,int y);
    bool areaControl(int xstart,int xfinish,int ystart,int yfinish);
    void ShowContextMenu(const QPoint &pos);
    void XML_Save(QString name,QString shortcut,int ontoolbar);


private:
    Ui::MainWindow *ui;
    std::vector<Layout> m_Layouts;
    std::vector<Layout> createdLayout;
    TempLayout templayout;

private slots:
    void showInfoDisplay();
    void appQuit();
    void undo();

public slots:
    void clearForm();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void keyReleaseEvent(QKeyEvent *e) override;
};

#endif // MAINWINDOW_H
