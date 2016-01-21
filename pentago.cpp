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
        // reverse (anticlockwise) rotation of quadrant A
        
        // A1 => C1
        state c1 = get(C1);
        setx( C1, get(A1) );
        
        // A2 => B1
        state b1 = get(B1);
        setx( B1, get(A2) );
        
        // A3 => A1
        setx( A1, get(A3) );
        
        // B1 => C2
        state c2 = get(C2);
        setx( C2, b1 );
        
        // B3 => A2
        setx( A2, get(B3) );
        
        // C1 => C3
        state c3 = get(C3);        
        setx( C3, c1 );
        
        // C2 => B3
        setx( B3, c2 );
        
        // C3 => A3
        setx( A3, c3 );
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
        // reverse (anticlockwise) rotation of quadrant B
        
        // A1 => C1
        state c4 = get(C4);
        setx( C4, get(A4) );
        
        // A2 => B1
        state b4 = get(B4);
        setx( B4, get(A5) );
        
        // A3 => A1
        setx( A4, get(A6) );
        
        // B1 => C2
        state c5 = get(C5);
        setx( C5, b4 );
        
        // B3 => A2
        setx( A5, get(B6) );
        
        // C1 => C3
        state c6 = get(C6);        
        setx( C6, c4 );
        
        // C2 => B3
        setx( B6, c5 );
        
        // C3 => A3
        setx( A6, c6 );
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
        // reverse (anticlockwise) rotation of quadrant C
        
        // D1 => F1
        state f1 = get(F1);
        setx( F1, get(D1) );
        
        // D2 => e1
        state e1 = get(E1);
        setx( E1, get(D2) );
        
        // D3 => D1
        setx( D1, get(D3) );
        
        // E1 => F2
        state f2 = get(F2);
        setx( F2, e1 );
        
        // E3 => A2
        setx( D2, get(E3) );
        
        // F1 => F3
        state f3 = get(F3);        
        setx( F3, f1 );
        
        // F2 => E3
        setx( E3, f2 );
        
        // F3 => D3
        setx( D3, f3 );
    }    

    // D+
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
        // reverse (anticlockwise) rotation of quadrant D
        
        // D4 => F4
        state f4 = get(F4);
        setx( F4, get(D4) );
        
        // D5 => e4
        state e4 = get(E4);
        setx( E4, get(D5) );
        
        // D6 => D4
        setx( D4, get(D6) );
        
        // E4 => F5
        state f5 = get(F5);
        setx( F5, e4 );
        
        // E6 => A5
        setx( D5, get(E6) );
        
        // F4 => F6
        state f6 = get(F6);        
        setx( F6, f4 );
        
        // F5 => E6
        setx( E6, f5 );
        
        // F6 => D6
        setx( D6, f6 );
    }
    
    // Rotational symmetry in the A quadrant:
    //   1 2 3 4 5 6
    // A C3C2C1
    // B B3  B1
    // C A3A2A1
    // D
    // E
    // F
    // returns true if transpose_a == transpose_ar
    bool board_18::symetrical_a() const
    {
        // A1 = C1
        return 
            get(A1) == get(C3) &&
            get(A2) == get(C2) &&
            get(A3) == get(C1) &&
            get(B3) == get(B1);
    }
    
    // Rotational symmetry in the B quadrant:
    // this is as per A but with 
    // 1 => 4
    // 2 => 5
    // 3 => 6
    // returns true if transpose_b == transpose_br
    bool board_18::symetrical_b() const
    {
        // A1 = C1
        return 
            get(A4) == get(C6) &&
            get(A5) == get(C5) &&
            get(A6) == get(C4) &&
            get(B6) == get(B4);
    }
    
    // Rotational symmetry in the C quadrant:
    // this is as per A but with 
    // C => F
    // B => E
    // A => D
    // returns true if transpose_c == transpose_cr
    bool board_18::symetrical_c() const
    {
        return 
            get(D1) == get(F3) &&
            get(D2) == get(F2) &&
            get(D3) == get(F1) &&
            get(E3) == get(E1);
    }
    
    // Rotational symmetry in the D quadrant:
    // this is as per A+ but with 
    // C => F, 1 => 4
    // B => E, 2 => 5
    // A => D, 3 => 6
    // returns true if transpose_c == transpose_cr
    bool board_18::symetrical_d() const
    {
        return 
            get(D4) == get(F6) &&
            get(D5) == get(F5) &&
            get(D6) == get(F4) &&
            get(E6) == get(E4);
    }    
    
    
    state board_18::winningrow(int x)const
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

    state board_18::winningcol(int y)const
    {
        // exactly the same logic as rows, but on the other axis
        const state r = get(1,y);
        if (r==get(2,y) && r==get(3,y) && r==get(4,y))
            if (r==get(0,y) || r==get(5,y))
                return r;
        
        return empty;
    }
    
    state board_18::winningdiag()const
    {
        // 6 ways to win on the diag
        // center 0,0-5,5 line
        // + either side of it
        // and 0,5-5,0 line
        // + either side of it

        // both players can win at the same time
        state result = empty;
        
        // center diagnals have the same logic as rows and columns
        
        // 0,0-5,5 line
        state r = get(1,1);
        if (r==get(2,2) && r==get(3,3) && r==get(4,4))
            if (r==get(0,0) || r==get(5,5))
                result = (state)(result | r);
        
        // and 0,5-5,0 line
        r = get(1,4);
        if (r==get(2,3) && r==get(3,2) && r==get(4,1))
            if (r==get(0,5) || r==get(5,0))
                result = (state)(result | r);
        
        // 0,1-4,5 line
        r = get(A2);
        if (r==get(B3) && r==get(C4) && r==get(D5) && r==get(E6))
            result = (state)(result | r);

        // 1,0-5,4 line
        r = get(B1);
        if (r==get(C2) && r==get(D3) && r==get(E4) && r==get(F5))
            result = (state)(result | r);
        
        // 0,1-5,0
        r = get(E1);
        if (r==get(D2) && r==get(C3) && r==get(B4) && r==get(A5))
            result = (state)(result | r);
        
        // 0,4-1,5
        r = get(F2);
        if (r==get(E3) && r==get(D4) && r==get(C5) && r==get(B6))
            result = (state)(result | r);

        return result;
    }
    
    state board_18::winning()const
    {
        // both players can win at the same time
        state rc = empty, rr = empty;
        for (int n=0;n!=6;++n)
        {
            rr = (state)(rr | winningrow(n));
            rc = (state)(rc | winningcol(n));
        }
        return (state)(rc | rr | winningdiag());
    }
            
    char tochar( state s )
    {
        switch (s) {
            case pentago::empty:
                return '.';
            case pentago::black:
                return 'X';
            case pentago::white:
                return 'O';
            default:
                return '#';
        }
    }

    state fromchar( char c )
    {
        switch (c) {
            case '.':
                return pentago::empty;
            case 'X':
            case 'x':
                return pentago::black;
            case '0':
            case 'O':
            case 'o':            
                return pentago::white;
            default:
                return pentago::invalid;
        }
    }
    
    board_18 board_18::fromstring( const char* str )
    {
        board result;
    
        for(int x=0;x!=6;++x)
        {
            for(int y=0;y!=6;++y)
            {
                result.set(position(x,y), fromchar(str[y+x*7]));
            }
        }  
        
        return result;
    }
    
    std::string tostring( const board& b )
    {
        std::string result;
    
        for(int x=0;x!=6;++x)
        {
            for(int y=0;y!=6;++y)
            {
                result.push_back( tochar(b.get(x,y)) );
            }
            result.push_back('\n');
        }
    
        return result;    
    }
    
    std::string tostring_fancy( const board& b )
    {
        std::string result;
    
        result = "  1 2 3 4 5 6\n";
        for(int x=0;x!=6;++x)
        {
            result.push_back('A'+x);
            for(int y=0;y!=6;++y)
            {
                result += (y==3) ? '|' : ' ';
                result.push_back( tochar(b.get(x,y)) );
                
            }
            if (x==2)
                result += "\n  -----------";
            result.push_back('\n');
        }
    
        return result;    
    }
    
    void move::apply(board_18* board, int turn) const
    {
        board->set( mP, turntostate(turn) );
        if (board->winning()==empty)
            mR.apply( board );
    }
    
    void move::undo(board_18* board) const
    {
        mR.invert().apply( board );
        board->clear( mP );
    }
    
    move move::fromstring( const char* str )
    {
        position p( toupper(str[0])-'A', str[1]-'1' );
        rotation::quadrant q = (rotation::quadrant) (toupper(str[2])-'A');
        rotation::direction d = (rotation::direction) (str[3]==0 || str[3]=='+' || isspace(str[3]))
            ? rotation::clockwise
            : rotation::anticlockwise;
        
        move result(p, rotation( q, d));
        
        return result;
    }
    
    std::string tostring( const move& b )
    {
        std::string result;
        result.push_back( 'A'+b.mP.getx() );
        result.push_back( '1'+b.mP.gety() );
        result.push_back( 'A'+b.mR.get_quadrant() );
        result.push_back( (b.mR.get_direction()==rotation::clockwise) ? '+' : '-' );
        return result;
    }
    
    void empty_positions::next()
    {
        do
        {
            step();
        }while(!finished() && !valid());
    }
    
    position empty_positions::get()
    {
        return mP;
    }
    
    bool empty_positions::finished()
    {
        return mP.gety()>=6;
    }
    
    bool empty_positions::valid()
    {
        return mB.get(mP) == empty;
    }
    
    void empty_positions::step()
    {
        int x = mP.getx();
        int y = mP.gety();
        
        x++;
        if (x>5)
        {
            x = 0;
            y++;
        }
        
        mP.set(x,y);
    }
            
}