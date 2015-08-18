#include "gravity.h"

#include "app.h"
#include <cmath>

void updateGravityField(SpaceObj *obj, bool invert) {
    int gfX = obj->x/GF_BLOCK_SIZE;
    int gfY = obj->y/GF_BLOCK_SIZE;
    if (obj->type != NULL_GRAVITY) {
        // TODO: Thread this compute-heavy step better?
        for (int i = 0; i < gfHeight; ++i) {
            for (int j = 0; j < gfWidth; ++j) {
                int gfInd = i*gfWidth + j;
                // Vec from gf coord to hole
                float dx = (j-gfX);
                float dy = (i-gfY);
                float dist = sqrt(dx*dx + dy*dy);
                // Normalize
                dx /= dist;
                dy /= dist;
                const float str = 10.0;
                int mul = invert ? -1 : 1;
                // Gravity field weakens with distance squared
                if (obj->type == BLACK_HOLE) {
                    gravityFieldX[gfInd] += -mul*dx*str/(dist*dist);
                    gravityFieldY[gfInd] += -mul*dy*str/(dist*dist);
                }
                else if (obj->type == WHITE_HOLE) {
                    gravityFieldX[gfInd] += mul*dx*str/(dist*dist);
                    gravityFieldY[gfInd] += mul*dy*str/(dist*dist);
                }
                else if (obj->type == CURL_HOLE) {
                    gravityFieldX[gfInd] += -mul*dy*str/(dist*dist);
                    gravityFieldY[gfInd] += mul*dx*str/(dist*dist);
                }
                // Clear tiny residuals after a delete
                if (invert) {
                    if (fabs(gravityFieldX[gfInd]) < 0.0001) gravityFieldX[gfInd] = 0.0;
                    if (fabs(gravityFieldY[gfInd]) < 0.0001) gravityFieldY[gfInd] = 0.0;
                }
                // Update the display if not masked
                if (!gravityMask[gfInd]) {
                    float gx = gravityFieldX[gfInd], gy = gravityFieldY[gfInd];
                    float mag = sqrt(gx*gx+gy*gy);
                    gravCoords[4*gfInd+2] = gravCoords[4*gfInd] +
                        gravityFieldX[gfInd]/(mag*gfWidth);
                    // Invert Y coordinate because OpenGL y is the opposite of game y
                    gravCoords[4*gfInd+3] = gravCoords[4*gfInd+1] -
                        gravityFieldY[gfInd]/(mag*gfHeight);
                    gravMag[2*gfInd+0] = mag;
                    gravMag[2*gfInd+1] = mag;
                }
            }
        }
    }
    // Null gravity
    else {
        int gfEndX = obj->ex/GF_BLOCK_SIZE;
        int gfEndY = obj->ey/GF_BLOCK_SIZE;
        // Swap to ensure we iterate the correct way
        if (gfEndX < gfX) {
            int tmp = gfX;
            gfX = gfEndX;
            gfEndX = tmp;
        }
        if (gfEndY < gfY) {
            int tmp = gfY;
            gfY = gfEndY;
            gfEndY = tmp;
        }
        for (int i = gfY; i <= gfEndY; ++i) {
            for (int j = gfX; j <= gfEndX; ++j) {
                int gfInd = i*gfWidth+j;
                if (!invert) {
                    gravityMask[gfInd]++;
                    gravMag[2*gfInd+0] = 0.0;
                    gravMag[2*gfInd+1] = 0.0;
                    gravCoords[4*gfInd+2] = gravCoords[4*gfInd];
                    gravCoords[4*gfInd+3] = gravCoords[4*gfInd+1];
                }
                else {
                    gravityMask[gfInd]--;
                    // Update the display if unmasked
                    if (!gravityMask[gfInd]) {
                        float gx = gravityFieldX[gfInd], gy = gravityFieldY[gfInd];
                        float mag = sqrt(gx*gx+gy*gy);
                        gravCoords[4*gfInd+2] = gravCoords[4*gfInd] +
                            gravityFieldX[gfInd]/(mag*gfWidth);
                        // Invert Y coordinate because OpenGL y is the opposite of game y
                        gravCoords[4*gfInd+3] = gravCoords[4*gfInd+1] -
                            gravityFieldY[gfInd]/(mag*gfHeight);
                        gravMag[2*gfInd+0] = mag;
                        gravMag[2*gfInd+1] = mag;
                    }
                }
            }
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
        // Short-circuit on a masked index
        if (gravityMask[gfInd]) {
            *fx = 0.0;
            *fy = 0.0;
            if (fmag != NULL) *fmag = 0.0;
            return;
        }
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

bool makeBlackHole(int x, int y) {
    if (numSpaceObjs < MAX_SPACE_OBJS) {
        SpaceObj *next = &spaceObjs[numSpaceObjs];
        numSpaceObjs++;
        next->type = BLACK_HOLE;
        next->x = x - x%GF_BLOCK_SIZE;
        next->y = y - y%GF_BLOCK_SIZE;
        updateGravityField(next, false);
        return true;
    }
    return false;
}

bool makeWhiteHole(int x, int y) {
    if (numSpaceObjs < MAX_SPACE_OBJS) {
        SpaceObj *next = &spaceObjs[numSpaceObjs];
        numSpaceObjs++;
        next->type = WHITE_HOLE;
        next->x = x - x%GF_BLOCK_SIZE;
        next->y = y - y%GF_BLOCK_SIZE;
        updateGravityField(next, false);
        return true;
    }
    return false;
}

bool makeCurlHole(int x, int y) {
    if (numSpaceObjs < MAX_SPACE_OBJS) {
        SpaceObj *next = &spaceObjs[numSpaceObjs];
        numSpaceObjs++;
        next->type = CURL_HOLE;
        next->x = x - x%GF_BLOCK_SIZE;
        next->y = y - y%GF_BLOCK_SIZE;
        updateGravityField(next, false);
        return true;
    }
    return false;
}

bool makeNullGravity(int x, int y, int ex, int ey) {
    if (numSpaceObjs < MAX_SPACE_OBJS) {
        SpaceObj *next = &spaceObjs[numSpaceObjs];
        numSpaceObjs++;
        next->type = NULL_GRAVITY;
        next->x = x - x%GF_BLOCK_SIZE;
        next->y = y - y%GF_BLOCK_SIZE;
        next->ex = ex - ex%GF_BLOCK_SIZE;
        next->ey = ey - ey%GF_BLOCK_SIZE;
        updateGravityField(next, false);
        return true;
    }
    return false;
}