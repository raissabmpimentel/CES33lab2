import os
import matplotlib.pyplot as plt
import pandas as pd

col_names_days = ['test number','2020-05-20','2020-05-05','2020-05-28','2020-05-08','2020-05-14',
			 '2020-05-29','2020-05-04','2020-05-18','2020-05-19','2020-05-27','2020-05-26',
			 '2020-05-07', '2020-05-21','2020-05-06','2020-05-12','2020-05-22','2020-05-15',
			 '2020-05-13','2020-05-11','2020-05-25']

col_names_month = ['test number', '2020-05']

def plot(path, date, x, y, month, seq):

	# a new figure window
	fig = plt.figure()

	ax = fig.add_subplot(1, 1, 1)  # specify (nrows, ncols, axnum)

	ref = date
	ref = ref.split("-")
	
	# set a title for y-axis
	ax.set_ylabel("Tempo de Execução", fontsize = 12)
	ax.set_xlabel("Iteração", fontsize = 12)

	if month: 
		ref = ref[1] + "-" + ref[0]
		path = path.replace("results_month.csv", "")
	else:
		ref = ref[2] + "-" + ref[1] + "-" + ref[0]
		path = path.replace("results_days.csv", "")
		
	if seq:
		ax.plot(x, y, 'b.-')
	else:
		ax.plot(x, y, 'r.-')

	title = ref
	ax.set_title(title, fontsize = 16)

	med = sum(y)/len(y)
	ax.axhline(med, xmin=0.03, xmax=0.97, color='black', linewidth=2)

	# print plot
	plt.tight_layout()

	plt.savefig(path + title + ".png")
	plt.close()


def generate_plots_from_file(folder, month, seq):
	path = os.getcwd() + folder
	if month:
		file_name = "results_month.csv"
	else:
		file_name = "results_days.csv"

	path += file_name

	x_label = []
	x = [] # dates
	y = [] # prices

	if not month:
		df = pd.read_csv(path, usecols=col_names_days)
		x = df["test number"].values.tolist()

		for col in range(1, len(col_names_days)):
			c = col_names_days[col]
			y = df[c].values.tolist()
			y = [float(i) for i in y] 
			plot(path, c, x, y, month, seq)
	else:
		df = pd.read_csv(path, usecols=col_names_month)
		x = df["test number"].values.tolist()

		for col in range(1, len(col_names_month)):
			c = col_names_month[col]
			y = df[c].values.tolist()
			y = [float(i) for i in y] 
			plot(path, c, x, y, month, seq)



def generate_all_plots():
	generate_plots_from_file("/tests_thread/", True, False)
	generate_plots_from_file("/tests_seq/", True, True)
	generate_plots_from_file("/tests_thread/", False, False)
	generate_plots_from_file("/tests_seq/", False, True)

generate_all_plots()