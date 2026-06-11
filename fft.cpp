// copy-pasted from
// https://www.w3computing.com/articles/how-to-implement-a-fast-fourier-transform-fft-in-cpp/

#include <complex>
#include <valarray>

using namespace std;
using Complex = complex<double>;
using CArray = valarray<Complex>;

const double PI = acos(-1);

void fft(CArray &x) {
    const size_t N = x.size();
    if (N <= 1) return;

    // Divide
    CArray even(N / 2);
    CArray odd(N / 2);
    for (size_t i = 0; i < N / 2; ++i) {
        even[i] = x[i * 2];
        odd[i] = x[i * 2 + 1];
    }

    // Conquer
    fft(even);
    fft(odd);

    // Combine
    for (size_t k = 0; k < N / 2; ++k) {
        Complex t = polar(1.0, -2 * PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
}