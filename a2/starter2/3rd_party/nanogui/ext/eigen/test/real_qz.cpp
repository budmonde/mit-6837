// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2012 Alexey Korepanov <kaikaikai@yandex.ru>
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "main.h"
#include <limits>
#include <Eigen/Eigenvalues>

template<typename MatrixType> void real_qz(const MatrixType& m)
{
  /* this test covers the following files:
     RealQZ.h
  */
  using std::abs;
  typedef typename MatrixType::Index Index;
  typedef typename MatrixType::Scalar Scalar;
  
  Index dim = m.cols();
  
  MatrixType A = MatrixType::Random(dim,dim),
             B = MatrixType::Random(dim,dim);

  RealQZ<MatrixType> qz(A,B);
  
  VERIFY_IS_EQUAL(qz.info(), Success);
  // check for zeros
  bool all_zeros = true;
  for (Index i=0; i<A.cols(); i++)
    for (Index j=0; j<i; j++) {
      if (abs(qz.matrixT()(i,j))!=Scalar(0.0))
        all_zeros = false;
      if (j<i-1 && abs(qz.matrixS()(i,j))!=Scalar(0.0))
        all_zeros = false;
      if (j==i-1 && j>0 && abs(qz.matrixS()(i,j))!=Scalar(0.0) && abs(qz.matrixS()(i-1,j-1))!=Scalar(0.0))
        all_zeros = false;
    }
  VERIFY_IS_EQUAL(all_zeros, true);
  VERIFY_IS_APPROX(qz.matrixQ()*qz.matrixS()*qz.matrixZ(), A);
  VERIFY_IS_APPROX(qz.matrixQ()*qz.matrixT()*qz.matrixZ(), B);
  VERIFY_IS_APPROX(qz.matrixQ()*qz.matrixQ().adjoint(), MatrixType::Identity(dim,dim));
  VERIFY_IS_APPROX(qz.matrixZ()*qz.matrixZ().adjoint(), MatrixType::Identity(dim,dim));
}

void test_real_qz()
{
  int s = 0;
  for(int i = 0; i < g_repeat; i++) {
    CALL_SUBTEST_1( real_qz(Matrix4f()) );
    s = internal::random<int>(1,EIGEN_TEST_MAX_SIZE/4);
    CALL_SUBTEST_2( real_qz(MatrixXd(s,s)) );

    // some trivial but implementation-wise tricky cases
    CALL_SUBTEST_2( real_qz(MatrixXd(1,1)) );
    CALL_SUBTEST_2( real_qz(MatrixXd(2,2)) );
    CALL_SUBTEST_3( real_qz(Matrix<double,1,1>()) );
    CALL_SUBTEST_4( real_qz(Matrix2d()) );
  }
  
  TEST_SET_BUT_UNUSED_VARIABLE(s)
}
