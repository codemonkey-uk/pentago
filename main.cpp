#include "pentago.h"

#include <cassert>
#include <cstdio>

#include <vector>
#include <string>

#include <iostream>

#include "mcts.h"

using namespace std;
using namespace pentago;

bool autoai[2] = {false, false};

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

bool valid_move(const string& rhs)
{
    // valid moves are always 3 or 4 characters
    if (rhs.length()<3) return false;
    if (rhs.length()>4) return false;
    
    // Position [A-F][1-6]
    if (toupper(rhs[0])>'F') return false;
    if (toupper(rhs[0])<'A') return false;
    if (rhs[1]>'6') return false;
    if (rhs[1]<'1') return false;
    
    // Quadrant A,B,C, or D
    if (toupper(rhs[2])>'D') return false;
    if (toupper(rhs[2])<'A') return false;

    // Direction (optional) + or -
    if (rhs.length()==4 && (rhs[3]!='+' && rhs[3]!='-'))
        return false;
    
    return true;
}

template< typename ItrOut >
void all_moves(const board& b, int turn, ItrOut moves)
{
    // simple test for empty positions generator
    empty_positions itr(b);
    
    while (itr.finished()==false)
    {
        position p = itr.get();
        rotation r;
        while (r.valid())
        {
            // filter out anti-clockwise rotations of symmetrical quadrants
            if (r.get_direction()==rotation::clockwise ||
                r.symetrical(&b)==false)
            {
                *moves++ = pentago::move( p, r );
            }
            r.next();
        }
        itr.next();
    }
}

void all_moves(const board& b, int turn, vector< pentago::move >* moves)
{
    moves->reserve( (6*6*4*2)-turn );
    moves->resize( 0 );
    all_moves(b, turn, std::back_inserter( *moves ));
}

pentago::move random_move(const board& b, int turn)
{
    static vector< pentago::move > moves;
    all_moves(b, turn, &moves);
    
    return moves[rand()%moves.size()];
}

pentago::move best_move(const board& b, int turn)
{
    vector< pentago::move > moves;
    all_moves(b, turn, &moves);
    
    vector< int > score;
    score.resize( moves.size() );

    for (int s=0;s!=128;++s)
    {
        for (int m=0;m!=moves.size();++m)
        {
            board b2 = b;
            int t2 = turn;
            
            moves[m].apply(&b2, t2++);
            while(b2.winning()==empty && t2<6*6)
            {
                random_move(b2,t2).apply(&b2,t2);
                t2++;
            }
            
            state result = b2.winning();
            if (result==turntostate(turn))
                score[m]++;
            else if (result==turntostate(turn+1))
                score[m]--;
            
        }
    }
    
    int best = 0;
    for (int m=1;m!=moves.size();++m)
    {
        // cout << tostring(moves[m]) << " " << score[m] << endl;
        if (score[m]>score[best]) best = m;
    }
    
    return moves[best];
}

pentago::move ai(const board& b, int turn)
{
    return best_move(b, turn);
}

// mcts adaptor for board_18 class
struct GameState
{
    board mBoard;
    int mTurn;
    
    GameState(board b, int t) : mBoard(b), mTurn(t) {}
    GameState() : mTurn(0) {}
    
    int GetCurrentPlayer() const { return mTurn & 1; }
    bool Finished() const { return mBoard.winning()!=empty || mTurn==6*6; }
    int GetWinner() const { return ((int)mBoard.winning())-1; }
    
    template< typename OutItr >
    void GetAllMoves(OutItr itr)
    {
        all_moves(mBoard, mTurn, itr);
    }
    
    GameState PlayMove( pentago::move move )
    {
        GameState result(*this);
        move.apply( &result.mBoard, result.mTurn++ );
        return result;
    }
};

pentago::move ai_mcts(const board& b, int turn)
{
    static const clock_t ticks_per_s = sysconf(_SC_CLK_TCK);
    
    GameState game(b,turn);
    return mcts::Node< pentago::move >::GetMove( game, ticks_per_s );
}

