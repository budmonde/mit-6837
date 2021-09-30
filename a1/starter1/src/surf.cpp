#include "surf.h"
#include "vertexrecorder.h"
using namespace std;

const float c_pi = 3.14159265358979323846f;

namespace
{
    
    // We're only implenting swept surfaces where the profile curve is
    // flat on the xy-plane.  This is a check function.
    static bool checkFlat(const Curve &profile)
    {
        for (unsigned i=0; i<profile.size(); i++)
            if (profile[i].V[2] != 0.0 ||
                profile[i].T[2] != 0.0 ||
                profile[i].N[2] != 0.0)
                return false;
    
        return true;
    }
}

// DEBUG HELPER
Surface quad() { 
	Surface ret;
	ret.VV.push_back(Vector3f(-1, -1, 0));
	ret.VV.push_back(Vector3f(+1, -1, 0));
	ret.VV.push_back(Vector3f(+1, +1, 0));
	ret.VV.push_back(Vector3f(-1, +1, 0));

	ret.VN.push_back(Vector3f(0, 0, 1));
	ret.VN.push_back(Vector3f(0, 0, 1));
	ret.VN.push_back(Vector3f(0, 0, 1));
	ret.VN.push_back(Vector3f(0, 0, 1));

	ret.VF.push_back(Tup3u(0, 1, 2));
	ret.VF.push_back(Tup3u(0, 2, 3));
	return ret;
}

Surface makeSurfRev(const Curve &profile, unsigned steps)
{
    Surface surface;
    
    if (!checkFlat(profile)) {
        cerr << "surfRev profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    for (size_t y=0; y < profile.size()-1; y++) {
      CurvePoint profile_p = profile[y];
      for (size_t x=0; x < steps; x++) {
        float angle = 2 * c_pi * x / steps;

        Matrix3f rot = Matrix3f::rotateY(angle);
        Matrix3f rot_n = rot.inverse().transposed();

        Vector3f new_p = rot * profile_p.V;
        Vector3f new_n = rot_n * (-profile_p.N);

        surface.VV.push_back(new_p);
        surface.VN.push_back(new_n);

        // The triangles look like:
        // a-b
        // |\|
        // c-d
        size_t a = y * steps + x,
               b = y * steps + ((x+1) % steps),
               c = (y+1) * steps + x,
               d = (y+1) * steps + ((x+1) % steps);
        surface.VF.push_back(Tup3u(a, c, d));
        surface.VF.push_back(Tup3u(a, d, b));
      }
    }
 
    return surface;
}

Surface makeGenCyl(const Curve &profile, const Curve &sweep )
{
    Surface surface;

    if (!checkFlat(profile)) {
        cerr << "genCyl profile curve must be flat on xy plane." << endl;
        exit(0);
    }

    for (size_t y=0; y < profile.size()-1; y++) {
      CurvePoint profile_p = profile[y];
      for (size_t x=0; x < sweep.size(); x++) {
        CurvePoint sweep_p = sweep[x];

        Matrix3f rot(sweep_p.N, sweep_p.B, sweep_p.T);
        Matrix3f rot_n = rot.inverse().transposed();

        Vector3f new_p = rot * profile_p.V + sweep_p.V;
        Vector3f new_n = rot_n * (-profile_p.N);

        surface.VV.push_back(new_p);
        surface.VN.push_back(new_n);

        // The triangles look like:
        // a-b
        // |\|
        // c-d
        size_t a = y * sweep.size() + x,
               b = y * sweep.size() + ((x+1) % sweep.size()),
               c = (y+1) * sweep.size() + x,
               d = (y+1) * sweep.size() + ((x+1) % sweep.size());
        surface.VF.push_back(Tup3u(a, c, d));
        surface.VF.push_back(Tup3u(a, d, b));
      }
    }

    return surface;
}

void recordSurface(const Surface &surface, VertexRecorder* recorder) {
	const Vector3f WIRECOLOR(0.4f, 0.4f, 0.4f);
    for (int i=0; i<(int)surface.VF.size(); i++)
    {
		recorder->record(surface.VV[surface.VF[i][0]], surface.VN[surface.VF[i][0]], WIRECOLOR);
		recorder->record(surface.VV[surface.VF[i][1]], surface.VN[surface.VF[i][1]], WIRECOLOR);
		recorder->record(surface.VV[surface.VF[i][2]], surface.VN[surface.VF[i][2]], WIRECOLOR);
    }
}

void recordNormals(const Surface &surface, VertexRecorder* recorder, float len)
{
	const Vector3f NORMALCOLOR(0, 1, 1);
    for (int i=0; i<(int)surface.VV.size(); i++)
    {
		recorder->record_poscolor(surface.VV[i], NORMALCOLOR);
		recorder->record_poscolor(surface.VV[i] + surface.VN[i] * len, NORMALCOLOR);
    }
}

void outputObjFile(ostream &out, const Surface &surface)
{
    
    for (int i=0; i<(int)surface.VV.size(); i++)
        out << "v  "
            << surface.VV[i][0] << " "
            << surface.VV[i][1] << " "
            << surface.VV[i][2] << endl;

    for (int i=0; i<(int)surface.VN.size(); i++)
        out << "vn "
            << surface.VN[i][0] << " "
            << surface.VN[i][1] << " "
            << surface.VN[i][2] << endl;

    out << "vt  0 0 0" << endl;
    
    for (int i=0; i<(int)surface.VF.size(); i++)
    {
        out << "f  ";
        for (unsigned j=0; j<3; j++)
        {
            unsigned a = surface.VF[i][j]+1;
            out << a << "/" << "1" << "/" << a << " ";
        }
        out << endl;
    }
}
