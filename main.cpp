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

int main()
{
    vector<position> moves;
    moves.reserve(6*6);
    
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
    
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "OOO...\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n");
    
    b.transpose_a();
    if (verbose) printboard(b);
    assert( stringify(b) ==
        "..O...\n"
        "..O...\n"
        "..O...\n"
        "......\n"
        "......\n"
        "......\n");

    b.transpose_a();
    if (verbose) printboard(b);    
    assert( stringify(b) ==
        "......\n"
        "......\n"
        "OOO...\n"
        "......\n"
        "......\n"
        "......\n");
    
    b.transpose_a();
    if (verbose) printboard(b);    
    assert( stringify(b) ==
        "O.....\n"
        "O.....\n"
        "O.....\n"
        "......\n"
        "......\n"
        "......\n");    
    
    return 0;
}