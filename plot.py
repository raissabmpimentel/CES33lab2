import matplotlib.pyplot as plt
import matplotlib.dates as mdates
import datetime
import csv

def plot(file_name, month):
	x = [] # dates
	y = [] # prices

	with open(file_name + ".csv", 'r') as file:
		plots = csv.reader(file, delimiter = ',')
		for row in plots:
			date_time_str = str(row[0]) + " " + str(row[1])
			x.append(datetime.datetime.strptime(date_time_str, '%Y-%m-%d %H:%M'))
			y.append(row[2])


	# a new figure window
	fig = plt.figure()

	ax = fig.add_subplot(1, 1, 1)  # specify (nrows, ncols, axnum)

	title = 'Magalu ' + file_name
	ax.set_title(title, fontsize = 16)

	# set a title for x-axis
	ax.set_xlabel("Times", fontsize = 12)
	# set a title for y-axis
	ax.set_ylabel("Prices", fontsize = 12)

	ax.set_xticks(x) # set tick for x-axis

	# format labels for x-ticks (dates) and rotate them (45 degrees) for readability
	ax.set_xticklabels(x, rotation = 45)
	if month: 
		ax.xaxis.set_major_formatter(mdates.DateFormatter('%d/%m/%Y'))
		# plot prices as a function of dates.
		ax.plot(x, y, 'ro-')
	else:
		ax.xaxis.set_major_formatter(mdates.DateFormatter('%H:%M'))
		# plot prices as a function of dates.
		ax.plot(x, y, 'bo-')

	# print plot
	plt.grid()
	plt.tight_layout()

	plt.savefig(title + '.png')
	print("Plot efetuado!")



file_name = input("Digite o nome do arquivo de dados: ")
month = input("Este arquivo se refere a um mes (M) ou a um dia (D)? ")
month = (month == "M") or (month == "m")
plot(file_name, month)