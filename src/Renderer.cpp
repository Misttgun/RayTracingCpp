#include "Renderer.h"

Color Renderer::get_impact_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene) const
{
    const int nb_light = scene.nb_lights();
    Ray normal = obj.get_normal(impact, ray.origin);
    const Material curr_mat = obj.get_material();

    Color specular;
    Color diffuse;
    const float shadow_min = .25;
    int shadowed_by_n_lights = 0;

    for (int i = 0; i < nb_light; i++)
    {
        const auto curr_light = scene.get_light(i);
        Vector light_direction = curr_light->get_vector_to_light(impact).normalized();
        Vector n_normal = normal.direction.normalized();
        Vector to_cam = Vector(ray.origin - impact).normalized();

        Ray shadow_ray(impact + n_normal, light_direction);
        Color light_color = curr_light->color;

        if (shadow)
        {
            Vector shadow_impact;
            auto intersected = scene.closer_intersected(shadow_ray, shadow_impact);
            if (intersected)
            {
                ++shadowed_by_n_lights;
            }
        }

        Vector reflect = ((2 * (light_direction.dot(n_normal))) * n_normal - light_direction).normalized();

        const auto cos_a = n_normal.dot(light_direction);
        const auto cos_b = reflect.dot(to_cam);
        auto temp_color = (light_color * curr_mat.light_influence + curr_mat.color * (1 - curr_mat.light_influence));

        diffuse += temp_color * curr_mat.kd * std::fmax(cos_a, 0);
        specular += temp_color * curr_mat.ks * pow(std::fmax(cos_b, 0), curr_mat.shininess);
    }

    const Color ambient = (scene.get_ambiant() * curr_mat.light_influence + curr_mat.color * (1 - curr_mat.light_influence)) * curr_mat.ka;

    const float iluminated_by_lights = nb_light - shadowed_by_n_lights;
    const float light_proportion = (iluminated_by_lights / nb_light);
    float shadow_multiplier;
    shadowed_by_n_lights > 0 ? shadow_multiplier = light_proportion + shadow_min : shadow_multiplier = 1;

    return (ambient + specular + diffuse) * shadow_multiplier;


}

float Renderer::get_shadow_color(const Vector& impact, const Scene& scene) const
{
    const float shadow_min = .45;
    int shadowed_by_n_lights = 0;

    const int nb_light = scene.nb_lights();

    for (int i = 0; i < nb_light; i++)
    {
        const auto curr_light = scene.get_light(i);
        Vector light_direction = curr_light->get_vector_to_light(impact).normalized();
        Ray shadow_ray(impact * 0.8f, light_direction * 0.8f);

        Vector shadow_impact;
        auto intersected = scene.closer_intersected(shadow_ray, shadow_impact);
        if (intersected)
        {
            ++shadowed_by_n_lights;
        }
    }

    const float iluminated_by_lights = nb_light - shadowed_by_n_lights;
    const float light_proportion = (iluminated_by_lights / nb_light);
    return (shadowed_by_n_lights > 0) ? light_proportion + shadow_min : 1;
}

void Renderer::save_ppm(std::string file, Color** pixel_map, int width, int height) const
{

}
