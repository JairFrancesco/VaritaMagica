#ifndef FELZENSZWALB_H
#define FELZENSZWALB_H

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <climits>

template <class T>
class image {
 public:
  /* create an image */
  image(const int width, const int height, const bool init = true);

  /* delete an image */
  ~image();

  /* init an image */
  void init(const T &val);

  /* copy an image */
  image<T> *copy() const;

  /* get the width of an image. */
  int width() const { return w; }

  /* get the height of an image. */
  int height() const { return h; }

  /* image data. */
  T *data;

  /* row pointers. */
  T **access;

 private:
  int w, h;
};

/* use imRef to access image data. */
#define imRef(im, x, y) (im->access[y][x])

/* use imPtr to get pointer to image data. */
#define imPtr(im, x, y) &(im->access[y][x])

template <class T>
image<T>::image(const int width, const int height, const bool init) {
  w = width;
  h = height;
  data = new T[w * h];  // allocate space for image data
  access = new T*[h];   // allocate space for row pointers

  // initialize row pointers
  for (int i = 0; i < h; i++)
    access[i] = data + (i * w);

  if (init)
    memset(data, 0, w * h * sizeof(T));
}

template <class T>
image<T>::~image() {
  delete [] data;
  delete [] access;
}

template <class T>
void image<T>::init(const T &val) {
  T *ptr = imPtr(this, 0, 0);
  T *end = imPtr(this, w-1, h-1);
  while (ptr <= end)
    *ptr++ = val;
}


template <class T>
image<T> *image<T>::copy() const {
  image<T> *im = new image<T>(w, h, false);
  memcpy(im->data, data, w * h * sizeof(T));
  return im;
}

/* convolve src with mask.  dst is flipped! */
static void convolve_even(image<float> *src, image<float> *dst,
              std::vector<float> &mask) {
  int width = src->width();
  int height = src->height();
  int len = mask.size();

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      float sum = mask[0] * imRef(src, x, y);
      for (int i = 1; i < len; i++) {
    sum += mask[i] *
      (imRef(src, std::max(x-i,0), y) +
       imRef(src, std::min(x+i, width-1), y));
      }
      imRef(dst, y, x) = sum;
    }
  }
}

// disjoint-set forests using union-by-rank and path compression (sort of).

typedef struct {
  int rank;
  int p;
  int size;
} uni_elt;

class universe {
public:
  universe(int elements){
      elts = new uni_elt[elements];
      num = elements;
      for (int i = 0; i < elements; i++) {
        elts[i].rank = 0;
        elts[i].size = 1;
        elts[i].p = i;
      }
  }
  ~universe()
  {
      delete [] elts;
  };
  int find(int x)
  {
      int y = x;
      while (y != elts[y].p)
        y = elts[y].p;
      elts[x].p = y;
      return y;
  };
  void join(int x, int y)
  {
      if (elts[x].rank > elts[y].rank) {
        elts[y].p = x;
        elts[x].size += elts[y].size;
      } else {
        elts[x].p = y;
        elts[y].size += elts[x].size;
        if (elts[x].rank == elts[y].rank)
          elts[y].rank++;
      }
      num--;
  }
  int size(int x) const { return elts[x].size; }
  int num_sets() const { return num; }

private:
  uni_elt *elts;
  int num;
};



#ifndef M_PI
#define M_PI 3.141592653589793
#endif

typedef unsigned char uchar;

typedef struct { uchar r, g, b; } rgb;

inline bool operator==(const rgb &a, const rgb &b) {
  return ((a.r == b.r) && (a.g == b.g) && (a.b == b.b));
}

template <class T>
inline T abs(const T &x) { return (x > 0 ? x : -x); };

template <class T>
inline int sign(const T &x) { return (x >= 0 ? 1 : -1); };

template <class T>
inline T square(const T &x) { return x*x; };

template <class T>
inline T bound(const T &x, const T &min, const T &max) {
  return (x < min ? min : (x > max ? max : x));
}

template <class T>
inline bool check_bound(const T &x, const T&min, const T &max) {
  return ((x < min) || (x > max));
}

inline int vlib_round(float x) { return (int)(x + 0.5F); }

inline int vlib_round(double x) { return (int)(x + 0.5); }

inline double gaussian(double val, double sigma) {
  return exp(-square(val/sigma)/2)/(sqrt(2*M_PI)*sigma);
}

/*
 * Image utilites
 *
 *
 */

/* compute minimum and maximum value in an image */
template <class T>
void min_max(image<T> *im, T *ret_min, T *ret_max) {
  int width = im->width();
  int height = im->height();

  T min = imRef(im, 0, 0);
  T max = imRef(im, 0, 0);
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      T val = imRef(im, x, y);
      if (min > val)
    min = val;
      if (max < val)
    max = val;
    }
  }

  *ret_min = min;
  *ret_max = max;
}

/* threshold image */
template <class T>
image<uchar> *threshold(image<T> *src, int t) {
  int width = src->width();
  int height = src->height();
  image<uchar> *dst = new image<uchar>(width, height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(dst, x, y) = (imRef(src, x, y) >= t);
    }
  }

  return dst;
}

/*
 *
 * imconv
 *
 *
 *
 */

#define	RED_WEIGHT	0.299
#define GREEN_WEIGHT	0.587
#define BLUE_WEIGHT	0.114

static image<float> *imageUCHARtoFLOAT(image<uchar> *input) {
  int width = input->width();
  int height = input->height();
  image<float> *output = new image<float>(width, height, false);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      imRef(output, x, y) = imRef(input, x, y);
    }
  }
  return output;
}


/*
 *
 *
 * FILTROS
 *
 *
 */

#define WIDTH 4.0

/* normalize mask so it integrates to one */
static void normalize(std::vector<float> &mask) {
  int len = mask.size();
  float sum = 0;
  for (int i = 1; i < len; i++) {
    sum += fabs(mask[i]);
  }
  sum = 2*sum + fabs(mask[0]);
  for (int i = 0; i < len; i++) {
    mask[i] /= sum;
  }
}

/* make filters */
#define MAKE_FILTER(name, fun)                                \
static std::vector<float> make_ ## name (float sigma) {       \
  int len = (int)ceil(sigma * WIDTH) + 1;                     \
  std::vector<float> mask(len);                               \
  for (int i = 0; i < len; i++) {                             \
    mask[i] = fun;                                            \
  }                                                           \
  return mask;                                                \
}

MAKE_FILTER(fgauss, exp(-0.5*square(i/sigma)))

/* convolve image with gaussian filter */
inline image<float>* smooth(image<float> *src, float sigma) {
  std::vector<float> mask = make_fgauss(sigma);
  normalize(mask);
  image<float> *tmp = new image<float>(src->height(), src->width(), false);
  image<float> *dst = new image<float>(src->width(), src->height(), false);
  convolve_even(src, tmp, mask);
  convolve_even(tmp, dst, mask);
  delete tmp;
  return dst;
}

image<float> *smooth(image<uchar> *src, float sigma) {
  image<float> *tmp = imageUCHARtoFLOAT(src);
  image<float> *dst = smooth(tmp, sigma);
  delete tmp;
  return dst;
}



#endif // FELZENSZWALB_H
