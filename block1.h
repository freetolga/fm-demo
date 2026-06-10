#include <cmath>
#include <valarray>

struct message_signal {
public:
    // time vector
    std::valarray<double> time;
    // frequency vector
    std::valarray<double> freq;
    // original and modulated signal
    std::valarray<double> original;
    std::valarray<double> modulated;
    // constant carrier frequency
    double fc = 0;
    // constant sampling frequency
    double fs = 0;
    // changeable carrier amplitude
    double ac = 1.0;
    // changeable FM constant
    double kf = 1.0;
    message_signal(double fm1, double fm2, double fm3, double fc, std::valarray<double> time);
    void set_ac(double newac);
    void modulate();
};
