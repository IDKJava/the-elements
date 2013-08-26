/*
 * specials.c
 * ------------------------------
 * Contains functions related to element specials
 */

#include "specials.h"
#include "macros.h"
#include "points.h"

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
            if (tempX + diffX >= 0 && tempX + diffX < workWidth && tempY + diffY >= 0 && tempY + diffY < workHeight)
            {
                tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                if (tempAllCoords != -1 && a_element[tempAllCoords] == elements[GENERATOR_ELEMENT]) //There's a generator adjacent
                {
                    setElement(tempAllCoords, elements[SPAWN_ELEMENT]);
                    setParticleSpecialVal(tempAllCoords, SPECIAL_SPAWN, getParticleSpecialVal(particle, SPECIAL_SPAWN));
                }
                else if (tempAllCoords == -1 && rand() % GENERATOR_SPAWN_PROB == 0 && loq < MAX_POINTS - 1) //There's an empty spot
                {
                    createPoint(tempX + diffX, tempY + diffY, elements[getParticleSpecialVal(particle, SPECIAL_SPAWN)]);
                }
            }
        }
    }
}

void specialBreak(int particle)
{
    if (a_xVel[particle] > getElementSpecialVal(a_element[particle], SPECIAL_BREAK)
        || a_yVel[particle] > getElementSpecialVal(a_element[particle], SPECIAL_BREAK))
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
                if((diffX!=0||diffY!=0) && tempX+diffX < workWidth && tempX+diffX >= 0 && tempY+diffY < workHeight && tempY+diffY >= 0)
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

    int randVal = rand()%100;
    // Randomly wander
    int wanderVal = getElementSpecialVal(tempElement, SPECIAL_WANDER);
    if (randVal <= wanderVal)
    {
        if (a_xVel[particle] <= 4)
        {
            a_xVel[particle] += 2;
        }
    }
    else if (randVal >= wanderVal+1 && randVal <= wanderVal*2)
    {
        if (a_xVel[particle] >= -4)
        {
            a_xVel[particle] -= 2;
        }
    }

    randVal = rand()%100;
    if (randVal <= wanderVal)
    {
        if (a_yVel[particle] >= -4)
        {
            a_yVel[particle] -= 2;
        }
    }
    if (randVal >= wanderVal + 1 && randVal <= wanderVal*2)
    {
        if (a_yVel[particle] <= 4)
        {
            a_yVel[particle] += 2;
        }
    }
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

void specialTunnel(int particle)
{
    struct Element* tempElement = a_element[particle];
    int targetElementIndex = getElementSpecialVal(tempElement, SPECIAL_TUNNEL);
    int state = getParticleSpecialVal(particle, SPECIAL_TUNNEL);

    int curX = a_x[particle], curY = a_y[particle];
    int diffX, diffY;

    if (state == SPECIAL_VAL_UNSET)
    {
        // Look in a random diagonal
        int randomDir = rand()%4;
        diffX = 2*(randomDir%2) - 1;
        diffY = (randomDir - randomDir%2) - 1;
        if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
        {
            return;
        }
        int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
        if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
        {
            // Remove the tempAllCoords particle, and move this particle there
            unSetPoint(tempAllCoords);
            allCoords[getIndex(curX+diffX, curY+diffY)] = particle;
            setBitmapColor(curX+diffX, curY+diffY, tempElement);
            allCoords[getIndex(curX, curY)] = -1;
            clearBitmapColor(curX, curY);
            a_x[particle] = curX + diffX;
            a_y[particle] = curY + diffY;
            // Set the y velocity to the fall velocity to counteract movement
            a_yVel[particle] = a_element[particle]->fallVel;
            setParticleSpecialVal(particle, SPECIAL_TUNNEL, randomDir);


            // Add particles around this point, forming a "tunnel"
            if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
            {
                if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == -1)
                {
                    createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                }
                if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == -1)
                {
                    createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                }
                if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == -1)
                {
                    createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                }
            }
        }
    }
    // We're already moving in a direction
    else
    {
        // Move the particle back to it's old location, to avoid
        // collision velocities messing up tunneling
        if (allCoords[getIndex(a_oldX[particle], a_oldY[particle])] == -1)
        {
            allCoords[getIndex(curX, curY)] = -1;
            clearBitmapColor(curX, curY);
            allCoords[getIndex(a_oldX[particle], a_oldY[particle])] = particle;
            setBitmapColor(a_oldX[particle], a_oldY[particle], tempElement);

            curX = a_x[particle] = a_oldX[particle];
            curY = a_y[particle] = a_oldY[particle];
        }

        diffX = 2*(state%2) - 1;
        diffY = (state - state%2) - 1;
        if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
        {
            // Go back to the unset state
            setParticleSpecialVal(particle, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);

            // Look in a random diagonal
            int randomDir = rand()%4;
            diffX = 2*(randomDir%2) - 1;
            diffY = (randomDir - randomDir%2) - 1;
            if (curX+diffX < 0 || curX+diffX >= workWidth || curY+diffY < 0 || curY+diffY >= workHeight)
            {
                return;
            }
            int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
            if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
            {
                // Remove the tempAllCoords particle, and move this particle there
                unSetPoint(tempAllCoords);
                allCoords[getIndex(curX+diffX, curY+diffY)] = particle;
                setBitmapColor(curX+diffX, curY+diffY, tempElement);
                allCoords[getIndex(curX, curY)] = -1;
                clearBitmapColor(curX, curY);
                a_x[particle] = curX + diffX;
                a_y[particle] = curY + diffY;
                // Set the y velocity to the fall velocity to counteract movement
                a_yVel[particle] = a_element[particle]->fallVel;
                setParticleSpecialVal(particle, SPECIAL_TUNNEL, randomDir);


                // Add particles around this point, forming a "tunnel"
                if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
                {
                    if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == -1)
                    {
                        createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                    }
                    if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == -1)
                    {
                        createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                    }
                    if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == -1)
                    {
                        createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                    }
                }
            }
        }

        int tempAllCoords = allCoords[getIndex(curX+diffX, curY+diffY)];
        if (tempAllCoords == -1 || a_element[tempAllCoords]->index != targetElementIndex)
        {
            // Go back to the unset state
            setParticleSpecialVal(particle, SPECIAL_TUNNEL, SPECIAL_VAL_UNSET);
            // Look for any particle back the way we came and (if there is one)
            // tunnel into it
            tempAllCoords = allCoords[getIndex(curX, curY+diffY)];
            if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
            {
                // Remove the tempAllCoords particle, and move this particle there
                unSetPoint(tempAllCoords);
                allCoords[getIndex(curX, curY+diffY)] = particle;
                setBitmapColor(curX, curY+diffY, tempElement);
                allCoords[getIndex(curX, curY)] = -1;
                clearBitmapColor(curX, curY);
                a_x[particle] = curX;
                a_y[particle] = curY + diffY;
            }
            else
            {
                tempAllCoords = allCoords[getIndex(curX+diffX, curY)];
                if (tempAllCoords != -1 && a_element[tempAllCoords]->index == targetElementIndex)
                {
                    // Remove the tempAllCoords particle, and move this particle there
                    unSetPoint(tempAllCoords);
                    allCoords[getIndex(curX+diffX, curY)] = particle;
                    setBitmapColor(curX+diffX, curY, tempElement);
                    allCoords[getIndex(curX, curY)] = -1;
                    clearBitmapColor(curX, curY);
                    a_x[particle] = curX + diffX;
                    a_y[particle] = curY;
                }
            }
            // Add velocity so that the particle stays still for one step
            a_yVel[particle] = a_element[particle]->fallVel;
            return;
        }
        else
        {
            // Remove the tempAllCoords particle, and move this particle there
            unSetPoint(tempAllCoords);
            allCoords[getIndex(curX+diffX, curY+diffY)] = particle;
            setBitmapColor(curX+diffX, curY+diffY, tempElement);
            allCoords[getIndex(curX, curY)] = -1;
            clearBitmapColor(curX, curY);
            a_x[particle] = curX + diffX;
            a_y[particle] = curY + diffY;
            // Set the y velocity to the fall velocity to counteract movement
            a_yVel[particle] = a_element[particle]->fallVel;


            // Add particles around this point, forming a "tunnel"
            if (curX+2*diffX >= 0 && curX+2*diffX < workWidth && curY+2*diffY >= 0 && curY+2*diffY < workHeight)
            {
                if (allCoords[getIndex(curX+2*diffX, curY+2*diffY)] == -1)
                {
                    createPoint(curX+2*diffX, curY+2*diffY, elements[targetElementIndex]);
                }
                if (allCoords[getIndex(curX+2*diffX, curY+diffY)] == -1)
                {
                    createPoint(curX+2*diffX, curY+diffY, elements[targetElementIndex]);
                }
                if (allCoords[getIndex(curX+diffX, curY+2*diffY)] == -1)
                {
                    createPoint(curX+diffX, curY+2*diffY, elements[targetElementIndex]);
                }
            }
        }
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
        return;
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
                if((diffX!=0||diffY!=0) && tempX+diffX < workWidth && tempX+diffX >= 0 && tempY+diffY < workHeight && tempY+diffY >= 0)
                {
                    tempAllCoords = allCoords[getIndex(tempX+diffX,tempY+diffY)];
                    if(tempAllCoords != -1)
                    {
                        changeHeat(&(a_heat[tempAllCoords]),
				   1000/(state*getElementSpecialVal(tempElement, SPECIAL_BURN)));
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
            if (curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
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
                if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
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
                if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
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
                    if ( curX + i <= workWidth && curX + i > 0 && curY + j <= workHeight && curY + j > 0)
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
}
