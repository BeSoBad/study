#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <cmath>
#include <string>
#include <QGraphicsTextItem>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


double f(double x) {
    return x * x * -x;
}

double func_lemniscate_1(double x, double a, bool& success) {
    if (std::sqrt(a*a*a*a/4 + 2*x*x*a*a) - x*x - a*a/2 >= 0) {
        success = true;
        double tmp = std::sqrt(std::sqrt(a*a*a*a/4 + 2*x*x*a*a) - x*x - a*a/2);
        return tmp;
    }
    else {
        success = false;
        return -1;
    }
}

double func_lemniscate_2(double x, double a, bool& success) {
    if (std::sqrt(a*a*a*a/4 + 2*x*x*a*a) - x*x - a*a/2 >= 0) {
        success = true;
        double tmp = -std::sqrt(std::sqrt(a*a*a*a/4 + 2*x*x*a*a) - x*x - a*a/2);
        return tmp;
    }
    else {
        success = false;
        return -1;
    }
}

int stoi(QString s) {
    int ans = 0, mult = 1;
    for (int i = s.length() - 1; i >= 0; i--) {
        ans += (s[i].toLatin1() - '0')*mult;
        mult *= 10;
    }
    return ans;
}

QString itos(int i) {
    if (i == 0)
        return "0";
    QString ans, rev_ans;
    while (i > 0) {
        ans += i % 10 + '0';
        i /= 10;
    }
    for (int i = ans.size() - 1; i >= 0; i--) {
        rev_ans += ans[i];
    }
    return rev_ans;
}

void MainWindow::on_pushButton_clicked() {

}

void MainWindow::on_pushButton_2_clicked()
{
    QGraphicsScene *scene = new QGraphicsScene(ui->graphicsView);

    QPen axes(Qt::black);
    int width = ui->graphicsView->width(), height = ui->graphicsView->height();
    QString s_c = ui->lineEdit_2->text();
    QString p_a = ui->lineEdit->text();
    bool success = true;
    for (int i = 0; i < s_c.size(); i++) {
        if (s_c[i].toLatin1() < '0' || s_c[i].toLatin1() > '9')
            success = false;
    }
    for (int i = 0; i < p_a.size(); i++) {
        if (p_a[i].toLatin1() < '0' || p_a[i].toLatin1() > '9')
            success = false;
    }
    if (p_a[0].toLatin1() == '0' || s_c[0].toLatin1() == '0') {
        success = false;
    }
    if (!success) {
        QMessageBox::critical(this, "Ошибка", "Вводимые параметры должны быть положительными числами, большими нуля");
    }
    else {
        double size_coef = stoi(ui->lineEdit_2->text());
        double param_a = stoi(ui->lineEdit->text());

        QPen graph(Qt::blue);
        double a = -param_a;
        double b =  param_a;
        double h = 0.01;
        int N = (b - a)/h;

        QVector<double> x(N), y(N);
        int i = 0;
        double max_y = -1000000000, min_y = 1000000000;
        for (double X = a; X < b; X += h)
        {
            if (i > N - 1) {
                break;
            }
            x[i] = X;
            bool success = false;
            double tmp_y = func_lemniscate_1(X, param_a, success);
                y[i] = tmp_y;
            if (y[i] > max_y)
                max_y = y[i];
            if (y[i] < min_y)
                min_y = y[i];
            i++;
        }
        if (x[x.size() - 1] != b) {
            x.push_back(b);
            bool success = true;
            y.push_back(func_lemniscate_1(b, param_a, success));
            if (y[y.size() - 1] > max_y)
                max_y = y[y.size() - 1];
            if (y[y.size() - 1] < min_y)
                min_y = y[y.size() - 1];
        }
        for (int i = 0; i < x.size() - 1; i++) {
            scene->addLine(x[i]*size_coef, y[i]*size_coef, x[i + 1]*size_coef, y[i + 1]*size_coef, graph);
        }
        i = 0;
        for (double X = a; X <= b; X += h)
        {
            if (i > N - 1) {
                break;
            }
            x[i] = X;
            bool success = false;
            double tmp_y = func_lemniscate_2(X, param_a, success);
                y[i] = tmp_y;
            if (y[i] > max_y)
                max_y = y[i];
            if (y[i] < min_y)
                min_y = y[i];
            i++;
        }

        if (x[x.size() - 1] != b) {
            x.push_back(b);
            bool success = true;
            y.push_back(func_lemniscate_2(b, param_a, success));
            if (y[y.size() - 1] > max_y)
                max_y = y[y.size() - 1];
            if (y[y.size() - 1] < min_y)
                min_y = y[y.size() - 1];
        }
        for (int i = 0; i < x.size() - 1; i++) {
            scene->addLine(x[i]*size_coef, y[i]*size_coef, x[i + 1]*size_coef, y[i + 1]*size_coef, graph);
        }
        if (max_y != int(max_y)) {
            max_y = int(max_y) + 1;
        }
        if (min_y != int(min_y)) {
            min_y = int(min_y) - 1;
        }
        scene->addLine(a*size_coef, 0, b*size_coef, 0, axes);
        scene->addLine(0, min_y*size_coef, 0, max_y*size_coef, axes);
        QPen scale(Qt::black);
        i = a;
        for (int X = a*size_coef; X <= b*size_coef; X += size_coef) {
            scene->addLine(X, 0.03*size_coef, X, -0.03*size_coef, scale);
            QString s;
            if (i < 0) {
                s = "-" + itos(-i);
            }
            else {
                s = itos(i);
            }
            QGraphicsTextItem * io = new QGraphicsTextItem;
            io->setPos(X, 0.03*size_coef);
            io->setPlainText(s);
            io->setScale(0.015*size_coef);
            scene->addItem(io);
            i++;
        }
        i = min_y;
        for (int Y = min_y*size_coef; Y <= max_y*size_coef; Y += size_coef) {
            scene->addLine(0.03*size_coef, Y, -0.03*size_coef, Y, scale);
            QString s;
            if (i > 0) {
                s = "-" + itos(i);
            }
            else if (i < 0){
                s = itos(-i);
            }
            QGraphicsTextItem * io = new QGraphicsTextItem;
            io->setPos(0.03*size_coef, Y);
            io->setPlainText(s);
            io->setScale(0.015*size_coef);
            scene->addItem(io);
            i++;
        }
        ui->graphicsView->setScene(scene);
    }
}

