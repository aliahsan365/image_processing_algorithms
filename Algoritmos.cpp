#include "Algoritmos.hpp"

//---------------INTRO---------

//The general idea in order to acces the  pixels of a given Image:
//array bytes (named as "bit" ) => (to) image pixel matrix (rgb)
//array of bytes =======> matrix of pixels
//                        c1  c2 c3       cn
//|---|0  ========>   f1|0,0 0,1 0,2 ..     |  
//|---|1              f2|1,0 1,1 ..         |
//|---|2                |                   |
//|---|                 |                   |  
//|---|               fn|                   | 
// ...              
//|---| array_total_size  = fn*cn*4      


//therefore, 
//array bytes dest_index = 4*f1 + 4*cn*c1 = matrix pixel (f1,c1), in the image pixel matrix
//where, cn is the weight of the image (pixels in X direction)
//Left most matrix pixel is the first (0) in the array of bytes
//so, red = bit[dest_index+0]; green = bit[dest_index+1]; blue = bit[dest_index+2];
//is 4 and not 3, because of the alpha channel of the image
//1 byte = 8 bits = 1 uchar = values within[0..255]
//---------------END INTRO---------



vector< vector<double> >  Algoritmos::image_intensities(QImage proc) {
  uchar *bit = proc.bits();
  int filas = proc.height();
  int columnas = proc.width();
  vector <vector<double>> intensities(filas,vector<double> (columnas,0));
  for (int f = 0; f < filas; ++f) {
    for (int c = 0; c < columnas; ++c) {
      int dest_index = 4*c + 4*columnas*f;
      int r,g,b;
      r = int(bit[dest_index+0]); 
      g = int(bit[dest_index+1]);
      b = int(bit[dest_index+2]);
      double intensity = r+g+b/3;
      intensities[f][c] = intensity;
    }
  }
  return intensities;
}

//counts the image white pixels in a image
int Algoritmos::count_white_pixels(vector< vector<double> >& intensities, double K1) {
  int N = 0;  // #white pixels
  int filas,columnas;
  double h = K1 * 255 / 100;
  filas =  intensities.size();
  columnas = intensities[0].size();
  for (int f = 0; f < filas; ++f) {
    for (int c = 0; c < columnas; ++c) {
      if (intensities[f][c] > h) { // if white
        ++N;
      }
    }
  }
  return N;
}

//PRECONDITION : K1 >= 0 and K1 <= 100.00 ; K2 >= 0 and K2 <= 100.00  
//POSTCONDITION : AUTOTHRESHOLD VALUE >= 0 and AUTOTHRESHOLD VALUE <= 100.00 
double Algoritmos::autothreshold(const QImage& proc, double K1, double K2) {
  bool mycondition = false;
  int filas = proc.height();
  int columnas = proc.width();
  vector< vector<double> > intensities = image_intensities(proc);
  int total_pixeles = filas * columnas;
  int itr = 0;
  while (!mycondition and abs(round(K1)) < 100 ) {      
    int N =  count_white_pixels(intensities,K1);
    double NN  = N*100/total_pixeles; // we work with percentages
    double E = NN - K2;
    if (abs(E) <= 1) {
      mycondition = true;
    }
    else if (E > 0 and abs(E) > 1) {
      K1 = K1 - E/20;
    }
    else if (E < 0 and abs(E) > 1) {
      K1 = K1 + E/20;
    }
    ++itr;
  }
  return abs(K1);
}



QImage Algoritmos::binarization(double h ,const QImage& proc) {
  QImage res = QImage(proc);
  uchar *bit = res.bits();
  int filas = res.height();
  int columnas = res.width();
  int f, c, dest_index;
  for(f = 0; f < filas; ++f){
    for(c = 0; c < columnas; ++c){
      dest_index = 4*c + 4*columnas*f;
      int r,g,b;
      r = int(bit[dest_index+0]);
      g = int(bit[dest_index+1]);
      b = int(bit[dest_index+2]);
      double intensity = (r+g+b)/3;
      if (intensity > h) {
        bit[dest_index+0] = uchar(255);
        bit[dest_index+1] = uchar(255);
        bit[dest_index+2] = uchar(255);
      }
      else {
        bit[dest_index+0] = uchar(0);
        bit[dest_index+1] = uchar(0);
        bit[dest_index+2] = uchar(0);
      }
    }
  }
  return res;
}

