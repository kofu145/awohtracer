#pragma once

#include "ray.h"
#include "interval.h"

class hit_record {
    public:
        point3 p; // point of collision
        vec3 normal; // surface normal
        double t; // what T was during P(t) = A + tb
        bool front_face; // determining the side of the surface the ray intersects
        // our normals are going to always point against where the ray intersects, so
        // we use this to track which side of the surface the ray is on

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            // Sets the hit record normal vector.
            // outward_normal is assumed to have unit length

            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
    };

class hittable {
    public:
        virtual ~hittable() = default;

        // pure virtual func for this abstract class
        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

};