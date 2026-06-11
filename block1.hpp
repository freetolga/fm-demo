#include <valarray>
#include <complex>

struct message_signal {
public:
    // time vector
    std::valarray<double> time;
    // frequency vector
    std::valarray<long> freq;
    // original and modulated signal
    std::valarray<double> original;
    std::valarray<double> modulated;
    std::valarray<std::complex<double>> original_f;
    std::valarray<std::complex<double>> modulated_f;
    // constant carrier frequency
    double fc = 0;
    // constant sampling frequency
    double fs = 0;
    // changeable carrier amplitude
    double ac = 1.0;
    // changeable FM constant
    double kf = 1.0;
    message_signal(double fm1, double fm2, double fm3, double fc);
    void set_ac(double newac);
    void modulate();
    void take_fft_message();
    void take_fft_modulated();
};
