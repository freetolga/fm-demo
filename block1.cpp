#include <cmath>
#include <valarray>

#include "block1.h"

constexpr double pi = 3.14159265358979;


// utility functions
[[gnu::always_inline]]
inline double triple_max(double a, double b, double c) {
    return std::max(std::max(a,b),c);
}


message_signal::message_signal(double fm1, double fm2, double fm3, double fc, std::valarray<double> time) {
        // set the time valarray passed in
        this->time = time;
        // set the constant fc given
        this->fc = fc;
        // calculate fs and set it
        this->fs = 2*(fc + triple_max(fm1, fm2,fm3));

        // calculate frequency axis based on given data
        freq.resize(time.size(), 0);
        for(int i = 0; i < freq.size(); ++i) {
            freq[i] = -time.size()/2 + i;
        }
        // normalize this new vector
        freq*(-fs/time.size());

        original.resize(time.size(), 0);
        modulated.resize(time.size(), 0);

        // set the original signal
        original = cos(2*pi*fm1*time) + cos(2*pi*fm2*time) + cos(2*pi*fm3*time);
}

void message_signal::set_ac(double newac) {
    this->ac = newac;
    original *=newac;
}

void message_signal::modulate() {
    // calculate the cumulative sum for every element in this new vector
    std::valarray<double> inst_freq;
    inst_freq.resize(original.size(), 0);
    for(int i = 0; i < original.size(); ++i) {
        inst_freq[i] = 2*pi*kf*original[i];
    }
    // finish FM
    modulated = cos(2*pi*fc*time + inst_freq);
}

