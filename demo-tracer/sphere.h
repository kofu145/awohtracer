#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
    public:
        sphere(const point3& static_center, double radius, shared_ptr<material> mat) : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {
            auto rvec = vec3(radius, radius, radius);
            bbox = aabb(static_center - rvec, static_center + rvec);
        }

        aabb bounding_box() const override { return bbox; }
        
        sphere(const point3& center1, const point3& center2, double radius, shared_ptr<material> mat) : center(center1, center2-center1), radius(std::fmax(0, radius)), mat(mat) {
            // bounding box around the ENTIRE possible range of motion for the sphere
            auto rvec = vec3(radius, radius, radius);
            aabb box1(center.at(0) - rvec, center.at(0) + rvec);
            aabb box2(center.at(1) - rvec, center.at(1) + rvec);
            bbox = aabb(box1, box2);
        }
        
        bool hit(const ray& r, interval ray_t, hit_record& rec) const override{
            point3 current_center = center.at(r.time());
            vec3 oc = current_center - r.origin(); // C vec - eye origin point - (c - P(t))
            
            /*
            quadratic formula calc
            // following is quadratic formula members a, b, c to find t
            auto a = dot(r.direction(), r.direction());
            auto b = -2.0 * dot(r.direction(), oc);
            auto c = dot(oc, oc) - radius*radius;
            auto discriminant = b*b - 4*a*c;

            */
            auto a = r.direction().length_squared();
            auto h = dot(r.direction(), oc);
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = h*h - a*c;

            if (discriminant < 0){ // not hit
                return false;
            }

            auto sqrtd = std::sqrt(discriminant);

            auto root = (h - sqrtd) / a;
            if (!ray_t.surrounds(root)){
                root = (h + sqrtd) / a;
                if (!ray_t.surrounds(root))
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            // rec.normal = (rec.p - center) / radius;
            vec3 outward_normal = (rec.p - current_center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }
    private:
        ray center;
        double radius;
        shared_ptr<material> mat;
        aabb bbox;
};
