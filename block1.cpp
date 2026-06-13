#include <cmath>
#include <random>
#include <valarray>
#include <iostream>
#include <AudioFile.h>

#include "block1.hpp"
#include "fft.hpp"

constexpr double pi = 3.1415926535897932384626433832795028841971693993751058209749445923078164062;


// utility functions
[[gnu::always_inline]]
inline double triple_max(double a, double b, double c) {
    return std::max(std::max(a,b),c);
}

void fftshift_double(std::valarray<double> in, std::valarray<double> out) {

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
        freq*=(-fs/time.size());

        original.resize(time.size(), 0);
        modulated.resize(time.size(), 0);
        original_f.resize(freq.size(), 0 + 0j);
        modulated_f.resize(freq.size(), 0 + 0j);
        // set the original signal
        original = cos(2*pi*fm1*time) + cos(2*pi*fm2*time) + cos(2*pi*fm3*time);
}

message_signal::message_signal(std::string audio_path) {
    AudioFile<double> audio_file;
    audio_file.load(audio_path);
    audio_file.setNumChannels(1);
    double fs = audio_file.getSampleRate();
    size_t n = audio_file.getNumSamplesPerChannel();
    original.resize(n);
    time.resize(n);
    for(size_t i = 0; i < n; ++i) {
        original[i] = audio_file.samples[0][i];
        time[i] = i;
    }
    // calculate frequency axis based on given data
    freq.resize(time.size(), 0);
    for(int i = 0; i < freq.size(); ++i) {
        freq[i] = -time.size()/2 + i;
    }
    // normalize this new vector
    freq*=(-fs/time.size());

    // initialize the rest of the data
    modulated.resize(time.size(), 0);
    original_f.resize(freq.size(), 0 + 0j);
    modulated_f.resize(freq.size(), 0 + 0j);
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
    size_t n = freq.size();
    for(size_t i = 0; i < n/2; ++i) {
        auto buffer = original_f[n/2 + i];
        original_f[n/2 + i] = original_f[i];
        original_f[i] = buffer;
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
    size_t n = freq.size();
    for(size_t i = 0; i < n/2; ++i) {
        auto buffer = modulated_f[n/2 + i];
        modulated_f[n/2 + i] = modulated_f[i];
        modulated_f[i] = buffer;
    }

}

void message_signal::add_noise() {
    double mean = 0.0;
    double stddef = 0.1;
    std::default_random_engine rng(std::random_device{}());
    std::normal_distribution<double> dist {mean, stddef};
    for(double &d: modulated) {
        d += dist(rng);
    }
}

void message_signal::save_to_file(std::string path) {
    AudioFile<double> buffer;
    size_t n = this->original.size();
    buffer.setAudioBufferSize(1, n); 
    for(int i = 0; i < n; ++i) {
        buffer.samples[0][i] = original[i];
    }
    buffer.save(path, AudioFileFormat::Wave);
}