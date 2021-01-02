/*
 * specials.c
 * ------------------------------
 * Contains functions related to element specials
 */

#include "specials.h"
#include "macros.h"
#include "points.h"
#include "gravity.h"
#include "collide.h"
#include "portals.h"

#include <cmath>

#ifndef NDEBUG // Debug
#define LOGGING 1
#else // Release
#define LOGGING 0
#endif

// Called before a collision happens, and can override the collision
// Coming in, fp and sp have the same x and y coords. Allcoords hasn't been
// updated yet, so the location fp and sp both point to is actually sp still.
// If returning true, fp and sp must have consistent coords and allcoords.
// Set hasMoved for fp, if it's allcoords has changed.
bool collisionSpecials(int firstParticle, int secondParticle)
{
    // If the conduction specials match on the particles, override the collision
    if (hasSpecial(firstParticle, SPECIAL_CONDUCTABLE)
        && hasSpecial(secondParticle, SPECIAL_CONDUCTIVE))
    {
        //Delete first Particle
        a_x[firstParticle] = a_oldX[firstParticle];
        a_y[firstParticle] = a_oldY[firstParticle];
        deletePoint(firstParticle);
        a_hasMoved[firstParticle] = FALSE;

        //Electrify the second particle
        setParticleSpecialVal(secondParticle, SPECIAL_CONDUCTIVE, ELECTRIC_NO_DIR);

        return TRUE;
    } else if (specialPortal(firstParticle, secondParticle)) {
        return TRUE;
    }
    // Fluids flow around obstacles
    else if (a_element[firstParticle]->state > 0) {
        return specialFlow(firstParticle, secondParticle);
    }

    return FALSE;
}


void specialSpawn(int particle)
{
    int tempX = a_x[particle];
    int tempY = a_y[particle];
    int diffX, diffY;
    int tempAllCoords;
    for (diffX = -2; diffX <= 2; diffX++)
    {
        for (diffY = -2; diffY <= 2; diffY++)
        {
            if (coordInBounds(tempX+diffX, tempY+diffY))
            {
                tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                if (tempAllCoords != -1 &&
                        (a_element[tempAllCoords] == elements[GENERATOR_ELEMENT] ||
                         a_element[tempAllCoords]->base == GENERATOR_ELEMENT)) //There's a generator adjacent
                {
                    setElement(tempAllCoords, elements[SPAWN_ELEMENT]);
                    setParticleSpecialVal(tempAllCoords, SPECIAL_SPAWN, getParticleSpecialVal(particle, SPECIAL_SPAWN));
                }
                else if (tempAllCoords == -1 && rand() % GENERATOR_SPAWN_PROB == 0 && loq < MAX_POINTS - 1) //There's an empty spot
                {
                    int eltIndex = getParticleSpecialVal(particle, SPECIAL_SPAWN);
                    if (eltIndex < 0 || eltIndex >= numElements) {
                        LOGE("Invalid special spawn value: %d", eltIndex);
                        eltIndex = SAND_ELEMENT;
                    }
                    createPoint(tempX + diffX, tempY + diffY, elements[eltIndex]);
                }
            }
        }
    }
}

void specialBreak(int particle)
{
    if (abs(a_xVel[particle]) > getElementSpecialVal(a_element[particle], SPECIAL_BREAK)
        || abs(a_yVel[particle]) > getElementSpecialVal(a_element[particle], SPECIAL_BREAK))
    {
        setElement(particle, elements[NORMAL_ELEMENT]);
    }
}

void specialGrow(int particle)
{
    int tempX = a_x[particle];
    int tempY = a_y[particle];
    int diffX, diffY;
    int tempAllCoords;
    for (diffX = -1; diffX <= 1; diffX++)
    {
        for (diffY = -1; diffY <= 1; diffY++)
        {
            if (tempY + diffY >= 0 && tempY + diffY < workHeight && tempX + diffX >= 0 && tempX + diffX < workWidth)
            {
                tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                if (tempAllCoords != -1 && a_element[tempAllCoords]->index == getElementSpecialVal(a_element[particle], SPECIAL_GROW) && rand() % 10 == 0)
                {
                    setElement(tempAllCoords, a_element[particle]);
                }
            }
        }
    }
}

