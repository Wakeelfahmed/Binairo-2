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
#include <QLabel>
#include <QTimer>
#include <QTime>
#include <QMessageBox>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <cstdlib> // For rand()

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
    string rest_input;
    // cout << "Enter two coordinates and a fill symbol: ";

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
        QMessageBox::warning(nullptr, "Error", "Can't add. Please try valid position.");
        return false;
    }
    else{
        // Jos peli päättyy täyteen pelilautaan, pelaaja voitti
        // If the game ends up to a totally filled gameboard, the player won
        if(board.is_game_over()){
            cout << WIN << endl;
        }
        // Jos annettu merkki voitiin lisätä, tulostetaan muuttunut pelilauta
        // If the given symbol was possible to add, print the changed gameboard
        board.print();
        return true;
    }
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
    // getline(cin, choice);
    if(choice != "R" and choice != "r" and choice != "I" and choice != "i")
    {
        return false;
    }
    else if(choice == "R" or choice == "r")
    {
        string seed_string = "";
        cout << "Enter a seed value: ";
        // getline(cin, seed_string);
        return board.fill_randomly(stoi_with_check(seed_string));
    }
    else // if(choice == "I" or choice == "i")
    {
        string input = "";
        cout << "Input: ";
        // getline(cin, input);
        return board.fill_from_input(input);
    }
}

void MainWindow::createTopLayout(QVBoxLayout  *mainLayout)
{
    QWidget *topWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(topWidget);

    radioButton1->setChecked(1);

    scoreLabel->setStyleSheet("QLabel { background-color: black; color: white; }");

    elapsedTimeLabel->setStyleSheet("QLabel { background-color: black; color: white; }");

    connect(radioButton1, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(radioButton2, &QRadioButton::clicked, this, &MainWindow::onRadioButtonClicked);
    connect(pushButton1, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);
    connect(pushButton2, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);

    topLayout->addWidget(radioButton1);
    topLayout->addWidget(radioButton2);
    topLayout->addWidget(pushButton1);
    topLayout->addWidget(pushButton2);
    topLayout->addWidget(scoreLabel);
    topLayout->addWidget(elapsedTimeLabel);

    topWidget->setLayout(topLayout);

    mainLayout->addWidget(topWidget); // Add top layout to main layout
}

void MainWindow::createGridLayout(QVBoxLayout *mainLayout)
{
    QWidget *gridWidget = new QWidget(this);
    gridLayout = new QGridLayout(gridWidget);

    for (int row = 0; row < board.get_SIZE(); ++row)
    {
        for (int col = 0; col < board.get_SIZE(); ++col)
        {
            board.buttons[row][col]->setObjectName(QString("Button %1%2").arg(row, 2, 10, QChar('0')).arg(col, 2, 10, QChar('0')));
            board.buttons[row][col]->setMaximumSize(80, 80);  // Set maximum size for button
            gridLayout->addWidget(board.buttons[row][col], row, col);
            connect(board.buttons[row][col], &QPushButton::clicked, this, &MainWindow::handleButtonClick);
        }
    }

    gridWidget->setLayout(gridLayout);

    mainLayout->addWidget(gridWidget); // Add grid layout to main layout
}

void MainWindow::createInterface(){
    QVBoxLayout *mainLayout = new QVBoxLayout(); // Create main layout
    createTopLayout(mainLayout); // Pass the main layout to createTopLayout
    createGridLayout(mainLayout); // Pass the main layout to createGridLayout

    QWidget *centralWidget = new QWidget(this); // Create a central widget
    centralWidget->setLayout(mainLayout); // Set main layout as central widget's layout
    setCentralWidget(centralWidget); // Set central widget
}

void MainWindow::storeOriginalPalette()
{
    originalPalette = this->palette();
}

void MainWindow::restoreOriginalPalette()
{
    this->setPalette(originalPalette);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    Pause = false;
    radioButton1 = new QRadioButton("0");
    radioButton2 = new QRadioButton("1");
    elapsedTimeLabel = new QLabel("Time: 0 sec");  // Create the QLabel dynamically
    scoreLabel = new QLabel("Score: 0");  // Create the QLabel dynamically
    pushButton1 = new QPushButton("Pause");
    pushButton2 = new QPushButton("Reset");

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateElapsedTime()));

    // Start the timer when the MainWindow is created
    timer->start(1000); // 1000 ms = 1 second

    startTime = QTime::currentTime();

    // Example QLabel to display elapsed time
    elapsedTimeLabel = new QLabel(this);
    setCentralWidget(elapsedTimeLabel);

    updateElapsedTime(); // Update the label initially

    storeOriginalPalette();
    Select_Starting_Method();
}

// Function to generate a random number within a given range
int getRandomNumber(int min, int max) {
    return min + rand() % (max - min + 1);
}

