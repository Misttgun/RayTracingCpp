#pragma once

#ifndef _UTILS_H_
#define _UTILS_H_

#include "Vector.h"
#include <algorithm>

class Utils
{
public:
    static Vector refract(const Vector& normal, const Vector& direction, const float coef)
    {
        double cosi = std::max(-1.0f, std::min(normal.dot(direction), 1.0f));
        double etai = 1;
        double etat = coef;
        Vector n_normal = normal;

        if (cosi < 0)
        {
            cosi = -cosi;
        }
        else
        {
            std::swap(etai, etat);
            n_normal *= -1.0;
        }

        double eta = etai / etat;
        double k = 1 - eta * eta * (1 - cosi * cosi);
        Vector refracted = (k < 0) ? Vector(0, 0, 0) : direction * eta + normal * (eta * cosi - sqrt(k));

        //const float c1 = -(normal.dot(direction));
        //const float c2 = sqrt(1 - pow(coef, 2) * (1 - pow(c1, 2)));
        //const Vector refracted = (direction * coef) + (coef * c1 - c2) * normal;

        return refracted;
    }

    static Vector reflect(const Vector& normal, const Vector& direction)
    {
        Vector reflected = (2.0 * (direction.dot(normal))) * normal - direction;

        return reflected;
    }

    static double fresnel(double indice_of_refraction, const Vector& normal, const Vector& direction)
    {
        double cosi = std::max(-1.0f, std::min(normal.dot(direction), 1.0f));
        double etai = 1.0;
        double etat = indice_of_refraction;
        double kr;

        if (cosi > 0)
            std::swap(etai, etat);

        double sint = etai / etat * sqrt(std::max(0.0, 1.0 - cosi * cosi));

        if (sint >= 1.0)
            kr = 1.0;

        else
        {
            double cost = sqrt(std::max(0.0, 1.0 - sint * sint));
            cosi = abs(cosi);
            double rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
            double rp = ((etat * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
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