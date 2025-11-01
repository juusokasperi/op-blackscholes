#pragma once
/**
 * @file bs_analytic.hpp
 * @brief Black-Scholes Analytic Greeks.
 *
 */

#include "bs_call_price.h"
#include <cmath>

inline double bs_delta_analytic(double S, double K, double r, double q, double sigma, double T) {
	const double F				= F_calc(S, r, q, T);
	const double sigmaT			= sigmaT_calc(sigma, T);
	if (sigmaT == 0.0) {
		if (F > K) return std::exp(-q * T);
		else if (F < K) return 0.0;
		else return 0.5 * std::exp(-q * T);
	}
	const double ln_F_over_K	= ln_F_over_K_calc(F, K);
	const double d1				= d1_calc(ln_F_over_K, sigma, T, sigmaT);
	return std::exp(-q * T) * Phi_real(d1);
}

inline double bs_gamma_analytic(double S, double K, double r, double q, double sigma, double T) {
	const double F		= F_calc(S, r, q, T);
	const double sigmaT	= sigmaT_calc(sigma, T);
	if (sigmaT == 0.0) return 0.0;

	const double ln_F_over_K	= ln_F_over_K_calc(F, K);
	const double d1				= d1_calc(ln_F_over_K, sigma, T, sigmaT);
	const double log_sqrt_2pi	= std::log(std::sqrt(2.0 * M_PI));
	const double log_phi_d1		= -0.5 * d1 * d1 - log_sqrt_2pi;
	const double phi_d1_stable	= std::exp(log_phi_d1);
	return std::exp(-q * T) * phi_d1_stable / (S * sigmaT);
}
