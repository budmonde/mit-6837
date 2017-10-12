#include "timestepper.h"

#include <cstdio>

using namespace std;

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize) {
    vector<Vector3f> curr = particleSystem->getState();
    vector<Vector3f> f = particleSystem->evalF(curr);
    for (int i=0; i < (int)curr.size(); ++i) {
        curr[i] += stepSize * f[i];
    }
    particleSystem->setState(curr);
}

void Trapezoidal::takeStep(ParticleSystem* particleSystem, float stepSize) {
    vector<Vector3f> curr = particleSystem->getState();
    vector<Vector3f> f0 = particleSystem->evalF(curr);

    vector<Vector3f> next;
    for (int i=0; i < (int)curr.size(); ++i) {
        next.push_back(curr[i] + stepSize * f0[i]);
    }
    vector<Vector3f> f1 = particleSystem->evalF(next);

    for (int i=0; i < (int)curr.size(); ++i) {
        curr[i] += (stepSize/2) * (f0[i] + f1[i]);
    }
    particleSystem->setState(curr);
}


void RK4::takeStep(ParticleSystem* particleSystem, float stepSize) {
}

