#ifndef PENTAGO_H_INCLUDED
#define PENTAGO_H_INCLUDED

#include <stdint.h>
#include <cstring>

namespace pentago
{
    enum state {
        empty = 0,
        white = 1,
        black = 2,
        invalid = 3
    };
    
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
            //   aaabbb
            //   aaabbb
            //   cccddd
            //   cccddd
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

            // returns the winning state of any given row (along x axis)
            // winning being the colour that holds 5 in a row
            state winningrow(int index);
            state winningcol(int index);
        
        private:
            // 3 bits per node gives misaligned bytes
            // (get/set more complicated) but 12b array
            // 4 bits per locale wastes 6 bytes with 18b array
            static const int bits_per = 4; 
            static const int bit_mask = 7;
            uint8_t mV[18];
    };
    
    typedef board_18 board;
}

#endif