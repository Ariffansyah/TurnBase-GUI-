#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <string>
#include <QDebug>

using namespace std;

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

void MainWindow::opponentTurn(Character& attacker, Character& defender) {
    if (randomInRange(0, 1) == 0) {
        attack(attacker, defender);
    } else {
        heal(attacker);
    }
}

void MainWindow::attack(Character& attacker, Character& defender) {
    int missChance = randomInRange(0, 100);

    if (missChance < 10) {
        updateMessage(attacker, QString::fromStdString(attacker.name) + " missed the attack!");
        return;
    }

    int damage = max(1, attacker.attack - defender.defense);
    defender.health -= damage;

    updateMessage(attacker, QString::fromStdString(attacker.name) + " attacks for " + QString::number(damage) + " damage!");
}


void MainWindow::heal(Character& player) {
    if (player.mana < 10) {
        updateMessage(player, QString::fromStdString(player.name) + " doesn't have enough mana!");
        return;
    }

    if (player.health == player.maxHealth) {
        updateMessage(player, QString::fromStdString(player.name) + " is already at full health. Turn skipped.");
        return;
    }

    int healAmount = player.name == "Heiter" ? randomInRange(40, 60) : randomInRange(10, 30);
    healAmount = min(healAmount, player.maxHealth - player.health);

    player.health += healAmount;
    player.mana -= 10;

    updateMessage(player, QString::fromStdString(player.name) + " heals for " + QString::number(healAmount) + " health!");
}


void MainWindow::updateMessage(Character& character, const QString& message) {
    if (character.name == player1.name) {
        ui->label_11->setText(message);
    } else {
        ui->label_12->setText(message);
    }
}

void MainWindow::updateStats() {
    ui->label_6->setText("HP: " + QString::number(player1.health));
    ui->label_7->setText("Mana: " + QString::number(player1.mana));
    ui->label_8->setText("HP: " + QString::number(opponent.health));
    ui->label_9->setText("Mana: " + QString::number(opponent.mana));
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1) {
    storedNickname = arg1.isEmpty() ? "Default Nickname" : arg1;
    qDebug() << "Nickname stored: " << storedNickname;
}

void MainWindow::on_pushButton_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_pushButton_4_clicked() {
    player1.Nickname = ui->lineEdit->text().toStdString();
    ui->label_3->setText(QString::fromStdString(player1.Nickname) + ", choose your character!");
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

void MainWindow::on_pushButton_5_clicked() {
    setupPlayerAndOpponent();
    ui->stackedWidget->setCurrentWidget(ui->page_4);
}

void MainWindow::on_pushButton_7_clicked() {
    attack(player1, opponent);
    opponentTurn(opponent, player1);
    checkGameOver();
    updateStats();
}

void MainWindow::on_pushButton_11_clicked() {
    heal(player1);
    opponentTurn(opponent, player1);
    checkGameOver();
    updateStats();
}

void MainWindow::on_pushButton_10_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->page);
}

void MainWindow::setupPlayerAndOpponent() {
    player1 = { "Frieren", ui->lineEdit->text().toStdString(), 150, 150, 200, 200, 35, 10 };
    opponent = { "Solar Dragon", "Solar Dragon", 500, 500, 200, 200, 35, 15 };

    ui->label_4->setText(QString::fromStdString(player1.Nickname) + "`s Stats (" + QString::fromStdString(player1.name) + ")");
    ui->label_5->setText(QString::fromStdString(opponent.Nickname) + "`s Stats");
    updateStats();
}

void MainWindow::checkGameOver() {
    if (player1.health <= 0) {
        ui->label_13->setText("You are defeated!");
        ui->stackedWidget->setCurrentWidget(ui->page_5);
    } else if (opponent.health <= 0) {
        ui->label_13->setText(QString::fromStdString(player1.Nickname) + " is the winner!");
        ui->stackedWidget->setCurrentWidget(ui->page_5);
    }
}
