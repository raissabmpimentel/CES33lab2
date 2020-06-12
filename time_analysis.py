import os
import matplotlib.pyplot as plt
import pandas as pd

# define list of names of cols stored in results_days.csv
col_names_days = ['test number','2020-05-20','2020-05-05','2020-05-28','2020-05-08','2020-05-14',
			 '2020-05-29','2020-05-04','2020-05-18','2020-05-19','2020-05-27','2020-05-26',
			 '2020-05-07', '2020-05-21','2020-05-06','2020-05-12','2020-05-22','2020-05-15',
			 '2020-05-13','2020-05-11','2020-05-25']

# define list of names of cols stored in results_month.csv
col_names_month = ['test number', '2020-05']


def plot(path, date, x, y, month, seq):
	# split month/date reference for title
	ref = date
	ref = ref.split("-")

	# create a new figure window
	fig = plt.figure()
	ax = fig.add_subplot(1, 1, 1)
	
	# set a label for x-axis and y-axis
	ax.set_xlabel("Iteração", fontsize = 12)
	ax.set_ylabel("Tempo de Execução", fontsize = 12)

	if month: 
		# format month reference
		ref = ref[1] + "-" + ref[0]

		# grab path without file_name
		path = path.replace("results_month.csv", "")
	else:
		# format date reference
		ref = ref[2] + "-" + ref[1] + "-" + ref[0]

		# grab path without file_name
		path = path.replace("results_days.csv", "")
	
	# plot time spent to execute as a function of iterations
	# if seq, it should be blue
	if seq:
		ax.plot(x, y, 'b.-')
	# if not seq, it should be red
	else:
		ax.plot(x, y, 'r.-')

	# set title to the month/date reference
	title = ref
	ax.set_title(title, fontsize = 16)

	# insert a horizontal line in plot with the average time spent to execute
	med = sum(y)/len(y)
	ax.axhline(med, xmin=0.03, xmax=0.97, color='black', linewidth=2)

	# print plot with margins
	plt.tight_layout()

	# save fig to the same folder it comes from
	plt.savefig(path + title + ".png")
	plt.close()


def generate_plots_from_file(folder, month, seq):
	# discover entire path of the file you want to generate the plot from
	path = os.getcwd() + folder
	if month:
		file_name = "results_month.csv"
	else:
		file_name = "results_days.csv"
	path += file_name

	x = [] # iteration
	y = [] # time spent to execute

	# read each col of file and generate plot
	if month:
		df = pd.read_csv(path, usecols = col_names_month)
		x = df["test number"].values.tolist()

		for col in range(1, len(col_names_month)):
			c = col_names_month[col]
			y = df[c].values.tolist()
			y = [float(i) for i in y] 
			plot(path, c, x, y, month, seq)
	else:
		df = pd.read_csv(path, usecols = col_names_days)
		x = df["test number"].values.tolist()

		for col in range(1, len(col_names_days)):
			c = col_names_days[col]
			y = df[c].values.tolist()
			y = [float(i) for i in y] 
			plot(path, c, x, y, month, seq)


def generate_all_plots():
	generate_plots_from_file("/tests_thread/", True, False)
	generate_plots_from_file("/tests_seq/", True, True)
	generate_plots_from_file("/tests_thread/", False, False)
	generate_plots_from_file("/tests_seq/", False, True)


generate_all_plots()