#include "Material.h"

Vector3f Material::shade(const Ray &ray,
    const Hit &hit,
    const Vector3f &dirToLight,
    const Vector3f &lightIntensity) {

    float ldotn = max(0.f, Vector3f::dot(dirToLight.normalized(), hit.getNormal().normalized()));
    Vector3f R = ray.getDirection() - hit.getNormal().normalized() * 2 * Vector3f::dot(ray.getDirection(), hit.getNormal());
    float ldotr = max(0.f, Vector3f::dot(dirToLight.normalized(), R.normalized()));
    Vector3f diff = ldotn * lightIntensity * _diffuseColor;
    Vector3f spec = pow(ldotr, _shininess) * lightIntensity * _specularColor;
    return diff + spec;
}
