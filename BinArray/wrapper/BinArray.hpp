#include "../BinArray.h"

namespace ba {
/*
    Сделать счётчик тяжёлых операций. 


*/
class binarray {
    BinArray* arr_;

    int id_;
    static int global_id;
    static int global_id_max;

public:
    binarray (size_t size);
    ~binarray () noexcept;

    binarray (const binarray& other);
    binarray (binarray&& other) noexcept;

    binarray& operator= (binarray   other) noexcept;
    binarray& operator= (binarray&& other) noexcept;
    
    binarray& operator|= (const binarray& rhs);
    binarray& operator&= (const binarray& rhs);

    class proxyBit {
        binarray& binarr_;
        size_t num_bit_;
        
    public:
        proxyBit (binarray& binarr, size_t num_bit) :
            binarr_ (binarr),
            num_bit_ (num_bit)
        {}

        proxyBit& operator  = (const bool val);
        proxyBit& operator &= (const bool val);
        proxyBit& operator |= (const bool val);
        operator bool ();
    };

    proxyBit operator[] (size_t num_bit);

    bool operator[] (size_t num_bit) const;

    void CheckNumBits (const binarray& other) const;
    void swap (binarray& other) noexcept;
    void invert ();
    void logicAnd (const binarray& other);

    size_t getNumBits () const noexcept;
};

    binarray operator &  (const binarray& lhs, const binarray& rhs);
    binarray operator |  (const binarray& lhs, const binarray& rhs);
    binarray operator && (const binarray& lhs, const binarray& rhs);
}