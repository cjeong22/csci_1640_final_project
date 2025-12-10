#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <stdexcept>
#include <cmath>

using std::vector;
using std::cout;
using std::runtime_error;
using int64 = long long;

using Matrix = vector<vector<int64>>;
using Vector = vector<int64>;

// -----------------------------
// Parameters
// -----------------------------
struct Params {
    int n;      // secret dimension
    int m;      // number of LWE samples
    int ell;    // gadget length (rows of G, C)
    int64 q;    // modulus
    int B;      // gadget base
};

struct SecretKey {
    Vector s; // length n
};

struct PublicKey {
    Matrix A; // m x n
    Vector u; // length m, u = A*s + e (mod q)
};

// -----------------------------
// Modular arithmetic
// -----------------------------
int64 modq(int64 x, int64 q) {
    int64 r = x % q;
    if (r < 0) r += q;
    return r;
}

// -----------------------------
// RNG + small error
// -----------------------------
struct RNG {
    std::mt19937_64 gen;
    std::uniform_int_distribution<int64> dist_uniform;

    RNG(int64 seed = 0)
        : gen(seed ? seed : std::random_device{}()),
          dist_uniform(0, std::numeric_limits<int64>::max())
    {}

    int64 uniformMod(int64 q) {
        return dist_uniform(gen) % q;
    }

    int bit() {
        return (int)(dist_uniform(gen) & 1);
    }

    int smallError() {
        int r = (int)(dist_uniform(gen) % 10);
        if (r == 0) return 1;
        if (r == 1) return -1;
        return 0;
    }
};

Matrix zeros(int rows, int cols) {
    return Matrix(rows, Vector(cols, 0));
}

Matrix randomMatrixMod(int rows, int cols, int64 q, RNG &rng) {
    Matrix M(rows, Vector(cols));
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            M[i][j] = rng.uniformMod(q);
    return M;
}

Vector randomVectorMod(int len, int64 q, RNG &rng) {
    Vector v(len);
    for (int i = 0; i < len; ++i)
        v[i] = rng.uniformMod(q);
    return v;
}

Vector matVecMulMod(const Matrix &A, const Vector &x, int64 q) {
    int rows = (int)A.size();
    int cols = (int)A[0].size();
    Vector res(rows, 0);
    for (int i = 0; i < rows; ++i) {
        long long acc = 0;
        for (int j = 0; j < cols; ++j)
            acc += A[i][j] * x[j];
        res[i] = modq(acc, q);
    }
    return res;
}

// -----------------------------
// Gadget matrix G
// G is ell x (n+1), only last column non-zero: G[i][n] = B^i
// -----------------------------
Matrix gadgetMatrixG(const Params &params) {
    Matrix G = zeros(params.ell, params.n + 1);
    for (int i = 0; i < params.ell; ++i) {
        int64 val = 1;
        for (int j = 0; j < i; ++j) {
            val = (val * params.B) % params.q;
        }
        G[i][params.n] = val;
    }
    return G;
}

// -----------------------------
// Key generation: u = A*s + e  (with small error e)
// -----------------------------
void keygen(const Params &params, RNG &rng, SecretKey &sk, PublicKey &pk) {
    sk.s = randomVectorMod(params.n, params.q, rng);
    pk.A = randomMatrixMod(params.m, params.n, params.q, rng);

    // small error vector e
    Vector e(params.m);
    for (int i = 0; i < params.m; ++i)
        e[i] = rng.smallError();

    Vector As = matVecMulMod(pk.A, sk.s, params.q);
    pk.u.resize(params.m);
    for (int i = 0; i < params.m; ++i) {
        pk.u[i] = modq(As[i] + e[i], params.q);
    }
}

// -----------------------------
// Encryption (bit) with scaled encoding
//
// C = X^T * [A | u] + m * T * G  (mod q)
//
// where T = q/4 gives a big gap between 0 and 1 clusters.
// -----------------------------
Matrix encryptBit(int m, const PublicKey &pk, const Params &params, RNG &rng) {
    if (m != 0 && m != 1) {
        throw runtime_error("encryptBit expects m in {0,1}");
    }

    // Build [A | u] (m x (n+1))
    Matrix Ahat(params.m, Vector(params.n + 1));
    for (int i = 0; i < params.m; ++i) {
        for (int j = 0; j < params.n; ++j) {
            Ahat[i][j] = pk.A[i][j];
        }
        Ahat[i][params.n] = pk.u[i];
    }

    Matrix G = gadgetMatrixG(params);
    Matrix C = zeros(params.ell, params.n + 1);

    int64 T = params.q / 8; // message scale

    for (int row = 0; row < params.ell; ++row) {
        // random binary column Xcol
        vector<int> Xcol(params.m);
        for (int i = 0; i < params.m; ++i)
            Xcol[i] = rng.bit();

        // sum_i Xcol[i] * Ahat[i]
        for (int i = 0; i < params.m; ++i) {
            if (!Xcol[i]) continue;
            for (int j = 0; j < params.n + 1; ++j)
                C[row][j] += Ahat[i][j];
        }

        // add m * T * G[row]
        if (m == 1) {
            for (int j = 0; j < params.n + 1; ++j) {
                int64 add = (T * G[row][j]) % params.q;
                C[row][j] += add;
            }
        }

        // reduce mod q
        for (int j = 0; j < params.n + 1; ++j)
            C[row][j] = modq(C[row][j], params.q);
    }

    return C;
}

// -----------------------------
// Homomorphic addition
//
// For GSW, addition is just matrix addition:
//   C_sum = C1 + C2 mod q
// This encodes m_sum = m1 + m2 (as integers).
// -----------------------------
Matrix addCiphertexts(const Matrix &C1, const Matrix &C2, const Params &params) {
    int rows = (int)C1.size();
    int cols = (int)C1[0].size();
    Matrix Csum = zeros(rows, cols);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            Csum[i][j] = modq(C1[i][j] + C2[i][j], params.q);
        }
    }
    return Csum;
}

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
