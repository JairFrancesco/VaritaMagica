#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tgs.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void establecerImagen(QImage imagen);
    void mostrarImagen(QString filename);
    //void segmentarImagen();
    ~MainWindow();

private slots:
    void on_btnOpen_clicked();

private:
    TGs * sceneTo;
    Ui::MainWindow *ui;
    QImage imagen;
    QImage res;
};

#endif // MAINWINDOW_H
