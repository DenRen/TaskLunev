#include "BinArray.hpp"
#include <stdexcept>

namespace ba {
    void binarray::CheckNumBits (const binarray& other) const {
        if (getNumBits () != other.getNumBits ())
            throw std::runtime_error ("Sizes are different");
    }

    binarray::binarray (size_t size) :
        arr_ (baCreate (size))
    {
        if (arr_ == NULL)
            throw std::runtime_error ("baCreate error");
    }

    binarray::binarray (const binarray& other) :
        arr_ (baGetClone (other.arr_))
    {
        if (arr_ == NULL)
            throw std::runtime_error ("baGetClone error");
    }

    binarray::binarray (binarray&& other) noexcept {
        swap (other);
    }

    binarray::~binarray () noexcept {
        baDestroy (&arr_);
    }

    binarray& binarray::operator= (binarray other) noexcept {
        swap (other);
        
        return *this;
    }

    binarray& binarray::operator= (binarray&& other) noexcept {
        swap (other);

        return *this;
    }

    binarray& binarray::operator|= (const binarray& other) {
        size_t num_bits = getNumBits ();

        if (num_bits != other.getNumBits ())
            throw std::runtime_error ("Sizes are different");

        while (num_bits--)
            (*this)[num_bits] |= other[num_bits];

        return *this;
    }

    binarray& binarray::operator&= (const binarray& rhs) {
        CheckNumBits (rhs);

        size_t num_bits = getNumBits ();

        while (num_bits--)
            (*this)[num_bits] &= rhs[num_bits];

        return *this;
    }

    binarray::proxyBit binarray::operator[] (size_t num_bit) {
        return proxyBit {*this, num_bit};
    }

    binarray::proxyBit& binarray::proxyBit::operator= (const bool val) {
        baSetValue (binarr_.arr_, num_bit_, val);
        return *this;
    }

    binarray::proxyBit::operator bool () {
        return baGetValue (binarr_.arr_, num_bit_);
    }

    bool binarray::operator[] (size_t num_bit) const {
        int val = baGetValue (arr_, num_bit);
        if (val != -1)
            return val;
        else
            throw std::runtime_error ("baGetValue error");
    }

    binarray::proxyBit& binarray::proxyBit::operator&= (const bool val) {
        int elem = baGetValue (binarr_.arr_, num_bit_);
        baSetValue (binarr_.arr_, num_bit_, elem & val);
        
    }
    binarray::proxyBit& binarray::proxyBit::operator|= (const bool val) {
        int elem = baGetValue (binarr_.arr_, num_bit_);
        baSetValue (binarr_.arr_, num_bit_, elem | val);
        
    }

    void binarray::swap (binarray& other) noexcept {
        std::swap (arr_, other.arr_);
    }

    void binarray::invert () {
        if (baInvert (arr_) == -1)
            throw std::runtime_error ("baInvert error");
    }
    
    void binarray::logicAnd (const binarray& other) {
        CheckNumBits (other);

        size_t num_bits = getNumBits ();

        while (num_bits--)
            (*this)[num_bits] = (*this)[num_bits] && other[num_bits];
    }

    size_t binarray::getNumBits () const noexcept {
        return baGetNumBits (arr_);
    }

    binarray operator & (const binarray& lhs, const binarray& rhs) {
        binarray result {lhs};
        result &= rhs;
        return result;
    }

    binarray operator | (const binarray& lhs, const binarray& rhs) {
        binarray result {lhs};
        result |= rhs;
        return result;
    }

    binarray operator && (const binarray& lhs, const binarray& rhs) {
        binarray result {lhs};
        result.logicAnd (rhs);
        return result;
    }
}