QImage Algoritmos::invert(const QImage& proc) {
  QImage res = QImage(proc);
  uchar *bit = res.bits();
  int h = res.height();
  int w = res.width();
  int i, j, dest_index;
  for(i = 0; i < w; ++i){
    for(j = 0; j < h; ++j){
      dest_index = 4*i + 4*w*j;
      int r,g,b;
      r = ~int(bit[dest_index+0]);
      g = ~int(bit[dest_index+1]);
      b = ~int(bit[dest_index+2]);
      bit[dest_index+0] = uchar(r);
      bit[dest_index+1] = uchar(g);
      bit[dest_index+2] = uchar(b);            
    }
  }
  return res;
}

QImage Algoritmos::grayscale(QImage& proc) {
  QImage res = QImage(proc);
  uchar *bit = res.bits();
  int h = res.height();
  int w = res.width();
  int i, j, dest_index;
  for(i = 0; i < w; ++i){
    for(j = 0; j < h; ++j){
      dest_index = 4*i + 4*w*j;
      int r,g,b;
      r = int(bit[dest_index+0]);
      g = int(bit[dest_index+1]);
      b = int(bit[dest_index+2]);
      int intensity = (r+g+b)/3;            
      bit[dest_index+0] = uchar(intensity);
      bit[dest_index+1] = uchar(intensity);
      bit[dest_index+2] = uchar(intensity);
    }
  }
  return res;
}

//PRECONDITION : proc is an binary image
QImage Algoritmos::filter(QImage& proc, int k) {
  uchar *bit = proc.bits();
  int filas = proc.height();
  int columnas = proc.width();
  //pixel i,j matrix contains the region id.
  vector <vector<int>> regions(filas,vector<int> (columnas,0));
  //pixel i,j matrix contains whether that pixels has been or not visited yet.
  vector <vector<bool>> visited(filas,vector<bool> (columnas,false));
  //dictionary that contains the region id => and is area (#number of pixels that belong to that region)
  map<int,int> regions_area;
  int n_bfs = 0; // number of regions; +1 implies +1 region labeled
  for (int f = 0; f < filas; ++f) {
    for (int c = 0; c < columnas; ++c) {
      if (check_white_pixel(bit,f,c,columnas) and !visited[f][c]) {
        ++n_bfs;
        pair<int,int> pos_inicial = make_pair(f,c);
        //algorithm : code for the one-component-at-a-time algorithm
        //https://en.wikipedia.org/wiki/Connected-component_labeling
        bfs_4(proc,regions,visited,n_bfs,pos_inicial,regions_area);
      }
    }
  }
  
  //finds the region with the maximum area in a dictionary
  int maxim = Utils::maxim_map(regions_area);
  
  //K is in percentage, therefore the area need to be in percentage also
  Utils::map_percentage(regions_area,maxim);
  
  QImage res_image = QImage(proc);
  uchar *bit_processed = res_image.bits();
  map<int,int>::iterator it;
  for(it = regions_area.begin(); it!=regions_area.end(); ++it) {
    if (it->second < k) {// if percentage area is < k then filter that region
      int r = it->first;  
      for(int f = 0; f < filas; ++f){
        for(int c = 0; c < columnas; ++c) {
          if (r == regions[f][c]) {
            //put that regions pixels as black pixels (filtered region)
            int dest_index = 4*f*columnas + 4*c;
            bit_processed[dest_index+0] = 0; 
            bit_processed[dest_index+1] = 0;
            bit_processed[dest_index+2] = 0;
          }
        }
      }
    }
  }
  return res_image;
}


