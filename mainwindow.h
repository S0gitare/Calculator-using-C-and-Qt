#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qapplication.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QString dir = qApp->applicationDirPath();
    QString user = dir + "C:\\Users\\Alecs\\Documents\\QT-Apps\\Calculadora\\resource\\calculadora.db";
    ~MainWindow();

private slots:

    void numero_clicado();
    void apagar();
    void limpar();
    void ponto();
    void operacao_selecionada();
    void resultado();
    void porcentagem();
private:
    Ui::MainWindow *ui;
    QString operacao;
    float primeironumero;
};
#endif // MAINWINDOW_H
