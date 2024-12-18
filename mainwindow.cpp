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

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::opponentTurn(Character& attacker, Character& defender) {
    int choice = randomInRange(0, 1);

        if (choice == 0) {
            attack(attacker, defender);
            return;
        }
        if (choice == 1) {
            heal(attacker);
            return;
        }
}

void MainWindow::attack(Character& attacker, Character& defender) {
    int miss = randomInRange(0, 100);

    if (miss < 10) {
        if(attacker.name == player1.name) {
            ui->label_11->setText(QString::fromStdString(attacker.name) + " miss the attack!");
            return;
        }

        if(attacker.name == player2.name) {
            ui->label_12->setText(QString::fromStdString(attacker.name) + " miss the attack!");
            return;
        }

        if(attacker.name == opponent.name) {
            ui->label_12->setText(QString::fromStdString(attacker.name) + " miss the attack!");
            return;
        }
        return;
    }

    int damage = max(0, attacker.attack - defender.defense);
    defender.health -= damage;

    if(attacker.name == player1.name) {
        ui->label_11->setText(QString::fromStdString(attacker.name) + " attack for " + QString::number(damage));
        return;
    }

    if(attacker.name == player2.name) {
        ui->label_12->setText(QString::fromStdString(attacker.name) + " attack for " + QString::number(damage));
        return;
    }

    if(attacker.name == opponent.name) {
        ui->label_12->setText(QString::fromStdString(attacker.name) + " attack for " + QString::number(damage));
        return;
    }
}

void MainWindow::heal(Character& player) {
    int heal = randomInRange(30, 40);

    if (player.mana >= 15) {
        if (player.health + heal > player.maxHealth) {
            int heal = player.maxHealth - player.health;
            player.health += heal;
            player.mana -= 10;
            ui->label_11->setText(QString::fromStdString(player.name) + " heal for " + QString::number(heal) + " health points!");
            return;
        }
    } else {
        if(player.name == player1.name) {
            ui->label_11->setText(QString::fromStdString(player.name) + " doesnt have enough mana!");
            return;
        }

        if(player.name == player2.name) {
            ui->label_12->setText(QString::fromStdString(player.name) + " doesnt have enough mana!");
            return;
        }

        if(player.name == opponent.name) {
            ui->label_12->setText(QString::fromStdString(player.name) + " doesnt have enough mana!");
            return;
        }
    }
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    storedNickname = arg1;
    qDebug() << "Stored Nickname updated: " << storedNickname;

    if (storedNickname.isEmpty()) {
        qDebug() << "Stored Nickname is empty. Setting default value.";
        storedNickname = "Default Nickname";
    }
    qDebug() << "Nickname stored: " << storedNickname;
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString fromStoredNickname1 = ui->lineEdit->text();
    Character player1;

    player1.Nickname = fromStoredNickname1.toStdString();

    ui->label_3->setText(QString::fromStdString(player1.Nickname) + " please choose your character!");

    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


void MainWindow::on_pushButton_5_clicked()
{
    QString fromStoredNickname1 = ui->lineEdit->text();

    player1.name = "Frieren";
    player1.Nickname = fromStoredNickname1.toStdString();
    player1.health = 150;
    player1.maxHealth = 150;
    player1.mana = 200;
    player1.maxMana = 200;
    player1.defense = 10;
    player1.attack = 35;

    opponent.name = "Solar Dragon";
    opponent.Nickname = opponent.name;
    opponent.health = 500;
    opponent.maxHealth = 500;
    opponent.mana = 200;
    opponent.maxMana = 200;
    opponent.defense = 15;
    opponent.attack = 50;

    ui->label_4->setText(QString::fromStdString(player1.Nickname) + "` Stats. (" + QString::fromStdString(player1.name) +")");

    ui->label_5->setText(QString::fromStdString(opponent.Nickname + "`Stats."));

    ui->label_6->setText("HP: " + QString::number(player1.health));

    ui->label_7->setText("Mana: " + QString::number(player1.mana));

    ui->label_8-> setText("HP: " + QString::number(opponent.health));

    ui->label_9-> setText("Mana: " + QString::number(opponent.mana));

    ui->stackedWidget->setCurrentWidget(ui->page_4);
}


void MainWindow::on_pushButton_7_clicked()
{
    attack(player1, opponent);

    opponentTurn(opponent, player1);

    if(player1.health <= 0) {

        ui->label_13->setText("you are defeated!");

        ui->stackedWidget->setCurrentWidget(ui->page_5);
    }

    if(opponent.health <= 0) {
        ui->label_13->setText(QString::fromStdString(player1.Nickname) + " is the winner!");

        ui->stackedWidget->setCurrentWidget(ui->page_5);
    }


    ui->label_6->setText("HP: " + QString::number(player1.health));

    ui->label_7->setText("Mana: " + QString::number(player1.mana));

    ui->label_8->setText("HP: " + QString::number(opponent.health));

    ui->label_9->setText("Mana: " + QString::number(opponent.mana));
}


void MainWindow::on_pushButton_10_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->page);
}

