// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2006-2008 Benoit Jacob <jacob.benoit.1@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "main.h"

template<typename MatrixType> void product_extra(const MatrixType& m)
{
  typedef typename MatrixType::Index Index;
  typedef typename MatrixType::Scalar Scalar;
  typedef Matrix<Scalar, 1, Dynamic> RowVectorType;
  typedef Matrix<Scalar, Dynamic, 1> ColVectorType;
  typedef Matrix<Scalar, Dynamic, Dynamic,
                         MatrixType::Flags&RowMajorBit> OtherMajorMatrixType;

  Index rows = m.rows();
  Index cols = m.cols();

  MatrixType m1 = MatrixType::Random(rows, cols),
             m2 = MatrixType::Random(rows, cols),
             m3(rows, cols),
             mzero = MatrixType::Zero(rows, cols),
             identity = MatrixType::Identity(rows, rows),
             square = MatrixType::Random(rows, rows),
             res = MatrixType::Random(rows, rows),
             square2 = MatrixType::Random(cols, cols),
             res2 = MatrixType::Random(cols, cols);
  RowVectorType v1 = RowVectorType::Random(rows), vrres(rows);
  ColVectorType vc2 = ColVectorType::Random(cols), vcres(cols);
  OtherMajorMatrixType tm1 = m1;

  Scalar s1 = internal::random<Scalar>(),
         s2 = internal::random<Scalar>(),
         s3 = internal::random<Scalar>();

  VERIFY_IS_APPROX(m3.noalias() = m1 * m2.adjoint(),                 m1 * m2.adjoint().eval());
  VERIFY_IS_APPROX(m3.noalias() = m1.adjoint() * square.adjoint(),   m1.adjoint().eval() * square.adjoint().eval());
  VERIFY_IS_APPROX(m3.noalias() = m1.adjoint() * m2,                 m1.adjoint().eval() * m2);
  VERIFY_IS_APPROX(m3.noalias() = (s1 * m1.adjoint()) * m2,          (s1 * m1.adjoint()).eval() * m2);
  VERIFY_IS_APPROX(m3.noalias() = ((s1 * m1).adjoint()) * m2,        (numext::conj(s1) * m1.adjoint()).eval() * m2);
  VERIFY_IS_APPROX(m3.noalias() = (- m1.adjoint() * s1) * (s3 * m2), (- m1.adjoint()  * s1).eval() * (s3 * m2).eval());
  VERIFY_IS_APPROX(m3.noalias() = (s2 * m1.adjoint() * s1) * m2,     (s2 * m1.adjoint()  * s1).eval() * m2);
  VERIFY_IS_APPROX(m3.noalias() = (-m1*s2) * s1*m2.adjoint(),        (-m1*s2).eval() * (s1*m2.adjoint()).eval());

  // a very tricky case where a scale factor has to be automatically conjugated:
  VERIFY_IS_APPROX( m1.adjoint() * (s1*m2).conjugate(), (m1.adjoint()).eval() * ((s1*m2).conjugate()).eval());


  // test all possible conjugate combinations for the four matrix-vector product cases:

  VERIFY_IS_APPROX((-m1.conjugate() * s2) * (s1 * vc2),
                   (-m1.conjugate()*s2).eval() * (s1 * vc2).eval());
  VERIFY_IS_APPROX((-m1 * s2) * (s1 * vc2.conjugate()),
                   (-m1*s2).eval() * (s1 * vc2.conjugate()).eval());
  VERIFY_IS_APPROX((-m1.conjugate() * s2) * (s1 * vc2.conjugate()),
                   (-m1.conjugate()*s2).eval() * (s1 * vc2.conjugate()).eval());

  VERIFY_IS_APPROX((s1 * vc2.transpose()) * (-m1.adjoint() * s2),
                   (s1 * vc2.transpose()).eval() * (-m1.adjoint()*s2).eval());
  VERIFY_IS_APPROX((s1 * vc2.adjoint()) * (-m1.transpose() * s2),
                   (s1 * vc2.adjoint()).eval() * (-m1.transpose()*s2).eval());
  VERIFY_IS_APPROX((s1 * vc2.adjoint()) * (-m1.adjoint() * s2),
                   (s1 * vc2.adjoint()).eval() * (-m1.adjoint()*s2).eval());

  VERIFY_IS_APPROX((-m1.adjoint() * s2) * (s1 * v1.transpose()),
                   (-m1.adjoint()*s2).eval() * (s1 * v1.transpose()).eval());
  VERIFY_IS_APPROX((-m1.transpose() * s2) * (s1 * v1.adjoint()),
                   (-m1.transpose()*s2).eval() * (s1 * v1.adjoint()).eval());
  VERIFY_IS_APPROX((-m1.adjoint() * s2) * (s1 * v1.adjoint()),
                   (-m1.adjoint()*s2).eval() * (s1 * v1.adjoint()).eval());

  VERIFY_IS_APPROX((s1 * v1) * (-m1.conjugate() * s2),
                   (s1 * v1).eval() * (-m1.conjugate()*s2).eval());
  VERIFY_IS_APPROX((s1 * v1.conjugate()) * (-m1 * s2),
                   (s1 * v1.conjugate()).eval() * (-m1*s2).eval());
  VERIFY_IS_APPROX((s1 * v1.conjugate()) * (-m1.conjugate() * s2),
                   (s1 * v1.conjugate()).eval() * (-m1.conjugate()*s2).eval());

  VERIFY_IS_APPROX((-m1.adjoint() * s2) * (s1 * v1.adjoint()),
                   (-m1.adjoint()*s2).eval() * (s1 * v1.adjoint()).eval());

  // test the vector-matrix product with non aligned starts
  Index i = internal::random<Index>(0,m1.rows()-2);
  Index j = internal::random<Index>(0,m1.cols()-2);
  Index r = internal::random<Index>(1,m1.rows()-i);
  Index c = internal::random<Index>(1,m1.cols()-j);
  Index i2 = internal::random<Index>(0,m1.rows()-1);
  Index j2 = internal::random<Index>(0,m1.cols()-1);

  VERIFY_IS_APPROX(m1.col(j2).adjoint() * m1.block(0,j,m1.rows(),c), m1.col(j2).adjoint().eval() * m1.block(0,j,m1.rows(),c).eval());
  VERIFY_IS_APPROX(m1.block(i,0,r,m1.cols()) * m1.row(i2).adjoint(), m1.block(i,0,r,m1.cols()).eval() * m1.row(i2).adjoint().eval());
  
  // regression test
  MatrixType tmp = m1 * m1.adjoint() * s1;
  VERIFY_IS_APPROX(tmp, m1 * m1.adjoint() * s1);
}

