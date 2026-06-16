#include "block2.hpp"
#include "butterworth.hpp"
#include "fft.hpp"
#include "hilbert.hpp"
#include <cmath>
#include <valarray>
#include <iostream>
#include <complex>

constexpr double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;

[[gnu::always_inline]]
inline double sum(std::valarray<double> array) {
    return array.sum();
}


void block2::pll_demodulate() {
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
    pll_demodulate();
}


//https://dsp.stackexchange.com/questions/44710/hilbert-transform-of-an-fm-signal
// https://stackoverflow.com/questions/15634400/continous-angles-in-c-eq-unwrap-function-in-matlab
void block2::hilbert_demodulate() {
    // make buffer such that for every z in buffer
    // z = m + 0j
    // where m is the same index element in modulated signal
    std::valarray<std::complex<double>> buffer(modulated.size());
    for(int i = 0; i < modulated.size(); ++i) {
        buffer[i] = {modulated[i], 0};
        std::cout << "buffer: " << buffer[i] << std::endl;
    }
    // take the hilbert transform of the buffer by mutating it
    hilbert(buffer);
    std::valarray<double> phase(modulated.size());
    for(int i = 0; i < modulated.size(); ++i) {
        phase[i] = arg(buffer[i]);
        std::cout << "phase: " << phase[i] << std::endl;
    }

    // uncompacted angle unwrap
    // https://stackoverflow.com/questions/15634400/continous-angles-in-c-eq-unwrap-function-in-matlab
    std::valarray<double> diff_phase(modulated.size());
    for(int i = 1; i < modulated.size(); ++i) {
        double d = phase[i] - phase[i - 1];
        if(d > pi) {
            diff_phase[i] = d - 2*pi;
        } else if(d < -pi) {
            diff_phase[i] = d + 2*pi;
        } else {
            diff_phase[i] = d;
        }
    }

    // take a forward divided difference and divide it by 2*pi*dt
    // where dt = 1/fs
    // save result in output
    for(int i = 1; i < modulated.size(); ++i) {
        output[i] = (diff_phase[i] - diff_phase[i - 1])*fs/(2*pi);
    }

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