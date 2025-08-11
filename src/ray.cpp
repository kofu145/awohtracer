#include "ray.h"

ray::ray(const vec3 &origin, const vec3 &direction, double time)
    : orig(origin), dir(direction), tm(time) {}

ray::ray(const vec3 &origin, const vec3 &direction)
    : ray(origin, direction, 0) {}

const vec3 &ray::origin() const { return orig; }
const vec3 &ray::direction() const { return dir; }

double ray::time() const { return tm; }

vec3 ray::at(double t) const {
    // P(t) = A + tb
    return orig + t * dir;
}
