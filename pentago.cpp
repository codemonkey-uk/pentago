// pentago.cpp

#include "pentago.h"

// Quadrants
// AB
// CD

// + clockwise = (default)
// - anti-clockwise = (suffix r)

namespace pentago
{
    static const position A1(0,0);
    static const position A2(0,1);
    static const position A3(0,2);
    static const position A4(0,3);
    static const position A5(0,4);
    static const position A6(0,5);

    static const position B1(1,0);
    static const position B2(1,1);
    static const position B3(1,2);
    static const position B4(1,3);
    static const position B5(1,4);
    static const position B6(1,5);
    
    static const position C1(2,0);
    static const position C2(2,1);
    static const position C3(2,2);
    static const position C4(2,3);
    static const position C5(2,4);
    static const position C6(2,5);

    static const position D1(3,0);
    static const position D2(3,1);
    static const position D3(3,2);
    static const position D4(3,3);
    static const position D5(3,4);
    static const position D6(3,5);

    static const position E1(4,0);
    static const position E2(4,1);
    static const position E3(4,2);
    static const position E4(4,3);
    static const position E5(4,4);
    static const position E6(4,5);
    
    static const position F1(5,0);
    static const position F2(5,1);
    static const position F3(5,2);
    static const position F4(5,3);
    static const position F5(5,4);
    static const position F6(5,5);
    
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
    
    void board_18::transpose_ar()
    {
        // placeholder
        // 3x 90 degree rotations clockwise equals one rotation anticlockwise
        transpose_a();
        transpose_a();
        transpose_a();
    }
    
    // B+
    //   1 2 3 4 5 6
    // A      C4B4A4
    // B      C5  A5
    // C      C6B6A6
    // D
    // E
    // F  

    // this is as per A+ but with 
    // 1 => 4
    // 2 => 5
    // 3 => 6
    
    void board_18::transpose_b()
    {
        // A4 = C4
        state a4 = get(A4);
        setx( A4, get(C4) );
        
        // A5 = B4
        state a5 = get(A5);
        setx( A5, get(B4) );
        
        // A6 = A4
        state a6 = get(A6);
        setx( A6, a4 );
        
        // B4 = C6
        setx( B4, get(C5) );
        
        // B6 = A5
        state b6 = get(B6);
        setx( B6, a5 );
        
        // C4 = C6
        setx( C4, get(C6) );
        
        // C5 = B6
        setx( C5, b6 );
        
        // C6 = A6
        setx( C6, a6 );
    }    
    
    void board_18::transpose_br()
    {
        // placeholder
        // 3x 90 degree rotations clockwise equals one rotation anticlockwise
        transpose_b();
        transpose_b();
        transpose_b();
    }   
    
    // C+
    //   1 2 3 4 5 6
    // A 
    // B 
    // C 
    // D F1E1D1
    // E F2  D2
    // F F3E3D3
    
    // this is as per A+ but with 
    // C => F
    // B => E
    // A => D
    
    void board_18::transpose_c()
    {
        // D1 = F1
        state a1 = get(D1);
        setx( D1, get(F1) );
        
        // D2 = E1
        state a2 = get(D2);
        setx( D2, get(E1) );
        
        // D3 = D1
        state a3 = get(D3);
        setx( D3, a1 );
        
        // E1 = F3
        setx( E1, get(F2) );
        
        // E3 = D2
        state b3 = get(E3);
        setx( E3, a2 );
        
        // F1 = F3
        setx( F1, get(F3) );
        
        // F2 = E3
        setx( F2, b3 );
        
        // F3 = D3
        setx( F3, a3 );
    }
    
    void board_18::transpose_cr()
    {
        // placeholder
        // 3x 90 degree rotations clockwise equals one rotation anticlockwise
        transpose_c();
        transpose_c();
        transpose_c();
    }    

    // C+
    //   1 2 3 4 5 6
    // A 
    // B 
    // C 
    // D       F4E4D4
    // E       F5  D5
    // F       F6E5D6
    
    // this is as per A+ but with 
    // C => F, 1 => 4
    // B => E, 2 => 5
    // A => D, 3 => 6
    
    void board_18::transpose_d()
    {
        // D4 = F4
        state a4 = get(D4);
        setx( D4, get(F4) );
        
        // D5 = E4
        state a5 = get(D5);
        setx( D5, get(E4) );
        
        // D6 = D4
        state a6 = get(D6);
        setx( D6, a4 );
        
        // E4 = F6
        setx( E4, get(F5) );
        
        // E6 = D5
        state b6 = get(E6);
        setx( E6, a5 );
        
        // F4 = F6
        setx( F4, get(F6) );
        
        // F5 = E6
        setx( F5, b6 );
        
        // F6 = D6
        setx( F6, a6 );
    }
    
    void board_18::transpose_dr()
    {
        // placeholder
        // 3x 90 degree rotations clockwise equals one rotation anticlockwise
        transpose_d();
        transpose_d();
        transpose_d();
    }
    
    state board_18::winningrow(int x)
    {
        // [012345]
        // [?1234?]
        // winning rows are equal along 1234
        // AND either 0 or 5
        const state r = get(x,1);
        if (r==get(x,2) && r==get(x,3) && r==get(x,4))
            if (r==get(x,0) || r==get(x,5))
                return r;
        
        return empty;
    }

    state board_18::winningcol(int y)
    {
        // exactly the same logic as rows, but on the other axis
        const state r = get(1,y);
        if (r==get(2,y) && r==get(3,y) && r==get(4,y))
            if (r==get(0,y) || r==get(5,y))
                return r;
        
        return empty;
    }
}