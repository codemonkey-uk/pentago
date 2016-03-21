// MCTS.h
//
// Given a Move value type, 
// and GameState type that supports the interface:
//
//    - Who's turn is it?
//    int GetCurrentPlayer() const
//
//    - How many different moves could be taken on the current turn.
//      Must be exact, or overestimate. 
//      Underestimates will result in a buffer overrun!
//    int CountPossibleMoves() const
//
//    - What moves could be taken on the current turn.
//      A template member function that writes "Move" objects to an output iterator
//      Must not output more moves than returned by CountPossibleMoves (less is okay)
//    template< typename OutItr >
//    OutItr GetPossibleMoves(OutItr itr) const
//
//    - Return the new game state, given the Move to be played.
//    GameState PlayMove( Move ) const
//
//    - Is the game over? Returns true if there are no more turns to play.
//    bool Finished() const;
//
//    - If the game is finished, who won?
//    int GetWinner() const;
//
//    - How many turns left in this game?
//      used to guide pre-allocations for play out, doesn't have to be 100% accurate
//      over estimation == over allocation in setting a stack size
//      under estimation == search will reallocate during a playouts to size the stack
//    int TurnsLeft() const;
//        
// Then call as follows to get a "good" guess of the next move to play, in bounded time:
//
// Move ai_move = mcts::Node< Move >::GetMove( gameState, timeOutFn );
//
// - Where timeOutFn is a function object that returns false when the AI time has expired:
//
//     static const clock_t ticks_per_s = sysconf(_SC_CLK_TCK);
//     struct OneSecondTimeOut
//     {
//         OneSecondTimeOut()
//         {
//             tms t;
//             currentTurnClockStart = times(&t);
//         }
//     
//         bool operator()()
//         {
//             tms t;
//             dt = times(&t) - currentTurnClockStart;
//             return dt < ticks_per_s;
//         }
//     
//         clock_t dt, currentTurnClockStart;
//     };
// 

#ifndef MCTS_H_INCLUDED
#define MCTS_H_INCLUDED

#include <cmath>
#include <cfloat>
#include <cassert>

// TODO: Still want to remove the use of std::vector
// it's really just me being a bit lazy about allocations
#include <vector>

namespace mcts
{
    const float uct_c = sqrt(2);
    
    template< typename NodeType >
    struct PlayoutTurn
    {
        PlayoutTurn( NodeType* n, int p ) : mNode(n), mPlayer(p) { }
        NodeType* mNode;
        int mPlayer;
    };
    
    template< typename Move >
    class Node
    {
        public:
            Node( const Move& move=Move() )
                : mMove(move)
                , mWins(0)
                , mSims(0)
                , mChildCount(0)
                , mChildren(0)
            { }
        
            float UCT(float lnt) const
            {
                if (mSims==0) return FLT_MAX;
                float uct = (float)mWins / (float)mSims;
                uct += uct_c * sqrt(lnt / (float)mSims);
                return uct;
            }
        
            float Ratio() const
            {
                return (float)mWins / (float)mSims;
            }

            static int CountTrials(Node<Move>* nodes, size_t n);
            static void Cleanup(Node<Move>* nodes, size_t n);
            static int CountNodes(Node<Move>* nodes, size_t n);
            
            static Node<Move>* SelectNode(Node<Move>* nodes, size_t n);
            
            typedef std::vector< PlayoutTurn< Node<Move> > > PlayoutStack;
            
            template< typename GameState > 
            static int Explore( Node< Move >* node, GameState theGame, PlayoutStack& stack );
            
            template< typename GameState, typename TimeoutFn > 
            static Move GetMove( GameState theGame, TimeoutFn timeOut );
        
            int ChildCount() const;
        private:
            Move mMove;
            int mWins;
            int mSims;
            int mChildCount;
            Node* mChildren;
    };
    
    template< typename Move >    
    int Node<Move>::ChildCount() const
    {
        assert(mChildren);
        return mChildCount;
    }
    

