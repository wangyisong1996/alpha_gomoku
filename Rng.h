#ifndef RNG_H
#define RNG_H

#include <cstdint>

class RNG {
public:
    RNG() {}
    virtual ~RNG() = 0;
};

class Xorshift : public RNG {
public:
    Xorshift(uint32_t x, uint32_t y, uint32_t z, uint32_t w)
        : x(x), y(y), z(z), w(w) {}
    ~Xorshift(){};

    uint32_t rand() {
        uint32_t t = x;
        t ^= t << 11;
        t ^= t >> 8;
        x = y;
        y = z;
        z = w;
        w ^= w >> 19;
        w ^= t;
        return w;
    }

private:
    /* The state must be seeded so that it is not everywhere zero. */
    uint32_t x, y, z, w;
};

#endif  // RNG_H