//PRECONDITION : img1 and img2 has same size (same width, height)
QImage Algoritmos::substraction(QImage& img1,QImage& img2) {
  uchar *bit_p = img1.bits();
  uchar *bit_q = img2.bits();
  QImage res = QImage(img1);
  uchar *bit = res.bits();
  int h = img1.height();
  int w = img2.width();
  int i, j, dest_index;
  for(i = 0; i < w; ++i){
    for(j = 0; j < h; ++j){
      dest_index = 4*i + 4*w*j;
      int r1,g1,b1,r2,g2,b2;
      r1 = int(bit_p[dest_index+0]); 
      r2 = int(bit_q[dest_index+0]);
      g1 = int(bit_p[dest_index+1]);
      g2 = int(bit_q[dest_index+1]);
      b1 = int(bit_p[dest_index+2]);
      b2 = int(bit_q[dest_index+2]);
      int intensity_p = r1+g1+b1/3;
      int intensity_q = r2+g2+b2/3;
      //substracted  intensitie = r + g + b / 3
      int intensity_r = abs(intensity_p-intensity_q); 
      bit[dest_index+0] = uchar(intensity_r);
      bit[dest_index+1] = uchar(intensity_r);
      bit[dest_index+2] = uchar(intensity_r);            
    }
  }
  return res;
}


//PRECONDITION : img1 and img2 has same size (same width, height)
QImage Algoritmos::intersection(QImage& img1,QImage& img2) {
  uchar *bit_p = img1.bits();
  uchar *bit_q = img2.bits();
  QImage res = QImage(img1);
  uchar *bit = res.bits();
  int h = img1.height();
  int w = img2.width();
  int i, j, dest_index;
  for(i = 0; i < w; ++i){
    for(j = 0; j < h; ++j){
      dest_index = 4*i + 4*w*j;
      int r1,g1,b1,r2,g2,b2;
      r1 = int(bit_p[dest_index+0]); 
      r2 = int(bit_q[dest_index+0]);
      g1 = int(bit_p[dest_index+1]);
      g2 = int(bit_q[dest_index+1]);
      b1 = int(bit_p[dest_index+2]);
      b2 = int(bit_q[dest_index+2]);
      bit[dest_index+0] = r1&r2;
      bit[dest_index+1] = g1&g2;
      bit[dest_index+2] = b1&b2;
    }
  }
  return res;
}


//PRECONDITION : proc is an binary image
//returns a labeled image using the one-component-at-a-time algorithm
QImage Algoritmos::labelling(const QImage& proc) {
  const uchar *bit = proc.bits();
  int filas = proc.height();
  int columnas = proc.width();
  //pixel i,j matrix contains the region id.
  vector <vector<int>> regions(filas,vector<int> (columnas,0));
  //pixel i,j matrix contains whether that pixels has been or not visited yet.
  vector <vector<bool>> visited(filas,vector<bool> (columnas,false));
  //dictionary that contains the region id => and is area (#number of pixels that belong to that region)
  map<int,int> regions_area;
  int n_bfs = 0; // numeber of regions , +1 n_bfs implies +1 region labeled
  for (int f = 0; f < filas; ++f) {
    for (int c = 0; c < columnas; ++c) {
      if (check_white_pixel(bit,f,c,columnas) and !visited[f][c]) {
        ++n_bfs;
        pair<int,int> pos_inicial = make_pair(f,c);
        //algorithm : code for the one-component-at-a-time algorithm
        //https://en.wikipedia.org/wiki/Connected-component_labeling
        bfs_4(proc,regions,visited,n_bfs,pos_inicial,regions_area);
      }
    }
  }
  QImage labeled = paint_image(regions);
  return labeled;
}


//moves X,Y pixels in a image.
QImage Algoritmos::move(QImage proc, int x, int y) {
    
  QImage moved = QImage(proc);
  
  uchar *bit_moved = moved.bits();
  uchar *bit = proc.bits();
  
  int filas = proc.height();
  int columnas = proc.width();
  
  for(int f = 0; f < filas-x; ++f){
    for(int c = 0 ; c < columnas-y; ++c){
      int dest_index = 4*f*columnas + 4*c;
      int r,g,b;
      r = int(bit[dest_index+0]);
      g = int(bit[dest_index+1]);
      b = int(bit[dest_index+2]);
      int moved_index =  4*(f+x)*columnas + 4*(c+y);         
      bit_moved[moved_index+0] = uchar(r);
      bit_moved[moved_index+1] = uchar(g);
      bit_moved[moved_index+2] = uchar(b);
    }
  }
  return moved;
}