// Regression test for bug reported at http://forum.kde.org/viewtopic.php?f=74&t=96947
void mat_mat_scalar_scalar_product()
{
  Eigen::Matrix2Xd dNdxy(2, 3);
  dNdxy << -0.5, 0.5, 0,
           -0.3, 0, 0.3;
  double det = 6.0, wt = 0.5;
  VERIFY_IS_APPROX(dNdxy.transpose()*dNdxy*det*wt, det*wt*dNdxy.transpose()*dNdxy);
}
  
void zero_sized_objects()
{
  // Bug 127
  //
  // a product of the form lhs*rhs with
  //
  // lhs:
  // rows = 1, cols = 4
  // RowsAtCompileTime = 1, ColsAtCompileTime = -1
  // MaxRowsAtCompileTime = 1, MaxColsAtCompileTime = 5
  //
  // rhs:
  // rows = 4, cols = 0
  // RowsAtCompileTime = -1, ColsAtCompileTime = -1
  // MaxRowsAtCompileTime = 5, MaxColsAtCompileTime = 1
  //
  // was failing on a runtime assertion, because it had been mis-compiled as a dot product because Product.h was using the
  // max-sizes to detect size 1 indicating vectors, and that didn't account for 0-sized object with max-size 1.

  Matrix<float,1,Dynamic,RowMajor,1,5> a(1,4);
  Matrix<float,Dynamic,Dynamic,ColMajor,5,1> b(4,0);
  a*b;
}

void unaligned_objects()
{
  // Regression test for the bug reported here:
  // http://forum.kde.org/viewtopic.php?f=74&t=107541
  // Recall the matrix*vector kernel avoid unaligned loads by loading two packets and then reassemble then.
  // There was a mistake in the computation of the valid range for fully unaligned objects: in some rare cases,
  // memory was read outside the allocated matrix memory. Though the values were not used, this might raise segfault.
  for(int m=450;m<460;++m)
  {
    for(int n=8;n<12;++n)
    {
      MatrixXf M(m, n);
      VectorXf v1(n), r1(500);
      RowVectorXf v2(m), r2(16);

      M.setRandom();
      v1.setRandom();
      v2.setRandom();
      for(int o=0; o<4; ++o)
      {
        r1.segment(o,m).noalias() = M * v1;
        VERIFY_IS_APPROX(r1.segment(o,m), M * MatrixXf(v1));
        r2.segment(o,n).noalias() = v2 * M;
        VERIFY_IS_APPROX(r2.segment(o,n), MatrixXf(v2) * M);
      }
    }
  }
}

void test_product_extra()
{
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST_1( product_extra(MatrixXf(internal::random<int>(1,EIGEN_TEST_MAX_SIZE), internal::random<int>(1,EIGEN_TEST_MAX_SIZE))) );
    CALL_SUBTEST_2( product_extra(MatrixXd(internal::random<int>(1,EIGEN_TEST_MAX_SIZE), internal::random<int>(1,EIGEN_TEST_MAX_SIZE))) );
    CALL_SUBTEST_2( mat_mat_scalar_scalar_product() );
    CALL_SUBTEST_3( product_extra(MatrixXcf(internal::random<int>(1,EIGEN_TEST_MAX_SIZE/2), internal::random<int>(1,EIGEN_TEST_MAX_SIZE/2))) );
    CALL_SUBTEST_4( product_extra(MatrixXcd(internal::random<int>(1,EIGEN_TEST_MAX_SIZE/2), internal::random<int>(1,EIGEN_TEST_MAX_SIZE/2))) );
  }
  CALL_SUBTEST_5( zero_sized_objects() );
  CALL_SUBTEST_6( unaligned_objects() );
}
