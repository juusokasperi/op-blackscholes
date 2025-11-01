#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "bs_analytic.hpp"
#include "bs_call_price_t.hpp"
#include "bs_call_price.h"
#include "bs_complex_step.hpp"
#include "bs_forward_diff.hpp"

using namespace std;

void run_validation_sweep(double S, double K, double r, double q, double sigma, double T, const std::string &filename)
{
	std::ofstream	outfile(filename);
	if (!outfile)
		throw std::runtime_error("Unable to open an output filestream with given filename " + filename);
	outfile	<< "h_rel,h,"
			<< "Delta_analytic,Delta_fd,Delta_cs,"
			<< "err_D_fd,err_D_cs,"
			<< "Gamma_analytic,Gamma_fd,Gamma_cs_real,Gamma_cs_45,"
			<< "err_G_fd,err_G_cs_real,err_G_cs_45\n";
	outfile	<< std::fixed << std::setprecision(16);

	const double	delta_analytic = bs_delta_analytic(S, K, r, q, sigma, T);
	const double	gamma_analytic = bs_gamma_analytic(S, K, r, q, sigma, T);

	const double	log_h_min = -16.0;
	const double	log_h_max = -4.0;

	const int		num_steps = 24;
	for (int i = 0; i < num_steps; ++i) {
		double log_h = log_h_min + (log_h_max - log_h_min) * static_cast<double>(i) / (num_steps - 1);
		double h_rel = std::pow(10.0, log_h);
		double h = h_rel * S;

		double delta_fd = bs_delta_fd(S, K, r, q, sigma, T, h);
		double delta_cs = bs_delta_cs(S, K, r, q, sigma, T, h);

		double gamma_fd = bs_gamma_fd(S, K, r, q, sigma, T, h);
		double gamma_cs_real = bs_gamma_cs_real(S, K, r, q, sigma, T, h);
		double gamma_cs_45 = bs_gamma_cs_45(S, K, r, q, sigma, T, h);

		double err_D_fd = std::abs(delta_fd - delta_analytic);
		double err_D_cs = std::abs(delta_cs - delta_analytic);

		double err_G_fd = std::abs(gamma_fd - gamma_analytic);
		double err_G_cs_real = std::abs(gamma_cs_real - gamma_analytic);
		double err_G_cs_45 = std::abs(gamma_cs_45 - gamma_analytic);

		outfile	<< h_rel << "," << h << ","
				<< delta_analytic << "," << delta_fd << "," << delta_cs << ","
				<< err_D_fd << "," << err_D_cs << ","
				<< gamma_analytic << "," << gamma_fd << ","
				<< gamma_cs_real << "," << gamma_cs_45 << ","
				<< err_G_fd << "," << err_G_cs_real << "," << err_G_cs_45 << "\n";
	};
	outfile.close();
	cout << "  File " + filename + " generated.\n";
};

int main() {
	cout << "-- Running Black-Scholes Assignment --\n";
	bool error = false;
	string errorMsg;
	try {
		double S = 100.0, K = 100.0, r = 0, q = 0, sigma = 0.20, T = 1;
		run_validation_sweep(S, K, r, q, sigma, T, "bs_fd_vs_complex_scenario1.csv");
	} catch (const std::exception &e) {
		cerr << "Error: " << e.what() << "\n";
		error = true;
		errorMsg = "scenario 1";
	}
	try {
		double S = 100.0, K = 100.0, r = 0, q = 0, sigma = 0.01, T = 1.0 / 365.0;
		run_validation_sweep(S, K, r, q, sigma, T, "bs_fd_vs_complex_scenario2.csv");
	} catch (const std::exception &e) {
		cerr << "Error: " << e.what() << "\n";
		error = true;
		if (!errorMsg.empty())
			errorMsg = "both scenarios";
		else
			errorMsg = "scenario 2";
	}
	cout << "  Validation sweeps complete.\n";
	if (error)
		cout << "    Failure running " + errorMsg + "\n";
	cout << "-- Exiting program --\n";
	return error ? 1 : 0;
};
