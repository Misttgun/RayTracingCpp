#include "Renderer.h"

Color Renderer::get_impact_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene, int depth) const
{
    const int nb_light = scene.nb_lights();
    Ray normal = obj.get_normal(impact, ray.origin);
    const Vector n_normal = normal.direction.normalized();
    const Vector to_cam = (ray.origin - impact).normalized();
    const Material curr_mat = obj.get_material(impact);

    Color specular;
    Color diffuse;

    if(curr_mat.reflect)
    {
        const Vector reflect = 2 * to_cam.dot(n_normal)* n_normal - to_cam;
        const Ray reflected_ray = Ray(impact + n_normal, reflect);
        Vector new_impact;
        return scene.cast_ray(reflected_ray, new_impact, *this, depth + 1);
    }

    for (int i = 0; i < nb_light; i++)
    {
        const auto curr_light = scene.get_light(i);
        Vector light_direction = curr_light->get_vector_to_light(impact).normalized();

        Color light_color = curr_light->color;
        Vector reflect = ((2 * (light_direction.dot(n_normal))) * n_normal - light_direction).normalized();

        const auto cos_a = n_normal.dot(light_direction);
        const auto cos_b = reflect.dot(to_cam);
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

    for (int i = 0; i < nb_light; i++)
    {
        const auto curr_light = scene.get_light(i);
        Vector light_direction = curr_light->get_vector_to_light(impact).normalized();
        Vector n_normal = normal.direction.normalized();

        Ray shadow_ray(impact + n_normal, light_direction);

        Vector shadow_impact;
        auto intersected = scene.closer_intersected(shadow_ray, shadow_impact);
        if (intersected)
            ++shadowed_by_n_lights;
    }

    const float iluminated_by_lights = nb_light - shadowed_by_n_lights;
    const float light_proportion = (iluminated_by_lights / nb_light);
    return (shadowed_by_n_lights > 0) ? light_proportion + shadow_min : 1;
}

void Renderer::save_ppm(std::string file, Color** pixel_map, int width, int height) const
{

}
