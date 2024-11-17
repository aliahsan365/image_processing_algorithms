#include "interface.h"
#include "ui_interface.h"
#include <math.h>

Interface::Interface(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::Interface)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(1080,1080));
    this->connect(ui->thresholdSlider,SIGNAL(valueChanged(int)),this,SLOT(binarization(int)));
    this->connect(ui->filterSlider,SIGNAL(valueChanged(int)),this,SLOT(filter(int)));
    this->connect(ui->grayscale,SIGNAL(clicked()),this,SLOT(grayscale()));
    this->connect(ui->labelling,SIGNAL(clicked()),this,SLOT(label()));
    this->connect(ui->invert_btn,SIGNAL(clicked()),this,SLOT(invert()));
    this->connect(ui->openp,SIGNAL(clicked()),this,SLOT(open_image_p()));
    this->connect(ui->openq,SIGNAL(clicked()),this,SLOT(open_image_q()));
    this->connect(ui->openr,SIGNAL(clicked()),this,SLOT(open_image_r()));
    this->connect(ui->save_res,SIGNAL(clicked()),this,SLOT(save_res_image()));
    this->connect(ui->substraction_btn,SIGNAL(clicked()),this,SLOT(substraction()));
    this->connect(ui->move_btn,SIGNAL(clicked()),this,SLOT(move()));
    this->connect(ui->autothreshold_btn,SIGNAL(clicked()),this,SLOT(image_autothreshold()));
    this->connect(ui->intersection_btn,SIGNAL(clicked()),this,SLOT(intersection()));
    this->connect(ui->proces_a_btn,SIGNAL(clicked()),this,SLOT(proces_A()));
}

void Interface::proces_A() {
    QImage original;
    
    //select image to process (p or q frame)
    if (ui->p_selected_box->isChecked()) {
        original = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        original = q.copy().toImage();
    }

    int x = ui->box_x->value();
    int y = ui->box_y->value();

    QImage moved = Algoritmos::move(original,x,y); 

    QImage substracted = Algoritmos::substraction(original,moved);
        
    double K1=ui->box_K1->value();
    double K2=ui->box_K2->value();
    
    K1 = Algoritmos::autothreshold(substracted,K1,K2);
    
    double h = K1 * 255 / 100;
    
    QImage binary = Algoritmos::binarization(h,substracted);

    ui->autothreshold_label->setText(QString::number(K1));
        
    r = r.fromImage(binary);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
    
}

void Interface::intersection() {

    QImage img1,img2,res;
    
    img1 = p.copy().toImage();
    img2 = q.copy().toImage();
    
    res = Algoritmos::intersection(img1,img2);
    
    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));

}


void Interface::move() {
    QImage proc;
    
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }
    
    int x = ui->box_x->value();
    int y = ui->box_y->value();

    QImage moved = Algoritmos::move(proc,x,y); 
    
    r = r.fromImage(moved);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
}



void Interface::filter(int k) {

    QImage proc;
    
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }
   
    //filtramos la imagen segun k
    QImage res = Algoritmos::filter(proc,k);

    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
}


void Interface::image_autothreshold() {   
    QImage proc;
    
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }

    double K1=ui->box_K1->value();
    double K2=ui->box_K2->value();

    K1 = Algoritmos::autothreshold(proc,K1,K2);
    
    //using percentage because of our interface goes from 0 to 100.
    double h = K1 * 255 / 100; 
    
    QImage res = Algoritmos::binarization(h,proc);


    ui->autothreshold_label->setText(QString::number(K1));
    
    
    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
}


void Interface::substraction() {
    QImage img1,img2,res;
    
    img1 = p.copy().toImage();
    img2 = q.copy().toImage();
    
    res = Algoritmos::substraction(img1,img2);
    
    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
}


void Interface::open_image_p() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Image File", nullptr,
        "Images (*.jpg *.jpeg *.JPG *.png *.PNG *.bmp *.BMP );;"
        "JPG (*.jpg *.jpeg *.JPG);;"
        "PNG (*.png *.PNG);;"
        "BMP (*.bmp *.BMP)");
    if(!p.load(filename))
    {
        return;
    }
    else {
        ui->p_frame->setPixmap(p.scaled(ui->p_frame->size(), Qt::KeepAspectRatio));
    }

}

void Interface::open_image_q() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Image File", nullptr,
        "Images (*.jpg *.jpeg *.JPG *.png *.PNG *.bmp *.BMP );;"
        "JPG (*.jpg *.jpeg *.JPG);;"
        "PNG (*.png *.PNG);;"
        "BMP (*.bmp *.BMP)");
    if(!q.load(filename))
    {
        return;
    }
    else {
        ui->q_frame->setPixmap(q.scaled(ui->q_frame->size(), Qt::KeepAspectRatio));
    }
}



void Interface::open_image_r() {
    QString filename = QFileDialog::getOpenFileName(this, "Open Image File", nullptr,
        "Images (*.jpg *.jpeg *.JPG *.png *.PNG *.bmp *.BMP );;"
        "JPG (*.jpg *.jpeg *.JPG);;"
        "PNG (*.png *.PNG);;"
        "BMP (*.bmp *.BMP)");
    if(!r.load(filename))
    {
        return;
    }
    else {
        ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
    }

}


void Interface::invert()  {    
    QImage proc;
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }  
    QImage res = Algoritmos::invert(proc);

    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
}




Interface::~Interface()
{
    delete ui;
}

void Interface::binarization(int threshold)
{
    double h = 255 * double(threshold) / 100;
    QImage proc;
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }
    QImage res = Algoritmos::binarization(h,proc);
    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));
}



void Interface::grayscale() {

    QImage proc;
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }

    proc = Algoritmos::grayscale(proc);

    r = r.fromImage(proc);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));

}

void Interface::label() {

    QImage proc;
    
    if (ui->p_selected_box->isChecked()) {
        proc = p.copy().toImage();
    }    
    if (ui->q_selected_box->isChecked()) {
        proc = q.copy().toImage();
    }
    
    QImage res = Algoritmos::labelling(proc);
    

    r = r.fromImage(res);
    ui->r_frame->setPixmap(r.scaled(ui->r_frame->size(), Qt::KeepAspectRatio));    
}


void Interface::save_res_image() {
    QString saveFileName = QFileDialog::getSaveFileName(this, "Save processed image as..", "output.png",
        "jpg, png, bmp (*.jpg, *.png *.bmp)");
    QString ext = QFileInfo(saveFileName).suffix();
    QPixmap pixmap;
    if(ext=="jpg" || ext=="png" || ext=="bmp")
    {
        QFile outfile(saveFileName);
        if (!outfile.open(QFile::WriteOnly)){
            QMessageBox::warning(this, "Warning", "Cannot save file: " + outfile.errorString());
            return;
        }
        r.save(&outfile);
    }
}