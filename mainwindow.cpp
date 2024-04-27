#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameboard.h"

#include <QDialog>   //for Init Inputs (Size & Starting Method) Window
#include <QFormLayout>
#include <QRadioButton>     //for Input or Random or File option
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>

using namespace std;

// Tulosteet
// Output messages
const string QUIT = "Quitting ...";
const string OUT_OF_BOARD = "Out of board";
const string INVALID_INPUT = "Invalid input";
const string CANT_ADD = "Can't add";
const string WIN = "You won!";

// Muuttaa numeerisen merkkijonon vastaavaksi kokonaisluvuksi
// (kutsumalla stoi-funktiota) ja palauttaa tämän kokonaisluvun.
// Jos annettu merkkijono ei ollut numeerinen, palauttaa nollan.
//
// Converts the given numeric string to the corresponding integer
// (by calling stoi) and returns the integer.
// If the given string is not numeric, returns zero.
unsigned int stoi_with_check(const string& str)
{
    bool is_numeric = true;
    for(unsigned int i = 0; i < str.length(); ++i)
    {
        if(not isdigit(str.at(i)))
        {
            is_numeric = false;
            break;
        }
    }
    if(is_numeric)
        return stoi(str);

    else
        return 0;

}

// Poistaa tyhjät merkit (kuten ' ') annetusta merkkijonosta.
// Palauttaa toden, jos annetussa merkkijonossa on täsmälleen yksi ei-tyhjä
// merkki, joka on '0' tai '1', muussa tapauksessa palauttaa epätoden.
//
// Removes empty characters (such as ' ' etc.) from the given string.
// Returns true if the given string has exactly one non-empty character,
// which is either '0' or '1', otherwise returns false.
bool find_fill_symbol(string& str)
{
    string fill_str = "";
    for(unsigned int i = 0; i < str.size(); ++i)
    {
        if(not isspace(str.at(i)))
        {
            fill_str += str.at(i);
        }
    }
    str = fill_str;
    return (fill_str.size() == 1 and
            (fill_str.at(0) == '0' or fill_str.at(0) == '1'));
}

// Mahdollistaa pelin pelaamisen eli kysyy toistuvasti lisättävää merkkiä
// ja sen sijaintia, kunnes peli päättyy.
//
// Enables the user to play the game, i.e. by repeatedly asking an element
// to be added and its position, until the game is over.
bool play_game(GameBoard& board, string x_str, string y_str)
{
    board.print();
    string rest_input = "0";
    cout << "Enter two coordinates and a fill symbol: ";

    // Luetaan x-koordinaatti merkkijonona ja tarkistetaan, oliko kyseessä
    // lopetuskomento
    //
    // Reading x coordinate as a string and checking if it was quit command
    // cin >> x_str;
    if(x_str.at(0) == 'q' or x_str.at(0) == 'Q')
    {
        cout << QUIT << endl;
        return false;
    }

    // Luetaan y-koordinaatti
    // Reading y coordinate
    // cin >> y_str;

    // Muutetaan koordinaatit merkkijonoista kokonaisluvuiksi ja
    // tarkistetaan, ovatko ne pelilaudan sisällä
    //
    // Changing coordinates from string to int and checking if they are
    // inside the board
    unsigned int x = stoi_with_check(x_str);
    unsigned int y = stoi_with_check(y_str);
    if(not (1 <= x and x <= board.get_SIZE() and 1 <= y and y <= board.get_SIZE()))
    {
        cout << OUT_OF_BOARD << endl;
        getline(cin, rest_input);
        return false;
    }

    // Luetaan loppusyöte, joka sisältää täyttömerkin, ja tarkistetaan,
    // koostuuko loppusyöte täsmälleen yhdestä merkistä '0' tai '1'
    //
    // Reading the rest of the input line including fill symbol and
    // checking if the rest input consists exactly of one '0' or '1'
    // getline(cin, rest_input);
    rest_input = to_string(board.get_fillsymbol());
    if(not find_fill_symbol(rest_input))
    {
        cout << INVALID_INPUT << endl;
        return false;
    }

    // Tässä kohdassa tiedetään, että syöte oli hyväksyttävä, mutta ei
    // ole varmaa, voidaanko annettu merkki lisätä annettuun kohtaan
    //
    // At this point, we know that the input is valid, but we don't know
    // if it is possible to add the given symbol on the given position
    --x;
    --y;
    if(not board.add_symbol(x, y, rest_input.at(0)))
    {
        cout << CANT_ADD << endl;
        return false;
    }
    else{    board.print();

        return true;
    }
    // Jos annettu merkki voitiin lisätä, tulostetaan muuttunut pelilauta
    // If the given symbol was possible to add, print the changed gameboard
    board.print();

    // Jos peli päättyy täyteen pelilautaan, pelaaja voitti
    // If the game ends up to a totally filled gameboard, the player won
    if(board.is_game_over())
        cout << WIN << endl;
}