void MainWindow::Select_Starting_Method(){
    QDialog dialog;
    // Create a dialog with radio buttons for Random, Input and File
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
    connect(&randomButton, &QRadioButton::clicked, [&InputLine, &SizeInputLine] {
        InputLine.setPlaceholderText("Seed Value");
        SizeInputLine.setEnabled(true);
    });
    connect(&inputButton, &QRadioButton::clicked, [&InputLine, &SizeInputLine] {
        InputLine.setPlaceholderText("Board Input");
        SizeInputLine.setEnabled(true);
    });
    connect(&fileButton, &QRadioButton::clicked, [&InputLine, &SizeInputLine] {
        InputLine.setPlaceholderText("Default_inputs.txt");
        SizeInputLine.setEnabled(false);
    });

    // Dialog buttons
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    layout.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        board.set_SIZE(SizeInputLine.text().toInt());
        if(board.get_SIZE() < 2 && !fileButton.isChecked()){
            QMessageBox::critical(nullptr, "Size too small", "Please try again.");
            board.cleanup();
            Select_Starting_Method();
        }
        else{
            board.init();
            createInterface();
            if (randomButton.isChecked()) {
                // Handle Random Method
                int seed = InputLine.text().toInt();
                if(InputLine.text() == "")
                {
                    qDebug() << "Missing seed value.";
                    board.cleanup();
                    QMessageBox::critical(nullptr, "Error", "Missing seed value. Please try again.");
                    Select_Starting_Method();   //Retake inputs

                }
                else if (board.fill_randomly(seed)){
                    qDebug() << "Random selected with seed:" << seed;
                    startTime = QTime::currentTime();
                    show();
                }
                else{
                    qDebug() << "Invalid seed value.";
                    board.cleanup();
                    QMessageBox::critical(nullptr, "Error", "Invalid seed value. Please try again.");
                    Select_Starting_Method();   //Retake inputs
                }

            } else if (inputButton.isChecked()) {
                // Handle Input option
                QString input = InputLine.text();

                if (board.fill_from_input(input.toStdString())){
                    qDebug() << "Input selected with input:" << input;
                    startTime = QTime::currentTime();
                    show();
                }
                else{
                    qDebug() << "Invalid Input.";
                    board.cleanup();
                    QMessageBox::critical(nullptr, "Error", "Invalid Input. Please try again.\n Ensure input is enclosed with \"");
                    Select_Starting_Method();   //Retake inputs
                }
            }
            else if (fileButton.isChecked()) {
                // Handle File option
                QString input = InputLine.text();
                 input = "Default_inputs.txt";

                QFile file(input);
                if(InputLine.text() == "")
                {
                    qDebug() << "Missing file name.";
                    board.cleanup();
                    QMessageBox::critical(nullptr, "Error", "Missing file name. Ensure file is placed in build dir then try again.");
                    Select_Starting_Method();   //Retake inputs

                }
                else{
                    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                        qDebug() << "Failed to open file:" << input;
                        board.cleanup();
                        QMessageBox::critical(nullptr, "Error", "File not found/Missing file. Please try again.");
                        Select_Starting_Method();   //Retake inputs
                    }
                    else
                    {
                        QTextStream in(&file);
                        vector<string> inputs;
                        int lineCount = 0;
                        while (!in.atEnd()) {
                            QString line = in.readLine();
                            inputs.push_back(line.toStdString());
                            lineCount++;
                        }
                        file.close();

                        int randomNumber = getRandomNumber(0, lineCount - 1); // Adjust range for array indexing

                        board.cleanup();

                        board.set_SIZE(sqrt(inputs[randomNumber].size() -2));
                        board.init();
                        createInterface();

                        if (board.fill_from_input(inputs[randomNumber])){
                            qDebug() << "File selected with filename:" << input;
                            show();
                        }
                        else{
                            qDebug() << "Invalid File Input.";
                            board.cleanup();
                            QMessageBox::critical(nullptr, "Error", "Invalid file. Please try again.\n Ensure input is enclose with \"");
                            Select_Starting_Method();   //Retake inputs
                        }
                    }
                }
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
        board.buttons[x-1][y-1]->setText(QString::fromStdString(to_string(board.get_fillsymbol())));
        board.set_score(board.get_SIZE() + 5);
        scoreLabel->setText(QString("Score: %1").arg(board.get_SIZE()));
    }
    if(board.is_game_over()){
        setBackgroundColor(Qt::green); // Set background color to green
        QMessageBox::information(nullptr, "WON", "You have won the game.");
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
    if (Pause == false) {
        if (timer->isActive()) {
            // Pause the timer
            timer->stop();

            // Calculate elapsed time since start time
            QTime currentTime = QTime::currentTime();
            int elapsedSeconds = startTime.secsTo(currentTime);

            // Store the elapsed time for resuming later
            pausedTime = elapsedSeconds;
            for (int row = 0; row < board.get_SIZE(); ++row)
                for (int col = 0; col < board.get_SIZE(); ++col)
                    board.buttons[row][col]->setEnabled(false);

            pushButton2->setEnabled(false);
            radioButton1->setEnabled(false);
            radioButton2->setEnabled(false);

            qDebug() << "Game Paused.";
            QMessageBox::information(nullptr, "Information", "Game Paused");
            Pause = true;
        }
    } else {
        if (!timer->isActive()) {
            // Adjust the start time to resume from where it was paused
            startTime = QTime::currentTime().addSecs(-pausedTime);

            // Restart the timer
            timer->start();

            for (int row = 0; row < board.get_SIZE(); ++row) {
                for (int col = 0; col < board.get_SIZE(); ++col) {
                    if(board.buttons[row][col]->text() != "1" && board.buttons[row][col]->text() != "0")
                        board.buttons[row][col]->setEnabled(true);
                }
            }

            pushButton2->setEnabled(true);
            radioButton1->setEnabled(true);
            radioButton2->setEnabled(true);

            qDebug() << "Game Resumed.";
            Pause = false;
        }
    }
}

void MainWindow::onResetButtonClicked()
{
    // Handle reset button click
    qDebug() << "Reset button.";
    board.cleanup();
    restoreOriginalPalette();
    Select_Starting_Method();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setBackgroundColor(const QColor &color)
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, color);
    this->setPalette(palette);
}

void MainWindow::updateElapsedTime()
{
    if(!board.is_game_over()){
        // Calculate elapsed time since start time
        QTime currentTime = QTime::currentTime();
        int elapsedSeconds = startTime.secsTo(currentTime);

        // Update the QLabel to display the elapsed time
        elapsedTimeLabel->setText(QString("Time: %1 sec").arg(elapsedSeconds));
    }
}
