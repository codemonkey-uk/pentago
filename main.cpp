#include "pentago.h"

#include <cassert>
#include <cstdio>

#include <vector>
#include <string>

using namespace std;
using namespace pentago;

bool verbose = false;

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

string stringify(const board& b)
{
    string result;
    
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

void printboard(const board& b)
{
    printf("%s\n", stringify(b).c_str());
}

int main(int argc, char** argv)
{
    vector<position> moves;
    moves.reserve(6*6);
    
    if (argc>1) verbose = true;
    
    if (verbose) 
    {
        printf("board size: %lub.\n", sizeof(board));
        printf("coord size: %lub.\n", sizeof(position));
    }

    // create list of all positions, check position type get/set ops
    for(int x=0;x!=6;++x)
    {
        for(int y=0;y!=6;++y)
        {
            position p(x,y);
            assert( p.getx()==x );
            assert( p.gety()==y );
            moves.push_back(p);
        }
    }
    
    // reorder moves to provide better test coverage of operation order
    random_shuffle(moves.begin(), moves.end());
    
    // test setting pieces (stacotic move order)
    board b;
    for (int m=0;m!=moves.size();++m)
    {
        if (verbose) printboard(b);
        for (int n=0;n!=m;++n)
        {
            assert( b.get(moves[n]) == (n&1) ? black : white );
        }
        for (int n=m;n!=moves.size();++n)
        {
            assert( b.get(moves[n]) == empty );
        }
        b.set(moves[m], (m&1) ? black : white);
        if (verbose) printf("%i,%i\n", moves[m].getx(), moves[m].gety());
    }
    
    // test clearing the board (random access)
    for (int m=0;m!=moves.size();++m)
    {
        if (verbose) printboard(b);
        for (int n=0;n!=m;++n)
        {
            assert( b.get(moves[n]) == empty );
        }
        for (int n=m;n!=moves.size();++n)
        {
            assert( b.get(moves[n]) == (n&1) ? black : white );
        }
        b.clear(moves[m]);
        if (verbose) printf("%i,%i\n", moves[m].getx(), moves[m].gety());
    }
    
    b.set( position(0,0), white );
    b.set( position(0,1), white );
    b.set( position(0,2), white );
    b.set( position(0,3), black );
    b.set( position(0,4), black );
    b.set( position(0,5), black );
    b.set( position(5,0), black );
    b.set( position(5,1), black );
    b.set( position(5,2), black );
    b.set( position(5,3), white );
    b.set( position(5,4), white );
    b.set( position(5,5), white );
    
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n");
    
    b.transpose_a();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "..OXXX\n"
        "..O...\n"
        "..O...\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_a();
    if (verbose) printboard(b);    
    assert( stringify(b) ==
        "...XXX\n"
        "......\n"
        "OOO...\n"
        "......\n"
        "......\n"
        "XXXOOO\n");
    
    b.transpose_a();
    if (verbose) printboard(b);    
    assert( stringify(b) ==
        "O..XXX\n"
        "O.....\n"
        "O.....\n"
        "......\n"
        "......\n"
        "XXXOOO\n");    

    // and back again
    b.transpose_ar();
    if (verbose) printboard(b);    
    assert( stringify(b) ==
        "...XXX\n"
        "......\n"
        "OOO...\n"
        "......\n"
        "......\n"
        "XXXOOO\n");
        
    b.transpose_ar();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "..OXXX\n"
        "..O...\n"
        "..O...\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_ar();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n"); 

    b.transpose_b();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOO..X\n"
        ".....X\n"
        ".....X\n"
        "......\n"
        "......\n"
        "XXXOOO\n"); 
        
    b.transpose_b();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOO...\n"
        "......\n"
        "...XXX\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_b();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOX..\n"
        "...X..\n"
        "...X..\n"
        "......\n"
        "......\n"
        "XXXOOO\n");         

    b.transpose_b();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n");          

    b.transpose_br();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOX..\n"
        "...X..\n"
        "...X..\n"
        "......\n"
        "......\n"
        "XXXOOO\n"); 

    b.transpose_br();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOO...\n"
        "......\n"
        "...XXX\n"
        "......\n"
        "......\n"
        "XXXOOO\n"); 

    b.transpose_br();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOO..X\n"
        ".....X\n"
        ".....X\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_br();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_c();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "X.....\n"
        "X.....\n"
        "X..OOO\n");

    b.transpose_c();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "XXX...\n"
        "......\n"
        "...OOO\n");

    b.transpose_c();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "..X...\n"
        "..X...\n"
        "..XOOO\n");

    b.transpose_c();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_cr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "..X...\n"
        "..X...\n"
        "..XOOO\n");
        
    b.transpose_cr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "XXX...\n"
        "......\n"
        "...OOO\n");

    b.transpose_cr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "X.....\n"
        "X.....\n"
        "X..OOO\n");   
        
    b.transpose_cr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n");

    b.transpose_d();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "...O..\n"
        "...O..\n"
        "XXXO..\n");

    b.transpose_d();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "...OOO\n"
        "......\n"
        "XXX...\n");   
        
    b.transpose_d();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        ".....O\n"
        ".....O\n"
        "XXX..O\n");   

    b.transpose_d();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n"); 
        
    b.transpose_dr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        ".....O\n"
        ".....O\n"
        "XXX..O\n");    
        
    b.transpose_dr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "...OOO\n"
        "......\n"
        "XXX...\n"); 
        
    b.transpose_dr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "...O..\n"
        "...O..\n"
        "XXXO..\n");  
        
    b.transpose_dr();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOOXXX\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "XXXOOO\n");         
    return 0;
}