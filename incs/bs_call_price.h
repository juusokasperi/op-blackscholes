#pragma once
/**
 * @file bs_call_price.hpp
 * @brief Compact Black–Scholes helpers + call price.
 *
 * Exposes:
 *  - Phi_real(z): standard normal CDF Φ(z).
 *  - phi(z):      standard normal PDF φ(z).
 *  - bs_price_call(S,K,r,q,σ,T): European call price (with continuous yield q).
 *  - F_calc(S,r,q,t)
 *  - sigmaT_calc(sigma,T)
 *  - ln_F_over_K_calc(F,K)
 *  - d1_calc(ln_F_over_K,sigma,T,sigmaT)
 *
 * Intended as the minimal building block for Greeks.
 */

#include <cmath>
#include <algorithm>

// Φ(z): standard normal CDF
inline double Phi_real(double z) {
    static constexpr double INV_SQRT_2 = 0.70710678118654752440;
    return 0.5 * std::erfc(-z * INV_SQRT_2);
}

// φ(z): standard normal PDF
inline double phi(double z) {
    // 1/sqrt(2π)
    static constexpr double INV_SQRT_2PI = 0.39894228040143267794;
    return INV_SQRT_2PI * std::exp(-0.5 * z * z);
}

inline double F_calc(double S, double r, double q, double T) {
    return S * std::exp((r - q) * T);
}

inline double sigmaT_calc(double sigma, double T) {
    return sigma * std::sqrt(std::max(T, 0.0));
}

inline double ln_F_over_K_calc(double F, double K) {
    double ln_F_over_K;
    if (K > 0.0) {
        const double x = (F - K) / K;
        ln_F_over_K = (std::abs(x) <= 1e-12) ? std::log1p(x) : std::log(F / K);
    } else {
        ln_F_over_K = std::log(F / K);
    }
    return ln_F_over_K;
}

inline double d1_calc(double ln_F_over_K, double sigma, double T, double sigmaT) {
    return (ln_F_over_K + 0.5 * sigma * sigma * T) / sigmaT;
}

// Black-Scholes call-price
inline double bs_price_call(double S, double K, double r, double q, double sigma, double T) {
    const double DF     = std::exp(-r * T);
    const double F      = F_calc(S, r, q, T);
    const double sigmaT = sigmaT_calc(sigma, T);
    if (sigmaT == 0.0) return DF * std::max(F - K, 0.0);

    const double ln_F_over_K = ln_F_over_K_calc(F, K);

    const double d1 = d1_calc(ln_F_over_K, sigma, T, sigmaT);
    const double d2 = d1 - sigmaT;

    return DF * (F * Phi_real(d1) - K * Phi_real(d2));
}
