#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "database.h"

#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

void setup_database(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Users\\Alecs\\Documents\\QT-Apps\\Calculadora\\resource\\calculadora.db");

    if (!db.open()){
        qDebug() << "Erro ao abrir banco de dados: " << db.lastError().text();
        return;
    }

    QSqlQuery query;
    query.exec("CREAT TABLE IF NOT EXIST calculos ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "operacao TEXT,"
               "resultado REAL");
}

bool resultado_mostrado = false;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->botao_0, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_1, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_2, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_3, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_4, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_5, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_6, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_7, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_8, &QPushButton::clicked, this, &MainWindow::numero_clicado);
    connect(ui->botao_9, &QPushButton::clicked, this, &MainWindow::numero_clicado);

    connect(ui->btn_backspace, &QPushButton::clicked, this, &MainWindow::apagar);

    connect(ui->btn_clear, &QPushButton::clicked, this, &MainWindow::limpar);

    connect(ui->virgula, &QPushButton::clicked, this, &MainWindow::ponto);

    connect(ui->soma, &QPushButton::clicked, this, &MainWindow::operacao_selecionada);
    connect(ui->subtracao, &QPushButton::clicked, this, &MainWindow::operacao_selecionada);
    connect(ui->multiplicacao, &QPushButton::clicked, this, &MainWindow::operacao_selecionada);
    connect(ui->divisao, &QPushButton::clicked, this, &MainWindow::operacao_selecionada);
    connect(ui->porcentagem, &QPushButton::clicked, this, &MainWindow::porcentagem);

    connect(ui->igual, &QPushButton::clicked, this, &MainWindow::resultado);
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::numero_clicado(){
    if (resultado_mostrado == true){
        ui->painel->clear();
        ui->operacao_atual->clear();
    }
    resultado_mostrado = false;
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString anterior = ui->painel->text();
        anterior = anterior + button->text();
        ui->painel->setText(anterior);
    }
}
void MainWindow::apagar(){
    QString anterior = ui->painel->text();
    if (!anterior.isEmpty()){
        anterior.chop(1);
        ui->painel->setText(anterior);
        ui->operacao_atual->clear();
    }
}
void MainWindow::limpar(){
    ui->painel->clear();
    ui->operacao_atual->clear();
}
void MainWindow::ponto(){
    QString anterior = ui->painel->text();
    if (!anterior.contains(".")){
        if (anterior.isEmpty()){
            anterior = "0.";
        }else{
            anterior = anterior + ".";
        }
        ui->painel->setText(anterior);
    }
}
void MainWindow::operacao_selecionada(){
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button){
        primeironumero = ui->painel->text().toFloat();
        operacao = button->text();
        ui->operacao_atual->setText(QString::number(primeironumero) + " " + operacao);
        ui->painel->clear();
    }
}
void MainWindow::resultado(){
    float segundo_numero = ui->painel->text().toFloat();
    float resultado = 0;

    if (operacao == "+"){
        resultado = primeironumero + segundo_numero;
    }else if (operacao == "-"){
        resultado = primeironumero - segundo_numero;
    }else if (operacao == "x"){
        resultado = primeironumero * segundo_numero;
    }else if (operacao == "÷"){
        if (segundo_numero != 0){
            resultado = primeironumero / segundo_numero;
        }else {
            ui->painel->setText("Erro");
            return;
        }
    }
    QString operacao_completa = QString::number(primeironumero) + " " + operacao + " " + QString::number(segundo_numero);
    ui->operacao_atual->setText(operacao_completa);
    ui->painel->setText(QString::number(resultado));


    qDebug() << "Inserindo operação:" << operacao_completa << " com resultado:" << resultado;

    QSqlQuery query;
    query.prepare("INSERT INTO calculadora (operacao, resultado) VALUES (:operacao, :resultado)");
    query.bindValue(":operacao", operacao_completa);
    query.bindValue(":resultado", resultado);

    if (!query.exec()){
        qDebug() << "Erro ao inserir no banco de dados: " << query.lastError().text();
    }

    resultado_mostrado = true;

}
void MainWindow::porcentagem(){
    float segundo_numero = ui->painel->text().toFloat();
    float resultado = 0;

    if (operacao == "+"){
        resultado = primeironumero + (primeironumero * segundo_numero / 100);
    }else if (operacao == "-"){
        resultado = primeironumero - (primeironumero * segundo_numero / 100);
    }else {
        resultado = primeironumero * (segundo_numero / 100);
    }

    ui->painel->setText(QString::number(resultado));
    ui->operacao_atual->clear();
}








