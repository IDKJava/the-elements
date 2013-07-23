/*
 * specials.c
 * ------------------------------
 * Contains functions related to element specials
 */

#include "macros.h"
#include "app.h"
#include "points.h"
#include "stdbool.h"

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
