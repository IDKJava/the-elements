#include "portals.h"
#include "points.h"
#include "update.h"

#include "app.h"
#include "macros.h"
#include <cmath>

#ifndef NDEBUG // Debug
#define LOGGING 1
#else // Release
#define LOGGING 0
#endif

bool advanceNextPortal() {
    int start = nextPortal;
    nextPortal = (nextPortal + 1) % MAX_PORTALS;
    while (portals[nextPortal].isActive) {
        if (nextPortal == start) {
            nextPortal = MAX_PORTALS;
            return false;
        }
        nextPortal = (nextPortal + 1) % MAX_PORTALS;
    }
    return true;
}

bool makePortal(int x, int y, int ex, int ey) {
    // We'll need to draw a line from the start to end...
    // We also need to save information regarding the whole portal, and the pixels which are part
    // of the portal need to remember which portal they're part of maybe?
    if (nextPortal >= MAX_PORTALS) {
        return false;
    }
    Portal *next = &portals[nextPortal];
    next->x = (float)x;
    next->y = (float)y;
    next->ex = (float)ex;
    next->ey = (float)ey;
    next->isActive = true;
    next->hasPair = false;
    next->pairIdx = NOT_PAIRED;
    next->width = (float)brushSize;

    // Pair to the previously made portal if we're the next in line
    // Because we always delete in pairs, the previously made portal is always at index-1
    // if it exists.
    if (nextPortal > 0) {
        Portal *prev = &portals[nextPortal-1];
        if (!prev->hasPair && prev->isActive) {
            prev->pairIdx = nextPortal;
            next->pairIdx = nextPortal - 1;
            next->hasPair = true;
            prev->hasPair = true;
        }
    }

    // Now we need to actually draw the portal in the game world.
    // We'll set the elements we create as having special value equal to our portal index
    // We set the default special val of the portal element to be the portal we're creating right now
    elements[PORTAL_ELEMENT]->specialVals[0] = nextPortal;

    Element* savedElement = cElement;
    cElement = elements[PORTAL_ELEMENT];
    drawCircleyLine(x,y,ex,ey);
    cElement = savedElement;

    advanceNextPortal();

    return true;
}


bool deletePortal(int index) {
    Portal *toDelete = &portals[index];
    if (!toDelete->isActive) {
        return false;
    }
    toDelete->isActive = false;
    nextPortal = index;

    // Delete pair as well if present
    // This done so that things are easier, since we don't have to figure out how to pair
    // portals at times other than creation
    if (toDelete->hasPair) {
        Portal *paired = &portals[toDelete->pairIdx];
        nextPortal = min(nextPortal, toDelete->pairIdx);
        paired->isActive = false;
        deletePortalPoints(toDelete->pairIdx);
        toDelete->hasPair = false;
    }
    deletePortalPoints(index);
    return true;
}

void deletePortalPoints(int portalIndex) {
    // Iterate over all points and delete the ones that have a matching value

    for (int tempParticle = 0; tempParticle < MAX_POINTS; ++tempParticle) {
        if (a_set[tempParticle] && hasSpecial(tempParticle, SPECIAL_PORTAL) && getParticleSpecialVal(tempParticle, SPECIAL_PORTAL) == portalIndex) {
            deletePoint(tempParticle);
        }
    }
}


