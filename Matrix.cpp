#include "Matrix.h"

// Identity matrix definition
const FMatrix FMatrix::Identity = [] {
    FMatrix identity;
    for (int i = 0; i < 4; ++i)
        identity.M[i][i] = 1.0f;
    return identity;
    }();