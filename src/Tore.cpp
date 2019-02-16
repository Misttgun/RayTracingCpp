#include "Tore.h"

bool Tore::intersect(const Ray& ray, Vector& impact) const
{
    Ray local_ray = global_to_local(ray);
    Vector local_ori = local_ray.origin;
    Vector local_dir = local_ray.direction.normalized();

    // - define tore radius size
    const float r = 0.25f;

    const float ox = local_ori[0];
    const float oy = local_ori[1];
    const float oz = local_ori[2];
    const float dz = local_dir[2];
    const float od = local_ori.dot(local_dir);
    const float sq_od = pow(od, 2.0f);
    const float sq_o = local_ori.dot(local_ori);
    const float sq_R = 1.0f;
    const float sq_r = pow(r, 2.0f);
    const float sq_ox = ox * ox;
    const float sq_oy = oy * oy;
    const float sq_oz = oz * oz;

    // - compute coefficients
    const float b = 4.0f * od;
    const float c = 2.0f * (sq_o + sq_R - sq_r) - 4.0f * sq_R * (1.0f - dz * dz) + 4.0f * sq_od;
    const float d = 8.0f * sq_R * (dz * oz) + 4.0f * od * (sq_o - sq_R - sq_r);
    float e = pow(ox, 4.0f) + pow(oy, 4.0f) + pow(oz, 4.0f) + pow(sq_R - sq_r, 2.0f);
    e += 2.0f * (sq_ox * sq_oy + sq_oz * (sq_R - sq_r) + (sq_ox + sq_oy) * (sq_oz - sq_R - sq_r));

    float t = -1.0f;
    if (!solve_polynomial_4(b, c, d, e, t))
        return false;

    const Vector local_impact = local_ray.origin + t * local_ray.direction.normalized();

    impact = local_to_global_point(local_impact);

    return true;
}

Ray Tore::get_normal(const Vector& impact, const Vector& observator) const
{
    Vector local_impact = global_to_local_point(impact);
    const Vector local_obs = global_to_local_point(observator);
    Vector local_ray_direction = local_impact - local_obs;

    // tore formula : (x^2 + y^2 + z^2 + R^2 + r^2) - 4*(x^2 + z^2) = 0
    // gradient is the vector of partial derivative with respect to all variables
    // df / dx = 4 * x * (x^2 + y^2 + z^2 + R^2 - r^2) - 8 * R^2 * x
    // df / dy = 4 * y * (x^2 + y^2 + z^2 + R^2 - r^2) - 8 * R^2 * y
    // df / dz = 4 * z * (x^2 + y^2 + z^2 + R^2 - r^2) - 8 * R^2 

    const float x = local_impact[0];
    const float y = local_impact[1];
    const float z = local_impact[2];
    const float R2 = 1.0f;
    const float r2 = 0.25f * 0.25f;


    const float x_normal = 4.0f*x*(x*x + y * y + z * z + R2 - r2) - 8.0f*R2*x;
    const float y_normal = 4.0f*y*(x*x + y * y + z * z + R2 - r2) - 8.0f*R2*y;
    const float z_normal = 4.0f*z*(x*x + y * y + z * z + R2 - r2);


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
    const float x = local_impact[0], y = local_impact[1], z = local_impact[2];

    if (z >= 0)
    {
        if (x < 0 && y >= 0 || x >= 0 && y < 0)
            return _material;

        return _material2;
    }
    
    if (x < 0 && y >= 0 || x >= 0 && y < 0)
        return _material2;

    return _material;
}
