#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct Character {
    string name;
    string Nickname;
    int health;
    int maxHealth;
    int mana;
    int maxMana;
    int attack;
    int defense;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void attack(Character& attacker, Character& defender);

    void heal(Character& player);

    void opponentTurn(Character& attacker, Character& defender);

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void setupPlayerAndOpponent();

    void checkGameOver();

    void updateMessage(Character& character, const QString& message);

    void updateStats();

private:
    Ui::MainWindow *ui;
    QString storedNickname;
    QString storedNickname2;
    Character player1;
    Character player2;
    Character opponent;
};
#endif // MAINWINDOW_H
