//Zero Hanami (C) 2020
#include "common.h"

enum ternary s_ternary_relation(s c * c S1, s c * c S2) { return *S1 < *S2 ? ternary_2 : *S1 > *S2 ? ternary_1 : ternary_0; }

enum ternary z_ternary_relation(z c * c Z1, z c * c Z2) { return *Z1 < *Z2 ? ternary_2 : *Z1 > *Z2 ? ternary_1 : ternary_0; }

v copy(s Size, v * c O1, v c * c O2) { while(Size-- > 0) { ((u*)O1)[Size] = ((u*)O2)[Size]; } }

v swap(s Size, v * c O1, v * c O2) { while(Size-- > 0) { u O = ((u*)O1)[Size]; ((u*)O1)[Size] = ((u*)O2)[Size]; ((u*)O2)[Size] = O; } }