// first particle is the particle moving into the portal, the second particle
// should be a portal
bool specialPortal(int firstParticle, int secondParticle)
{
    if (!hasSpecial(secondParticle, SPECIAL_PORTAL)) {
        return FALSE;
    }

    int portalIdx = getParticleSpecialVal(secondParticle, SPECIAL_PORTAL);
    Portal* firstPortal = &portals[portalIdx];

    if (!firstPortal->hasPair) {
        return FALSE;
    }

    // I had to look up how change of basis works for this code
    // https://www.khanacademy.org/math/linear-algebra/alternate-bases/change-of-basis/v/linear-algebra-change-of-basis-matrix

    // Step one: change of basis so that we get the first particle's position in the vector
    // space of the first portal.
    float portal_l_x = firstPortal->x - firstPortal->ex;
    float portal_l_y = firstPortal->y - firstPortal->ey;
    float portal_norm = sqrt(portal_l_x*portal_l_x + portal_l_y*portal_l_y);

    // Perpendicular is scaled to the width of the portal line
    float portal_perp_x = -portal_l_y * (firstPortal->width/portal_norm);
    float portal_perp_y = portal_l_x * (firstPortal->width/portal_norm);

    // x and y relative to the first portal start
    float x = a_x[firstParticle] - firstPortal->x;
    float y = a_y[firstParticle] - firstPortal->y;

    // Determinant needed to compute inverse of matrix
    float det = (portal_l_x * portal_perp_y) - (portal_l_y * portal_perp_x);

    float a_c_x = ((portal_perp_y * x) - (portal_perp_x * y))/det;
    float a_c_y = (-(portal_l_y * x) + (portal_l_x * y))/det;

    // flip so that things come out on the other side.
    a_c_y *= -1;

    Portal* secondPortal = &portals[firstPortal->pairIdx];
    // now get the second portal properties
    float portal2_l_x = secondPortal->x - secondPortal->ex;
    float portal2_l_y = secondPortal->y - secondPortal->ey;
    float portal2_norm = sqrt(portal2_l_x * portal2_l_x + portal2_l_y * portal2_l_y);

    float portal2_extended_width;
    float portal2_perp_x;
    float portal2_perp_y;
    int final_position_x;
    int final_position_y;
    int particleAtTarget;

    // super hacky way of dealing with integer conversion issues
    // just adjust the width until we don't hit the portal.
    for (int i = 0; i < 8; i++) {
        portal2_extended_width = secondPortal->width + (i/2.0);
        portal2_perp_x = -portal2_l_y * (portal2_extended_width / portal2_norm);
        portal2_perp_y = portal2_l_x * (portal2_extended_width / portal2_norm);

        // get the offset in the original coordinate space after interpreting the coordinates
        // as being in the space of the second portal.
        float final_offset_x = portal2_l_x * a_c_x + portal2_perp_x * a_c_y;
        float final_offset_y = portal2_l_y * a_c_x + portal2_perp_y * a_c_y;

        final_position_x = (int) (secondPortal->x + final_offset_x);
        final_position_y = (int) (secondPortal->y + final_offset_y);

        if (!coordInBounds(final_position_x, final_position_y)) continue;
        particleAtTarget = allCoords[getIndex(final_position_x, final_position_y)];
        if (particleAtTarget != -1 && a_element[particleAtTarget]->index == PORTAL_ELEMENT) {
            if (getParticleSpecialVal(particleAtTarget, SPECIAL_PORTAL) == firstPortal->pairIdx) {
                continue;
            }
        }
        break;
    }

    if (!coordInBounds(final_position_x, final_position_y)) {
        return false;
    }
    if (particleAtTarget != -1) { // TODO: Is there a way to support collisions through portals?
        return false;
    }

    /// Move the particle if we can...
    // you could create an infinite loop of portals which would freeze the game
    if (particleAtTarget != -1 && a_element[particleAtTarget]->index == PORTAL_ELEMENT) {
        return false;
    }
    // If we moved, we need to adjust the velocity as well
    // First we get the velocity vector in the vector space of the first portal
    float xv = a_xVel[firstParticle];
    float yv = a_yVel[firstParticle];

    // For velocity, we don't want to adjust the magnitude of the vector, so normalize
    // the length to be 1.
    float perp_norm = sqrt(portal_perp_x*portal_perp_x +  portal_perp_y*portal_perp_y);
    float l_norm = sqrt(portal_l_x*portal_l_x +  portal_l_y*portal_l_y);

    float det_unit = det/(perp_norm*l_norm);
    float v_c_x = ((portal_perp_y/perp_norm * xv) - (portal_perp_x/perp_norm * yv))/det_unit;
    float v_c_y = (-(portal_l_y/l_norm * xv) + (portal_l_x/l_norm * yv))/det_unit;

    float perp2_norm = sqrt(portal2_perp_x*portal2_perp_x +  portal2_perp_y*portal2_perp_y);
    float l2_norm = sqrt(portal2_l_x*portal2_l_x +  portal2_l_y*portal2_l_y);

    float final_vel_x = portal2_l_x/l2_norm * v_c_x + portal2_perp_x/perp2_norm * v_c_y;
    float final_vel_y = portal2_l_y/l2_norm * v_c_x + portal2_perp_y/perp2_norm * v_c_y;

    a_xVel[firstParticle] = final_vel_x;
    a_yVel[firstParticle] = final_vel_y;

    a_x[firstParticle] = (float) final_position_x;
    a_y[firstParticle] = (float) final_position_y;
    // FIXME: our code doesn't support chaining collisions like this, and I don't think
    // there is a nice way to do it in general. For now, I just return false
    // when particleAtTarget == -1 is detected above.
//    if (particleAtTarget != -1)
//    {
//        collide(firstParticle, particleAtTarget);
//    }
    a_hasMoved[firstParticle] = true;
    return true;
}