// Kysyy käyttäjältä pelilaudan täyttötapaa.
// Palauttaa toden, jos pelilaudan täyttäminen onnistui,
// muuten palauttaa epätoden.
//
// Gives the user a possibility to select a filling way.
// Returns true if filling succeeded, otherwise returns false.
bool select_start(GameBoard& board)
{
    string choice = "";
    cout << "Select start (R for random, I for input): ";
    getline(cin, choice);
    if(choice != "R" and choice != "r" and choice != "I" and choice != "i")
    {
        return false;
    }
    else if(choice == "R" or choice == "r")
    {
        string seed_string = "";
        cout << "Enter a seed value: ";
        getline(cin, seed_string);
        return board.fill_randomly(stoi_with_check(seed_string));
    }
    else // if(choice == "I" or choice == "i")
    {
        string input = "";
        cout << "Input: ";
        getline(cin, input);
        return board.fill_from_input(input);
    }
}

void MainWindow::Select_Starting_Method()
{
    // createTopLayout();
    // createGridLayout();
}
void MainWindow::clearGridLayout()
{
    QLayoutItem *child;
    while ((child = gridLayout->takeAt(0)) != nullptr)
    {
        if (child->widget())
        {
            delete child->widget();
        }
        delete child;
    }
}
void MainWindow::createTopLayout(QVBoxLayout  *mainLayout)
{
    QWidget *topWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);

    QRadioButton *radioButton1 = new QRadioButton("Option 1");
    QRadioButton *radioButton2 = new QRadioButton("Option 2");

    QPushButton *pushButton1 = new QPushButton("Button 1");
    QPushButton *pushButton2 = new QPushButton("Button 2");

    QLineEdit *lineEdit = new QLineEdit();

    topLayout->addWidget(radioButton1);
    topLayout->addWidget(radioButton2);
    topLayout->addWidget(pushButton1);
    topLayout->addWidget(pushButton2);
    topLayout->addWidget(lineEdit);

    topWidget->setLayout(topLayout);

    mainLayout->addWidget(topWidget); // Add top layout to main layout
}

