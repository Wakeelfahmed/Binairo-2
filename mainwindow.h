#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QTime>
#include <QRadioButton>     //for Input or Random or File option

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
    void Select_Starting_Method();      //Seperate window for selecting method (size, random, input, file)
    void createTopLayout(QVBoxLayout *);//create top interface(fill symbol, pause button, reset button, timer display)
    void createGridLayout(QVBoxLayout *mainLayout); //create grid interface(buttons of size)
    void createInterface(); //makes top and grid layout
    void setBackgroundColor(const QColor &color);   //change bg color at win
private:
    GameBoard board;
    Ui::MainWindow *ui;
    QGridLayout *gridLayout;        //store the grid
    void storeOriginalPalette();    //save the org background color
    void restoreOriginalPalette();  //restore org bg color
    QPalette originalPalette;       //stores the orginal bg color for restoration

    QRadioButton *radioButton1, *radioButton2;

    QPushButton *pushButton1, *pushButton2;

    QLabel *elapsedTimeLabel; // QLabel for displaying elapsed time
    QLabel *scoreLabel; // QLabel for displaying score

    QTimer *timer;
    QTime startTime;
    int pausedTime = 0; // Initialize to 0
    bool Pause; //pause/resume

private slots:
    void onRadioButtonClicked();
    void onPauseButtonClicked();
    void onResetButtonClicked();
    void handleButtonClick();
    void updateElapsedTime();
};
#endif // MAINWINDOW_H
