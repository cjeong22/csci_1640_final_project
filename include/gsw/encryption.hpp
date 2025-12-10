#include "../../include/gsw/keys.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"

Matrix gadgetMatrixG(const Params &params);
Matrix encryptBit(int m, const PublicKey &pk, const Params &params, RNG &rng);