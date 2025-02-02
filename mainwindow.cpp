#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <string>
#include <QDebug>
#include <QTimer>
#include <fstream>
#include <ctime>

using namespace std;

//base

int currentTurn = 0;

int randomInRange(int x, int y) {
    return x + rand() % (y - x + 1);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    srand(static_cast<unsigned int>(time(0)));

    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page);
    storedNickname = "";
    storedNickname2 = "";
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::roundLog(const QString message) {
    ofstream outputFile("playersave.txt", ios::app);

    time_t currentTime = time(nullptr);

    struct tm* timeInfo = localtime(&currentTime);

    if(outputFile.is_open()){
        outputFile << "date and time: "
                    << 1900 + timeInfo->tm_year << "-"
                    << 1 + timeInfo->tm_mon << "-"
                    << timeInfo->tm_mday << " "
                    << timeInfo->tm_hour << ":"
                    << timeInfo->tm_min << ":"
                    << timeInfo->tm_sec << endl;
        outputFile << message.toStdString() << endl;
        outputFile.close();
    } else {
        qDebug() << "cant open the file";
        return;
    }
}

void MainWindow::randomOpp(Character& opponent) {
    int random = randomInRange(0, 100);

    if (random >= 0 && random <= 20) {
        opponent = { "Solar Dragon", "Solar Dragon", 200, 200, 100, 100, 25, 15, 15 };
        updateMessage(opponent, "A fierce " + QString::fromStdString(opponent.name) + " appears!");
        ui->label_5->setText("Solar Dragon' Stats");
        return;
    } else if (random >= 21 && random <= 40) {
        opponent = {"Aura", "Aura", 500, 500, 150, 150, 25, 10, 10};
        updateMessage(opponent, "A mighty " + QString::fromStdString(opponent.name) + " appears!");
        ui->label_5->setText("Aura' Stats");
        return;
    } else if (random >= 41 && random <= 50) {
        opponent = {"Demon King", "Demon King", 1000, 1000, 500, 500, 50, 20, 20};
        updateMessage(opponent, "The powerful " + QString::fromStdString(opponent.name) + " appears!");
        ui->label_5->setText("Demon King' Stats");
        return;
    } else {
        opponent = {"Draht", "Draht", 100, 100 , 40, 40, 20, 10, 10};
        updateMessage(opponent, "A weak " + QString::fromStdString(opponent.name) + " appears!");
        ui->label_5->setText("Draht' Stats");
        return;
    }
}

void MainWindow::opponentTurn(Character& attacker, Character& defender) {
    if (randomInRange(0, 1) == 0) {
        CritChance(attacker, defender);
    } else {
        heal(attacker);
    }
}

void MainWindow::attack(Character& attacker, Character& defender) {
    int damage = max(1, attacker.attack - defender.defense);
    defender.health -= damage;

    updateMessage(attacker, QString::fromStdString(attacker.name) + " attacks for " + QString::number(damage) + " damage!");
    updateMessageMP(attacker, QString::fromStdString(attacker.name) + " attacks for " + QString::number(damage) + " damage!");

}

void MainWindow::Crit(Character& attacker, Character& defender) {
    int damage = max(1, attacker.attack - defender.defense) * 2;
    defender.health -= damage;

    updateMessage(attacker, QString::fromStdString(attacker.name) + " lands crit attacks for " + QString::number(damage) + " damage!");
    updateMessageMP(attacker, QString::fromStdString(attacker.name) + " lands crit attacks for " + QString::number(damage) + " damage!");
}

void MainWindow::CritChance(Character& attacker, Character& defender) {
    int missChance = randomInRange(0, 100);
    int critChances = randomInRange(0, 100);

    if (missChance < 10) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        return;
    }

    if(critChances <= 15) {
        Crit(attacker, defender);
        return;
    } else {
        attack(attacker, defender);
        return;
    }
}

