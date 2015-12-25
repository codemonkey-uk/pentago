// pentago.cpp

#include "pentago.h"

// Quadrants
// AB
// CD

// + clockwise = 0
// - anti-clockwise = 1

namespace pentago
{
    static const position A1(0,0);
    static const position A2(0,1);
    static const position A3(0,2);
    static const position B1(1,0);
    static const position B2(1,1);
    static const position B3(1,2);
    static const position C1(2,0);
    static const position C2(2,1);
    static const position C3(2,2);
    
    // A+
    //   1 2 3 4 5 6
    // A C1B1A1
    // B C2  A2
    // C C3B3A3
    // D
    // E
    // F
    
    void board_18::transpose_a()
    {
        // A1 = C1
        state a1 = get(A1);
        setx( A1, get(C1) );
        
        // A2 = B1
        state a2 = get(A2);
        setx( A2, get(B1) );
        
        // A3 = A1
        state a3 = get(A3);
        setx( A3, a1 );
        
        // B1 = C3
        setx( B1, get(C2) );
        
        // B3 = A2
        state b3 = get(B3);
        setx( B3, a2 );
        
        // C1 = C3
        setx( C1, get(C3) );
        
        // C2 = B3
        setx( C2, b3 );
        
        // C3 = A3
        setx( C3, a3 );
    }
}