void specialHeat(int particle)
{
    struct Element* tempElement = a_element[particle];
    int tempX = a_x[particle];
    int tempY = a_y[particle];
    int diffX, diffY;
    int tempAllCoords;
    if (rand()%5 == 0)
    {
        for (diffX = -1; diffX <= 1; diffX++)
        {
            for(diffY = -1; diffY <=1; diffY++)
            {
                if((diffX!=0||diffY!=0) && coordInBounds(tempX+diffX, tempY+diffY))
                {
                    tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                    if(tempAllCoords != -1)
                    {
                        changeHeat(&(a_heat[tempAllCoords]), getElementSpecialVal(tempElement, SPECIAL_HEAT));
                    }
                }
            }
        }
    }
}

void specialExplode(int particle)
{
    struct Element* tempElement = a_element[particle];
    int tempX = a_x[particle];
    int tempY = a_y[particle];
    if (a_heat[particle] >= a_element[particle]->highestTemp) //If the heat is above the threshold
    {
        int diffX, diffY;
        int distance;
        int tempAllCoords;

        //In radius of explosion, add velocity with a 5% chance
        if(rand()%20 == 0)
        {
            int explosiveness = getElementSpecialVal(tempElement, SPECIAL_EXPLODE);
            for (diffX = -explosiveness; diffX <= explosiveness; diffX++)
            {
                for (diffY = -explosiveness; diffY <= explosiveness; diffY++)
                {
                    if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
                    {
                        tempAllCoords = allCoords[getIndex(tempX + diffX, tempY + diffY)];
                        if (tempAllCoords != -1)
                        {
                            if(diffX != 0 && a_xVel[tempAllCoords] < explosiveness)
                            {
                                a_xVel[tempAllCoords] += (2*(diffX > 0)-1);
                            }
                            if(diffY != 0 && a_yVel[tempAllCoords] < explosiveness)
                            {
                                a_yVel[tempAllCoords] += (2*(diffY > 0)-1);
                            }
                        }
                    }
                }
            }
        }

        // Add heat
        for(diffX = -2; diffX <= 2; diffX++)
        {
            for(diffY = -2; diffY <= 2; diffY++)
            {
                if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
                {
                    tempAllCoords = allCoords[getIndex(tempX + diffX, tempY + diffY)];
                    if(tempAllCoords != -1)
                    {
                        changeHeat(&(a_heat[tempAllCoords]), 50);
                    }
                }
            }
        }

        // Change this particle to fire, and quit the specials loop
        setElement(particle, elements[10]);
    }
}

void specialLife(int particle)
{
    struct Element* tempElement = a_element[particle];
    if (rand()%getElementSpecialVal(tempElement, SPECIAL_LIFE) == 0)
    {
        deletePoint(particle);
    }
}

void specialWander(int particle)
{
    struct Element* tempElement = a_element[particle];

    // Don't wander while tunneling
    // FIXME: This is a hacky solution, come up with something more elegant
    if (getParticleSpecialVal(particle, SPECIAL_TUNNEL) != SPECIAL_VAL_UNSET)
    {
        return;
    }

    // TODO(gkanwar): Track what direction we're moving in and try to
    // wander in that direction and neighboring directions first with high
    // probability.

    int randVal = rand()%100;
    // Randomly wander
    int wanderVal = getElementSpecialVal(tempElement, SPECIAL_WANDER);
    if (randVal <= wanderVal)
    {
        float curX = a_x[particle];
        float curY = a_y[particle];
        // Random direction starting at bottom left, going clockwise.
        // Straight up or down are ignored as possible wander directions.
        int randDir = rand()%6;
        float diffX = randDir < 3 ? -1.0 : 1.0;
        float diffY = randDir%3 - 1.0;
        // Desired point and below
        if (coordInBounds(curX+diffX, curY+diffY) &&
            allCoords[getIndex(curX+diffX, curY+diffY)] == -1)
        {
            // Good! Let's move
            allCoords[getIndex(curX+diffX, curY+diffY)] = particle;
            setBitmapColor(curX+diffX, curY+diffY, tempElement);
            allCoords[getIndex(curX, curY)] = -1;
            clearBitmapColor(curX, curY);
            a_x[particle] = curX+diffX;
            a_y[particle] = curY+diffY;
        }
    }
}

