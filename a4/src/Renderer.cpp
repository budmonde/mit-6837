#include "Renderer.h"

#include "ArgParser.h"
#include "Camera.h"
#include "Image.h"
#include "Ray.h"
#include "VecUtils.h"

#include <limits>


Renderer::Renderer(const ArgParser &args) :
    _args(args),
    _scene(args.input_file) {
}

void Renderer::Render() {
    int w = _args.width;
    int h = _args.height;

    Image image(w, h);
    Image nimage(w, h);
    Image dimage(w, h);

    // loop through all the pixels in the image
    // generate all the samples

    // This look generates camera rays and callse traceRay.
    // It also write to the color, normal, and depth images.
    // You should understand what this code does.
    Camera* cam = _scene.getCamera();
    for (int y = 0; y < h; ++y) {
        float ndcy = 2 * (y / (h - 1.0f)) - 1.0f;
        for (int x = 0; x < w; ++x) {
            float ndcx = 2 * (x / (w - 1.0f)) - 1.0f;
            // Use PerspectiveCamera to generate a ray.
            // You should understand what generateRay() does.
            Ray r = cam->generateRay(Vector2f(ndcx, ndcy));

            Hit h;
            Vector3f color = traceRay(r, cam->getTMin(), _args.bounces, h);

            image.setPixel(x, y, color);
            nimage.setPixel(x, y, (h.getNormal() + 1.0f) / 2.0f);
            float range = (_args.depth_max - _args.depth_min);
            if (range) {
                dimage.setPixel(x, y, Vector3f((h.t - _args.depth_min) / range));
            }
        }
    }
    // END SOLN

    // save the files 
    if (_args.output_file.size()) {
        image.savePNG(_args.output_file);
    }
    if (_args.depth_file.size()) {
        dimage.savePNG(_args.depth_file);
    }
    if (_args.normals_file.size()) {
        nimage.savePNG(_args.normals_file);
    }
}



Vector3f Renderer::traceRay(const Ray &r,
    float tmin,
    int bounces,
    Hit &h) const {
    // The starter code only implements basic drawing of sphere primitives.
    // You will implement phong shading, recursive ray tracing, and shadow rays.
    if (_scene.getGroup()->intersect(r, tmin, h)) {
        Vector3f out(0.f, 0.f, 0.f);
        // Add Ambient Light
        out += _scene.getAmbientLight() * h.getMaterial()->getDiffuseColor();
        // Add Direct Diffuse and Specular Light
        Vector3f p = r.pointAtParameter(h.getT());
        int numLights = _scene.getNumLights();
        for (int i=0; i < numLights; ++i) {
            Light * l = _scene.getLight(i);
            // calculate illumination by light source
            Vector3f tolight,
                     intensity;
            float distToLight;
            l->getIllumination(p, tolight, intensity, distToLight);
            if (_args.shadows) {
                // check for obstructions before reaching light
                float epsilon = 1e-2;
                Vector3f origin = p+tolight.normalized()*epsilon;
                Ray shdR(origin, tolight);
                Hit shdH;
                if (_scene.getGroup()->intersect(shdR, epsilon, shdH)) {
                    Vector3f distVec = shdR.pointAtParameter(shdH.getT()) - p;
                    if (distVec.abs() < distToLight)
                        continue;
                }
            }
            out += h.getMaterial()->shade(r,h,tolight,intensity);
        }
        // Add Indirect Light
        if (bounces > 0) {
            float epsilon = 1e-2;
            Vector3f dir = r.getDirection() - h.getNormal() * 2 * Vector3f::dot(r.getDirection(), h.getNormal());
            Vector3f origin = p+dir.normalized()*epsilon;
            Ray rflR(origin, dir);
            Hit rflH;
            out += h.getMaterial()->getSpecularColor() * traceRay(rflR, 0.f, bounces-1, rflH);
        }
        return out;
    }
    return _scene.getBackgroundColor(r.getDirection());
}

