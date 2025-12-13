#include <iostream>

#include "../../include/utils/matrix.hpp"
#include "../../include/utils/rng.hpp"
#include "../../include/gsw/params.hpp"

using std::cout;

Matrix::Matrix (int k_) : k(k_), a(k_ * k_, 0) {}

Matrix zeros(int k) {
    return Matrix(k);
}

Vector matVecMulMod(const Matrix &A, const Vector &x, int64 q) {
    int rows = (int)A.k;
    int cols = (int)A.k;
    Vector res(rows, 0);
    for (int i = 0; i < rows; ++i) {
        long long acc = 0;
        for (int j = 0; j < cols; ++j)
            acc += A(i, j) * x[j];
        res[i] = modq(acc, q);
    }
    return res;
}

Matrix matMulMod(const Matrix& A, const Matrix& B, const Params& p) {
  int n=A.k;
  Matrix C(n);
  const int BS = 32; 

  for(int ii=0; ii<n; ii+=BS)
    for(int kk=0; kk<n; kk+=BS)
      for(int jj=0; jj<n; jj+=BS)
        for(int i=ii; i<std::min(ii+BS,n); ++i)
          for(int k=kk; k<std::min(kk+BS,n); ++k){
            __int128 aik = A(i,k);
            for(int j=jj; j<std::min(jj+BS,n); ++j){
              __int128 v = aik * B(k,j) + C(i,j);
              C(i,j) = (int64_t)(v % p.q);
            }
          }

  for(auto &x: C.a){ x%=p.q; if(x<0) x+=p.q; }
  return C;
}

