#ifndef SEGMENTACION_H
#define SEGMENTACION_H

#include <cstdlib>
#include <iostream>
#include <algorithm>

//Medida de similitud entre pixeles
static inline float diff(image<float> *r, image<float> *g, image<float> *b,
             int x1, int y1, int x2, int y2) {
  return sqrt(square(imRef(r, x1, y1)-imRef(r, x2, y2)) +
          square(imRef(g, x1, y1)-imRef(g, x2, y2)) +
          square(imRef(b, x1, y1)-imRef(b, x2, y2)));
}

inline rgb colorAleatorio(){
  rgb c;
  c.r = (uchar)(255.0 * (rand() / (RAND_MAX + 1.0)));
  c.g = (uchar)(255.0 * (rand() / (RAND_MAX + 1.0)));
  c.b = (uchar)(255.0 * (rand() / (RAND_MAX + 1.0)));
  return c;
}

// threshold function
#define THRESHOLD(size, c) (c/size)

class Arista
{
    public:
        float peso;
        int origen, destino;
};


inline bool operator<(const Arista &a, const Arista &b) {
  return a.peso < b.peso;
}


class Segmentacion
{
    public:
        universe *segmentarGrafo(int numVertices, int numAristas, Arista *aristas, float constanteThreshold) {
          std::sort(aristas, aristas + numAristas); //Ordenar las aristas
          universe *u = new universe(numVertices);
          float *threshold = new float[numVertices];
          for (int i = 0; i < numVertices; i++)
            threshold[i] = THRESHOLD(1,constanteThreshold);
          for (int i = 0; i < numAristas; i++) { //Para cada arista en orden no decreciente
            Arista * pArista = &aristas[i];
            // componentes conectados por esta arista
            int origen = u->find(pArista->origen);
            int destino = u->find(pArista->destino);
            if (origen != destino) {
              if ((pArista->peso <= threshold[origen]) && (pArista->peso <= threshold[destino])) {
                    u->join(origen, destino);
                    origen = u->find(origen);
                    threshold[origen] = pArista->peso + THRESHOLD(u->size(origen), constanteThreshold);
              }
            }
          }
          delete threshold;
          return u;
        }

        image<rgb> *segmentarImagen(image<rgb> *imagen, float sigma, float constanteThreshold, int min_size, int xCord, int yCord) {
          int ancho = imagen->width();
          int altura = imagen->height();
          image<float> *r = new image<float>(ancho, altura);
          image<float> *g = new image<float>(ancho, altura);
          image<float> *b = new image<float>(ancho, altura);
          for (int y = 0; y < altura; y++) { //Suavizar cada canal de color
            for (int x = 0; x < ancho; x++) {
              imRef(r, x, y) = imRef(imagen, x, y).r;
              imRef(g, x, y) = imRef(imagen, x, y).g;
              imRef(b, x, y) = imRef(imagen, x, y).b;
            }
          }
          image<float> *smooth_r;
          image<float> *smooth_g;
          image<float> *smooth_b;
          if (sigma > 0) {
              smooth_r = smooth(r, sigma);
              smooth_g = smooth(g, sigma);
              smooth_b = smooth(b, sigma);
              delete r;
              delete g;
              delete b;
          } else {
              smooth_r = r;
              smooth_g = g;
              smooth_b = b;
          }

          Arista *aristas = new Arista[ancho*altura*4]; // Construir grafo
          int num = 0;
          for (int y = 0; y < altura; y++) {
            for (int x = 0; x < ancho; x++) {
              if (x < ancho-1) {
                aristas[num].origen = y * ancho + x;
                aristas[num].destino = y * ancho + (x+1);
                aristas[num].peso = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
                num++;
              }

              if (y < altura-1) {
                aristas[num].origen = y * ancho + x;
                aristas[num].destino = (y+1) * ancho + x;
                aristas[num].peso = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
                num++;
              }

              if ((x < ancho-1) && (y < altura-1)) {
                aristas[num].origen = y * ancho + x;
                aristas[num].destino = (y+1) * ancho + (x+1);
                aristas[num].peso = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
                num++;
              }

              if ((x < ancho-1) && (y > 0)) {
                aristas[num].origen = y * ancho + x;
                aristas[num].destino = (y-1) * ancho + (x+1);
                aristas[num].peso = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
                num++;
              }
            }
          }
          delete smooth_r;
          delete smooth_g;
          delete smooth_b;

          universe *u = this->segmentarGrafo(ancho*altura, num, aristas, constanteThreshold);
          if (min_size > 0) {
              for (int i = 0; i < num; i++) {
                int a = u->find(aristas[i].origen);
                int b = u->find(aristas[i].destino);
                if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
                  u->join(a, b);
              }
          }
          delete [] aristas;

          image<rgb> *resultado = new image<rgb>(ancho, altura);
          rgb *colors = new rgb[ancho*altura];
          for (int i = 0; i < ancho*altura; i++)
              colors[i] = colorAleatorio(); //Elegir colores aleatorios para cada componente
          int tmpComp = u->find(yCord * ancho + xCord);

          for (int y = 0; y < altura; y++) {
            for (int x = 0; x < ancho; x++) {
                int comp = u->find(y * ancho + x);
                if (tmpComp == comp)
                {
                    imRef(resultado, x, y) = colors[comp];
                }
                else
                {
                    imRef(resultado, x, y) = imRef(imagen, x, y);
                }
            }
          }

          delete [] colors;
          delete u;
          return resultado;
        }

};

#endif // SEGMENTACION_H
