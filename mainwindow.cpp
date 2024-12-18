#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <string>
#include <QDebug>

using namespace std;

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->stackedWidget->setCurrentWidget(ui->page);

    storedNickname = "";
    storedNickname2 = "";
}

MainWindow::~MainWindow()
{
    delete ui;
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
    Character player1;
    Character opponent;

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

    ui->label_6->setText("HP: " + QString::number(player1.health));

    ui->label_7->setText("Mana: " + QString::number(player1.mana));

    ui->label_5->setText(QString::fromStdString(opponent.Nickname + "`Stats."));

    ui->label_8-> setText("HP: " + QString::number(opponent.health));

    ui->label_9-> setText("Mana: " + QString::number(opponent.mana));

    while (player1.health > 0 || opponent.health > 0) {

    }


    ui->stackedWidget->setCurrentWidget(ui->page_4);
}
