#pragma once
/**
 * @file bs_forward_diff.hpp
 * @brief Black-Scholes Complex Step.
 *
 */

#include <complex>
#include "bs_call_price_t.hpp"
#include "bs_call_price.h"

const std::complex<double>	I(0.0, 1.0);
const double				INV_SQRT_2 = 1.0 / std::sqrt(2.0);
const std::complex<double>	OMEGA = (1.0 + I) * INV_SQRT_2;

inline double bs_delta_cs(double S, double K, double r, double q, double sigma, double Tmat, double h) {
	std::complex<double>	S_cs = std::complex<double>(S, 0.0) + I * h;
	std::complex<double>	K_cs = std::complex<double>(K, 0.0);
	std::complex<double>	r_cs = std::complex<double>(r, 0.0);
	std::complex<double>	q_cs = std::complex<double>(q, 0.0);
	std::complex<double>	sigma_cs = std::complex<double>(sigma, 0.0);
	std::complex<double>	Tmat_cs = std::complex<double>(Tmat, 0.0);

	std::complex<double>	C_cs = bs_price_call_t(S_cs, K_cs, r_cs, q_cs, sigma_cs, Tmat_cs);
	return C_cs.imag() / h;
}

inline double bs_gamma_cs_real(double S, double K, double r, double q, double sigma, double Tmat, double h) {
	std::complex<double>	S_cs = std::complex<double>(S, 0.0) + I * h;
	std::complex<double>	K_cs = std::complex<double>(K, 0.0);
	std::complex<double>	r_cs = std::complex<double>(r, 0.0);
	std::complex<double>	q_cs = std::complex<double>(q, 0.0);
	std::complex<double>	sigma_cs = std::complex<double>(sigma, 0.0);
	std::complex<double>	Tmat_cs = std::complex<double>(Tmat, 0.0);
	std::complex<double>	C_cs = bs_price_call_t(S_cs, K_cs, r_cs, q_cs, sigma_cs, Tmat_cs);

	double C_real = bs_price_call(S, K, r, q, sigma, Tmat);
	return -2.0 * (C_cs.real() - C_real) / (h * h);
}

inline double bs_gamma_cs_45(double S, double K, double r, double q, double sigma, double Tmat, double h) {
	std::complex<double>	K_cs = std::complex<double>(K, 0.0);
	std::complex<double>	r_cs = std::complex<double>(r, 0.0);
	std::complex<double>	q_cs = std::complex<double>(q, 0.0);
	std::complex<double>	sigma_cs = std::complex<double>(sigma, 0.0);
	std::complex<double>	Tmat_cs = std::complex<double>(Tmat, 0.0);

	std::complex<double>	S_real(S, 0.0);
	std::complex<double>	S_plus	= S_real + h * OMEGA;
	std::complex<double>	S_minus	= S_real - h * OMEGA;

	std::complex<double>	C_plus = bs_price_call_t(S_plus, K_cs, r_cs, q_cs, sigma_cs, Tmat_cs);
	std::complex<double>	C_minus = bs_price_call_t(S_minus, K_cs, r_cs, q_cs, sigma_cs, Tmat_cs);
	std::complex<double>	C_sum = C_plus + C_minus;
	return C_sum.imag() / (h * h);
}
