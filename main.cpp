#include "pentago.h"

#include <cassert>
#include <cstdio>

#include <vector>
#include <string>

using namespace std;
using namespace pentago;

bool verbose = false;

string stringify(const board& b)
{
    return tostring(b);
}

void printboard(const board& b)
{
    printf("%s\n", tostring_fancy(b).c_str());
}

board create(const char * const v)
{   
    return board::fromstring(v);
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
    
    assert( r1.invert().get_quadrant()==rotation::A );
    assert( r1.invert().get_direction()==rotation::clockwise );
           
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

    assert( r2.invert().get_quadrant()==rotation::B );
    assert( r2.invert().get_direction()==rotation::anticlockwise );
    
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

    assert( r3.invert().get_quadrant()==rotation::C );
    assert( r3.invert().get_direction()==rotation::clockwise );
    
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

    assert( r4.invert().get_quadrant()==rotation::D );
    assert( r4.invert().get_direction()==rotation::anticlockwise );

    r4.apply(&b);
    if (verbose) printboard(b);
    assert( stringify(b) ==
           "O....X\n"
           "O....X\n"
           "O....X\n"
           "..X...\n"
           "..X...\n"
           "..XOOO\n");
    
    b.clear();
    
    pentago::move m = move::fromstring("A1A+");
    assert( tostring( m ) == "A1A+" );
    assert( m.mP == position(0,0) );
    assert( m.mR.get_quadrant() == rotation::A );
    assert( m.mR.get_direction() == rotation::clockwise );
    
    int turn = 0;
    m.apply(&b, turn++);
    if (verbose) printboard(b);
    assert( stringify(b) ==
           "..O...\n"
           "......\n"
           "......\n"
           "......\n"
           "......\n"
           "......\n");
    
    m = move::fromstring("C2A+");
    assert( tostring( m ) == "C2A+" );
    assert( m.mP == position(2,1) );
    assert( m.mR.get_quadrant() == rotation::A );
    assert( m.mR.get_direction() == rotation::clockwise );
    
    m.apply(&b, turn++);
    if (verbose) printboard(b);
    assert( stringify(b) ==
        //  123456
           "......\n"  //A
           "X.....\n"  //B
           "..O...\n"  //C
           "......\n"  //D
           "......\n"  //E
           "......\n");//F
    
    m = move::fromstring("B3D-");
    assert( tostring( m ) == "B3D-" );
    assert( m.mP == position(1,2) );
    assert( m.mR.get_quadrant() == rotation::D );
    assert( m.mR.get_direction() == rotation::anticlockwise );
    
    m.apply(&b, turn++);
    if (verbose) printboard(b);
    assert( stringify(b) ==
        //  123456
           "......\n"  //A
           "X.O...\n"  //B
           "..O...\n"  //C
           "......\n"  //D
           "......\n"  //E
           "......\n");//F
    
    return 0;
}