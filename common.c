//Zero Hanami (C) 2020
#include "common.h"

s c S = sizeof(s)*8;
z c Z = ~((s)-1/2);

enum ternary S_Greater(v c * c S1, v c * c S2) { return *(s c * c)S1 < *(s c * c)S2 ? ternary_2 : *(s c * c)S1 > *(s c * c)S2 ? ternary_1 : ternary_0; }
enum ternary Z_Greater(v c * c Z1, v c * c Z2) { return *(z c * c)Z1 < *(z c * c)Z2 ? ternary_2 : *(z c * c)Z1 > *(z c * c)Z2 ? ternary_1 : ternary_0; }

v Copy(s Size, v * c O1, v c * c O2) { while(Size-- > 0) { ((u*)O1)[Size] = ((u*)O2)[Size]; } }

v Swap(s Size, v * c O1, v * c O2) { while(Size-- > 0) { u c O = ((u*)O1)[Size]; ((u*)O1)[Size] = ((u*)O2)[Size]; ((u*)O2)[Size] = O; } }
