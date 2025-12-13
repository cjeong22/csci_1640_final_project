#include "../../include/gsw/decryption.hpp"
#include "../../include/gsw/encryption.hpp"
#include "../../include/gsw/params.hpp"
#include "../../include/utils/matrix.hpp"


int decryptInt(const Matrix &C, const Params &p, const Matrix &G, int max_m) {
    Matrix T = gadgetInverse(C, p, G);

    long double sumDiag = 0.0;
    for (int i = 0; i < p.k; ++i)
        sumDiag += T(i, i);

    long double avg = sumDiag / (long double)p.k;
    int m_hat = (int)std::floor(avg + 0.5);

    if (m_hat < 0) m_hat = 0;
    if (m_hat > max_m) m_hat = max_m;
    return m_hat;
}