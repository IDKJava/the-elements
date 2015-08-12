#include "gravity.h"

#include "app.h"
#include <cmath>

void updateGravityField(SpaceHole *hole) {
    int gfX = hole->x/GF_BLOCK_SIZE;
    int gfY = hole->y/GF_BLOCK_SIZE;
    // TODO: Thread this compute-heavy step better?
    for (int i = 0; i < gfHeight; ++i) {
        for (int j = 0; j < gfWidth; ++j) {
            int gfInd = i*gfWidth + j;
            // Vec from gf coord to black hole
            float dx = (j-gfX);
            float dy = (i-gfY);
            float dist = sqrt(dx*dx + dy*dy);
            // Normalize
            dx /= dist;
            dy /= dist;
            const float str = 10.0;
            // Gravity field weakens with distance squared
            if (hole->type == BLACK_HOLE) {
                gravityFieldX[gfInd] += -dx*str/(dist*dist);
                gravityFieldY[gfInd] += -dy*str/(dist*dist);
            }
            else if (hole->type == WHITE_HOLE) {
                gravityFieldX[gfInd] += dx*str/(dist*dist);
                gravityFieldY[gfInd] += dy*str/(dist*dist);
            }
            else if (hole->type == CURL_HOLE) {
                gravityFieldX[gfInd] += -dy*str/(dist*dist);
                gravityFieldY[gfInd] += dx*str/(dist*dist);
            }
            gravCoords[4*gfInd+2] = gravCoords[4*gfInd]+gravityFieldX[gfInd];
            // Invert Y coordinate because OpenGL y is the opposite of game y
            gravCoords[4*gfInd+3] = gravCoords[4*gfInd+1]-gravityFieldY[gfInd];
        }
    }
}

void getFallField(float x, float y, float *fx, float *fy, float *fmag) {
    if (world == WORLD_EARTH) {
        if (accelOn) {
            float norm = sqrt(xGravity*xGravity + yGravity*yGravity);
            *fx = xGravity/norm;
            *fy = yGravity/norm;
            if (fmag != NULL) *fmag = norm;
        }
        else {
            *fx = 0.0;
            *fy = 1.0;
            if (fmag != NULL) *fmag = 1.0;
        }
    }
    else if (world == WORLD_SPACE) {
        int gfInd = getGravityIndex((int)x, (int)y);
        float gx = gravityFieldX[gfInd];
        float gy = gravityFieldY[gfInd];
        float norm = sqrt(gx*gx+gy*gy);
        if (norm > 0.0) {
            *fx = gx/norm;
            *fy = gy/norm;
            if (fmag != NULL) *fmag = 1.0;
        }
        else {
            *fx = 0.0;
            *fy = 0.0;
            if (fmag != NULL) *fmag = 0.0;
        }
    }
    else {
        // Just assume down
        *fx = 0.0;
        *fy = 1.0;
        if (fmag != NULL) *fmag = 1.0;
    }
}