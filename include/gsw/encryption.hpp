#include "../../include/gsw/keys.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"

Matrix gadgetMatrixG(const Params &params);
Matrix gadgetInverse(const Matrix &C, const Params &params, const Matrix &G);
Matrix encryptInt(int m, const Params &p, RNG &rng, const Matrix &G);