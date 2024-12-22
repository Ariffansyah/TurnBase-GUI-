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
    int basedefense;
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

    void defense(Character& player);

    void opponentTurn(Character& attacker, Character& defender);

    void Zoltraak(Character& attacker, Character& defender);

    void LightingStrike(Character& attacker, Character& defender);

    void Swordsman(Character& attacker, Character& defender);

    void FullRecover(Character& player);

    void Crit(Character& attacker, Character& defender);

    void CritChance(Character& attacker, Character& defender);

    void randomOpp(Character& opponent);

    void endTurn();

    void endTurnMP(Character& player);

    void on_pushButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void SinglePlayerFrieren();

    void SinglePlayerEisen();

    void SinglePlayerHeiter();

    void SinglePlayerHimmel();

    void checkGameOver();

    void checkGameOverMP();

    void roundLog(const QString message);

    void ManaRegen(Character& player);

    void ResetDef();

    void updateMessage(Character& character, const QString& message);

    void updateStats();

    void updateMessageMP(Character& character, const QString& message);

    void updateStatsMP();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_lineEdit_2_textChanged(const QString &arg1);

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_lineEdit_3_textChanged(const QString &arg1);

    void on_pushButton_3_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_25_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_23_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_24_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();

    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_33_clicked();

    void nextTurn();

    void updateUIForTurn();

    void Passive(Character& player);
private:
    Ui::MainWindow *ui;
    QString storedNickname;
    QString storedNickname2;
    Character player1;
    Character player2;
    Character opponent;
};
#endif // MAINWINDOW_H