bool specialFlow(int particle, int sp) {
    struct Element* tempElement = a_element[particle];
    int thisDensity = tempElement->density;
    float gx, gy;
    float x = a_x[particle], y = a_y[particle];
    float oldX = a_oldX[particle], oldY = a_oldY[particle];
    int oldInd = getIndex((int)oldX, (int)oldY);
    getFallField(x, y, &gx, &gy, NULL);
    int curInd = getIndex(x, y);
    float ang = atan2(gy, gx);
    // Invert fall angle if fallVel is negative
    if (tempElement->fallVel < 0) {
        ang *= -1;
    }
    // Randomize direction check order
    int dir = (rand()%2 == 0) ? -1 : 1;
    float leftAng = ang+dir*M_PI/2.0, rightAng = ang-dir*M_PI/2.0,
          leftUpAng = ang+dir*M_PI*3.0/4.0, rightUpAng = ang-dir*M_PI*3.0/4.0;
    float leftX = x+sqrt(2.0)*cos(leftAng), leftY = y+sqrt(2.0)*sin(leftAng);
    int leftInd = getIndex(leftX, leftY);
    int leftPart = allCoords[leftInd];
    float rightX = x+sqrt(2.0)*cos(rightAng), rightY = y+sqrt(2.0)*sin(rightAng);
    int rightInd = getIndex(rightX, rightY);
    int rightPart = allCoords[rightInd];
    float leftUpX = x+sqrt(2.0)*cos(leftUpAng), leftUpY = y+sqrt(2.0)*sin(leftUpAng);
    int leftUpInd = getIndex(leftUpX, leftUpY);
    int leftUpPart = allCoords[leftUpInd];
    float rightUpX = x+sqrt(2.0)*cos(rightUpAng), rightUpY = y+sqrt(2.0)*sin(rightUpAng);
    int rightUpInd = getIndex(rightUpX, rightUpY);
    int rightUpPart = allCoords[rightUpInd];

    // If movement is possible, we do the allcoords update ourselves, because
    // this extends outside the simple 2-particle collision
    if (coordInBounds(leftX, leftY) &&
        (leftPart == -1 || a_element[leftPart]->density < thisDensity)) {
        a_x[particle] = leftX;
        a_y[particle] = leftY;
        if (leftPart == -1) {
            a_hasMoved[particle] = true;
        }
        else {
            // Swap the two particles
            a_hasMoved[particle] = false;
            a_x[leftPart] = oldX;
            a_y[leftPart] = oldY;
            allCoords[oldInd] = leftPart;
            setBitmapColor(oldX, oldY, a_element[leftPart]);
            allCoords[leftInd] = particle;
            setBitmapColor(leftX, leftY, tempElement);
        }
        return true;
    }
    else if (coordInBounds(rightX, rightY) &&
        (rightPart == -1 || a_element[rightPart]->density < thisDensity)) {
        a_x[particle] = rightX;
        a_y[particle] = rightY;
        if (rightPart == -1) {
            a_hasMoved[particle] = true;
        }
        else {
            // Swap the two particles
            a_hasMoved[particle] = false;
            a_x[rightPart] = oldX;
            a_y[rightPart] = oldY;
            allCoords[oldInd] = rightPart;
            setBitmapColor(oldX, oldY, a_element[rightPart]);
            allCoords[rightInd] = particle;
            setBitmapColor(rightX, rightY, tempElement);
        }
        return true;
    }
    else if (coordInBounds(leftUpX, leftUpY) && leftUpPart != sp &&
        (leftUpPart == -1 || a_element[leftUpPart]->density < thisDensity)) {
        a_x[particle] = leftUpX;
        a_y[particle] = leftUpY;
        if (leftUpPart == -1) {
            a_hasMoved[particle] = true;
        }
        else {
            // Swap the two particles
            a_hasMoved[particle] = false;
            a_x[leftUpPart] = oldX;
            a_y[leftUpPart] = oldY;
            allCoords[oldInd] = leftUpPart;
            setBitmapColor(oldX, oldY, a_element[leftUpPart]);
            allCoords[leftUpInd] = particle;
            setBitmapColor(leftUpX, leftUpY, tempElement);
        }
        return true;
    }
    else if (coordInBounds(rightUpX, rightUpY) &&
        (rightUpPart == -1 || a_element[rightUpPart]->density < thisDensity)) {
        a_x[particle] = rightUpX;
        a_y[particle] = rightUpY;
        if (rightUpPart == -1) {
            a_hasMoved[particle] = true;
        }
        else {
            // Swap the two particles
            a_hasMoved[particle] = false;
            a_x[rightUpPart] = oldX;
            a_y[rightUpPart] = oldY;
            allCoords[oldInd] = rightUpPart;
            setBitmapColor(oldX, oldY, a_element[rightUpPart]);
            allCoords[rightUpInd] = particle;
            setBitmapColor(rightUpX, rightUpY, tempElement);
        }
        return true;
    }
    return false;
}

