#ifndef INTERFACE_H
#define INTERFACE_H


#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include "Algoritmos.hpp"
#include "utils.hpp"
using namespace std;

namespace Ui {
class Interface;
}

class Interface : public QMainWindow
{
    Q_OBJECT

public:
    explicit Interface(QWidget *parent = nullptr);
    ~Interface();


private slots:

    void binarization(int threshold);

    void label();

    void filter(int);

    void proces_A();

    void substraction();

    void invert();
    
    void image_autothreshold();

    void move();

    void intersection();

    void grayscale();

    void open_image_p();

    void open_image_q();

    void open_image_r();

    void save_res_image();

private:
    
    Ui::Interface *ui;

    QPixmap p,q,r; // pixmap p,q,r used to show the image in the frames
        
};

#endif // INTERFACE_H
