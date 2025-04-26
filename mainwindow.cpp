#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap logo(":/imagens/logo.png");
    ui->lblLogo->setPixmap(logo.scaled(ui->lblLogo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    QPixmap logo2(":/imagens/logo2.png");
    ui->lblLogo2->setPixmap(logo2.scaled(ui->lblLogo2->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Clique do botão "Converter"
void MainWindow::on_btnConverterBases_clicked()
{
    QString numeroTexto = ui->txtNumero->text().trimmed();
    QString baseTexto = ui->comboBase->currentText();
    bool ok;
    int numDecimal = 0;

    // Interpreta o número de acordo com a base escolhida
    if (baseTexto == "Decimal")
        numDecimal = numeroTexto.toInt(&ok, 10);
    else if (baseTexto == "Binário")
        numDecimal = numeroTexto.toInt(&ok, 2);
    else if (baseTexto == "Octal")
        numDecimal = numeroTexto.toInt(&ok, 8);
    else if (baseTexto == "Hexadecimal")
        numDecimal = numeroTexto.toInt(&ok, 16);

    if (!ok) {
        QMessageBox::warning(this, "Erro", "Número inválido para a base selecionada.");
        limparCamposConversao();
        return;
    }

    // Mostra os resultados
    ui->lblDecimal->setText(QString::number(numDecimal, 10));
    ui->lblBinario->setText(QString::number(numDecimal, 2));
    ui->lblOctal->setText(QString::number(numDecimal, 8));
    ui->lblHexadecimal->setText(QString::number(numDecimal, 16).toUpper());

    // Mostrar explicação
    mostrarExplicacaoConversao(numDecimal);
}

// Clique do botão "Calcular"
void MainWindow::on_btnCalcular_clicked()
{
    QString op1Text = ui->txtOperando1->text().trimmed();
    QString op2Text = ui->txtOperando2->text().trimmed();
    QString baseSelecionada = ui->comboBaseOperacao->currentText(); // uma base só
    QString operador = ui->comboOperador->currentText();

    int base = 10;
    if (baseSelecionada == "Binário") base = 2;
    else if (baseSelecionada == "Octal") base = 8;
    else if (baseSelecionada == "Decimal") base = 10;
    else if (baseSelecionada == "Hexadecimal") base = 16;

    bool ok1 = false, ok2 = false;
    int num1 = op1Text.toInt(&ok1, base);
    int num2 = op2Text.toInt(&ok2, base);

    // Tratamento de erro: formato inválido para a base
    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "Erro", "Um ou ambos os operandos estão inválidos para a base " + baseSelecionada + ".");
        ui->lblResultadoOp->setText("Resultado:");
        return;
    }

    // Divisão por zero
    if (operador == "÷" && num2 == 0) {
        QMessageBox::warning(this, "Erro", "Divisão por zero!");
        ui->lblResultadoOp->setText("Resultado:");
        return;
    }

    int resultado = 0;
    if (operador == "+")       resultado = num1 + num2;
    else if (operador == "-")  resultado = num1 - num2;
    else if (operador == "×")  resultado = num1 * num2;
    else if (operador == "÷")  resultado = num1 / num2;

    // Exibição do resultado apenas na base selecionada
    ui->lblResultadoOp->setWordWrap(true);
    QString texto = "<b>Resultado:</b><br><br>";

    if (baseSelecionada == "Decimal") {
        texto += "<p><b>Decimal:</b> " + QString::number(resultado) + "</p>";
    } else if (baseSelecionada == "Binário") {
        texto += "<p><b>Binário:</b> " + QString::number(resultado, 2) + "</p>";
    } else if (baseSelecionada == "Octal") {
        texto += "<p><b>Octal:</b> " + QString::number(resultado, 8) + "</p>";
    } else if (baseSelecionada == "Hexadecimal") {
        texto += "<p><b>Hexadecimal:</b> " + QString::number(resultado, 16).toUpper() + "</p>";
    }

    ui->lblResultadoOp->setText(texto);
}


void MainWindow::mostrarExplicacaoConversao(int numero)
{
    QString texto;
    texto += "- Decimal para Binário:\n" + mostrarDivisoes(numero, 2) + "\n";
    texto += "- Decimal para Octal:\n" + mostrarDivisoes(numero, 8) + "\n";
    texto += "- Decimal para Hexadecimal:\n" + mostrarDivisoes(numero, 16, true);

    ui->txtExplicacao->setPlainText(texto);
}

QString MainWindow::mostrarDivisoes(int numero, int base, bool hexa)
{
    if (numero == 0)
        return "0 (zero)\n";

    QString passos;
    int num = numero;
    while (num > 0) {
        int quociente = num / base;
        int resto = num % base;
        QString restoStr;

        if (hexa && resto >= 10)
            restoStr = QString(QChar('A' + resto - 10));
        else
            restoStr = QString::number(resto);

        passos += QString::number(num) + " ÷ " + QString::number(base) +
                  " = " + QString::number(quociente) +
                  " resto " + restoStr + "\n";

        num = quociente;
    }
    return passos;
}

void MainWindow::limparCamposConversao()
{
    ui->lblBinario->setText("");
    ui->lblOctal->setText("");
    ui->lblDecimal->setText("");
    ui->lblHexadecimal->setText("");
    ui->txtExplicacao->clear();
}
