#include "pentago.h"

#include <cassert>
#include <cstdio>

#include <vector>

using namespace std;
using namespace pentago;

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

void printboard(const board& b)
{
    for(int x=0;x!=6;++x)
    {
        for(int y=0;y!=6;++y)
        {
            printf("%c", tochar(b.get(x,y)) );
        }
        printf("\n");
    }
}

int main()
{
    vector<position> moves;
    moves.reserve(6*6);
    
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
    
    board b;
    random_shuffle(moves.begin(), moves.end());
    for (int m=0;m!=moves.size();++m)
    {
        printboard(b);
        for (int n=0;n!=m;++n)
        {
            assert( b.get(moves[n]) == (n&1) ? black : white );
        }
        for (int n=m;n!=moves.size();++n)
        {
            assert( b.get(moves[n]) == empty );
        }
        b.set(moves[m], (m&1) ? black : white);
        printf("%i,%i\n", moves[m].getx(), moves[m].gety());
    }
    
    
    return 0;
}