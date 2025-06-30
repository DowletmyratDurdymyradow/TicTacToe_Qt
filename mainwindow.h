#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QVector>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleButtonClick();
    void clearBoard();
    void updateGridSize(int size);

private:
    QWidget *centralWidget;
    QLabel *label;
    QGridLayout *gridLayout;
    QPushButton *clearButton;
    QSpinBox *sizeSelector;
    QVector<QVector<QPushButton*>> buttons;
    bool playerTurn; // true - player, false - AI
    int gridSize;
    void checkWin();
    void createBoard();
    void makeAIMove();
};

#endif // MAINWINDOW_H