void interactive()
{
    board b;
    int turn = 0;
    
    while (b.winning()==empty)
    {
        printboard(b);
        
        string movestr;
        while (valid_move(movestr)==false)
        {
            cout << tochar( turntostate(turn) ) << " to play: ";
            
            if (autoai[turn&1])
            {
                movestr = "ai";
            }
            else
            {
                cin >> movestr;
            }
            
            // quit
            if (movestr=="q" || movestr=="Q")
                return;
                
            if (movestr=="ai")
            {
                movestr = tostring( ai_mcts(b, turn) );
                cout << "ai selects: " << movestr << endl;
            }
        }
        
        move::fromstring(movestr.c_str()).apply( &b, turn++ );
    }
    
    // show final board state
    printboard(b);
    
    // call it
    if (b.winning()==invalid)
        cout << "Draw!";
    else
        cout << tochar(b.winning()) << " wins.";
    cout << endl;
}

void mcts_tests(bool verbose)
{
    static vector< pentago::move > moves;
    
    GameState game;
    all_moves(game.mBoard, 0, &moves);
    
    // template< typename Move, typename MoveItr >
    // std::vector< Node<Move> >* GetAllNodes( MoveItr itr, MoveItr end )
    
    auto nodes = mcts::GetAllNodes<pentago::move> ( moves.begin(), moves.end() );
    assert( nodes->size() == moves.size() );
    
    // template< typename Move, typename GameState > 
    // int Explore( Node< Move >* node, GameState theGame )
    
    int w = mcts::Node< pentago::move >::Explore( &(*nodes)[0], game );

    // winners player index
    assert( w==0 || w==1 || w==2 );
    
    clock_t ticks_per_s = sysconf(_SC_CLK_TCK);
    
    pentago::move m = mcts::Node< pentago::move >::GetMove( game, ticks_per_s );
    printf( "%s\n", tostring(m).c_str() );
}

void run_tests(bool verbose)
{
    vector<position> moves;
    moves.reserve(6*6);
    
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
    
    // simple test for empty positions generator
    empty_positions itr(b);
    assert( itr.get()==position(0,0) );
    assert( itr.finished()==false );
    
    int c=0;
    while (itr.finished()==false)
    {
        c++;
        if (verbose) printf( "%c%c, ", 'A'+itr.get().getx(), '1'+itr.get().gety() );
        itr.next();
    }
    if (verbose) printf(": c==%i\n",c);
    assert( c == (6*6)-3 );
    
    // test checks for rotational symmetry
    assert( create(
        ".X....\n"
        ".OX...\n"
        "..OX..\n"
        "...OX.\n"
        "....OX\n"
        ".....O\n").symetrical_a() == false
    );
    
    assert( create(
        "......\n"
        ".O....\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n").symetrical_a() == true
    );    

    assert( create(
        ".X....\n"
        "O.O...\n"
        ".X....\n"
        "......\n"
        "......\n"
        "......\n").symetrical_a() == true
    );    

    assert( create(
        ".X....\n"
        "O.O...\n"
        ".XX...\n"
        "......\n"
        "......\n"
        "......\n").symetrical_a() == false
    );
    
    assert( create(
        "....X.\n"
        "....OX\n"
        ".....O\n"
        "...OX.\n"
        "....OX\n"
        ".....O\n").symetrical_b() == false
    );
    
    assert( create(
        "......\n"
        "....O.\n"
        "......\n"
        "......\n"
        "......\n"
        "......\n").symetrical_b() == true
    );    

    assert( create(
        "....X.\n"
        "...O.O\n"
        "....X.\n"
        "......\n"
        "......\n"
        "......\n").symetrical_b() == true
    );    

    assert( create(
        "....X.\n"
        "...O.O\n"
        "....XX\n"
        "......\n"
        "......\n"
        "......\n").symetrical_b() == false
    );   
    
    mcts_tests(verbose);
}

int main(int argc, char** argv)
{
    bool verbose = false;
    bool test = false;
    
    for (int i=1; i!=argc; ++i)
    {
        const char * str = argv[i];
        while (*str=='-') ++str;
        if (strcmp(str,"test")==0)
            test = true;
        else if (strcmp(str,"v")==0)
            verbose = true;
        else if (strcmp(str,"verbose")==0)
            verbose = true;
        else if (strcmp(str,"ai1"))
            autoai[0] = true;
        else if (strcmp(str,"ai0"))
            autoai[1] = true;
        else
            cout << "ignoring unrecognised argument: " << str << endl;
    }
    
    if (test) run_tests(verbose);
    else interactive();
}