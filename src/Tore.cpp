#include "Tore.h"
#include <iostream>

bool Tore::intersect(const Ray& ray, Vector& impact) const
{
    Ray local_ray = global_to_local(ray);

    Vector local_ori = local_ray.origin;
    Vector local_dir = local_ray.direction.normalized();

    float R = 1.0f, r = 0.25f;

    float R2 = R*R, r2 = r*r;
    float x_dir = local_dir[0], y_dir = local_dir[1], z_dir = local_dir[2];
    float x_ori = local_ori[0], y_ori = local_ori[1], z_ori = local_ori[2];

    float dir_squared = local_dir.dot(local_dir);
    float ori_squared = local_ori.dot(local_ori) /*+ R*R - r*r*/;
    float ori_dot_dir = local_ori.dot(local_dir);

    float a = pow(dir_squared, 2.0f);
    float b = 4.0f*ori_dot_dir;
    float c = 2.0f*(ori_squared + R2 - r2) - 4.0f*R2*(1.0f - z_dir*z_dir) + 4.0f*pow(ori_dot_dir, 2.0f);
   float d = 8.0f*R2*z_ori*z_dir + 4.0f*ori_dot_dir*(ori_squared - R*R - r*r);
   float e = pow(x_ori, 4.0f) + pow(y_ori, 4.0f) + pow(z_ori, 4.0f) + pow(R*R - r*r, 2.0f) + 2.0f*(x_ori*x_ori*y_ori*y_ori + z_ori*z_ori*(R2 - r2) + (x_ori*x_ori + y_ori*y_ori)*(z_ori*z_ori - R2 - r2));


    float t = -1.0f;


    if(!Entity::solve_polynomial_4(b, c, d, e, t))
	return false;

    const Vector local_impact = local_ray.origin + t * local_ray.direction.normalized();

    impact = local_to_global_point(local_impact);

    return true;

}

Ray Tore::get_normal(const Vector& impact, const Vector& observator) const
{
    Vector local_impact = global_to_local_point(impact);
    Vector local_obs = global_to_local_point(observator);
    Vector local_ray_direction = local_impact - local_obs;

    // tore formula : (x^2 + y^2 + z^2 + R^2 + r^2) - 4*(x^2 + z^2) = 0
    // gradient is the vector of partial derivative with respect to all variables
    // df / dx = 4 * x * (x^2 + y^2 + z^2 + R^2 - r^2) - 8 * R^2 * x
    // df / dy = 4 * y * (x^2 + y^2 + z^2 + R^2 - r^2) - 8 * R^2 * y
    // df / dz = 4 * z * (x^2 + y^2 + z^2 + R^2 - r^2) - 8 * R^2 

    float x = local_impact[0];
    float y = local_impact[1];
    float z = local_impact[2];
    float R2 = 1.0f;
    float r2 = 0.25f * 0.25f;

    /* 
    float x_normal = 4*x*(x*x + y*y + z*z + R2 - r2) - 8*R2*x;
    float y_normal = 4*y*(x*x + y*y + z*z + R2 - r2) - 8*R2*y;
    float z_normal = 4*z*(x*x + y*y + z*z + R2 - r2); 
    */
    
    float x_normal = 4.0f*x*(x*x + y*y + z*z - 1.0625f);
    float y_normal = 4.0f*y*(x*x + y*y + z*z - 1.0625f);
    float z_normal = 4.0f*z*(x*x + y*y + z*z + 0.9375f);
    
    Vector local_normal(x_normal, y_normal, z_normal);
    
    if (local_ray_direction.dot(local_normal.normalized()) < 0)
        return Ray(impact, local_to_global_vector(local_normal).normalized());

    return Ray(impact, local_to_global_vector(local_normal * -1).normalized());
}
