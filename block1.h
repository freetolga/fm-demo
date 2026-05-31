#include <cmath>
#include <numbers>
#include <numeric>
#include <valarray>

class message_signal {
    public:
    std::valarray<double> time;
    std::valarray<double> original;
    std::valarray<double> modulated;
    // constant carrier frequency
    double fc;
    // constant sampling frequency
    double fs;
    // changeable carrier amplitude
    double ac = 1.0;
    // changeable FM constant
    double kf = 1.0;
    // constructor
    message_signal(double fm1, double fm2, double fm3, double fc, std::valarray<double> time);
    void set_ac(double newac);
    void modulate();

};