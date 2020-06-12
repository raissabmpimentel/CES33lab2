import os
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime
import csv

def plot(file_name, month, path):
	x_label = []
	x = [] # dates
	y = [] # prices

	with open(path, 'r') as file:
		plots = csv.reader(file, delimiter = ',')

		i = 0
		for row in plots:
			date_time_str = str(row[0]) + " " + str(row[1])
			date_time_obj = datetime.datetime.strptime(date_time_str, '%Y-%m-%d %H:%M')
			x.append(date_time_obj)
			y.append(float(row[2]))
			
			if (i % 15 == 0):
				x_label.append(date_time_obj)
			
			i += 1


	# a new figure window
	fig = plt.figure()

	ax = fig.add_subplot(1, 1, 1)  # specify (nrows, ncols, axnum)

	ref = file_name
	ref = ref.replace(".csv", "").split("-")
	
	# set a title for x-axis
	ax.set_xlabel("Times", fontsize = 12)
	# set a title for y-axis
	ax.set_ylabel("Prices", fontsize = 12)
	# ax.locator_params(axis='y', nbins=6)

	if month: 
		ref = ref[1] + "-" + ref[0]
		ax.set_xticklabels(x, rotation = 45, fontsize = 8)

		ax.xaxis.set_major_formatter(mdates.DateFormatter('%d/%m/%Y'))

		# plot prices as a function of dates.
		ax.plot(x, y, 'r.-')
	else:
		ref = ref[2] + "-" + ref[1] + "-" + ref[0]
		ax.set_xticks(x_label) # set tick for x-axis

		# format labels for x-ticks (dates) and rotate them (45 degrees) for readability
		ax.set_xticklabels(x_label, rotation = 45, fontsize = 8)

		ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
		# plot prices as a function of dates.
		ax.plot(x, y, 'b-')

	title = 'Magalu ' + ref
	ax.set_title(title, fontsize = 16)

	# print plot
	plt.tight_layout()

	plt.savefig(path.replace(file_name, "") + title + ".png")
	plt.close()
	print("Plot efetuado!")

def generate_plot_from_folder(folder, month):
	path = os.getcwd() + folder

	files = []
	# r=root, d=directories, f = files
	for r, d, f in os.walk(path):
	    for file in f:
	        if '.csv' in file:
	            files.append(os.path.join(r, file))


	file_name = ""

	for path in files:
		path_splitted = path.split("/")
		for s in path_splitted:
			if s.find(".csv"):
				file_name = s

		print(path)
		print(file_name)
		plot(file_name, month, path)



def generate_all_plots():
	generate_plot_from_folder("/output_seq/month/", True)
	generate_plot_from_folder("/output_thread/month/", True)
	generate_plot_from_folder("/output_seq/days/", False)
	generate_plot_from_folder("/output_thread/days/", False)
	

generate_all_plots()