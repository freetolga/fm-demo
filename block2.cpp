#include "block2.hpp"
#include "butterworth.hpp"
#include "fft.hpp"
#include <cmath>
#include <valarray>
#include <iostream>
#include <complex>

constexpr double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

[[gnu::always_inline]]
inline double sum(std::valarray<double> array) {
    return array.sum();
}


void block2::pll() {
    double prev_error = error;
    // check if we are done
    if (iterations > 10000) return;
    // apply phase detector only to find control for error detection
    for(int t = 0; t < output.size(); ++t) {
        output[t] *= cos(2*pi*carrier*time[t]); 
    }
    // apply LPF
    for(int t = 0; t < output.size(); ++t) {
        output[t] *= std::exp(-2*pi*carrier*time[t]);
    }
    // copy output signal before changing it for control
    control = output;
    // apply oscilator
    double inst_freq = carrier;
    double inst_phase = 2*pi*inst_freq/fs;
    for(int i = 1; i < output.size(); ++i) {
        inst_freq += (output[i] - output[i - 1])/(time[i] - time[i - 1]);
        inst_phase += 2*pi*inst_freq/fs;
        output[i] = sin(inst_phase);
    }
    // calculate mean square error
    mse();
    if(error > prev_error) return;
    // recurse
    iterations += 1;
    pll();
}

void block2::mse() {
    for(int i = 0; i < output.size(); ++i) {
        double diff = output[i] - control[i];
        error += diff*diff;
    }

    error /= output.size();
    std::cout << "MSE: " << error << std::endl;
}

void block2::take_fft_output() {
    // take imaginary part as 0
    for(int i = 0; i < time.size(); ++i) {
        output_f[i] = std::complex<double> {output[i], 0};
    }
    // take the fft
    fft(output_f);

    // shift the fft
    size_t n = freq.size();
    for(size_t i = 0; i < n/2; ++i) {
        auto buffer = output_f[n/2 + i];
        output_f[n/2 + i] = output_f[i];
        output_f[i] = buffer;
    }

}