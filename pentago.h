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
            position(int x, int y) : mV(0) 
            {
                set(x,y);
            }
            
            int getx() const 
            {
                return mV%6;
            }
            
            int gety() const 
            {
                return mV/6;
            }
            
            int get() const
            {
                return mV;
            }
            
            void set(int x, int y)
            {
                mV = x+y*6;
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
            uint8_t mV;
    };
    
    class board
    {
        public:
            board() 
            {
                memset(mV,0,12);
            }
            
            state get(position p)const
            {
                int b = 3*p.get();
                int bit = b%8;
                int byte = b/8;
                int mask = (3 << bit);
                int result = (mV[byte] & mask);
                result = result >> bit;
                return (state)(result);
            }
            
            state get(int x, int y)const
            {
                position p(x,y);
                return get(p);
            }
            
            void set(position p, state s)
            {
                int b = 3*p.get();
                int bit = b%8;
                int byte = b/8;
                mV[byte] |= (s << bit);
            }
            
            void set(int x, int y, state s)
            {
                position p(x,y);
                set(p, s);
            }
            
        private:
            uint8_t mV[12];
    };
}

#endif