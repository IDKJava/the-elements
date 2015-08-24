#include "camera.h"

#include "app.h"
#include "points.h"
#include "setup.h"

#include <cmath>
#include <algorithm>


#define SIZE 100
#define H_WEIGHT 12
#define L_WEIGHT 2
#define S_WEIGHT 1

hsl elementHSL[NUM_BASE_ELEMENTS];
int closestElementForHL[SIZE][SIZE][SIZE];


#ifndef NDEBUG
//Debug
#define LOGGING 1
#else
//Release
#define LOGGING 0
#endif



void preCalculateHSL() {
  for (int i = 0; i < NUM_BASE_ELEMENTS; i++) {
    Element* curElement = elements[i];
    elementHSL[i] = rgbToHsl(curElement->red, curElement->green, curElement->blue);
  }
}

void calculateClosestElement() {
  for (int h = 0; h < SIZE; h++) {
    for (int l = 0; l < SIZE; l++) {
      for ( int s = 0; s < SIZE; s++) {
        int lowestDiff = 1 << 30;
        int lowestElement = -1;
        for (int i = 0; i < NUM_BASE_ELEMENTS; i++) {
          hsl curEleHsl = elementHSL[i];
          int diffH = abs(curEleHsl.h - h);
          int diffS = abs(curEleHsl.s - s);
          int diffL = abs(curEleHsl.l - l);
          int curDiff = diffH*H_WEIGHT + (L_WEIGHT*diffL) + (S_WEIGHT*diffS);
          if (curDiff < lowestDiff) {
            lowestDiff = curDiff;
            lowestElement = i;
          }
        }
        closestElementForHL[h][s][l] = lowestElement;
      }
    }
  }
}


double threeway_max(double a, double b, double c) {
  return std::max(a, std::max(b, c));
}

double threeway_min(double a, double b, double c) {
  return std::min(a, std::min(b, c));
}

//I took some of this code from the internet
hsl rgbToHsl(char r, char g, char b) {
  double rd = (double) r/255;
  double gd = (double) g/255;
  double bd = (double) b/255;
  double max = threeway_max(rd, gd, bd);
  double min = threeway_min(rd, gd, bd);
  double h, s, l = (max + min) / 2;

  if (max == min) {
    h = s = 0; // achromatic
  } else {
    double d = max - min;
    s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
    if (max == rd) {
      h = (gd - bd) / d + (gd < bd ? 6 : 0);
    } else if (max == gd) {
      h = (bd - rd) / d + 2;
    } else if (max == bd) {
      h = (rd - gd) / d + 4;
    }
    h /= 6;
  }
  hsl out;
  out.h = (char)(SIZE*h);
  out.s = (char)(SIZE*s);
  out.l = (char)(SIZE*l);
  out.h = std::min((char)(SIZE-1), out.h);
  out.s = std::min((char)(SIZE-1), out.s);
  out.l = std::min((char)(SIZE-1), out.l);
  return out;
}

void setGameToImage(int* pixels, int w, int h) {
  pthread_mutex_lock(&update_mutex);
  gameSetup();
  for (int y = 0; y < h; y++) {
    for (int x = 0; x < w; x++) {
      int color = pixels[x + (y*w)];
      char a = (color >> 24) & 255;
      char r = (color >> 16) & 255;
      char g = (color >> 8) & 255;
      char b = color & 255;
      hsl pixelHsl = rgbToHsl(r,g,b);
      int element = closestElementForHL[pixelHsl.h][pixelHsl.s][pixelHsl.l];
      createPoint(x, y, elements[element]);
    }
  }
  pthread_mutex_unlock(&update_mutex);
}

