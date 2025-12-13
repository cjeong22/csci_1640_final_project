#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <cmath>

#include "./../include/gsw/decryption.hpp"
#include "./../include/gsw/encryption.hpp"
#include "./../include/gsw/operations.hpp"
#include "./../include/gsw/params.hpp"

#include "./../include/utils/int64.hpp"
#include "./../include/utils/matrix.hpp"

int main() {
    Params p{8, (1LL << 15), 2};
    RNG rng;
    Matrix G = gadgetMatrixG(p);

    const int MAX_M = 5; 

    int m1 = 3;
    int m2 = 4;

    Matrix C1   = encryptInt(m1, p, rng, G);
    Matrix C2   = encryptInt(m2, p, rng, G);
    Matrix Cadd = addC(C1, C2, p);
    Matrix Cmul = mulC(C1, C2, p, G);

    int dec1   = decryptInt(C1,   p, G, MAX_M);
    int dec2   = decryptInt(C2,   p, G, MAX_M);
    int decAdd = decryptInt(Cadd, p, G, 2 * MAX_M);    
    int decMul = decryptInt(Cmul, p, G, MAX_M * MAX_M); 

    std::cout << "m1 = " << m1 << ", m2 = " << m2 << "\n";
    std::cout << "Dec(m1):        " << dec1   << "\n";
    std::cout << "Dec(m2):        " << dec2   << "\n";
    std::cout << "Dec(m1 + m2):   " << decAdd << "\n";
    std::cout << "Dec(m1 * m2):   " << decMul << "\n\n";

    int trials = 1000;
    int ok = 0;

    for (int t = 0; t < trials; ++t) {
        int a = rng.randInt(MAX_M);
        int b = rng.randInt(MAX_M);

        Matrix Ca    = encryptInt(a, p, rng, G);
        Matrix Cb    = encryptInt(b, p, rng, G);
        Matrix Cadd2 = addC(Ca, Cb, p);
        Matrix Cmul2 = mulC(Ca, Cb, p, G);

        int da    = decryptInt(Ca,    p, G, MAX_M);
        int db    = decryptInt(Cb,    p, G, MAX_M);
        int dadd2 = decryptInt(Cadd2, p, G, 2 * MAX_M);
        int dmul2 = decryptInt(Cmul2, p, G, MAX_M * MAX_M);

        bool good =
            (da == a) &&
            (db == b) &&
            (dadd2 == a + b) &&
            (dmul2 == a * b);

        if (good) ok++;
        else {
            std::cout << "FAIL a=" << a << " b=" << b
                 << "  da=" << da
                 << " db=" << db
                 << " add=" << dadd2
                 << " mul=" << dmul2 << "\n";
        }
    }

    std::cout << "Passed " << ok << "/" << trials << " random trials.\n";
    return 0;
}