void MainWindow::heal(Character& player) {
    if (player.mana < 10) {
        updateMessage(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        updateMessageMP(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        return;
    }

    if (player.health == player.maxHealth) {
        updateMessage(player, QString::fromStdString(player.name) + " is already at full health. Turn skipped.");
        updateMessageMP(player, QString::fromStdString(player.name) + " is already at full health. Turn skipped.");
        return;
    }

    int healingRanges[][2] = {
        {30, 50}, // Heiter's
        {10, 30}
    };

    int healMin;
    int healMax;

    if (player.name == "Heiter") {
        healMin = healingRanges[0][0];
        healMax = healingRanges[0][1];

        for(int i = 0; i < 2; i++) {
            int healAmount = randomInRange(healMin, healMax);
            healAmount = min(healAmount, player.maxHealth - player.health);

            player.health += healAmount;

            updateMessage(player, QString::fromStdString(player.name) + " heals for " + QString::number(healAmount) + " health!");
            updateMessageMP(player, QString::fromStdString(player.name) + " heals for " + QString::number(healAmount) + " health!");
        }

        player.mana -= 10;
        return;
    }

    healMin = healingRanges[1][0];
    healMax = healingRanges[1][1];

    int healAmount = randomInRange(healMin, healMax);
    healAmount = min(healAmount, player.maxHealth - player.health);

    player.health += healAmount;
    player.mana -= 10;

    updateMessage(player, QString::fromStdString(player.name) + " heals for " + QString::number(healAmount) + " health!");
    updateMessageMP(player, QString::fromStdString(player.name) + " heals for " + QString::number(healAmount) + " health!");
}

void MainWindow::defense(Character& player) {
    if (player.mana < 10) {
        updateMessage(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        updateMessageMP(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        return;
    }

    int updefense = randomInRange(1, 10);

    player.defense += updefense;
    player.mana -= 10;

    updateMessage(player, QString::fromStdString(player.name) + " rise defense for " + QString::number(updefense));
    updateMessageMP(player, QString::fromStdString(player.name) + " rise defense for " + QString::number(updefense));
}

void MainWindow::ManaRegen(Character& player) {
    int mana = randomInRange(5, 10);
    mana = min(mana, player.maxMana - player.mana);

    player.mana += mana;
}

void MainWindow::ResetDef() {
    player1.defense = player1.basedefense;
    player2.defense = player2.basedefense;
    opponent.defense = opponent.basedefense;
}

void MainWindow::Zoltraak(Character& attacker, Character& defender) {
    if (attacker.mana < 50) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " doesn't have enough mana!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " doesn't have enough mana!");
        return;
    }

    int missChance = randomInRange(0, 100);

    if (missChance < 5) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        return;
    }

    int damage = max(1, attacker.attack - defender.defense) * 3;
    defender.health -= damage;
    attacker.mana -= 50;

    updateMessage(attacker, QString::fromStdString(attacker.name) + " blast Zoltraak for " + QString::number(damage) + " damage!");
    updateMessageMP(attacker, QString::fromStdString(attacker.name) + " blast Zoltraak for " + QString::number(damage) + " damage!");
}

void MainWindow::LightingStrike(Character& attacker, Character& defender) {
    if (attacker.mana < 30) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " doesn't have enough mana!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " doesn't have enough mana!");

        return;
    }

    int missChance = randomInRange(0, 100);

    if (missChance < 5) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        return;
    }

    int damage = max(1, attacker.attack - defender.defense) * 3;
    defender.health -= damage;
    attacker.mana -= 30;

    updateMessage(attacker, QString::fromStdString(attacker.name) + " Strike a Lighting for " + QString::number(damage) + " damage!");
    updateMessageMP(attacker, QString::fromStdString(attacker.name) + " Strike a Lighting for " + QString::number(damage) + " damage!");
}

void MainWindow::Swordsman(Character& attacker, Character& defender) {
    if (attacker.mana < 30) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " doesn't have enough mana!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " doesn't have enough mana!");
        return;
    }

    int missChance = randomInRange(0, 100);

    if (missChance < 5) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        updateMessageMP(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        return;
    }

    int damage = max(1, attacker.attack - defender.defense) * 3;
    defender.health -= damage;
    attacker.mana -= 30;

    updateMessage(attacker, QString::fromStdString(attacker.name) + " Slash his swords for " + QString::number(damage) + " damage!");
    updateMessageMP(attacker, QString::fromStdString(attacker.name) + " Slash his swords for " + QString::number(damage) + " damage!");
}

