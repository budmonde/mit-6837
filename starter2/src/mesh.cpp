#include "mesh.h"

#include "vertexrecorder.h"

using namespace std;

void Mesh::load( const char* filename ) {
    // 4.1. load() should populate bindVertices, currentVertices, and faces

    // Add your code here.

    // Load the vertices from file here.
    ifstream in(filename);

    if (!in) {
        cerr << filename << " not found\a" << endl;
        exit(0);
    }

    char type;
    float num[3];
    while (in >> type >> num[0] >> num[1] >> num[2]) {
        switch(type) {
            case 'v': {
                Vector3f vertex(num[0], num[1], num[2]);
                bindVertices.push_back(vertex);
                break;
            }
            case 'f': {
                Tuple3u face((size_t) num[0], (size_t) num[1], (size_t) num[2]);
                faces.push_back(face);
                break;
            }
        }
    }

    // make a copy of the bind vertices as the current vertices
    currentVertices = bindVertices;
}

void Mesh::draw() {
    // 4.2 Since these meshes don't have normals
    // be sure to generate a normal per triangle.
    // Notice that since we have per-triangle normals
    // rather than the analytical normals from
    // assignment 1, the appearance is "faceted".
    VertexRecorder rec;
    for (size_t i=0; i < faces.size(); i++) {
        Tuple3u face = faces[i];

        Vector3f a = currentVertices[face[1]-1] - currentVertices[face[0]-1];
        Vector3f b = currentVertices[face[2]-1] - currentVertices[face[0]-1];
        Vector3f N = Vector3f::cross(a, b).normalized();

        rec.record(currentVertices[face[0]-1], N);
        rec.record(currentVertices[face[1]-1], N);
        rec.record(currentVertices[face[2]-1], N);
    }
    rec.draw();
}

void Mesh::loadAttachments( const char* filename, int numJoints ) {
    // 4.3. Implement this method to load the per-vertex attachment weights
    // this method should update m_mesh.attachments

    // Load the attachments from file here.
    ifstream in(filename);

    if (!in) {
        cerr << filename << " not found\a" << endl;
        exit(0);
    }

    while(in) {
        vector<float> attachment;
        attachment.push_back(0.f);
        for (int i=1; i < numJoints; i++) {
            float weight;
            in >> weight;
            attachment.push_back(weight);
        }
        attachments.push_back(attachment);
    }
    // corner case of extra line being read
    attachments.pop_back();
}
