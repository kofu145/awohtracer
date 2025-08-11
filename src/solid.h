#pragma once

class material; // reference to class to solve circular dependency

class hit {
    public:
        point3 p;
        vec3 normal;
        shared_ptr<material> mat;
        double u;
        double v;
        double t;
        bool front_face;

        void set_face_normal(const ray &r, const vec3 &outward_normal);
}

class solid {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray &r, interval ray_t,
                         hit_record &rec) const = 0;

        aabb bounding_box() const = 0;
}
