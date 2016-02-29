#ifndef MCTS_H_INCLUDED
#define MCTS_H_INCLUDED

// todo: remove requirement on std::vector
#include <vector>
#include <cmath>
#include <cfloat>

// timing stuff
#include <sys/times.h>
#include <unistd.h>

namespace mcts
{
    const float uct_c = sqrt(2);
    
    template< typename Move >
    class Node
    {
        public:
            Node( const Move& move )
                : mMove(move)
                , mWins(0)
                , mSims(0)
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

            static int CountTrials(const std::vector< Node<Move> >* nodes);            
            static void Cleanup(const std::vector< Node<Move> >* nodes);
            static Node<Move>* SelectNode(std::vector< Node<Move> >* nodes);
            
            template< typename GameState > 
            static int Explore( Node< Move >* node, GameState theGame );
            
            template< typename GameState > 
            static Move GetMove( GameState theGame, clock_t timeLeft );
        
        private:
            Move mMove;
            int mWins;
            int mSims;
            std::vector<Node>* mChildren;
        
    };        

    template< typename Move >
    int Node<Move>::CountTrials(const std::vector< Node<Move> >* nodes)
    {
        int result=0;
        for (int i=0;i!=nodes->size();++i)
            result += (*nodes)[i].mSims;
        return result;
    }
    
    template< typename Move >
    void Node<Move>::Cleanup(const std::vector< Node<Move> >* nodes)
    {
        for (int i=0;i!=nodes->size();++i)
        {
            if ((*nodes)[i].mChildren)
                Cleanup((*nodes)[i].mChildren);
        }
        delete nodes;
    }
    
    template< typename Move >
    Node<Move>* Node<Move>::SelectNode(std::vector< Node<Move> >* nodes)
    {
        float best_uct = -FLT_MAX;
        Node<Move>* result = 0;
        const float lnt = log( (float)CountTrials(nodes) );
        for (typename std::vector< Node<Move> >::iterator i=nodes->begin(); i!=nodes->end(); ++i)
        {
            float uct = i->UCT(lnt);
            if (uct>best_uct) 
            {
                result = &*i;
                best_uct = uct;
            }
        }
        
        return result;
    }
    
    template< typename Move, typename MoveItr >
    std::vector< Node<Move> >* GetAllNodes( MoveItr itr, MoveItr end )
    {
        std::vector< Node<Move> >* result = new std::vector< Node<Move> >();
        std::copy( itr, end, std::back_inserter(*result) );
        return result;
    }
    
    template< typename Move, typename GameState >
    std::vector< Node<Move> >* GetAllNodes( GameState theGame )
    {
        // TODO: fix ugly/inefficient: code copies all moves to a vector, 
        // then those into a node vector, it should grab a generator object
        // and create the node array directly from that
        std::vector< Move > moves;
        theGame.GetAllMoves( std::back_inserter(moves) );
        return GetAllNodes<Move>( moves.begin(), moves.end() );
    }
    
    template< typename Move >
    template< typename GameState > 
    int Node<Move>::Explore( Node< Move >* node, GameState theGame )
    {
        // play out the game
        std::vector< std::pair< Node<Move>*, int> > stack;
        
        // TODO: find upper bound on total turns left
        // int turnsLeft = 0;
        // for (int i=0; i!=theGame.GetPlayerCount(); ++i)
        //    turnsLeft += theGame.GetPlayerChange( i ).GetTotalValue();
        //
        // size our stack for moves left
        // stack.reserve(turnsLeft);
        
        do
        {
            if (node->mChildren == 0)
            {
                node->mChildren = GetAllNodes<Move>( theGame );
            }
            
            node = SelectNode(node->mChildren);
            theGame = theGame.PlayMove( node->mMove );
            
            int p = theGame.GetCurrentPlayer();
            stack.push_back( std::pair< Node<Move>*, int>( node, p ) );
            
        }while(theGame.Finished()==false);
        
        int winner = theGame.GetWinner();
        
        // back propagate the explored nodes
        for (int i=0; i!=stack.size(); ++i)
        {
            stack[i].first->mSims++;
            stack[i].first->mWins += (winner==stack[i].second);
        }
        
        return winner;
    }
    
    template< typename Move >
    template< typename GameState > 
    Move Node<Move>::GetMove( GameState theGame, clock_t timeLeft )
    {
        // TODO externalise time-left
        tms t;
        clock_t dt, currentTurnClockStart = times(&t);

        std::vector<Node>* moveList = GetAllNodes<Move>( theGame );
        Node* best = &(*moveList)[0];
        
        if (moveList->size()>1)
        {
            do
            {
                Node* trial = SelectNode(moveList);
            
                GameState newGame = theGame.PlayMove( trial->mMove );
                trial->mSims++;
                if (Explore(trial, newGame)==theGame.GetCurrentPlayer())
                {
                    trial->mWins++;
                    if (trial->Ratio() > 
                        best->Ratio())
                        best = trial;
                }
                
                dt = times(&t) - currentTurnClockStart;
            }while( dt < timeLeft );
        }
        
        Move result = best->mMove;
        Cleanup( moveList );
            
        return result;
    }
}

#endif
