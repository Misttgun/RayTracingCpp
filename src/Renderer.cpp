#include "Renderer.h"
#include "Utils.h"

Color Renderer::get_impact_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene, int depth) const
{
    const int nb_light = scene.nb_lights();
    Ray normal = obj.get_normal(impact, ray.origin);
    const Vector n_normal = normal.direction.normalized();
    const Vector direction = (ray.origin - impact).normalized();
    const Material curr_mat = obj.get_material();

    Color specular;
    Color diffuse;

    if(curr_mat.mat_type == Type::Refraction)
    {
        //const Vector inside_ray_origin = impact + n_normal;
        //const Vector inside_ray_dir = Utils::refract(n_normal, direction, 1 / 1.33f); // Valeur en dur pour tester
        //const Ray ray_inside = Ray(inside_ray_origin, inside_ray_dir);
        //Vector inside_impact;
        //auto inside_obj = scene.closer_intersected(ray_inside, inside_impact);

        //if(inside_obj == nullptr)
        //    return scene.get_background();

        //Ray inside_normal = inside_obj->get_normal(inside_impact, ray_inside.origin);
        //const Vector inside_n_normal = inside_normal.direction.normalized();
        //const Vector inside_direction = (ray_inside.origin - inside_impact).normalized();
        //const Vector out_ray_origin = inside_impact + inside_n_normal;
        //const Vector out_ray_dir = Utils::refract(-1 *
        //                                          inside_n_normal, inside_direction, 1.33f); // Valeur en dur pour tester
        //const Ray ray_out = Ray(out_ray_origin, out_ray_dir);
        //Vector out_impact;

        //return scene.cast_ray(ray_out, out_impact, *this, depth + 1);
        // - Reflexion
        const Vector reflect = Utils::reflect(n_normal, direction);
        Vector reflect_origin = reflect.dot(n_normal) < 0 ? impact - n_normal : impact + n_normal;
        const Ray reflected_ray = Ray(reflect_origin, reflect);
        Vector reflected_impact;
        Color reflected_color = scene.cast_ray(reflected_ray, reflected_impact, *this, depth + 1);

        // - Refraction
        Vector refract = Utils::refract(-1 * n_normal, direction, 1.33f);
        Color refracted_color;

        if(refract != Vector(0.0f, 0.0f, 0.0f))
        {
            Vector refract_origin = refract.dot(n_normal) < 0 ? impact - n_normal : impact + n_normal;
            const Ray refracted_ray = Ray(refract_origin, refract);
            Vector refracted_impact;
            refracted_color = scene.cast_ray(refracted_ray, refracted_impact, *this, depth + 1);
        }
        else
        {
            refracted_color = scene.get_background();
        }

        double kr = Utils::fresnel(1.33f, n_normal, direction);

        return reflected_color * kr + refracted_color * (1 - kr);
    }

    if(curr_mat.mat_type == Type::Reflection)
    {
        const Vector reflect = Utils::reflect(n_normal, direction);
        Vector reflect_origin = reflect.dot(n_normal) < 0 ? impact - n_normal : impact + n_normal;
        const Ray reflected_ray = Ray(reflect_origin, reflect);
        Vector new_impact;
        return scene.cast_ray(reflected_ray, new_impact, *this, depth + 1);
    }

    for(int i = 0; i < nb_light; i++)
    {
        const auto curr_light = scene.get_light(i);
        Vector light_direction = curr_light->get_vector_to_light(impact).normalized();

        Color light_color = curr_light->color;
        Vector reflect = ((2 * (light_direction.dot(n_normal))) * n_normal - light_direction).normalized();

        const auto cos_a = n_normal.dot(light_direction);
        const auto cos_b = reflect.dot(direction);
        auto temp_color = (light_color * curr_mat.light_influence + curr_mat.color * (1 - curr_mat.light_influence));

        diffuse += temp_color * curr_mat.kd * std::fmax(cos_a, 0);
        specular += temp_color * curr_mat.ks * pow(std::fmax(cos_b, 0), curr_mat.shininess);
    }

    const Color ambient = (scene.get_ambiant() * curr_mat.light_influence + curr_mat.color * (1 - curr_mat.light_influence)) * curr_mat.ka;

    return ambient + specular + diffuse;
}

float Renderer::get_shadow_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene, int depth) const
{
    Ray normal = obj.get_normal(impact, ray.origin);
    const float shadow_min = .20f;
    int shadowed_by_n_lights = 0;

    const int nb_light = scene.nb_lights();

    for(int i = 0; i < nb_light; i++)
    {
        const auto curr_light = scene.get_light(i);
        Vector light_direction = curr_light->get_vector_to_light(impact).normalized();
        Vector n_normal = normal.direction.normalized();

        Ray shadow_ray(impact + n_normal, light_direction);

        Vector shadow_impact;
        auto intersected = scene.closer_intersected(shadow_ray, shadow_impact);
        if(intersected)
            ++shadowed_by_n_lights;
    }

    const float iluminated_by_lights = nb_light - shadowed_by_n_lights;
    const float light_proportion = (iluminated_by_lights / nb_light);
    return (shadowed_by_n_lights > 0) ? light_proportion + shadow_min : 1;
}

void Renderer::save_ppm(std::string file, Color** pixel_map, int width, int height) const
{

}
