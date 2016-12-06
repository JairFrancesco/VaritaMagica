#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <paper.h>
#include <segmentacion.h>

class Utilidades
{
public:
    Utilidades(){}
    static image <rgb> * qImageToImage(QImage image);
    static QImage imageToQImage(image<rgb> * im);
    static image<rgb> *segmentarImagen(image<rgb> *im, float sigma, float c, int min_size, int xCoord, int yCoord);
};

image <rgb> * Utilidades::qImageToImage(QImage im) {
    image<rgb> *input = new image<rgb>(im.width(), im.height(), true);

    for (int x = 0; x < im.width(); x++) {
        for (int y = 0; y < im.height(); y++) {
            rgb pix;
            QRgb pixF = im.pixel(x, y);
            pix.r = qRed(pixF);
            pix.g = qGreen(pixF);
            pix.b = qBlue(pixF);
            input->data[y * im.width() + x] = pix;
        }
    }

    return input;
}

QImage Utilidades::imageToQImage(image<rgb> * im) {
    QImage res(im->width(), im->height(), QImage::Format_RGB32);

    for (int x = 0; x < im->width(); x++) {
        for (int y = 0; y < im->height(); y++) {
            rgb pix = im->data[y * im->width() + x];
            res.setPixel(x, y, qRgb(pix.r, pix.g, pix.b));
        }
    }
    return res;
}

image<rgb> *Utilidades::segmentarImagen(image<rgb> *im, float sigma, float c, int min_size, int xCoord, int yCoord) {
    //return im;//
    Segmentacion* segment = new Segmentacion();
    return segment->segmentarImagen(im, sigma, c, min_size, xCoord, yCoord);
}


#endif // UTILIDADES_H
