#pragma once
/**
 * @file bs_call_price_t.hpp
 * @brief Compact Templated Black–Scholes helpers + call price.
 *
 */

#include <complex>
#include "bs_call_price.h"

template<typename T>
inline T Phi_t(T z) {
	return Phi_real(static_cast<double>(z));
}

template<>
inline std::complex<double> Phi_t(std::complex<double> z) {
	double z_r = z.real();
	double z_i = z.imag();

	return std::complex<double>(Phi_real(z_r), z_i * phi(z_r));
}

template<typename T>
inline T F_calc_t(T S, T r, T q, T Tmat) {
    return S * std::exp((r - q) * Tmat);
}

template<typename T>
inline T sigmaT_calc_t(T sigma, T Tmat) {
    return sigma * std::sqrt(Tmat);
}

template<typename T>
inline T ln_F_over_K_calc_t(T F, T K) {
    return std::log(F / K);
}

template<typename T>
inline T d1_calc_t(T ln_F_over_K, T sigma, T Tmat, T sigmaT) {
    return (ln_F_over_K + T(0.5) * sigma * sigma * Tmat) / sigmaT;
}

template<class T>
inline T bs_price_call_t(T S, T K, T r, T q, T sigma, T Tmat) {
    const T DF     = std::exp(-r * Tmat);
    const T F      = F_calc_t(S, r, q, Tmat);
    const T sigmaT = sigmaT_calc_t(sigma, Tmat);

    const T ln_F_over_K = ln_F_over_K_calc_t(F, K);

    const T d1 = d1_calc_t(ln_F_over_K, sigma, Tmat, sigmaT);
    const T d2 = d1 - sigmaT;

    return DF * (F * Phi_t(d1) - K * Phi_t(d2));
}
