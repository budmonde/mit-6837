#include "curve.h"
#include "vertexrecorder.h"
#include <assert.h>
using namespace std;

const float c_pi = 3.14159265358979323846f;
const Matrix4f BERNSTEIN_BASIS(
    1.f, -3.f,  3.f, -1.f,
    0.f,  3.f, -6.f,  3.f,
    0.f,  0.f,  3.f, -3.f,
    0.f,  0.f,  0.f,  1.f
    );
const Matrix4f BSPLINE_TEMP(
    1.f, -3.f,  3.f, -1.f,
    4.f,  0.f, -6.f,  3.f,
    1.f,  3.f,  3.f, -3.f,
    0.f,  0.f,  0.f,  1.f
    );
const Matrix4f BSPLINE_BASIS = BSPLINE_TEMP * (1.f/6.f);
const float epsilon = 0.0001f;

namespace
{
// Approximately equal to.  We don't want to use == because of
// precision issues with floating point.
inline bool approx(const Vector3f& lhs, const Vector3f& rhs)
{
	const float eps = 1e-8f;
	return (lhs - rhs).absSquared() < eps;
}


}


Curve evalBezier(const vector< Vector3f >& P, unsigned steps)
{
	// Check
	if (P.size() < 4 || P.size() % 3 != 1) {
		cerr << "evalBezier must be called with 3n+1 control points." << endl;
		exit(0);
	}

	// You should implement this function so that it returns a Curve
	// (e.g., a vector< CurvePoint >).  The variable "steps" tells you
	// the number of points to generate on each piece of the spline.
	// At least, that's how the sample solution is implemented and how
	// the SWP files are written.  But you are free to interpret this
	// variable however you want, so long as you can control the
	// "resolution" of the discretized spline curve with it.

	// Make sure that this function computes all the appropriate
	// Vector3fs for each CurvePoint: V,T,N,B.
	// [NBT] should be unit and orthogonal.

	// Also note that you may assume that all Bezier curves that you
	// receive have G1 continuity.  Otherwise, the TNB will not be
	// be defined at points where this does not hold.

  vector<CurvePoint> out = Curve();

  // Iteration variables
  size_t idx = 0;
  vector<Vector4f> current;

  Vector3f B_prev = -Vector3f::FORWARD;

  do { // Iterate through splines
    do { // Load CPs
      Vector4f point_padded(P[idx], 0.0f);
      current.push_back(point_padded);
      ++idx;
    } while (current.size() < 4);

    Matrix4f G(current[0], current[1], current[2], current[3]);

    for (size_t t_idx=0; t_idx < steps; t_idx++) {
      float t = (float)t_idx / (steps - 1);
      Vector4f basis(1, t, pow(t, 2), pow(t, 3));
      Vector4f basis_prime(0, 1, 2*t, 3*pow(t,2));

      Vector4f curve_point = G * BERNSTEIN_BASIS * basis;
      Vector4f curve_point_prime = G * BERNSTEIN_BASIS * basis_prime;

      CurvePoint point;
      point.V = curve_point.xyz();
      point.T = curve_point_prime.normalized().xyz();

      // corner case for init B choice
      if (Vector3f::cross(-Vector3f::FORWARD, point.T) == Vector3f::ZERO)
          B_prev = Vector3f::RIGHT;

      point.N = Vector3f::cross(B_prev, point.T).normalized();
      point.B = Vector3f::cross(point.T, point.N).normalized();

      B_prev = point.B;

      out.push_back(point);
    }
    current.erase(current.begin(), current.begin()+3);
  } while (idx < P.size());

  return out;
}

Curve evalBspline(const vector< Vector3f >& P, unsigned steps)
{
	// Check
	if (P.size() < 4) {
		cerr << "evalBspline must be called with 4 or more control points." << endl;
		exit(0);
	}

	// It is suggested that you implement this function by changing
	// basis from B-spline to Bezier.  That way, you can just call
	// your evalBezier function.

  // Iteration variables
  size_t idx = 0;
  vector<Vector4f> current;
  vector<Vector3f> bezier_basis;

  do { // Iterate through splines
    do { // Load CPs
      Vector4f point(P[idx], 0.0f);
      current.push_back(point);
      ++idx;
    } while (current.size() < 4);

    Matrix4f G(current[0], current[1], current[2], current[3]);
    Matrix4f bezier_points = G * BSPLINE_BASIS * BERNSTEIN_BASIS.inverse();

    const int COLS = 4;
    for (int col=0; col < COLS; col++) {
      // dismiss redundant first control point for all iterations except the first
      if (bezier_basis.size() >= 4 && col == 0) continue;
      bezier_basis.push_back(bezier_points.getCol(col).xyz());
    }

    current.erase(current.begin());
  } while (idx < P.size());

  return evalBezier(bezier_basis, steps);
}

Curve evalCircle(float radius, unsigned steps)
{
	// This is a sample function on how to properly initialize a Curve
	// (which is a vector< CurvePoint >).

	// Preallocate a curve with steps+1 CurvePoints
	Curve R(steps + 1);

	// Fill it in counterclockwise
	for (unsigned i = 0; i <= steps; ++i)
	{
		// step from 0 to 2pi
		float t = 2.0f * c_pi * float(i) / steps;

		// Initialize position
		// We're pivoting counterclockwise around the y-axis
		R[i].V = radius * Vector3f(cos(t), sin(t), 0);

		// Tangent vector is first derivative
		R[i].T = Vector3f(-sin(t), cos(t), 0);

		// Normal vector is second derivative
		R[i].N = Vector3f(-cos(t), -sin(t), 0);

		// Finally, binormal is facing up.
		R[i].B = Vector3f(0, 0, 1);
	}

	return R;
}

void recordCurve(const Curve& curve, VertexRecorder* recorder)
{
	const Vector3f WHITE(1, 1, 1);
	for (int i = 0; i < (int)curve.size() - 1; ++i)
	{
		recorder->record_poscolor(curve[i].V, WHITE);
		recorder->record_poscolor(curve[i + 1].V, WHITE);
	}
}
void recordCurveFrames(const Curve& curve, VertexRecorder* recorder, float framesize)
{
	Matrix4f T;
	const Vector3f RED(1, 0, 0);
	const Vector3f GREEN(0, 1, 0);
	const Vector3f BLUE(0, 0, 1);
	
	const Vector4f ORGN(0, 0, 0, 1);
	const Vector4f AXISX(framesize, 0, 0, 1);
	const Vector4f AXISY(0, framesize, 0, 1);
	const Vector4f AXISZ(0, 0, framesize, 1);

	for (int i = 0; i < (int)curve.size(); ++i)
	{
		T.setCol(0, Vector4f(curve[i].N, 0));
		T.setCol(1, Vector4f(curve[i].B, 0));
		T.setCol(2, Vector4f(curve[i].T, 0));
		T.setCol(3, Vector4f(curve[i].V, 1));
 
		// Transform orthogonal frames into model space
		Vector4f MORGN  = T * ORGN;
		Vector4f MAXISX = T * AXISX;
		Vector4f MAXISY = T * AXISY;
		Vector4f MAXISZ = T * AXISZ;

		// Record in model space
		recorder->record_poscolor(MORGN.xyz(), RED);
		recorder->record_poscolor(MAXISX.xyz(), RED);

		recorder->record_poscolor(MORGN.xyz(), GREEN);
		recorder->record_poscolor(MAXISY.xyz(), GREEN);

		recorder->record_poscolor(MORGN.xyz(), BLUE);
		recorder->record_poscolor(MAXISZ.xyz(), BLUE);
	}
}

