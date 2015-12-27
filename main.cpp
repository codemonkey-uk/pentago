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

state fromchar( char c )
{
    switch (c) {
        case '.':
            return pentago::empty;
        case 'X':
            return pentago::black;
        case 'O':
            return pentago::white;
        default:
            return pentago::invalid;
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

board create(const char * const v)
{
    board result;
    
    for(int x=0;x!=6;++x)
    {
        for(int y=0;y!=6;++y)
        {
            result.set(position(x,y), fromchar(v[y+x*7]));
        }
    }
    
    return result;
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
        printf("move size: %lub.\n", sizeof(pentago::move));
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
    
    const char * example =
        "O..XXX\n"
        "O.....\n"
        "O.....\n"
        ".....O\n"
        ".....O\n"
        "XXX..O\n";
    assert( stringify( create(example) ) == example );
    
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
    
    // not a win
    assert(b.winningrow(0)==empty);
    assert(b.winningrow(1)==empty);
    assert(b.winningrow(5)==empty);
    
    // win on 0 for white
    b.setx( position(0,3), white );
    b.setx( position(0,4), white );
    if (verbose) printboard(b);
    assert( b.winningrow(0)==white );
    assert( b.winning()==white );
    
    // win on 5 for black
    b.setx( position(5,3), black );
    b.setx( position(5,4), black );
    if (verbose) printboard(b);
    assert( b.winningrow(5)==black );
    assert( b.winning()==invalid );
    
    // adjust board set up to test wins on columns
    b.transpose_c();
    b.transpose_d();
    b.setx( position(1,0), black);
    b.setx( position(2,0), black);
    b.setx( position(1,3), black);
    b.setx( position(2,3), black);
    if (verbose) printboard(b);
    assert( b.winningcol(0)==black );
    assert( b.winningcol(3)==empty );
    
    // check a diagnal win
    b = create(
        "O.....\n"
        ".O....\n"
        "..O...\n"
        "...O..\n"
        "....O.\n"
        "......\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==white );
    assert( b.winning()==white );
    
    // break the win and check it again
    b.transpose_a();
    b.transpose_d();
    if (verbose) printboard(b);
    assert( b.winningdiag()==empty );
    assert( b.winning()==empty );
    
    // check another diagnal win
    b = create(
        "......\n"
        ".X....\n"
        "..X...\n"
        "...X..\n"
        "....X.\n"
        ".....X\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==black );
    
    // break the win and check it again
    b.transpose_a();
    b.transpose_d();
    if (verbose) printboard(b);
    assert( b.winningdiag()==empty );
    assert( b.winning()==empty );
    
    b = create(
        "......\n"
        "O.....\n"
        ".O....\n"
        "..O...\n"
        "...O..\n"
        "....O.\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==white );
    assert( b.winning()==white );
    
    b = create(
        ".X....\n"
        "..X...\n"
        "...X..\n"
        "....X.\n"
        ".....X\n"
        "......\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==black );
    assert( b.winning()==black );
    
    b = create(
        "......\n"
        "....X.\n"
        "...X..\n"
        "..X...\n"
        ".X....\n"
        "X.....\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==black );
    assert( b.winning()==black );
    
    b = create(
        ".....O\n"
        "....O.\n"
        "...O..\n"
        "..O...\n"
        ".O....\n"
        "......\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==white );
    assert( b.winning()==white );
    
    b = create(
        "....O.\n"
        "...O..\n"
        "..O...\n"
        ".O....\n"
        "O.....\n"
        "......\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==white );
    assert( b.winning()==white );
    
    b = create(
        "......\n"
        ".....X\n"
        "....X.\n"
        "...X..\n"
        "..X...\n"
        ".X....\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==black );
    assert( b.winning()==black );
    
    // double wins (ties)
    b = create(
        "....O.\n"
        "...O.X\n"
        "..O.X.\n"
        ".O.X..\n"
        "O.X...\n"
        ".X....\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==invalid );
    assert( b.winning()==invalid );
    
    b = create(
        ".X....\n"
        ".OX...\n"
        "..OX..\n"
        "...OX.\n"
        "....OX\n"
        ".....O\n");
    if (verbose) printboard(b);
    assert( b.winningdiag()==invalid );
    assert( b.winning()==invalid );
    
    // test class for rotation encoding in a byte
    b = create("OOOXXX\n"
               "......\n"
               "......\n"
               ".....O\n"
               ".....O\n"
               "XXX..O\n");
    
    rotation r1( rotation::A, rotation::anticlockwise );
    assert( r1.get_quadrant()==rotation::A );
    assert( r1.get_direction()==rotation::anticlockwise );
    
    r1.apply(&b);
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "O..XXX\n"
        "O.....\n"
        "O.....\n"
        ".....O\n"
        ".....O\n"
        "XXX..O\n");

    rotation r2( rotation::B, rotation::clockwise );
    assert( r2.get_quadrant()==rotation::B );
    assert( r2.get_direction()==rotation::clockwise );
    
    r2.apply(&b);
    if (verbose) printboard(b);
    assert( stringify(b) ==
           "O....X\n"
           "O....X\n"
           "O....X\n"
           ".....O\n"
           ".....O\n"
           "XXX..O\n");
    
    rotation r3( rotation::C, rotation::anticlockwise );
    assert( r3.get_quadrant()==rotation::C );
    assert( r3.get_direction()==rotation::anticlockwise );
    
    r3.apply(&b);
    if (verbose) printboard(b);
    assert( stringify(b) ==
           "O....X\n"
           "O....X\n"
           "O....X\n"
           "..X..O\n"
           "..X..O\n"
           "..X..O\n");
    
    rotation r4( rotation::D, rotation::clockwise );
    assert( r4.get_quadrant()==rotation::D );
    assert( r4.get_direction()==rotation::clockwise );
    
    r4.apply(&b);
    if (verbose) printboard(b);
    assert( stringify(b) ==
           "O....X\n"
           "O....X\n"
           "O....X\n"
           "..X...\n"
           "..X...\n"
           "..XOOO\n");
    
    return 0;
}