void specialJump(int particle)
{
    struct Element* tempElement = a_element[particle];
    if ((a_y[particle]+1 == workHeight) || (allCoords[getIndex(a_x[particle], a_y[particle]+1)] != -1))
    {
        int randVal = rand()%100;
        int jumpVal = getElementSpecialVal(tempElement, SPECIAL_JUMP);
        if (randVal < jumpVal)
        {
            a_yVel[particle] -= 5;
        }
    }
}

// 0 = left,
// 1 = up,
// 2 = right,
// 3 = down
void getCardinalDirection(int index, int* x, int* y)
{
    switch(index)
    {
    case 0:
        *x = 1;
        *y = 0;
        break;
    case 1:
        *x = 0;
        *y = -1;
        break;
    case 2:
        *x = -1;
        *y = 0;
        break;
    case 3:
        *x = 0;
        *y = 1;
        break;
    default:
        LOGE("Invalid cardinal direction index: %d", index);
        // Arbitrarily choose left.
        *x = 1;
        *y = 0;
        break;
    }
}

// 0 = bottom left,
// 1 = bottom right,
// 2 = top left,
// 3 = top right
void getDiagonalDirection(int index, int* x, int* y)
{
    if (index < 0 || index > 3)
    {
        LOGE("Invalid diagonal index: %d", index);
        *x = 1;
        *y = 1;
    }
    else
    {
        *x = index % 2 == 0 ? -1 : 1;
        *y = index < 2 ? -1 : 1;
    }
}

// Create a tunnel assuming we're tunneling from curX, curY
// to curX+diffX, curY+diffY. Builds all particles around the
// new point except directly back the way we came.
void createTunnel(int curX, int curY,
                  int diffX, int diffY,
                  struct Element* tunnelElt)
{
    // Assuming curX, curY is in bounds, this check should properly
    // guard all points below.
    if (coordInBounds(curX+2*diffX, curY+2*diffY))
    {
        int dx, dy;
        for (int i = 0; i < 4; ++i)
        {
            getCardinalDirection(i, &dx, &dy);
            if (!(dx == -diffX && dy == -diffY) &&
                allCoords[getIndex(curX+diffX+dx, curY+diffY+dy)] == -1)
            {
                createPoint(curX+diffX+dx, curY+diffY+dy, tunnelElt);
            }

            getDiagonalDirection(i, &dx, &dy);
            // Check for not backwards and empty
            if (!(dx == -diffX && dy == -diffY) &&
                allCoords[getIndex(curX+diffX+dx, curY+diffY+dy)] == -1)
            {
                createPoint(curX+diffX+dx, curY+diffY+dy, tunnelElt);
            }
        }
    }
}

