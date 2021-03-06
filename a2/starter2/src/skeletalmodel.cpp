#include "skeletalmodel.h"
#include <cassert>

#include "starter2_util.h"
#include "vertexrecorder.h"

using namespace std;

SkeletalModel::SkeletalModel() {
    program = compileProgram(c_vertexshader, c_fragmentshader_light);
    if (!program) {
        printf("Cannot compile program\n");
        assert(false);
    }
}

SkeletalModel::~SkeletalModel() {
    // destructor will release memory when SkeletalModel is deleted
    while (m_joints.size()) {
        delete m_joints.back();
        m_joints.pop_back();
    }

    glDeleteProgram(program);
}

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile) {
    loadSkeleton(skeletonFile);

    m_mesh.load(meshFile);
    m_mesh.loadAttachments(attachmentsFile, (int)m_joints.size());

    computeBindWorldToJointTransforms();
    updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(const Camera& camera, bool skeletonVisible) {
    // draw() gets called whenever a redraw is required
    // (after an update() occurs, when the camera moves, the window is resized, etc)

    m_matrixStack.clear();

    glUseProgram(program);
    updateShadingUniforms();
    if (skeletonVisible) {
        drawJoints(camera);
        drawSkeleton(camera);
    } else {
        // Tell the mesh to draw itself.
        // Since we transform mesh vertices on the CPU,
        // There is no need to set a Model matrix as uniform
        camera.SetUniforms(program, Matrix4f::identity());
        m_mesh.draw();
    }
    glUseProgram(0);
}

void SkeletalModel::updateShadingUniforms() {
    // UPDATE MATERIAL UNIFORMS
    GLfloat diffColor[] = { 0.4f, 0.4f, 0.4f, 1 };
    GLfloat specColor[] = { 0.9f, 0.9f, 0.9f, 1 };
    GLfloat shininess[] = { 50.0f };
    int loc = glGetUniformLocation(program, "diffColor");
    glUniform4fv(loc, 1, diffColor);
    loc = glGetUniformLocation(program, "specColor");
    glUniform4fv(loc, 1, specColor);
    loc = glGetUniformLocation(program, "shininess");
    glUniform1f(loc, shininess[0]);

    // UPDATE LIGHT UNIFORMS
    GLfloat lightPos[] = { 3.0f, 3.0f, 5.0f, 1.0f };
    loc = glGetUniformLocation(program, "lightPos");
    glUniform4fv(loc, 1, lightPos);

    GLfloat lightDiff[] = { 120.0f, 120.0f, 120.0f, 1.0f };
    loc = glGetUniformLocation(program, "lightDiff");
    glUniform4fv(loc, 1, lightDiff);
}

void SkeletalModel::loadSkeleton(const char* filename) {
    // Load the skeleton from file here.
    ifstream in(filename);

    if (!in) {
        cerr << filename << " not found\a" << endl;
        exit(0);
    }

    float jPos[3];
    int jParent;
    while (in >> jPos[0] >> jPos[1] >> jPos[2] >> jParent) {
        // Define the new joint
        Joint * j = new Joint;
        j->transform = Matrix4f::translation(jPos[0], jPos[1], jPos[2]);
        // Bind the joint to its parent
        if (jParent == -1)
            m_rootJoint = j;
        else
            m_joints[jParent]->children.push_back(j);
        // Add joint to list of joints
        m_joints.push_back(j);
    }

}

void SkeletalModel::drawJoints(const Camera& camera) {
    // Draw a sphere at each joint. You will need to add a recursive
    // helper function to traverse the joint hierarchy.
    //
    // We recommend using drawSphere( 0.025f, 12, 12 )
    // to draw a sphere of reasonable size.
    //
    // You should use your MatrixStack class. A function
    // should push it's changes onto the stack, and
    // use stack.pop() to revert the stack to the original
    // state.
    vector<Joint*> stack;
    stack.push_back(m_rootJoint);
    m_matrixStack.clear();
    while (stack.size()) {
        // Pop new joint
        Joint * joint = stack.back();
        stack.pop_back();

        // Pop joint's transform
        Matrix4f M = m_matrixStack.top() * joint->transform;
        m_matrixStack.pop();

        // Draw sphere at joint position
        camera.SetUniforms(program, M);
        drawSphere(0.025f, 12, 12);

        // Add child vertices to stack
        for (size_t i=0; i < joint->children.size(); i++) {
            stack.push_back(joint->children[i]);
            m_matrixStack.push(M);
        }
    }
}

void SkeletalModel::drawSkeleton(const Camera& camera) {
    // Draw cylinders between the joints. You will need to add a recursive 
    // helper function to traverse the joint hierarchy.
    //
    // We recommend using drawCylinder(6, 0.02f, <height>);
    // to draw a cylinder of reasonable diameter.
    vector<Joint*> stack;
    stack.push_back(m_rootJoint);
    m_matrixStack.clear();
    while (stack.size()) {
        // Pop new joint
        Joint * joint = stack.back();
        stack.pop_back();

        // Pop joint's transform
        Matrix4f M = m_matrixStack.top() * joint->transform;
        m_matrixStack.pop();

        for (size_t i=0; i < joint->children.size(); i++) {
            Vector3f Y = joint->children[i]->transform.getCol(3).xyz();
            if (Y != Vector3f::ZERO){
                Vector3f N = Vector3f::cross(Vector3f::UP, Y);
                float theta = acos(Vector3f::dot(Vector3f::UP, Y.normalized()));
                Matrix4f R = Matrix4f::rotation(N, theta);

                // Draw cylinder at joint position
                camera.SetUniforms(program, M*R);
                drawCylinder(6, 0.02f, Y.abs());
            }

            // Add child vertex to stack
            stack.push_back(joint->children[i]);
            m_matrixStack.push(M);
        }
    }
}

void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ) {
    // Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
    Joint * joint = m_joints[jointIndex];
    Matrix3f M = Matrix3f::rotateX(rX) * Matrix3f::rotateY(rY) * Matrix3f::rotateZ(rZ);
    joint->transform.setSubmatrix3x3(0,0,M);
}

