// using method here
// https://stackoverflow.com/questions/11942855/matlab-hilbert-transform-in-c
#include "hilbert.hpp"

void hilbert(std::valarray<std::complex<double>> &signal_mutable) {
    // take fft
    fft(signal_mutable);
    // delete upper half of array to delete negative frequencies
    for(size_t i = signal_mutable.size()/2; i < signal_mutable.size(); ++i) {
        signal_mutable[i] = std::complex<double> {0, 0};
    }
    // take ifft
    fft(signal_mutable);

    // negate the minus sign
    signal_mutable = -signal_mutable;
}