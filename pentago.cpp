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
    
    void board_18::transpose(const position & offset)
    {
        // A1 = C1
        state a1 = get(A1+offset);
        setx( A1+offset, get(C1+offset) );
        
        // A2 = B1
        state a2 = get(A2+offset);
        setx( A2+offset, get(B1+offset) );
        
        // A3 = A1
        state a3 = get(A3+offset);
        setx( A3+offset, a1 );
        
        // B1 = C2
        setx( B1+offset, get(C2+offset) );
        
        // C1 = C3
        setx( C1+offset, get(C3+offset) );

        // C2 = B3
        setx( C2+offset, get(B3+offset) );
        
        // B3 = A2
        setx( B3+offset, a2 );
        
        // C3 = A3
        setx( C3+offset, a3 );    
    }
    
    void board_18::transpose_r(const position & offset)
    {
        // A1 => C1
        state c1 = get(C1+offset);
        setx( C1+offset, get(A1+offset) );
        
        // A2 => B1
        state b1 = get(B1+offset);
        setx( B1+offset, get(A2+offset) );
        
        // A3 => A1
        setx( A1+offset, get(A3+offset) );
        
        // B1 => C2
        state c2 = get(C2+offset);
        setx( C2+offset, b1 );
        
        // B3 => A2
        setx( A2+offset, get(B3+offset) );
        
        // C3 => A3
        setx( A3+offset, get(C3+offset) );
        
        // C1 => C3      
        setx( C3+offset, c1 );
        
        // C2 => B3
        setx( B3+offset, c2 );
    }
    
    void board_18::transpose_a()
    {
        transpose( A1 );
    }
    
    void board_18::transpose_ar()
    {
        transpose_r( A1 );
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
        transpose( A4 );
    }    
    
    void board_18::transpose_br()
    {
        // reverse (anticlockwise) rotation of quadrant B
        transpose_r( A4 );
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
        transpose( D1 );
    }
    
    void board_18::transpose_cr()
    {
        // reverse (anticlockwise) rotation of quadrant C
        transpose_r( D1 );
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
        transpose( D4 );
    }
    
    void board_18::transpose_dr()
    {
        // reverse (anticlockwise) rotation of quadrant D
        transpose_r( D4 );
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
        state r = get(B2);
        if (r==get(C3) && r==get(D4) && r==get(E5))
            if (r==get(A1) || r==get(E5))
                result = (state)(result | r);
        
        // and 0,5-5,0 line
        r = get(B5);
        if (r==get(C4) && r==get(D3) && r==get(E2))
            if (r==get(A6) || r==get(F1))
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
        result.reserve(7*6);
        
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
        result.reserve(14*8);
        
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
        result.reserve(4);
        
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