// create and paint a new image using a given regions matrix.
QImage Algoritmos::paint_image(vector<vector<int>>& regions) {
  QVector<QColor> colores;
  QColor c0_qt,c1_qt,c2_qt, c3_qt,c4_qt,c5_qt, c6_qt,c7_qt,c8_qt, c9_qt,c10_qt,c11_qt, c12_qt,c13_qt; 

  c0_qt = QColor(255,0,0);
  c1_qt = QColor(0,255,0);
  c2_qt = QColor(0,0,255);

  c3_qt = QColor(255,255,0);
  c4_qt = QColor(255,0,255);
  c5_qt = QColor(0,255,255);

  c6_qt = QColor(255,127,127);
  c7_qt = QColor(127,255,127);
  c8_qt = QColor(127,127,255);


  c9_qt = QColor(127,127,127);
  c10_qt = QColor(255,127,0);
  c11_qt = QColor(255,0,127);

  colores.push_back(c0_qt);
  colores.push_back(c1_qt);
  colores.push_back(c2_qt);


  colores.push_back(c3_qt);
  colores.push_back(c4_qt);
  colores.push_back(c5_qt);

  colores.push_back(c6_qt);
  colores.push_back(c7_qt);
  colores.push_back(c8_qt);

  colores.push_back(c9_qt);
  colores.push_back(c10_qt);
  colores.push_back(c11_qt);


  int filas = regions.size();
  int columnas = regions[0].size();
  QImage painted = QImage(QSize(columnas,filas),QImage::Format_RGB888);
  
  int n = int(colores.size());

  for(int f = 0; f < filas; ++f){
    for(int c = 0; c < columnas; ++c){
      int r = regions[f][c];
      int color_index = r%n; // #color = #region MOD num_colors. 
      painted.setPixelColor(c,f,colores[color_index]);
      }
    }
  return painted;
}




bool Algoritmos::check_bounds(int filas, int columnas, int f, int c) {
  return (f >= 0 and c >= 0 and f < filas and c < columnas);
}


bool Algoritmos::check_white_pixel(const uchar* bit,int f,int c, int columnas) {
  int dest_index = 4*f*columnas + 4*c;
  int r,g,b;
  r = int(bit[dest_index+0]);
  g = int(bit[dest_index+1]);
  b = int(bit[dest_index+2]);
  return (r == 255 and g == 255 and b == 255);
}

void Algoritmos::bfs_4(const QImage& img_binaria, vector<vector<int>>& regions, vector<vector<bool>>& visited, int current_tag, pair<int,int> pos_inicial, map<int,int>& regions_area) {
  const uchar *bit = img_binaria.bits();
  int filas = img_binaria.height();
  int columnas = img_binaria.width();
  queue<pair<int,int>> myqueue;
  myqueue.push(pos_inicial);
  int area_counter = 1;
  while (!myqueue.empty()) {
    pair<int,int> new_pos;
    pair<int,int> pos = myqueue.front();
    myqueue.pop();
    int f,c;
    f = pos.first;
    c = pos.second;
    visited[f][c] = true;
    regions[f][c] = current_tag;
    ++area_counter;
    //up
    if (check_bounds(filas,columnas,f-1,c)) {
      if (check_white_pixel(bit,f-1,c,columnas) and !visited[f-1][c]){
        new_pos = make_pair(f-1,c);
        myqueue.push(new_pos);
        visited[f-1][c] = true;
      }
    }
    //down
    if (check_bounds(filas,columnas,f+1,c)) {
      if (check_white_pixel(bit,f+1,c,columnas) and !visited[f+1][c]) {
        new_pos = make_pair(f+1,c);
        myqueue.push(new_pos);
        visited[f+1][c] = true;
      }
    }
    //left
    if (check_bounds(filas,columnas,f,c-1)) {
      if (check_white_pixel(bit,f,c-1,columnas) and !visited[f][c-1]) {
        new_pos = make_pair(f,c-1);
        myqueue.push(new_pos);
        visited[f][c-1] = true;
      }
    }
    //right
    if (check_bounds(filas,columnas,f,c+1)) {
      if (check_white_pixel(bit,f,c+1,columnas) and !visited[f][c+1]) {
        new_pos = make_pair(f,c+1);
        myqueue.push(new_pos);
        visited[f][c+1] = true;
      }
    }
  }
  regions_area[current_tag] = area_counter;
}