void specialTunnel(int particle)
{
    struct Element* tempElement = a_element[particle];
    int targetElementIndex = getElementSpecialVal(tempElement, SPECIAL_TUNNEL);
    int dir = getParticleSpecialVal(particle, SPECIAL_TUNNEL);

    int curX = a_x[particle], curY = a_y[particle];
    int diffX, diffY;

    if (dir == SPECIAL_VAL_UNSET || dir >= 4 || // If unset/invalid OR
        rand() % 100 == 0) // 1% chance to change direction while set
    {
        // Look in a random diagonal
        int randomDir = rand()%4;
        getDiagonalDirection(randomDir, &diffX, &diffY);
        if (coordInBounds(curX+diffX, curY+diffY) &&
            allCoords[getIndex(curX+diffX, curY+diffY)] != -1 &&
            a_element[allCoords[getIndex(curX+diffX, curY+diffY)]]->index == targetElementIndex)
        {
            dir = randomDir;
            setParticleSpecialVal(particle, SPECIAL_TUNNEL, randomDir);
        }
    }

    // If we still did not set dir or dir is invalid, just quit
    if (dir == SPECIAL_VAL_UNSET || dir >= 4) { return; }

    getDiagonalDirection(dir, &diffX, &diffY);
    //LOGI("Diagonal dir: %d -> %d %d", dir, diffX, diffY);
    // Check out of bounds
    if (!coordInBounds(curX+diffX, curY+diffY))
    {
        // Stop tunneling and quit, we'll try to find a new direction
        setParticleSpecialVal(particle, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);
        return;
    }

    int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
    if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
    {
        if (rand() % 3) // 1/3 chance to move along a tunnel
        {
            // Remove the tempAllCoords particle, and move this particle there
            unSetPoint(tempAllCoords);
            allCoords[getIndex(curX+diffX, curY+diffY)] = particle;
            setBitmapColor(curX+diffX, curY+diffY, tempElement);
            allCoords[getIndex(curX, curY)] = -1;
            clearBitmapColor(curX, curY);
            a_x[particle] = curX + diffX;
            a_y[particle] = curY + diffY;
        }

        // Set the y velocity to the fall velocity to counteract movement
        a_yVel[particle] = a_element[particle]->fallVel;

        // Make the tunnel particles
        createTunnel(curX, curY, diffX, diffY, elements[targetElementIndex]);
    }
    // If we're blocked, 1/3 chance to move horizontally/vertically
    else if (rand() % 3 == 0)
    {
        // Randomly choose start cardinal direction to look in
        int randomDir = rand() % 4;
        // Cycle through all 4 directions from that point and look for
        // a particle to burrow into
        for (int i = 0; i < 4; ++i)
        {
            getCardinalDirection((randomDir+i)%4, &diffX, &diffY);
            if (coordInBounds(curX+diffX, curY+diffY) &&
                (tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)]) != -1 &&
                a_element[tempAllCoords]->index == targetElementIndex)
            {
                // Remove the tempAllCoords particle, and move this particle there
                unSetPoint(tempAllCoords);
                allCoords[getIndex(curX+diffX, curY+diffY)] = particle;
                setBitmapColor(curX+diffX, curY+diffY, tempElement);
                allCoords[getIndex(curX, curY)] = -1;
                clearBitmapColor(curX, curY);
                a_x[particle] = curX + diffX;
                a_y[particle] = curY + diffY;

                // Make tunnel particles
                createTunnel(curX, curY, diffX, diffY, elements[targetElementIndex]);

                break;
            }
        }
    }
    else if (rand() % 10 == 0) // 2/3 * 1/10 = 1/15 chance if we're blocked to reset
    {
        setParticleSpecialVal(particle, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);
    }
}

