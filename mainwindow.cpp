#include "mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), playerTurn(true), gridSize(5) {
    centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    label = new QLabel("Player", this);
    mainLayout->addWidget(label);

    sizeSelector = new QSpinBox(this);
    sizeSelector->setMinimum(3);
    sizeSelector->setValue(gridSize);
    connect(sizeSelector, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::updateGridSize);
    mainLayout->addWidget(sizeSelector);

    gridLayout = new QGridLayout();
    mainLayout->addLayout(gridLayout);
    createBoard();

    clearButton = new QPushButton("Clear", this);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearBoard);
    mainLayout->addWidget(clearButton);

    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

void MainWindow::createBoard() {
    for (int i = 0; i < gridLayout->count(); ++i) {
        QWidget *w = gridLayout->itemAt(i)->widget();
        if (w) delete w;
    }
    buttons.clear();

    for (int i = 0; i < gridSize; ++i) {
        QVector<QPushButton*> row;
        for (int j = 0; j < gridSize; ++j) {
            QPushButton *btn = new QPushButton("");
            btn->setFixedSize(50, 50);
            connect(btn, &QPushButton::clicked, this, &MainWindow::handleButtonClick);
            gridLayout->addWidget(btn, i, j);
            row.append(btn);
        }
        buttons.append(row);
    }
}

void MainWindow::handleButtonClick() {
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn || !btn->text().isEmpty() || !playerTurn) return;
    btn->setText("X");
    checkWin();
    playerTurn = false;
    label->setText("AI");
    makeAIMove();
}

void MainWindow::makeAIMove() {
    QVector<QPair<int,int>> freeCells;
    for (int i = 0; i < gridSize; ++i)
        for (int j = 0; j < gridSize; ++j)
            if (buttons[i][j]->text().isEmpty())
                freeCells.append({i,j});

    if (freeCells.isEmpty()) return;
    auto choice = freeCells[QRandomGenerator::global()->bounded(freeCells.size())];
    buttons[choice.first][choice.second]->setText("O");
    checkWin();
    playerTurn = true;
    label->setText("Player");
}

void MainWindow::clearBoard() {
    for (auto &row : buttons)
        for (auto &btn : row)
            btn->setText("");
    playerTurn = true;
    label->setText("Player");
}

void MainWindow::updateGridSize(int size) {
    gridSize = size;
    createBoard();
}

void MainWindow::checkWin() {
    auto symbol = [&](int i, int j) {
        return buttons[i][j]->text();
    };

    QString current = playerTurn ? "X" : "O";

    for (int i = 0; i < gridSize; ++i) {
        for (int j = 0; j <= gridSize - 4; ++j) {
            if (symbol(i,j) == current && symbol(i,j+1) == current &&
                symbol(i,j+2) == current && symbol(i,j+3) == current) {
                QMessageBox::information(this, "Win", current + " wins!");
                return;
            }
        }
    }

    for (int i = 0; i <= gridSize - 4; ++i) {
        for (int j = 0; j < gridSize; ++j) {
            if (symbol(i,j) == current && symbol(i+1,j) == current &&
                symbol(i+2,j) == current && symbol(i+3,j) == current) {
                QMessageBox::information(this, "Win", current + " wins!");
                return;
            }
        }
    }

    for (int i = 0; i <= gridSize - 4; ++i) {
        for (int j = 0; j <= gridSize - 4; ++j) {
            if (symbol(i,j) == current && symbol(i+1,j+1) == current &&
                symbol(i+2,j+2) == current && symbol(i+3,j+3) == current) {
                QMessageBox::information(this, "Win", current + " wins!");
                return;
            }
        }
    }

    for (int i = 3; i < gridSize; ++i) {
        for (int j = 0; j <= gridSize - 4; ++j) {
            if (symbol(i,j) == current && symbol(i-1,j+1) == current &&
                symbol(i-2,j+2) == current && symbol(i-3,j+3) == current) {
                QMessageBox::information(this, "Win", current + " wins!");
                return;
            }
        }
    }
}
