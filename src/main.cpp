#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <stdexcept>
#include <cmath>

using std::vector;
using std::cout;
using std::runtime_error;

#include "../include/utils/int64.hpp"
#include "../include/utils/matrix.hpp"
#include "../include/utils/rng.hpp"
#include "../include/gsw/params.hpp"
#include "../include/gsw/encryption.hpp"
#include "../include/gsw/keys.hpp"
#include "../include/gsw/operations.hpp"

// -----------------------------
// General integer decryption
//
// s' = (s, -1).
// [A|u] * s' = A s - u = -e (small).
// C s' = X^T (-e) + m T (G s').
// For our G, row 0 has G[0][n] = 1, so (G s')_0 = -1.
// Thus for row 0:
//   v0 = (C s')_0 ≈ -<Xcol0, e> - m T.
//
// After centering into (-q/2, q/2], we get
//   centered ≈ -m T + small_noise.
// Define y = -centered ≈ m T. Then:
//   m_hat ≈ round(y / T).
//
// max_m lets you clamp the decoded value (e.g. sum of bits -> up to 2).
// -----------------------------
int decryptInt(const Matrix &C, const SecretKey &sk, const Params &params, int max_m) {
    Vector sext(params.n + 1);
    for (int i = 0; i < params.n; ++i)
        sext[i] = sk.s[i];
    sext[params.n] = modq(-1, params.q); // q-1

    Vector v = matVecMulMod(C, sext, params.q);
    int64 coord = v[0];

    // center into (-q/2, q/2]
    int64 centered = coord;
    if (centered >= params.q / 2)
        centered -= params.q;

    int64 T = params.q / 8;
    double y = -static_cast<double>(centered);  // ~ m*T
    double est = y / static_cast<double>(T);   // ~ m

    int m_hat = (int)std::floor(est + 0.5);     // nearest integer
    if (m_hat < 0) m_hat = 0;
    if (m_hat > max_m) m_hat = max_m;
    return m_hat;
}

// Convenience wrapper for bits (m in {0,1})
int decryptBit(const Matrix &C, const SecretKey &sk, const Params &params) {
    int m_hat = decryptInt(C, sk, params, 1);
    return m_hat;
}

// -----------------------------
// Debug printers (optional)
// -----------------------------
void printVector(const Vector &v) {
    for (auto x : v) cout << x << " ";
    cout << "\n";
}

void printMatrix(const Matrix &M) {
    for (const auto &row : M) {
        for (auto x : row) cout << x << " ";
        cout << "\n";
    }
}

// -----------------------------
// Example usage
// -----------------------------
int main() {
    // Toy parameters (still small, still not secure)
    Params params;
    params.n   = 8;
    params.m   = 16;
    params.q   = 1 << 15;   // 32768
    params.B   = 2;
    params.ell = 8;

    RNG rng;
    SecretKey sk;
    PublicKey pk;

    cout << "Key generation...\n";
    keygen(params, rng, sk, pk);

    cout << "Secret key s:\n";
    printVector(sk.s);

    // Encrypt two bits
    int m1 = 1;
    int m2 = 1;

    cout << "\nEncrypting m1 = " << m1 << "...\n";
    Matrix C1 = encryptBit(m1, pk, params, rng);

    cout << "Encrypting m2 = " << m2 << "...\n";
    Matrix C2 = encryptBit(m2, pk, params, rng);

    // Homomorphic addition
    Matrix Csum = addCiphertexts(C1, C2, params);

    // Decrypt original bits
    int dm1 = decryptBit(C1, sk, params);
    int dm2 = decryptBit(C2, sk, params);

    // Decrypt the sum (expect ~2 when m1 = m2 = 1)
    int dsum = decryptInt(Csum, sk, params, 2);

    cout << "\nDecryption results:\n";
    cout << "Decrypted m1:    " << dm1 << "\n";
    cout << "Decrypted m2:    " << dm2 << "\n";
    cout << "Decrypted m1+m2: " << dsum << "\n";

    return 0;
}
