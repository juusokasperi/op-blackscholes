#pragma once
/**
 * @file bs_call_price_t.hpp
 * @brief Compact Templated Black–Scholes helpers + call price.
 *
 */

#include <complex>
#include "bs_call_price.h"

template<typename Cpx>
inline Cpx Phi_t(Cpx z) {
	return Phi_real(static_cast<double>(z));
}

template<>
inline std::complex<double> Phi_t(std::complex<double> z) {
	double z_r = z.real();
	double z_i = z.imag();

	return std::complex<double>(Phi_real(z_r), z_i * phi(z_r));
}

template<typename Cpx>
inline Cpx F_calc_t(Cpx S, Cpx r, Cpx q, Cpx T) {
    return S * std::exp((r - q) * T);
}

template<typename Cpx>
inline Cpx sigmaT_calc_t(Cpx sigma, Cpx T) {
    return sigma * std::sqrt(T);
}

template<typename Cpx>
inline Cpx ln_F_over_K_calc_t(Cpx F, Cpx K) {
    return std::log(F / K);
}

template<typename Cpx>
inline Cpx d1_calc_t(Cpx ln_F_over_K, Cpx sigma, Cpx T, Cpx sigmaT) {
    return (ln_F_over_K + Cpx(0.5) * sigma * sigma * T) / sigmaT;
}

template<class Cpx>
inline Cpx bs_price_call_t(Cpx S, Cpx K, Cpx r, Cpx q, Cpx sigma, Cpx T) {
    const Cpx DF     = std::exp(-r * T);
    const Cpx F      = F_calc_t(S, r, q, T);
    const Cpx sigmaT = sigmaT_calc_t(sigma, T);

    const Cpx ln_F_over_K = ln_F_over_K_calc_t(F, K);

    const Cpx d1 = d1_calc_t(ln_F_over_K, sigma, T, sigmaT);
    const Cpx d2 = d1 - sigmaT;

    return DF * (F * Phi_t(d1) - K * Phi_t(d2));
}