void SkeletalModel::computeBindWorldToJointTransforms() {
    // 2.3.1. Implement this method to compute a per-joint transform from
    // world-space to joint space in the BIND POSE.
    //
    // Note that this needs to be computed only once since there is only
    // a single bind pose.
    //
    // This method should update each joint's bindWorldToJointTransform.
    // You will need to add a recursive helper function to traverse the joint hierarchy.
    vector<Joint*> stack;
    stack.push_back(m_rootJoint);
    m_matrixStack.clear();
    while (stack.size()) {
        // Pop new joint
        Joint * joint = stack.back();
        stack.pop_back();

        // Pop joint's transform
        joint->bindWorldToJointTransform = joint->transform.inverse() * m_matrixStack.top();
        m_matrixStack.pop();

        // Add child vertices to stack
        for (size_t i=0; i < joint->children.size(); i++) {
            stack.push_back(joint->children[i]);
            m_matrixStack.push(joint->bindWorldToJointTransform);
        }
    }

}

void SkeletalModel::updateCurrentJointToWorldTransforms() {
    // 2.3.2. Implement this method to compute a per-joint transform from
    // joint space to world space in the CURRENT POSE.
    //
    // The current pose is defined by the rotations you've applied to the
    // joints and hence needs to be *updated* every time the joint angles change.
    //
    // This method should update each joint's currentJointToWorldTransform.
    // You will need to add a recursive helper function to traverse the joint hierarchy.
    vector<Joint*> stack;
    stack.push_back(m_rootJoint);
    m_matrixStack.clear();
    while (stack.size()) {
        // Pop new joint
        Joint * joint = stack.back();
        stack.pop_back();

        // Pop joint's transform
        joint->currentJointToWorldTransform = m_matrixStack.top() * joint->transform;
        m_matrixStack.pop();

        // Add child vertices to stack
        for (size_t i=0; i < joint->children.size(); i++) {
            stack.push_back(joint->children[i]);
            m_matrixStack.push(joint->currentJointToWorldTransform);
        }
    }

}

void SkeletalModel::updateMesh() {
    // 2.3.2. This is the core of SSD.
    // Implement this method to update the vertices of the mesh
    // given the current state of the skeleton.
    // You will need both the bind pose world --> joint transforms.
    // and the current joint --> world transforms.
    for (size_t i=0; i < m_mesh.currentVertices.size(); i++) {
        vector<float> weights = m_mesh.attachments[i];
        Vector4f bind(m_mesh.bindVertices[i], 1.f);
        Vector4f current(0.f);
        for (size_t j=0; j < weights.size(); j++) {
            Joint * joint = m_joints[j];
            current = current + weights[j] * joint->currentJointToWorldTransform * joint->bindWorldToJointTransform * bind;
        }
        m_mesh.currentVertices[i] = current.xyz();
    }
}