void MainWindow::createGridLayout(QVBoxLayout *mainLayout)
{
    QWidget *gridWidget = new QWidget(this);
    gridLayout = new QGridLayout(gridWidget);

    const int SIZE = 5; // Change this to your desired grid size

    for (int row = 0; row < SIZE; ++row)
    {
        for (int col = 0; col < SIZE; ++col)
        {
            QPushButton *button = new QPushButton(QString("Button %1,%2").arg(row).arg(col));
            gridLayout->addWidget(button, row, col);
        }
    }

    gridWidget->setLayout(gridLayout);

    mainLayout->addWidget(gridWidget); // Add grid layout to main layout
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(); // Create main layout
    createTopLayout(mainLayout); // Pass the main layout to createTopLayout
    createGridLayout(mainLayout); // Pass the main layout to createGridLayout

    QWidget *centralWidget = new QWidget(this); // Create a central widget
    centralWidget->setLayout(mainLayout); // Set main layout as central widget's layout
    setCentralWidget(centralWidget); // Set central widget


//     ui->setupUi(this); // Assuming you have this line in your constructor

    // Select_Starting_Method();

//     // Create top layout elements
//     QHBoxLayout *topLayout = new QHBoxLayout();

//     QRadioButton *radioButton1 = new QRadioButton("0");
//     radioButton1->setChecked(1);
//     QRadioButton *radioButton2 = new QRadioButton("1");

//     connect(radioButton1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
//     connect(radioButton2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);

//     QPushButton *pushButton1 = new QPushButton("Pause");
//     QPushButton *pushButton2 = new QPushButton("Reset");
//     connect(pushButton1, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);
//     connect(pushButton2, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);

//     QLineEdit *lineEdit = new QLineEdit();

//     topLayout->addWidget(radioButton1);
//     topLayout->addWidget(radioButton2);
//     topLayout->addWidget(pushButton1);
//     topLayout->addWidget(pushButton2);
//     topLayout->addWidget(lineEdit);

//     // Set maximum height for the top layout
//     // radioButton1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
//     // radioButton2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
//     // pushButton1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
//     // pushButton2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
//     // lineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

//     // // Create grid layout for buttons
//     // QGridLayout *gridLayout = new QGridLayout();

//     // // Create a QIcon object and set the icon from a file
//     // QIcon icon(":/new/prefix1/Black.png");

//     // // Set the icon to the button

//     // // Resize the button to accommodate the icon
//     // for (int row = 0; row < board.get_SIZE(); ++row) {
//     //     for (int col = 0; col < board.get_SIZE(); ++col) {
//     //         // button->setMaximumSize(80, 80);  // Set maximum size for button
//     //         // button->setIcon(icon);
//     //         // button->setIconSize(QSize(80, 80)); // Adjust the size as needed
//     //         // Apply stylesheet for circular appearance
//     //         // button->setStyleSheet("QPushButton {  border-color: rgb(66, 69, 183);  border-width: 3px;  border-style: solid;  border-radius: 40px;  margin:30px;  padding:30px;}");
//     //         board.buttons[row][col]->setObjectName(QString("Button %1%2").arg(row, 2, 10, QChar('0')).arg(col, 2, 10, QChar('0')));
//     //         gridLayout->addWidget(board.buttons[row][col], row, col);
//     //         connect(board.buttons[row][col], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
//     //     }
//     // }

//     // // Create a central widget and set layouts
//     QWidget *centralWidget = new QWidget(this);
//     QVBoxLayout *mainLayout = new QVBoxLayout();
//     centralWidget->setLayout(mainLayout);

//     mainLayout->addLayout(topLayout);
//     // mainLayout->addLayout(gridLayout);

//     setCentralWidget(centralWidget);
//     Set_Button_Interface();
}

