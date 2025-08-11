#pragma once

#include "math/vec3.h"

class ray {
    public:
        ray();

        ray(const vec3 &origin, const vec3 &direction, double time);

        ray(const vec3 &origin, const vec3 &direction);

        const vec3 &origin() const;
        const vec3 &direction() const;

        double time() const;

        vec3 at(double t) const;

    private:
        vec3 orig;
        vec3 dir;
        double tm;
};
