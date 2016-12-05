#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->sceneTo = new TGs(this);
    ui->graphicsViewTo->setScene(this->sceneTo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnOpen_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open image...", QString(), "Image Files (*.bmp *.gif *.jpg *.jpeg *.png *.pbm *.pgm *.ppm *.xbm *.xpm);;All files (*.*)");
    this->establecerImagen(QImage(filename));
    this->mostrarImagen(filename);
}

void MainWindow::mostrarImagen(QString filename)
{
    //QPixmap pixmapTo = QPixmap::fromImage(res);
    QPixmap pixmapTo(filename);
    this->sceneTo->clear();
    this->sceneTo->addPixmap(pixmapTo);
}

void MainWindow::establecerImagen(QImage imagen)
{
    this->imagen = imagen;
    this->sceneTo->imagen = imagen;
}

/*
void MainWindow::segmentarImagen()
{
    float sigma = 0.5;
    float k = 200;
    int min_size = 50;

    Utility* ut = new Utility();
    image <rgb> *input = ut->qImageToImage(this->imagen);
    int num_ccs;
    image<rgb> *seg = ut->segmentImage(input, sigma, k, min_size, &num_ccs);

    res = ut->imageToQImage(seg);

    //res = Utility::imageToQImage(input);

    delete input;
    delete seg;

    QPixmap pixmapTo = QPixmap::fromImage(res);
    this->sceneTo->clear();
    this->sceneTo->addPixmap(pixmapTo);
}
*/
