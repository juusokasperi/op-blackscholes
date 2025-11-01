#pragma once
/**
 * @file bs_forward_diff.hpp
 * @brief Black-Scholes Classical Forward Differences.
 *
 * Exposes:
 *  - bs_delta_fd(S,K,r,q,sigma,T,h)
 *  - bs_gamma_fd(S,K,r,q,sigma,T,h)
 *
 */

#include "bs_call_price.h"
#include <cmath>

inline double bs_delta_fd(double S, double K, double r, double q, double sigma, double T, double h) {
	double price_at_S_plus_h	= bs_price_call(S + h, K, r, q, sigma, T);
	double price_at_S			= bs_price_call(S, K, r, q, sigma, T);
	return (price_at_S_plus_h - price_at_S) / h;
}

inline double bs_gamma_fd(double S, double K, double r, double q, double sigma, double T, double h) {
	double price_at_S_plus_2h	= bs_price_call(S + 2 * h, K, r, q, sigma, T);
	double price_at_S_plus_h	= bs_price_call(S + h, K, r, q, sigma, T);
	double price_at_S			= bs_price_call(S, K, r, q, sigma, T);
	return (price_at_S_plus_2h - 2.0 * price_at_S_plus_h + price_at_S) / (h * h);
}
