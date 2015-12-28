#ifndef PENTAGO_H_INCLUDED
#define PENTAGO_H_INCLUDED

#include <cstdint>
#include <cstring>

#include <string>

namespace pentago
{
    enum state {
        empty = 0,
        white = 1,
        black = 2,
        invalid = 3
    };
    
    state fromchar( char c );
    char tochar( state s );
    
    inline state turntostate(int turn)
    {
        return (state)(1+(turn&1));
    }
    
    // 0-5 x 0-5
    class position
    {
        public:
            position() : mV(0) {}
            position(const position& rhs) : mV(rhs.mV) {}
            position(int x, int y) : mV(0)
            {
                set(x,y);
            }
            
            int getx() const 
            {
                return mV%width;
            }
            
            int gety() const 
            {
                return mV/width;
            }
            
            int get() const
            {
                return mV;
            }
            
            void set(int x, int y)
            {
                mV = x+y*width;
            }

            void setx(int x) 
            {
                set(x, gety());
            }
            
            void sety(int y)  
            {
                set(getx(), y);
            }
        
            bool operator==(const position& rhs)
            {
                return rhs.mV==mV;
            }
        
        private:
            // note, mV is encoded as a "tightly packed" index into the board array
            static const int width = 6;
            uint8_t mV;
    };
    
    // 4 bit board byte layout
    // [123-456-][789-ABC-]
    // simplifies get/set code
    // as it gives exact 2/byte
    
    class board_18
    {
        public:
            board_18()
            {
                clear();
            }
        
            void clear()
            {
                memset(mV,0,18);
            }
            
            state get(position p)const
            {
                int b = bits_per*p.get();
                int bit = b%8;
                int byte = b/8;
                int result = (mV[byte] >> bit) & bit_mask;
                return (state)(result);
            }
            
            state get(int x, int y)const
            {
                position p(x,y);
                return get(p);
            }
            
            void set(position p, state s)
            {
                int b = bits_per*p.get();
                int bit = b%8;
                int byte = b/8;
                mV[byte] |= (s << bit);
            }

            void clear(position p)
            {
                int b = bits_per*p.get();
                int bit = b%8;
                int byte = b/8;
                mV[byte] &= ~(bit_mask << bit);
            }
            
            // clear and set
            void setx(position p, state s)
            {
                int b = bits_per*p.get();
                int bit = b%8;
                int byte = b/8;
                mV[byte] &= ~(bit_mask << bit);
                mV[byte] |= (s << bit);
            }
        
            void set(int x, int y, state s)
            {
                position p(x,y);
                set(p, s);
            }
        
            // quadrant rotations, affecting :
            //   aaabbb
            //   a-ab-b
            //   aaabbb
            //   cccddd
            //   c-cd-d
            //   cccddd
            // clockwise rotation by default
            // method with "r" suffix for counter-clockwise
            void transpose_a();
            void transpose_ar();
            void transpose_b();
            void transpose_br();
            void transpose_c();
            void transpose_cr();
            void transpose_d();
            void transpose_dr();

            // methods to return the winning state of any:
            // row (along x axis)
            // column (along y axis)
            // and diagnal
            // winning being the colour that holds 5 in a row
            state winningrow(int index)const;
            state winningcol(int index)const;
            state winningdiag()const;
            state winning()const;
        
            static board_18 fromstring( const char* str );
        
        private:
            // 3 bits per node gives misaligned bytes
            // (get/set more complicated) but 12b array
            // 4 bits per locale wastes 6 bytes with 18b array
            static const int bits_per = 4; 
            static const int bit_mask = 7;
            uint8_t mV[18];
    };
    
    typedef board_18 board;

    std::string tostring( const board& b );
    std::string tostring_fancy( const board& b );
    
    class rotation
    {
        public:
            
            // rotation, quadrant and direction, stored as a byte
            
            // 4 quadrants: 00 = A, 01 = B, 10 = C, 11 = D, 1st & 2nd bits:
            enum quadrant {
                A = 0,
                B = 1,
                C = 2,
                D = 3
            };
            // 2 directions of spin, 0, & 1, 3rd bit:
            enum direction {
                clockwise = 0,
                anticlockwise = 4
            };
        
            rotation( quadrant q, direction d )
            {
                mV = (q | d);
            }
        
            quadrant get_quadrant() const 
            {
                return (quadrant)(mV & 3);
            }
        
            direction get_direction() const 
            {
                return (direction)(mV & 4);            
            }

            rotation invert() const
            {
                rotation result(*this);
                result.mV ^= 4;
                return result;
            }
        
            void apply(board_18* board) const
            {
                typedef void (board_18::*BoardTransformFn)(void);
                static const BoardTransformFn lut[] = {
                    &board_18::transpose_a,
                    &board_18::transpose_b,
                    &board_18::transpose_c,
                    &board_18::transpose_d,
                    &board_18::transpose_ar,
                    &board_18::transpose_br,
                    &board_18::transpose_cr,
                    &board_18::transpose_dr,
                };
                ((*board).*(lut[mV]))();
            }
        
        private:
            uint8_t mV;
    };
    
    struct move
    {
        move( const position& p , const rotation& r )
            : mP(p), mR(r)
        { }
        
        void apply(board_18* board, int turn) const;
        void undo(board_18* board) const;
        
        position mP;
        rotation mR;
        
        static move fromstring( const char* str );
    };
    
    std::string tostring( const move& b );
    
}

#endif