int specialBurn(int particle)
{
    struct Element* tempElement = a_element[particle];
    int tempX = a_x[particle];
    int tempY = a_y[particle];
    int shouldResolveHeatChanges = TRUE;

    // Burn doesn't trigger when explode is set
    if (getElementSpecialVal(tempElement, SPECIAL_EXPLODE) != SPECIAL_VAL_UNSET)
    {
        return shouldResolveHeatChanges;
    }

    int state = getParticleSpecialVal(particle, SPECIAL_BURN);
    // Not burning yet
    if (state == SPECIAL_VAL_UNSET)
    {
        //If the heat is above the threshold
        if (a_heat[particle] >= a_element[particle]->highestTemp)
        {
            int avgLife = getElementSpecialVal(tempElement, SPECIAL_BURN);
            int life = avgLife + rand()%5 - 2;
            if (life < 1) { life = 1; }
            setParticleSpecialVal(particle, SPECIAL_BURN, life);
            shouldResolveHeatChanges = FALSE;
        }
    }
    // Already burning
    else
    {
        shouldResolveHeatChanges = FALSE;

        // FIXME: This is identical to special heat, break this function out into specials.c
        int diffX, diffY;
        int tempAllCoords;
        for (diffX = -1; diffX <= 1; diffX++)
        {
            for(diffY = -1; diffY <=1; diffY++)
            {
                if((diffX!=0||diffY!=0) && coordInBounds(tempX+diffX, tempY+diffY))
                {
                    tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                    if(tempAllCoords != -1)
                    {
                        changeHeat(&(a_heat[tempAllCoords]),
				            1000/(10+state*getElementSpecialVal(tempElement, SPECIAL_BURN)));
                    }
                }
            }
        }

        // If we're done with our life total
        if (state <= 1)
        {
            // Change to fire
            setElement(particle, elements[FIRE_ELEMENT]);
        }
        else
        {
            // Decrement life total left
            setParticleSpecialVal(particle, SPECIAL_BURN, state-1);

            // Set the color to fire
            setBitmapColor(tempX, tempY, elements[FIRE_ELEMENT]);
        }
    }

    return shouldResolveHeatChanges;
}

