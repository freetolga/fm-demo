#include <cmath>
#include <valarray>
#include <iostream>

#include "block1.hpp"
#include "fft.hpp"

constexpr double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;


// utility functions
[[gnu::always_inline]]
inline double triple_max(double a, double b, double c) {
    return std::max(std::max(a,b),c);
}


message_signal::message_signal(double fm1, double fm2, double fm3, double fc) {

        this->fc = fc;
        this->fs = 2*(fc + triple_max(fm1, fm2,fm3));

        double t2 = 0.1;
        double dt = 1/fs;
        int size = (t2-0)/dt;
        time.resize((t2-0)/dt, 0);
        for(int i = 0; i < size; ++i) {
            time[i] = dt*i;
        }

        // calculate frequency axis based on given data
        freq.resize(time.size(), 0);
        for(int i = 0; i < freq.size(); ++i) {
            freq[i] = -time.size()/2 + i;
        }
        // normalize this new vector
        std::cout << fs;
        freq*=(-fs/time.size());

        original.resize(time.size(), 0);
        modulated.resize(time.size(), 0);
        original_f.resize(freq.size(), 0 + 0j);
        modulated_f.resize(freq.size(), 0 + 0j);
        // set the original signal
        original = cos(2*pi*fm1*time) + cos(2*pi*fm2*time) + cos(2*pi*fm3*time);
}

void message_signal::set_ac(double newac) {
    this->ac = newac;
    original *=newac;
}

void message_signal::modulate() {
    // calculate the instantaneous frequency
    std::valarray<double> inst_freq;
    inst_freq.resize(original.size(), 0);
    for(int i = 0; i < inst_freq.size(); ++i) {
        inst_freq[i] = 2*pi*fc*time[i] + 2*pi*kf*original[i];
    }

    // calculate the final angle for cosine
    std::valarray<double> final_angle;
    final_angle.resize(inst_freq.size(), 0);
    double total = 0.0;
    for(int i = 0; i < final_angle.size(); ++i) {
        total += inst_freq[i];
        final_angle[i] = total/fs;
    }
    // finish FM
    modulated = cos(final_angle);
}

void message_signal::take_fft_message() {
    // take imaginary part as 0
    for(int i = 0; i < time.size(); ++i) {
        original_f[i] = std::complex<double>(original[i],0);
    }
    // take the fft
    fft(original_f);

    // shift the fft
    for(int i = 0; i < freq.size()/2; ++i) {
        original_f[freq.size()/2 - i] = original_f[i];
        original_f[freq.size()/2 + i] = original_f[freq.size() - i];
    }

}

void message_signal::take_fft_modulated() {
    // take imaginary part as 0
    for(int i = 0; i < time.size(); ++i) {
        modulated_f[i] = std::complex<double>(modulated[i],0);
    }
    // take the fft
    fft(modulated_f);
   // shift the fft
    for(int i = 0; i < freq.size()/2; ++i) {
        modulated_f[freq.size()/2 - i] = modulated_f[i];
        modulated_f[freq.size()/2 + i] = modulated_f[freq.size() - i];
    }
}