void MainWindow::Set_Button_Interface(){
    QGridLayout *gridLayout = new QGridLayout();

    // Create a QIcon object and set the icon from a file
    QIcon icon(":/new/prefix1/Black.png");

    // Set the icon to the button

    // Resize the button to accommodate the icon
    for (int row = 0; row < board.get_SIZE(); ++row) {
        for (int col = 0; col < board.get_SIZE(); ++col) {
            // button->setMaximumSize(80, 80);  // Set maximum size for button
            // button->setIcon(icon);
            // button->setIconSize(QSize(80, 80)); // Adjust the size as needed
            // Apply stylesheet for circular appearance
            // button->setStyleSheet("QPushButton {  border-color: rgb(66, 69, 183);  border-width: 3px;  border-style: solid;  border-radius: 40px;  margin:30px;  padding:30px;}");
            board.buttons[row][col]->setObjectName(QString("Button %1%2").arg(row, 2, 10, QChar('0')).arg(col, 2, 10, QChar('0')));
            gridLayout->addWidget(board.buttons[row][col], row, col);
            connect(board.buttons[row][col], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
        }
    }

    // Create a central widget and set layouts
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();
    centralWidget->setLayout(mainLayout);

    mainLayout->addLayout(gridLayout);

    setCentralWidget(centralWidget);
}

void MainWindow::Select_Starting_Method(){
    QDialog dialog;
    // Create a dialog with radio buttons for Random and Input
    dialog.setWindowTitle("Enter Size & Select Starting Method");

    QFormLayout layout(&dialog);
    QRadioButton randomButton("Random", &dialog);
    randomButton.setChecked(true);
    QRadioButton inputButton("Input", &dialog);
    QRadioButton fileButton("File", &dialog);

    QLineEdit SizeInputLine;
    layout.addRow("Size:", &SizeInputLine);
    SizeInputLine.setPlaceholderText("Board Size");

    layout.addRow(&randomButton);
    layout.addRow(&inputButton);
    layout.addRow(&fileButton);

    // Add a line edit for the seed value (for Random option)
    QLineEdit InputLine;
    layout.addRow("Input:", &InputLine);
    InputLine.setPlaceholderText("Enter Seed Value");

    // Connect radio buttons to slot or lambda function to update placeholder text
    connect(&randomButton, &QRadioButton::clicked, [&InputLine] {
        InputLine.setPlaceholderText("Seed Value");
    });
    connect(&inputButton, &QRadioButton::clicked, [&InputLine] {
        InputLine.setPlaceholderText("Board Input");
    });
    connect(&fileButton, &QRadioButton::clicked, [&InputLine] {
        InputLine.setPlaceholderText("File Name");
    });

    // Dialog buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        board.set_SIZE(SizeInputLine.text().toInt());
        board.init();
        if (randomButton.isChecked()) {
            // Handle Random Method
            int seed = InputLine.text().toInt();

            if (board.fill_randomly(seed)){
                qDebug() << "Random selected with seed:" << seed;
            //     Init_Input_done = true;
            //     resetTimer();
            }
            else{
                // ToastWidget *toast = new ToastWidget();
                // toast->showToast("Invalid seed value", 1000);
                // qDebug() << "Invalid seed value.";
                // randomButton.setChecked(false);
                // Init_Input_done = false;
                // Take_Board_Init_Input(); //Reopen this window to again get input
            }

        } else if (inputButton.isChecked()) {
            // Handle Random option

            QString input = InputLine.text();

            if (board.fill_from_input(input.toStdString())){
                qDebug() << "Input selected with input:" << input;
                // select_start(board, "i", input.toStdString());
                // seed_or_input = input.toStdString();
                // Init_Input_done = true;
                // resetTimer();
            }
            else{
                // ToastWidget *toast = new ToastWidget();
                // toast->showToast("Invalid Input ", 1000);
                qDebug() << "Invalid Input .";
                //     Init_Input_done = false;
                //     inputButton.setChecked(false);
                //     Take_Board_Init_Input(); //Reopen this window to again get input
            }
        }
    } else
        qDebug() << "Selection canceled or dialog closed.";
}

void MainWindow::handleButtonClick(){
    // Get the object name of the clicked button
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString objectName = clickedButton->objectName();

    QString xStr = objectName.mid(7, 2);    // Assuming buttonText is of the form "Button XXYY"
    QString yStr = objectName.mid(9, 2);
    int x = xStr.toInt() + 1;
    int y = yStr.toInt() + 1;

    if(play_game(board, to_string(y), to_string(x)))
    {
        clickedButton->setText(QString::fromStdString(to_string(board.get_fillsymbol())));
    }
}

void MainWindow::onRadioButtonClicked()
{
    // Handle radio button click
    qDebug() << "Fill Radio button.";
    QRadioButton* radioButton = qobject_cast<QRadioButton*>(sender());

    qDebug() << "Radio button clicked:" << radioButton->text();

    QString selectedValue = radioButton->text();
    qDebug() << "Selected value:" << selectedValue;
    board.set_fillsymbol(selectedValue.toInt());
}

void MainWindow::onPauseButtonClicked()
{
    // Handle pause button click
    qDebug() << "Pause button.";
}

void MainWindow::onResetButtonClicked()
{
    // Handle reset button click
    qDebug() << "Reset button.";
    board.cleanup();
    Select_Starting_Method();
}
MainWindow::~MainWindow()
{
    delete ui;
}