void specialConductive(int particle)
{
    int property;
    int tempAllCoords;
    int curX = a_x[particle], curY = a_y[particle];
    property = getParticleSpecialVal(particle, SPECIAL_CONDUCTIVE);
    if (property != SPECIAL_VAL_UNSET)
    {
        // Wait one frame, remove the electric wait bit
        if (property & ELECTRIC_WAIT)
        {
            setBitmapColor(curX,curY,elements[ELECTRICITY_ELEMENT]);
            setParticleSpecialVal(particle, SPECIAL_CONDUCTIVE, property & 15); //15 == 00001111
            return;
        }
        // If no direction, select a random direction
        if (property == ELECTRIC_NO_DIR)
        {
            // 0 or 1 in XN, 1 in X1
            property = (((rand()%2) << 2) & ELECTRIC_XN) | ELECTRIC_X1;
        }
        int i = (property & ELECTRIC_X1) * ((ELECTRIC_XN & property) ? -1 : 1);
        int j = ((property & ELECTRIC_Y1) >> 1) * ((ELECTRIC_YN & property) ? -1 : 1);

        int tempI, k, transfered = FALSE;
        for (k = 0; k < 4; k++)
        {
            if (curX + i < workWidth && curX + i >= 0 && curY + j < workHeight && curY + j >= 0)
            {
                tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                if (tempAllCoords != -1)
                {
                    if(hasSpecial(tempAllCoords, SPECIAL_CONDUCTIVE))
                    {
                        setParticleSpecialVal(tempAllCoords, SPECIAL_CONDUCTIVE,
                                              (i ? ELECTRIC_X1 : 0) |
                                              (i < 0 ? ELECTRIC_XN : 0) |
                                              (j ? ELECTRIC_Y1 : 0) |
                                              (j < 0 ? ELECTRIC_YN :0) |
                                              ELECTRIC_WAIT);
                        setBitmapColor(curX+i,curY+j,elements[ELECTRICITY_ELEMENT]);
                        transfered = TRUE;
                        break;
                    }
                }
            }
            //There exists a nice binary operation to do this ( check right, left behind, then return), but this is clearer
            //If we need speed we can use binary operations to get these numbers
            tempI = i;
            switch ( k )
            {
            case 0:
            {
                i = j;
                j = tempI;
                break;
            }
            case 1:
            {
                i = -i;
                j = -j;
                break;
            }
            case 2:
            {
                i = j;
                j = -tempI;
                break;
            }
            case 3:
            {
                i = -i;
                j = -j;
                break;
            }
            }
        }
        if(!transfered)
        {
            i = i - j;
            j = i + j;
            for (k = 0; k < 4; k++ )
            {
                if ( curX + i < workWidth && curX + i >= 0 && curY + j < workHeight && curY + j >= 0)
                {
                    tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                    if (tempAllCoords != -1)
                    {
                        if(hasSpecial(tempAllCoords, SPECIAL_CONDUCTIVE))
                        {
                            setParticleSpecialVal(tempAllCoords, SPECIAL_CONDUCTIVE,
                                                  (i ? ELECTRIC_X1 : 0) |
                                                  (i < 0 ? ELECTRIC_XN : 0) |
                                                  (j ? ELECTRIC_Y1 : 0) |
                                                  (j < 0 ? ELECTRIC_YN :0) |
                                                  ELECTRIC_WAIT);
                            transfered = TRUE;
                            break;
                        }
                    }
                }
                switch ( k )
                {
                case 0:
                {
                    i = j;
                    j = -tempI;
                    break;
                }
                case 1:
                {
                    i = -i;
                    j = -j;
                    break;
                }
                case 2:
                {
                    i = j;
                    j = tempI;
                    break;
                }
                case 3:
                {
                    i = -i;
                    j = -j;
                    break;
                }
                }
            }
        }
        if(!transfered)
        {
            for ( k = 0; k < 4; k++)
            {
                if ( curX + i < workWidth && curX + i >= 0 && curY + j < workHeight && curY + j >= 0)
                {
                    tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                    if (tempAllCoords == -1)
                    {
                        createPoint(curX + i, curY + j, elements[ELECTRICITY_ELEMENT]);
                        transfered = TRUE;
                        break;
                    }
                }
                //There exists a nice binary operation to do this ( check right, left behind, then return), but this is clearer
                //If we need speed we can use binary operations to get these numbers
                tempI = i;
                switch ( k )
                {
                case 0:
                {
                    i = j;
                    j = tempI;
                    break;
                }
                case 1:
                {
                    i = -i;
                    j = -j;
                    break;
                }
                case 2:
                {
                    i = j;
                    j = -tempI;
                    break;
                }
                case 3:
                {
                    i = -i;
                    j = -j;
                    break;
                }
                }
            }
            if(!transfered)
            {
                i = i - j;
                j = i + j;
                for (k = 0; k < 4; k++ )
                {
                    if ( curX + i < workWidth && curX + i >= 0 && curY + j < workHeight && curY + j >= 0)
                    {
                        tempAllCoords = allCoords[getIndex(curX+i,curY+j)];
                        if (tempAllCoords == -1)
                        {
                            createPoint(curX + i, curY + j, elements[ELECTRICITY_ELEMENT]);
                            transfered = TRUE;
                        }
                    }
                    switch ( k )
                    {
                    case 0:
                    {
                        i = j;
                        j = -tempI;
                        break;
                    }
                    case 1:
                    {
                        i = -i;
                        j = -j;
                        break;
                    }
                    case 2:
                    {
                        i = j;
                        j = tempI;
                        break;
                    }
                    case 3:
                    {
                        i = -i;
                        j = -j;
                        break;
                    }
                    }
                }
            }
        }
        setParticleSpecialVal(particle, SPECIAL_CONDUCTIVE, SPECIAL_VAL_UNSET);
        setBitmapColor(curX, curY, a_element[particle]);
    }
}

void specialTrail(int particle)
{
    struct Element* tempElement = a_element[particle];

    // Pull out the special val and clean it, it will determine
    // how variable the path is (i.e. how much velocity is the max added)
    int variability = getElementSpecialVal(tempElement, SPECIAL_TRAIL);
    if (variability < 3)
    {
        variability = 3;
    }
    variability -= (variability+1)%2;

    struct Particle* tempAllCoords;
    int i, j,tempIndex, found = FALSE;
    int curX = a_x[particle], curY = a_y[particle];
    // Seed the rand function with the current location
    // (plus a randomizer that doesn't change very often)
    srand(curX*curY + randOffset);
    // Add random velocity based on seed (so the same location
    // will get the same velocity boost)
    a_xVel[particle] += rand()%variability - variability/2;
    // "Unseed"
    struct timeval time;
    gettimeofday(&time, NULL);
    srand(time.tv_usec);
}
