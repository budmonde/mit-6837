#include "Material.h"

Vector3f Material::shade(const Ray &ray,
    const Hit &hit,
    const Vector3f &dirToLight,
    const Vector3f &lightIntensity) {

    float ldotn = max(0.f, Vector3f::dot(dirToLight, hit.getNormal()));
    float ldotr = max(0.f, Vector3f::dot(dirToLight, ray.getDirection()));
    Vector3f diff = ldotn * lightIntensity * _diffuseColor;
    Vector3f spec = pow(ldotr, _shininess) * lightIntensity * _specularColor;
    return diff + spec;
}
