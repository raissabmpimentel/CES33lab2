import os
import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime
import csv

def plot(x, y, x_label, file_name, path, month, seq):
	# split month/date reference for title
	ref = file_name
	ref = ref.replace(".csv", "").split("-")
	
	# create a new figure window
	fig = plt.figure()
	ax = fig.add_subplot(1, 1, 1)
	
	# set a label for y-axis
	ax.set_ylabel("Preço", fontsize = 12)
	
	# if it's a month plot
	if month: 
		# set "Data" as label for x-axis
		ax.set_xlabel("Data", fontsize = 12)

		# Format month reference
		ref = ref[1] + "-" + ref[0]

		# Format x label, rotating it 45 degrees and only displaying the date from datetime
		ax.set_xticklabels(x, rotation = 45, fontsize = 8)
		ax.xaxis.set_major_formatter(mdates.DateFormatter('%d/%m/%Y'))

		# plot prices as a function of date
		# if seq, it should be blue
		if seq:
			ax.plot(x, y, 'b.-')
		# if not seq, it should be red
		else:
			ax.plot(x, y, 'r.-')

	# if it's a day plot
	else:
		# set "Horário" as label for x-axis
		ax.set_xlabel("Horário", fontsize = 12)

		# Format day reference
		ref = ref[2] + "-" + ref[1] + "-" + ref[0]

		# Format x label, rotating it 45 degrees and only displaying the time from datetime
		ax.set_xticks(x_label) # set tick for x-axis
		ax.set_xticklabels(x_label, rotation = 45, fontsize = 8)
		ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))

		# plot prices as a function of time
		# if seq, it should be blue
		if seq:
			ax.plot(x, y, 'b-')
		# if not seq, it should be red
		else:
			ax.plot(x, y, 'r-')

	# set title to the name of stock with month/date reference		
	title = 'Magalu ' + ref
	ax.set_title(title, fontsize = 16)

	# print plot with margins
	plt.tight_layout()

	# save fig to the same folder it comes from
	plt.savefig(path.replace(file_name, "") + title + ".png")
	
	plt.close()


def read_data(x, y, x_label, path, month, seq):
	# read file from specified path
	with open(path, 'r') as file:
		plots = csv.reader(file, delimiter = ',')

		i = 0
		for row in plots:
			# convert infos from row[0] and row[1] to datetime
			date_time_str = str(row[0]) + " " + str(row[1])
			date_time_obj = datetime.datetime.strptime(date_time_str, '%Y-%m-%d %H:%M')
			x.append(date_time_obj)
			
			y.append(float(row[2]))
			
			# just store data in x_label sometimes, to avoid too many labels on x-axis
			if (i % 15 == 0):
				x_label.append(date_time_obj)
			
			i += 1


def generate_plot_from_folder(folder, month, seq):
	# computing current path
	path = os.getcwd() + folder

	# computing a list with the path name of each file
	files = []
	for r, d, f in os.walk(path):
	    for file in f:
	        if '.csv' in file:
	            files.append(os.path.join(r, file))

	file_name = ""
	for file_path in files:
		# finding the file name from path name
		file_path_splitted = file_path.split("/")
		for s in file_path_splitted:
			if s.find(".csv"):
				file_name = s

		x_label = [] # dates/times labels
		x = [] # dates/times
		y = [] # prices
		
		# reading data from file
		read_data(x, y, x_label, file_path, month, seq)
		
		# ploting data
		plot(x, y, x_label, file_name, file_path, month, seq)


def generate_all_plots():
	generate_plot_from_folder("/output_seq/month/", True, True)
	generate_plot_from_folder("/output_thread/month/", True, False)
	generate_plot_from_folder("/output_seq/days/", False, True)
	generate_plot_from_folder("/output_thread/days/", False, False)
	

generate_all_plots()