    template< typename Move >
    int Node<Move>::CountTrials(Node<Move>* nodes, size_t n)
    {
        int result=0;
        for (int i=0;i!=n;++i)
            result += nodes[i].mSims;
        return result;
    }
    
    template< typename Move >
    void Node<Move>::Cleanup(Node<Move>* nodes, size_t n)
    {
        for (int i=0;i!=n;++i)
        {
            if (nodes[i].mChildren)
                Cleanup(nodes[i].mChildren, nodes[i].ChildCount());
        }
        delete nodes;
    }

    template< typename Move >
    int Node<Move>::CountNodes(Node<Move>* nodes, size_t n)
    {
        int total = n;
        for (int i=0;i!=n;++i)
        {
            if (nodes[i].mChildren)
                total += CountNodes(nodes[i].mChildren, nodes[i].ChildCount());
        }
        return total;
    }
    
    template< typename Move >
    Node<Move>* Node<Move>::SelectNode(Node<Move>* nodes, size_t n)
    {
        float best_uct = -FLT_MAX;
        Node<Move>* result = 0;
        const float lnt = log( (float)CountTrials(nodes, n) );
        for (int i=0; i!=n; ++i)
        {
            float uct = nodes[i].UCT(lnt);
            if (uct>best_uct) 
            {
                result = &nodes[i];
                best_uct = uct;
            }
        }
        
        return result;
    }
    
    template< typename Move, typename GameState >
    Node<Move>* GetAllNodes( GameState theGame, int* nodeCount )
    {
        const int m = theGame.CountPossibleMoves();
        Node<Move>* result = new Node<Move>[m];
        Node<Move>* end = theGame.GetPossibleMoves( result );
        *nodeCount = end-result;
        assert( *nodeCount<=m );
        return result;
    }
    
    template< typename Move >
    template< typename GameState > 
    int Node<Move>::Explore( Node< Move >* node, GameState theGame, PlayoutStack& stack )
    {
        stack.clear();
        
        do
        {
            if (node->mChildren == 0)
            {
                node->mChildren = GetAllNodes<Move>( theGame, &node->mChildCount );
            }
            
            node = SelectNode(node->mChildren, node->mChildCount);
            theGame = theGame.PlayMove( node->mMove );
            
            int p = theGame.GetCurrentPlayer();
            stack.push_back( PlayoutTurn< Node<Move> >( node, p ) );
            
        }while(theGame.Finished()==false);
        
        const int winner = theGame.GetWinner();
        
        // back propagate the explored nodes
        for (int i=0; i!=stack.size(); ++i)
        {
            stack[i].mNode->mSims++;
            stack[i].mNode->mWins += (winner==stack[i].mPlayer);
        }
        
        return winner;
    }
    
    template< typename Move >
    template< typename GameState, typename TimeoutFn > 
    Move Node<Move>::GetMove( GameState theGame, TimeoutFn timeOut )
    {
        int moveCount;
        Node* moveList = GetAllNodes<Move>( theGame, &moveCount );
        Node* best = moveList;
        
        PlayoutStack stack;
        stack.reserve(theGame.TurnsLeft());
        
        if (moveCount>1)
        {
            do
            {
                Node* trial = SelectNode(moveList, moveCount);
            
                GameState newGame = theGame.PlayMove( trial->mMove );
                trial->mSims++;
                if (Explore(trial, newGame, stack)==theGame.GetCurrentPlayer())
                {
                    trial->mWins++;
                    if (trial->Ratio() > best->Ratio())
                        best = trial;
                }
                
            }while( timeOut() );
        }
        
        // printf("%i / %i\n", CountTrials(moveList, moveCount), CountNodes(moveList, moveCount));
        // printf("%i%% of %i\n", static_cast<int>(best->Ratio()*100), best->mSims);
        
        Move result = best->mMove;
        Cleanup( moveList, moveCount );
            
        return result;
    }
}

#endif
