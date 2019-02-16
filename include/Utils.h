#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "Vector.h"
#include <algorithm>
#include <cmath>

class Utils
{
public:
    static Vector refract(const Vector& normal, const Vector& direction, const float coef)
    {
        float cosi = std::max(-1.0f, std::min(normal.dot(direction), 1.0f));
        Vector n_normal = normal;

        if (cosi < 0)
        {
            cosi = -cosi;
        }
        else
        {
            n_normal *= -1.0f;
        }

        const float eta = 1 / coef;
        const float k = 1 - eta * eta * (1 - cosi * cosi);
        Vector refracted = (k < 0) ? Vector(0, 0, 0) : direction * eta + n_normal * (eta * cosi - std::sqrt(k));

        return refracted.normalized();
    }

    static Vector reflect(const Vector& normal, const Vector& direction)
    {
        Vector reflected = direction - (2.0f * (direction.dot(normal))) * normal;

        return reflected.normalized();
    }

    static float fresnel(float indice_of_refraction, const Vector& normal, const Vector& direction)
    {
        float cosi = std::max(-1.0f, std::min(normal.dot(direction), 1.0f));
        float etai = 1.0f;
        float etat = indice_of_refraction;
        float kr;

        if (cosi > 0)
            std::swap(etai, etat);

        const float sint = etai / etat * std::sqrt(std::max(0.0f, 1.0f - cosi * cosi));

        if (sint >= 1.0)
            kr = 1.0;

        else
        {
            const float cost = std::sqrt(std::max(0.0f, 1.0f - sint * sint));
            cosi = std::abs(cosi);
            const float rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            const float rp = ((etat * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
            kr = (rs * rs + rp * rp) / 2;
        }

        return kr;
    }

    static bool float_comp(float a, float b)
    {
        return std::abs(b - a) < 0.0001f;
    }
};

#endif 
