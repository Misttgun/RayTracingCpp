#include "Tore.h"
#include <iostream>

bool Tore::intersect(const Ray& ray, Vector& impact) const
{
    Ray local_ray = global_to_local(ray);
    Vector local_ori = local_ray.origin;
    Vector local_dir = local_ray.direction.normalized();

    // - define tore radius size
    float R = 1.0f, r = 0.25f;

    // - cache usefull values
    float ox = local_ori[0], oy = local_ori[1], oz = local_ori[2];
    float dx = local_dir[0], dy = local_dir[1], dz = local_dir[2];
    float od = local_ori.dot(local_dir), sq_od = pow(od, 2.0f);
    float sq_o = local_ori.dot(local_ori);
    float sq_R = 1.0f, sq_r = pow(r, 2.0f);
    float sq_ox = ox*ox, sq_oy = oy*oy, sq_oz = oz*oz;
    
    // - compute coefficients
    float a = 1.0f; // a = d^4 a d is normalized
    float b = 4.0f*od; 
    float c = 2.0f*(sq_o + sq_R - sq_r) - 4.0f*sq_R*(1.0f - dz*dz) + 4.0f*sq_od;
    float d = 8.0f*sq_R*(dz*oz) + 4.0f*od*(sq_o - sq_R - sq_r);
    float e = pow(ox, 4.0f) + pow(oy, 4.0f) + pow(oz, 4.0f) + pow(sq_R - sq_r, 2.0f);
    e += 2.0f*(sq_ox*sq_oy + sq_oz*(sq_R - sq_r) + (sq_ox + sq_oy)*(sq_oz - sq_R - sq_r));

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

     
    float x_normal = 4.0f*x*(x*x + y*y + z*z + R2 - r2) - 8.0f*R2*x;
    float y_normal = 4.0f*y*(x*x + y*y + z*z + R2 - r2) - 8.0f*R2*y;
    float z_normal = 4.0f*z*(x*x + y*y + z*z + R2 - r2); 
    
    
    /*
    float x_normal = 4.0f*x*(x*x + y*y + z*z - 1.0625f);
    float y_normal = 4.0f*y*(x*x + y*y + z*z - 1.0625f);
    float z_normal = 4.0f*z*(x*x + y*y + z*z);
   */ 
    Vector local_normal(x_normal, y_normal, z_normal);
    
    if (local_ray_direction.dot(local_normal.normalized()) < 0)
        return Ray(impact, local_to_global_vector(local_normal).normalized());

    return Ray(impact, local_to_global_vector(local_normal * -1).normalized());
}

Material Tore::get_material(const Vector& impact) const
{
    Vector local_impact = global_to_local_point(impact);
    float x = local_impact[0], y = local_impact[1], z = local_impact[2];

    if (z >= 0)
    {
        if (x < 0 && y >= 0 || x >= 0 && y < 0)
            return _material;

        return _material2;
    }

    else
    {
        if (x < 0 && y >= 0 || x >= 0 && y < 0)
            return _material2;

        return _material;
    }    
}
