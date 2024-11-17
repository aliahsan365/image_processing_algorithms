#pragma once 
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <QImage>
#include <QVector>
#include "utils.hpp"
using namespace std;


class Algoritmos{  
public:
  //returns the binary image (black and white), using h as threshold.
  static QImage binarization(double h, const QImage& proc);
  
  //performs NOT operation on the given image. 
  static QImage invert(const QImage& proc);
  
  //performs the intensities substraction on two images of the same size (same width, height)
  static QImage substraction(QImage& img1,QImage& img2);
  
  //performs the AND operation on two images of the same size (same width, height)
  static QImage intersection(QImage& img1,QImage& img2);
  
  //returns the greayscale image, of a given Image.
  static QImage grayscale(QImage& proc);
  
  //returns a labeled image using the one-component-at-a-time algorithm
  static QImage labelling(const QImage& proc);
  
  //returns the resulting filtered image, using filter by surface, using region's area.
  static QImage filter(QImage& proc,int k);
  
  //returns the resulting image moved by (X,Y) pixels 
  static QImage move(QImage proc,int x,int y);
  
  //returns the autothreshold value according to the document "llindar_automatic.txt"
  static double autothreshold(const QImage& proc,double K1,double K2);


private:
  
  //check whether the image pixel is in the scope of the image pixel matrix
  static bool check_bounds(int filas, int columnas, int f, int c);
  
  //check whether the image pixel (f,c) is a white pixel (255,255,255; rgb)
  static bool check_white_pixel(const uchar* bit,int f,int c, int columnas);
  
  //algorithm : code for the one-component-at-a-time algorithm
  //https://en.wikipedia.org/wiki/Connected-component_labeling
  static void bfs_4(const QImage& img_binaria, vector<vector<int>>& regions, vector<vector<bool>>& visited, int current_tag, pair<int,int> pos_inicial, map<int,int>& regions_area);
  
  //compute the image pixel intensities (r+g+b/3)
  static vector< vector<double> > image_intensities(QImage proc);
  
  //compute the white pixels in a image; intensities[f][c] > K1
  static int  count_white_pixels(vector< vector<double> >& intensities, double K1);
  
  //create and paints the image using the image regions (contours) and our 12 colors
  static QImage paint_image(vector<vector<int>>& regions);
};




