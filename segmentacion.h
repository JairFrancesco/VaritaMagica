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

inline rgb random_rgb(){
  rgb c;

  c.r = (uchar)(255.0 * (rand() / (RAND_MAX + 1.0)));
  c.g = (uchar)(255.0 * (rand() / (RAND_MAX + 1.0)));
  c.b = (uchar)(255.0 * (rand() / (RAND_MAX + 1.0)));
}

// threshold function
#define THRESHOLD(size, c) (c/size)

typedef struct {
  float w;
  int a, b;
} edge;

inline bool operator<(const edge &a, const edge &b) {
  return a.w < b.w;
}


class Segmentacion
{
    public:
        universe *segmentarGrafo(int num_vertices, int num_edges, edge *edges, float c) {
          // sort edges by weight
          std::sort(edges, edges + num_edges);

          // make a disjoint-set forest
          universe *u = new universe(num_vertices);

          // init thresholds
          float *threshold = new float[num_vertices];
          for (int i = 0; i < num_vertices; i++)
            threshold[i] = THRESHOLD(1,c);

          // for each edge, in non-decreasing weight order...
          for (int i = 0; i < num_edges; i++) {
            edge *pedge = &edges[i];

            // components conected by this edge
            int a = u->find(pedge->a);
            int b = u->find(pedge->b);
            if (a != b) {
              if ((pedge->w <= threshold[a]) &&
              (pedge->w <= threshold[b])) {
            u->join(a, b);
            a = u->find(a);
            threshold[a] = pedge->w + THRESHOLD(u->size(a), c);
              }
            }
          }

          // free up
          delete threshold;
          return u;
        }


        image<rgb> *segmentarImagen(image<rgb> *im, float sigma, float c, int min_size, int xCord, int yCord) {
          int width = im->width();
          int height = im->height();

          image<float> *r = new image<float>(width, height);
          image<float> *g = new image<float>(width, height);
          image<float> *b = new image<float>(width, height);

          // smooth each color channel
          for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
              imRef(r, x, y) = imRef(im, x, y).r;
              imRef(g, x, y) = imRef(im, x, y).g;
              imRef(b, x, y) = imRef(im, x, y).b;
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

          // build graph
          edge *edges = new edge[width*height*4];
          int num = 0;
          for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
              if (x < width-1) {
            edges[num].a = y * width + x;
            edges[num].b = y * width + (x+1);
            edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y);
            num++;
              }

              if (y < height-1) {
            edges[num].a = y * width + x;
            edges[num].b = (y+1) * width + x;
            edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x, y+1);
            num++;
              }

              if ((x < width-1) && (y < height-1)) {
            edges[num].a = y * width + x;
            edges[num].b = (y+1) * width + (x+1);
            edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y+1);
            num++;
              }

              if ((x < width-1) && (y > 0)) {
            edges[num].a = y * width + x;
            edges[num].b = (y-1) * width + (x+1);
            edges[num].w = diff(smooth_r, smooth_g, smooth_b, x, y, x+1, y-1);
            num++;
              }
            }
          }
          delete smooth_r;
          delete smooth_g;
          delete smooth_b;

          // segment
          universe *u = this->segmentarGrafo(width*height, num, edges, c);

          std::cout<<"NUM:"<<num<<std::endl;
          if (min_size > 0) {
              for (int i = 0; i < num; i++) {
                int a = u->find(edges[i].a);
                int b = u->find(edges[i].b);
                if ((a != b) && ((u->size(a) < min_size) || (u->size(b) < min_size)))
                  u->join(a, b);
              }
          }
          delete [] edges;

          std::cout<<"NUM_SETS:"<<u->num_sets()<<std::endl;

          image<rgb> *output = new image<rgb>(width, height);

          //return output;

          std::cout<<"Ancho:"<<width<<"Altura"<<height<<std::endl;

          std::cout<<"Width*height:"<<width*height<<std::endl;
          // pick random colors for each component
          rgb *colors = new rgb[width*height];
          for (int i = 0; i < width*height; i++)
              colors[i] = random_rgb();

          int tmpComp = u->find(yCord * width + xCord);

          for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int comp = u->find(y * width + x);
                if (tmpComp == comp)
                {
                    imRef(output, x, y) = colors[comp];
                }
                else
                {
                    imRef(output, x, y) = imRef(im, x, y);
                }
            }
          }

          delete [] colors;
          delete u;

          return output;
        }

};

#endif // SEGMENTACION_H
