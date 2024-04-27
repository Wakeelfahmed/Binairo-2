#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>


#include"gameboard.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void Select_Starting_Method();
    void Set_Button_Interface();
    void createTopLayout(QVBoxLayout *);
    void createGridLayout(QVBoxLayout *mainLayout);
    void clearGridLayout();
private:
    GameBoard board;
    Ui::MainWindow *ui;
    QGridLayout *gridLayout;

private slots:
    void onRadioButtonClicked();
    void onPauseButtonClicked();
    void onResetButtonClicked();
    void handleButtonClick();

};
#endif // MAINWINDOW_H