void MainWindow::FullRecover(Character& player) {
    if (player.mana < 30) {
        updateMessage(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        updateMessageMP(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        return;
    }

    if (player.health == player.maxHealth) {
        updateMessage(player, QString::fromStdString(player.name) + " is already at full health. Turn skipped.");
        updateMessageMP(player, QString::fromStdString(player.name) + " is already at full health. Turn skipped.");
        return;
    }

    int healAmount = (player.maxHealth - player.health) / 2;
    player.health += healAmount;
    player.mana -= 30;

    updateMessage(player, QString::fromStdString(player.name) + " full recover " + QString::number(healAmount) + " health!");
    updateMessageMP(player, QString::fromStdString(player.name) + " full recover " + QString::number(healAmount) + " health!");
}

void MainWindow::Passive(Character& player){
    if (player.name == "Frieren") {
        if(player.health < player.maxHealth) {
            int manaAmount = randomInRange(0,  5);
            manaAmount = min(manaAmount, player.maxMana - player.mana);
            player.mana += manaAmount;
        }
        return;
    } else if (player.name == "Himmel") {
        if(player.health < player.maxHealth) {
            int healAmount = randomInRange(0,  5);
            healAmount = min(healAmount, player.maxHealth - player.health);
                    player.health += healAmount;
        }
        return;
    } else if (player.name == "Eisen") {
        if(player.health < player.maxHealth) {
            int healAmount = randomInRange(0,  5);
            healAmount = min(healAmount, player.maxHealth - player.health);
            player.health += healAmount;
        }
        return;
    } else if (player.name == "Heiter") {
        if(player.health < player.maxHealth) {
            int manaAmount = randomInRange(0,  5);
            manaAmount = min(manaAmount, player.maxMana - player.mana);
            player.mana += manaAmount;
        }
        return;
    }
}

void MainWindow::updateMessage(Character& character, const QString& message) {
    if (character.name == player1.name) {
        ui->label_11->setText(message);
        roundLog(message);
    } else {
        ui->label_12->setText(message);
        roundLog(message);
    }
}

void MainWindow::updateStats() {
    ui->label_6->setText("HP: " + QString::number(player1.health));
    ui->label_7->setText("Mana: " + QString::number(player1.mana));
    ui->label_8->setText("HP: " + QString::number(opponent.health));
    ui->label_9->setText("Mana: " + QString::number(opponent.mana));
}

void MainWindow::updateMessageMP(Character& character, const QString& message) {
    if (character.Nickname == player1.Nickname) {
        ui->label_24->setText(message);
        roundLog(message);
    } else if (character.Nickname == player2.Nickname){
        ui->label_25->setText(message);
        roundLog(message);
    }
}

void MainWindow::checkGameOver() {

    if (player1.health <= 0) {
        ui->label_13->setText("you are defeated!");
        ui->stackedWidget->setCurrentWidget(ui->page_5);
        return;
    }

    if (fightCount >= 3) {
        ui->label_13->setText(QString::fromStdString(player1.Nickname) + " is the winner!");
        ui->stackedWidget->setCurrentWidget(ui->page_5);
        return;
    }

    if (opponent.health <= 0) {
        updateMessage(opponent, QString::fromStdString(opponent.name) + " is defeated!");
        randomOpp(opponent);
        fightCount++;
    }

    QTimer::singleShot(1000, this, [this]() { checkGameOver(); });
}

void MainWindow::checkGameOverMP() {
    if (player1.health <= 0) {
        ui->label_13->setText(QString::fromStdString(player2.Nickname) + " is the winner!");
        ui->stackedWidget->setCurrentWidget(ui->page_5);
        currentTurn = 1;
    } else if (player2.health <= 0) {
        ui->label_13->setText(QString::fromStdString(player1.Nickname) + " is the winner!");
        ui->stackedWidget->setCurrentWidget(ui->page_5);
        currentTurn = 1;
    }
}

void MainWindow::updateStatsMP() {
    ui->label_18->setText("HP: " + QString::number(player1.health));
    ui->label_19->setText("Mana: " + QString::number(player1.mana));
    ui->label_21->setText("HP: " + QString::number(player2.health));
    ui->label_22->setText("Mana: " + QString::number(player2.mana));
}

void MainWindow::nextTurn() {

    currentTurn = (currentTurn == 1) ? 2 : 1;

    if (currentTurn == 1) {
        ui->label_27->setText("Player 1's turn");
    } else {
        ui->label_27->setText("Player 2's turn");
    }

    updateUIForTurn();
}

void MainWindow::endTurn(){
    opponentTurn(opponent, player1);
    checkGameOver();
    updateStats();
    ManaRegen(player1);
    Passive(player1);
}

void MainWindow::endTurnMP(Character& player){
    updateStatsMP();
    checkGameOverMP();
    nextTurn();
    ManaRegen(player);
    Passive(player);
}

void MainWindow::updateUIForTurn() {
    bool isPlayer1Turn = (currentTurn == 1);

    ui->pushButton_26->setEnabled(isPlayer1Turn);
    ui->pushButton_27->setEnabled(isPlayer1Turn);
    ui->pushButton_28->setEnabled(isPlayer1Turn);
    ui->pushButton_29->setEnabled(isPlayer1Turn);

    ui->pushButton_30->setEnabled(!isPlayer1Turn);
    ui->pushButton_31->setEnabled(!isPlayer1Turn);
    ui->pushButton_32->setEnabled(!isPlayer1Turn);
    ui->pushButton_33->setEnabled(!isPlayer1Turn);
}

//single player

void MainWindow::on_lineEdit_textChanged(const QString &arg1) {
    storedNickname = arg1.isEmpty() ? "Homan" : arg1;
    qDebug() << "Nickname stored: " << storedNickname;
}

void MainWindow::on_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_pushButton_4_clicked() {
    player1.Nickname = ui->lineEdit->text().toStdString();

    if (player1.Nickname == "") {
        ui->label_30->setText("Please fill the nickname!");
        return ui->stackedWidget->setCurrentWidget(ui->page_2);
    } else {
        ui->label_30->setText("");
    }

    ui->label_3->setText(QString::fromStdString(player1.Nickname) + " choose your character!");
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


void MainWindow::SinglePlayerFrieren() {
    player1 = { "Frieren", ui->lineEdit->text().toStdString(), 150, 150, 400, 400, 35, 10, 10 };

    ui->pushButton_9->setText("Zoltraak | Mana cost: 30");
    randomOpp(opponent);
    fightCount = 0;

    ui->label_4->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    ui->label_5->setText(QString::fromStdString(opponent.Nickname) + "`s Stats");
    updateStats();
}

void MainWindow::SinglePlayerEisen() {
    player1 = {"Eisen", ui->lineEdit->text().toStdString(), 200, 200, 100, 100, 30, 20, 20};

    ui->pushButton_9->setText("Lighting Strike | Mana cost: 30");
    randomOpp(opponent);
    fightCount = 0;

    ui->label_4->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    ui->label_5->setText(QString::fromStdString(opponent.Nickname) + "`s Stats");
    updateStats();

}

void MainWindow::SinglePlayerHimmel() {
    player1 = {"Himmel", ui->lineEdit->text().toStdString(), 200, 200, 100, 100, 35, 10, 10};

    ui->pushButton_9->setText("Swordsman | Mana cost: 30");
    randomOpp(opponent);
    fightCount = 0;

    ui->label_4->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    ui->label_5->setText(QString::fromStdString(opponent.Nickname) + "`s Stats");
    updateStats();
}

void MainWindow::SinglePlayerHeiter() {
    player1 = {"Heiter", ui->lineEdit->text().toStdString(), 150, 150, 350, 350, 25, 10, 10};

    ui->pushButton_9->setText("Recovery | Mana cost: 30");
    randomOpp(opponent);
    fightCount = 0;

    ui->label_4->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    ui->label_5->setText(QString::fromStdString(opponent.Nickname) + "`s Stats");
    updateStats();
}

void MainWindow::on_pushButton_7_clicked() {
    CritChance(player1, opponent);
    endTurn();
    ResetDef();
}

void MainWindow::on_pushButton_11_clicked() {
    heal(player1);
    endTurn();
    ResetDef();
}

void MainWindow::on_pushButton_8_clicked()
{
    defense(player1);
    endTurn();
}


void MainWindow::on_pushButton_9_clicked()
{
    if(player1.name == "Frieren") {
        Zoltraak(player1, opponent);
    } else if (player1.name == "Eisen") {
        LightingStrike(player1, opponent);
    } else if (player1.name == "Himmel") {
        Swordsman(player1, opponent);
    } else if (player1.name == "Heiter") {
        FullRecover(player1);
    }

    endTurn();
    ResetDef();
}

void MainWindow::on_pushButton_10_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::on_pushButton_5_clicked() {
    SinglePlayerFrieren();
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::on_pushButton_6_clicked() {
    SinglePlayerHimmel();
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::on_pushButton_13_clicked()
{
    SinglePlayerEisen();
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::on_pushButton_14_clicked()
{
    SinglePlayerHeiter();
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::on_pushButton_15_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

//MultiPlayer

void MainWindow::on_lineEdit_2_textChanged(const QString &arg1)
{
    storedNickname = arg1;
    qDebug() << "Nickname stored: " << storedNickname;
}

void MainWindow::on_lineEdit_3_textChanged(const QString &arg1)
{
    storedNickname2 = arg1;
    qDebug() << "Nickname2 stored: " << storedNickname2;
}

void MainWindow::on_pushButton_12_clicked()
{
    player1.Nickname = ui->lineEdit_2->text().toStdString();
    player2.Nickname = ui->lineEdit_3->text().toStdString();

    if (player1.Nickname == "" | player2.Nickname == "") {
        ui->label_29->setText("Filled the Nickname");
        return ui->stackedWidget->setCurrentWidget(ui->page_6);
    } else if(player1.Nickname == player2.Nickname) {
        ui->label_29->setText("player 1 and 2  name cannot be the same!");
        return ui->stackedWidget->setCurrentWidget(ui->page_6);
    } else {
        ui->label_29->setText("");
    }

    ui->label_15->setText(QString::fromStdString(player1.Nickname) + " choose your character!");
    ui->label_16->setText(QString::fromStdString(player2.Nickname) + " choose your character!");
    ui->stackedWidget->setCurrentWidget(ui->page_7);
}


void MainWindow::on_pushButton_16_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}



void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_6);
}


void MainWindow::on_pushButton_19_clicked()
{
    player1 = { "Frieren", ui->lineEdit_2->text().toStdString(), 150, 150, 400, 400, 35, 10, 10 };
    ui->pushButton_28->setText("Zoltraak | Mana cost: 30");
    ui->label_17->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    updateStatsMP();
}

void MainWindow::on_pushButton_17_clicked()
{
    player1 = {"Himmel", ui->lineEdit_2->text().toStdString(), 200, 200, 100, 100, 35, 10, 10};
    ui->pushButton_28->setText("Swordsman | Mana cost: 30");
    ui->label_17->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    updateStatsMP();
}

void MainWindow::on_pushButton_18_clicked()
{
    player1 = {"Eisen", ui->lineEdit_2->text().toStdString(), 200, 200, 100, 100, 30, 20, 20};
    ui->pushButton_28->setText("Lighting Strike | Mana cost: 30");
    ui->label_17->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    updateStatsMP();
}


void MainWindow::on_pushButton_20_clicked()
{
    player1 = {"Heiter", ui->lineEdit_2->text().toStdString(), 150, 150, 350, 350, 25, 10, 10};
    ui->pushButton_28->setText("Recovery | Mana cost: 30");
    ui->label_17->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    updateStatsMP();
}

void MainWindow::on_pushButton_25_clicked()
{

    if(player1.name == "" | player2.name =="") {
        ui->label_28->setText("Please choose character!");
        return ui->stackedWidget->setCurrentWidget(ui->page_7);;
    } else {
        ui->label_28->setText("");
    }

    ui->stackedWidget->setCurrentWidget(ui->page_8);
    nextTurn();
}

void MainWindow::on_pushButton_21_clicked()
{
    player2 = { "Frieren", ui->lineEdit_3->text().toStdString(), 150, 150, 400, 400, 35, 10, 10 };
    ui->pushButton_32->setText("Zoltraak | Mana cost: 30");
    ui->label_20->setText(QString::fromStdString(player2.Nickname) + "`s Stats (" + QString::fromStdString(player2.name) + ")");
    updateStatsMP();
}


void MainWindow::on_pushButton_23_clicked()
{
    player2 = {"Himmel", ui->lineEdit_3->text().toStdString(), 200, 200, 100, 100, 35, 10, 10};
    ui->pushButton_32->setText("Swordsman | Mana cost: 30");
    ui->label_20->setText(QString::fromStdString(player2.Nickname) + "`s Stats (" + QString::fromStdString(player2.name) + ")");
    updateStatsMP();
}


void MainWindow::on_pushButton_22_clicked()
{
    player2 = {"Eisen", ui->lineEdit_3->text().toStdString(), 200, 200, 100, 100, 30, 20, 20};
    ui->pushButton_32->setText("Lighting Strike | Mana cost: 30");
    ui->label_20->setText(QString::fromStdString(player2.Nickname) + "`s Stats (" + QString::fromStdString(player2.name) + ")");
    updateStatsMP();
}


void MainWindow::on_pushButton_24_clicked()
{
    player2 = {"Heiter", ui->lineEdit_3->text().toStdString(), 150, 150, 350, 350, 25, 10, 10};
    ui->pushButton_32->setText("Recovery | Mana cost: 30");
    ui->label_20->setText(QString::fromStdString(player2.Nickname) + "`s Stats (" + QString::fromStdString(player2.name) + ")");
    updateStatsMP();
    nextTurn();
}


void MainWindow::on_pushButton_26_clicked()
{
    CritChance(player1, player2);
    endTurnMP(player2);
    ResetDef();
}


void MainWindow::on_pushButton_27_clicked()
{
    defense(player1);
    endTurnMP(player2);
}

void MainWindow::on_pushButton_28_clicked()
{
    if(player1.name == "Frieren") {
        Zoltraak(player1, player2);
    } else if (player1.name == "Eisen") {
        LightingStrike(player1, player2);
    } else if (player1.name == "Himmel") {
        Swordsman(player1, player2);
    } else if (player1.name == "Heiter") {
        FullRecover(player1);
    }

    endTurnMP(player2);
    ResetDef();
}


void MainWindow::on_pushButton_29_clicked()
{
    heal(player1);
    endTurnMP(player2);
    ResetDef();
}


void MainWindow::on_pushButton_30_clicked()
{
    CritChance(player2, player1);
    endTurnMP(player2);
    ResetDef();
}


void MainWindow::on_pushButton_31_clicked()
{
    defense(player2);
    endTurnMP(player2);
}


void MainWindow::on_pushButton_32_clicked()
{
    if(player2.name == "Frieren") {
        Zoltraak(player2, player1);
    } else if (player2.name == "Eisen") {
        LightingStrike(player2, player1);
    } else if (player2.name == "Himmel") {
        Swordsman(player2, player1);
    } else if (player2.name == "Heiter") {
        FullRecover(player2);
    }

    checkGameOverMP();
    updateStatsMP();
    nextTurn();
    ResetDef();
    ManaRegen(player2);
    Passive(player2);
}


void MainWindow::on_pushButton_33_clicked()
{
    heal(player2);
    endTurnMP(player2);
    ResetDef();
}
