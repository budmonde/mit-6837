#include "Object3D.h"

bool Sphere::intersect(const Ray &r, float tmin, Hit &h) const {
    // BEGIN STARTER

    // We provide sphere intersection code for you.
    // You should model other intersection implementations after this one.

    // Locate intersection point ( 2 pts )
    const Vector3f &rayOrigin = r.getOrigin(); //Ray origin in the world coordinate
    const Vector3f &dir = r.getDirection();

    Vector3f origin = rayOrigin - _center;      //Ray origin in the sphere coordinate

    float a = dir.absSquared();
    float b = 2 * Vector3f::dot(dir, origin);
    float c = origin.absSquared() - _radius * _radius;

    // no intersection
    if (b * b - 4 * a * c < 0) {
        return false;
    }

    float d = sqrt(b * b - 4 * a * c);

    float tplus = (-b + d) / (2.0f*a);
    float tminus = (-b - d) / (2.0f*a);

    // the two intersections are at the camera back
    if ((tplus < tmin) && (tminus < tmin)) {
        return false;
    }

    float t = 10000;
    // the two intersections are at the camera front
    if (tminus > tmin) {
        t = tminus;
    }

    // one intersection at the front. one at the back 
    if ((tplus > tmin) && (tminus < tmin)) {
        t = tplus;
    }

    if (t < h.getT()) {
        Vector3f normal = r.pointAtParameter(t) - _center;
        normal = normal.normalized();
        h.set(t, this->material, normal);
        return true;
    }
    // END STARTER
    return false;
}

// Add object to group
void Group::addObject(Object3D *obj) {
    m_members.push_back(obj);
}

// Return number of objects in group
int Group::getGroupSize() const {
    return (int)m_members.size();
}

bool Group::intersect(const Ray &r, float tmin, Hit &h) const {
    // BEGIN STARTER
    // we implemented this for you
    bool hit = false;
    for (Object3D* o : m_members) {
        if (o->intersect(r, tmin, h)) {
            hit = true;
        }
    }
    return hit;
    // END STARTER
}


Plane::Plane(const Vector3f &normal, float d, Material *m) : Object3D(m) {
    _normal = normal.normalized();
    _d = d;
}
bool Plane::intersect(const Ray &r, float tmin, Hit &h) const {
    float cosAngle = Vector3f::dot(_normal, r.getDirection());
    // does not intersect
    if (cosAngle == 0.f) {
        return false;
    }
    float t = (_d - Vector3f::dot(_normal, r.getOrigin())) / cosAngle;
    // intersection behind camera
    if (t < tmin) {
        return false;
    }
    if (t < h.getT()) {
        h.set(t, this->material, _normal);
        return true;
    }
    return false;
}
bool Triangle::intersect(const Ray &r, float tmin, Hit &h) const {
    // Solving the linear equation Ax = b
    Matrix3f A(_v[0] - _v[1], _v[0] - _v[2], r.getDirection());
    // x.x() = \beta, x.y() = \gamma, x.z() = t
    Vector3f x = A.inverse() * (_v[0] - r.getOrigin());
    // does not intersect
    if (x.x() < 0.f || x.y() < 0.f || x.x() + x.y() > 1.f) {
        return false;
    }
    // intersection behind camera
    if (x.z() < tmin) {
        return false;
    }
    if (x.z() < h.getT()) {
        Vector3f normal = _normals[0] * (1 - x.x() - x.y()) + _normals[1] * x.x() + _normals[2] * x.y();
        h.set(x.z(), this->material, normal.normalized());
        return true;
    }
    return false;
}


Transform::Transform(const Matrix4f &m,
    Object3D *obj) : _object(obj), _m(m) {
}
bool Transform::intersect(const Ray &r, float tmin, Hit &h) const {
    const Vector3f &originWS = r.getOrigin(); //Ray origin in the world coordinate
    const Vector3f &dirWS = r.getDirection();

    Vector4f originOS = _m.inverse() * Vector4f(originWS, 1.f);
    Vector4f dirOS = _m.inverse() * Vector4f(dirWS, 0.f);

    Ray rayOS(originOS.xyz(), dirOS.xyz());
    Hit hitOS;
    if (_object->intersect(rayOS, tmin, hitOS) && hitOS.getT() < h.getT()) {
        Vector3f nWS = _m.getSubmatrix3x3(0,0).inverse().transposed() * hitOS.getNormal();
        h.set(hitOS.getT(), _object->material, nWS.normalized());
        return true;
    }
    return false;
}
