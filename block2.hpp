#include <valarray>
#include <complex>
class block2 {
public:
    std::valarray<double> modulated;
    std::valarray<double> control;
    std::valarray<double> output;
    std::valarray<std::complex<double>> output_f;
    std::valarray<double> time;
    std::valarray<long> freq;
    double am;
    double df;
    double kf;
    double fs;
    double beta;
    double error = 1.0;
    double carrier;
    size_t iterations = 0;
    block2(std::valarray<double> modulated, std::valarray<double> &time, double kf, double fs, double carrier) {
        this->modulated = modulated;
        this->time = time;
        this->kf = kf;
        this->fs = fs;
        this->carrier = carrier;
        output = modulated;
        control.resize(time.size());
        output.resize(time.size());
        output_f.resize(time.size());
    // calculate frequency axis based on given data
    freq.resize(time.size(), 0);
    for(int i = 0; i < freq.size(); ++i) {
        freq[i] = -time.size()/2 + i;
    }
    // normalize this new vector
    freq*=(-fs/time.size());
    }
    void take_fft_output();
    void pll();
    void mse();
};