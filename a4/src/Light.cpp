#include "Light.h"

void DirectionalLight::getIllumination(const Vector3f &p, 
                             Vector3f &tolight, 
                             Vector3f &intensity, 
                             float &distToLight) const {
    // the direction to the light is the opposite of the
    // direction of the directional light source

    // BEGIN STARTER
    tolight = -_direction;
    intensity  = _color;
    distToLight = std::numeric_limits<float>::max();
    // END STARTER
}
void PointLight::getIllumination(const Vector3f &p, 
                             Vector3f &tolight, 
                             Vector3f &intensity, 
                             float &distToLight) const {
    // tolight, intensity, distToLight are outputs
    tolight = _position - p;
    intensity = _color / (tolight.absSquared() * _falloff);
    distToLight = tolight.abs();
}

