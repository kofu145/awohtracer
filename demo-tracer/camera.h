#pragma once

#include "hittable.h"
#include "material.h"

class camera {
    public:
        double aspect_ratio = 16.0 / 9.0;
        int image_width = 100;
        int samples_per_pixel = 10; // count of random samples for each pixel
        int max_depth = 10; // maximum number of ray bounces
            
        double vfov = 90;
        point3 lookfrom = point3(0, 0, 0);
        point3 lookat = point3(0, 0, -1);
        vec3 vup = vec3(0, 1, 0);


        void render(const hittable& world) {
            initialize();

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for (int j = 0; j < image_height; j++){
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for (int i=0; i < image_width; i++){
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++){
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }

                    write_color(std::cout, pixel_samples_scale * pixel_color);
                }
            }
            std::clog << "\rDone.                 \n";
        }

    private:
        int image_height;
        double pixel_samples_scale; // color scale factor for a sum of pixel samples
        point3 center;
        point3 pixel00_loc;
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w;        

        void initialize() {
            // Calculate the image height, and ensure that it's at least 1.
            image_height = int(image_width / aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            pixel_samples_scale = 1.0 / samples_per_pixel;
            center = lookfrom;

            // Viewport dimensions
            auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focal_length;
            auto viewport_width = viewport_height * (double(image_width)/image_height);

            // calc u, v, w unit basis vectors for camera coordinate frame
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;

            // Calculate the horizontal and vertical delta vectors from pixel to pixel.
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


        }

        ray get_ray(int i, int j) const {
            // camera ray from origin to random sample of pixels at pixel point i, j
            
            auto offset = sample_square();
            auto pixel_sample = pixel00_loc 
                        + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

            auto ray_origin = center;
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }   

        vec3 sample_square() const {
            // returns the vector to a random point in the -.5 > .5 (x, y) unit square
            return vec3(random_double() - 0.5, random_double() - 0.5, 0);
        }
        
        color ray_color(const ray& r, int depth, const hittable& world) {
            if (depth <= 0){
                return color(0, 0, 0); // no more light gathered if exceeded ray bounce limit
            }
            /*
            auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
            if (t > 0.0){ // we've intersected the sphere
                // P direction - C (center point of sphere) = surface normal, N
                vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
                // color rep of surface normal
                return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
            }*/

            hit_record rec;
            if (world.hit(r, interval(0.001, infinity), rec)) {
                /*vec3 direction = random_on_hemisphere(rec.normal);
                return 0.5 * (rec.normal + color(1, 1, 1));
                return 0.5 * ray_color(ray(rec.p, direction), depth-1, world);*/

                ray scattered;
                color attenuation;
                if (rec.mat->scatter(r, rec, attenuation, scattered))
                    return attenuation * ray_color(scattered, depth-1, world);
                return color(0, 0, 0);
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5*(unit_direction.y() + 1.0);
            return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
        }
};
