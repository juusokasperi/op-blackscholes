#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>

#include "bs_analytic.hpp"
#include "bs_call_price_t.hpp"
#include "bs_call_price.h"
#include "bs_complex_step.hpp"
#include "bs_forward_diff.hpp"

using namespace std;

bool create_directory(const string& dir_path) {
	#ifdef _WIN32
		int result = _mkdir(dir_path.c_str());
	#else
		int result = mkdir(dir_path.c_str(), 0755);
	#endif

	if (result == 0)
		return true;
	else if (errno == EEXIST)
		return true;
	else
		return false;
}

void run_validation_sweep(double S, double K, double r, double q, double sigma, double T, const string &filename)
{
	constexpr double	LOG_H_MIN = -16.0;
	constexpr double	LOG_H_MAX = -4.0;
	constexpr int		NUM_STEPS = 24;

	ofstream	outfile(filename);
	if (!outfile)
		throw runtime_error("Unable to open an output filestream with given filename " + filename);
	outfile	<< "h_rel,h,"
			<< "Delta_analytic,Delta_fd,Delta_cs,"
			<< "err_D_fd,err_D_cs,"
			<< "Gamma_analytic,Gamma_fd,Gamma_cs_real,Gamma_cs_45,"
			<< "err_G_fd,err_G_cs_real,err_G_cs_45\n";
	outfile	<< fixed << setprecision(16);

	const double	delta_analytic = bs_delta_analytic(S, K, r, q, sigma, T);
	const double	gamma_analytic = bs_gamma_analytic(S, K, r, q, sigma, T);

	for (int i = 0; i < NUM_STEPS; ++i) {
		double log_h = LOG_H_MIN + (LOG_H_MAX - LOG_H_MIN) * static_cast<double>(i) / (NUM_STEPS - 1);
		double h_rel = pow(10.0, log_h);
		double h = h_rel * S;

		double delta_fd = bs_delta_fd(S, K, r, q, sigma, T, h);
		double delta_cs = bs_delta_cs(S, K, r, q, sigma, T, h);

		double gamma_fd = bs_gamma_fd(S, K, r, q, sigma, T, h);
		double gamma_cs_real = bs_gamma_cs_real(S, K, r, q, sigma, T, h);
		double gamma_cs_45 = bs_gamma_cs_45(S, K, r, q, sigma, T, h);

		double err_D_fd = abs(delta_fd - delta_analytic);
		double err_D_cs = abs(delta_cs - delta_analytic);

		double err_G_fd = abs(gamma_fd - gamma_analytic);
		double err_G_cs_real = abs(gamma_cs_real - gamma_analytic);
		double err_G_cs_45 = abs(gamma_cs_45 - gamma_analytic);

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
	string path;

	if (!create_directory("data")) {
		cerr << "  Warning: Could not create 'data' directory, .csv files will be saved in current directory.\n";
	} else
		path = "data/";
	try {
		double S = 100.0, K = 100.0, r = 0, q = 0, sigma = 0.20, T = 1;
		string outfile = path + "bs_fd_vs_complex_scenario1.csv";
		run_validation_sweep(S, K, r, q, sigma, T, outfile);
	} catch (const exception &e) {
		cerr << "Error: " << e.what() << "\n";
		error = true;
		errorMsg = "scenario 1";
	}
	try {
		double S = 100.0, K = 100.0, r = 0, q = 0, sigma = 0.01, T = 1.0 / 365.0;
		string outfile = path + "bs_fd_vs_complex_scenario1.csv";
		run_validation_sweep(S, K, r, q, sigma, T, path + "bs_fd_vs_complex_scenario2.csv");
	} catch (const exception &e) {
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
