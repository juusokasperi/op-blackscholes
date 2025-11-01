import pandas as pd
import matplotlib.pyplot as plt

def print_summary_table(data, scenario_title):
	print(f"\n--- Error Summary Table for {scenario_title} ---")

	error_columns = [
		'err_D_fd', 'err_D_cs',
		'err_G_fd', 'err_G_cs_real', 'err_G_cs_45'
	]

	summary_data = []
	for col in error_columns:
		summary_data.append({
			"Method": col,
			"Max Error (Worst)": data[col].max(),
			"Median Error": data[col].median(),
			"Min Error (Best)": data[col].min()
		})

	summary_df = pd.DataFrame(summary_data)
	print(f"| {'Method':<13} | {'Max Error (Worst)':<17} | {'Median Error':<17} | {'Min Error (Best)':<17} |")
	print(f"|:{'-'*13}:|:{'-'*17}:|:{'-'*17}:|:{'-'*17}:|")
	for _, row in summary_df.iterrows():
		print(f"| {row['Method']:<13} | {row['Max Error (Worst)']:<17.2e} | {row['Median Error']:<17.2e} | {row['Min Error (Best)']:<17.2e} |")
	print("| ------------------------------------------------------------------------- |\n")


def create_plots(csv_filename, scenario_title):
	try:
		data = pd.read_csv(csv_filename)
	except FileNotFoundError:
		print(f"Error: Could not find file '{csv_filename}'")
		return

	print_summary_table(data, scenario_title)

	fig, (ax_delta, ax_gamma) = plt.subplots(1, 2, figsize=(16, 7))
	fig.suptitle(f'Error Analysis for {scenario_title}', fontsize=16)

	ax_delta.set_title('Delta Δ Errors')
	ax_delta.plot(data['h_rel'], data['err_D_fd'], label='Δ_fd (Finite Diff)', marker='o', markersize=4)
	ax_delta.plot(data['h_rel'], data['err_D_cs'], label='Δ_cs (Complex Step)', marker='x', markersize=4)

	ax_delta.set_xscale('log')
	ax_delta.set_yscale('log')
	ax_delta.set_xlabel('Relative Step Size (h_rel)')
	ax_delta.set_ylabel('Absolute Error')
	ax_delta.legend()
	ax_delta.grid(True, which="both", ls="--", alpha=0.5)

	ax_gamma.set_title('Gamma Γ Errors')
	ax_gamma.plot(data['h_rel'], data['err_G_fd'], label='Γ_fd (Finite Diff)', marker='o', markersize=4)
	ax_gamma.plot(data['h_rel'], data['err_G_cs_real'], label='Γ_cs_real (Complex Step Real)', marker='s', markersize=4)
	ax_gamma.plot(data['h_rel'], data['err_G_cs_45'], label='Γ_cs_45 (Complex Step 45◦)', marker='x', markersize=4)

	ax_gamma.set_xscale('log')
	ax_gamma.set_yscale('log')
	ax_gamma.set_xlabel('Relative Step Size (h_rel)')
	ax_gamma.set_ylabel('Absolute Error')
	ax_gamma.legend()
	ax_gamma.grid(True, which="both", ls="--", alpha=0.5)

	outfile = f"{scenario_title.replace(' ', '_').lower()}_error_plots.png"
	plt.savefig(outfile)
	print(f"Saved plot to {outfile}")

	plt.tight_layout()
	plt.show()

if __name__ == "__main__":

	print("Generating plots for Scenario 1")
	create_plots(
		csv_filename="bs_fd_vs_complex_scenario1.csv",
		scenario_title="Scenario 1 (ATM)"
	)

	print("Generating plots for Scenario 2")
	create_plots(
		csv_filename="bs_fd_vs_complex_scenario2.csv",
		scenario_title="Scenario 2 (Stress)"
	)

	print("Done.")

