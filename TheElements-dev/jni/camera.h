#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED


struct hsl{
  char h;
  char s;
  char l;
};


void preCalculateHSL();
void calculateClosestElement();
hsl rgbToHsl(char r, char g, char b);
void setGameToImage(int* pixels, int offsetx, int offsety, int w, int h);

#endif
