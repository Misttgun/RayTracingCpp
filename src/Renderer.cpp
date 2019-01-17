#include "Renderer.h"

Color Renderer::get_impact_color(const Ray& ray, const Object& obj, const Vector& impact, const Scene& scene) const
{
    /*
	int nLight = scene.nb_lights();
	Material mat = obj.get_material();
	Ray N = obj.get_normal(impact, ray.origin);
	Color color = mat.ka * (scene.get_ambiant());

	for (int i = 0; i < nLight; i++)
	{
		auto light = scene.get_light(i);
		Vector L = light->get_vector_to_light(impact);
		Vector R = (L.dot(N.direction) * 2 * N.direction) - L;
		Vector V = ray.origin - impact;
		V = V.normalized();
		float beta = std::pow(R.dot(V), mat.shininess);
		float alpha = L.dot(N.direction);
		if (alpha > 0)
			color += mat.kd * (light->id) * alpha;
		if (beta > 0)
			color += mat.ks * (light->is) * beta;
	}

	return color;
    
    */
    
    int nb_light = scene.nb_lights();
    Ray normal = obj.get_normal(impact, ray.origin);
    Material current_material = obj.get_material();
    float shininess = current_material.shininess;

    Color ka = current_material.ka;
    Color kd = current_material.kd;
    Color ks = current_material.ks;
    Color ambiant = scene.get_ambiant();

    Color res = ka * ambiant;

    for (int i = 0; i < nb_light; i++)
    {
        auto current_light = scene.get_light(i);

        Vector light_direction = current_light->get_vector_to_light(impact).normalized();
        Vector n_normal = normal.direction.normalized();
        Vector to_cam = Vector(ray.origin - impact).normalized();

        Color id = current_light->id;
        Color is = current_light->is;

        Vector reflect = ((2 * (light_direction.dot(n_normal))) * n_normal - light_direction).normalized();

        auto cos_a = n_normal.dot(light_direction);

        // - la normale n'est pas dans la bonne direction
        if (cos_a < 0)
            continue;

        res += id * kd * cos_a;

        auto cos_b = reflect.dot(to_cam);

        if (cos_b < 0)
            continue;

        res += is * ks * pow(cos_b, shininess);
    }

    return res;
    
    
}

void Renderer::save_ppm(std::string file, Color** pixel_map, int width, int height) const
{
    
}