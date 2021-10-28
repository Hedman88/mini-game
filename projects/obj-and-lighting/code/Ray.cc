#include "Ray.h"
#include "render/Debug.h"

Ray::Ray(Vector position, Vector direction, int range){
    direction.Normalize();
    Vector endPos = position + (direction * range);
    Debug::DrawLine(position, endPos, Vector(0,0,1));
}
