import csv
import os
from datetime import datetime
import matplotlib.pyplot as plt

def convert_to_gpx(csv_file, gpx_file):
    gpx_header = """<?xml version="1.0" encoding="UTF-8"?>
<gpx version="1.1" creator="YourAppName">
<trk>
    <name>Track</name>
    <trkseg>
"""
    gpx_footer = """    </trkseg>
</trk>
</gpx>
"""
    altitudes = []
    with open(csv_file, 'r') as file:
        csv_reader = csv.reader(file, delimiter=';')
        next(csv_reader)  # Skip the header
        with open(gpx_file + "gpx", 'w') as gpx:
            gpx.write(gpx_header)
            for row in csv_reader:
                time_str = "{}/{}/{} {}:{}:{}".format(row[2], row[1], row[0], row[3], row[4], row[5])
                time_format = "%y/%m/%d %H:%M:%S"
                time = datetime.strptime(time_str, time_format).isoformat()
                gpx.write(f'        <trkpt lat="{row[6]}" lon="{row[7]}">\n')
                gpx.write(f'            <ele>{row[8]}</ele>\n')
                gpx.write(f'            <time>{time}</time>\n')
                gpx.write('        </trkpt>\n')
                altitudes.append(float(row[8]))  # Append altitude data
            gpx.write(gpx_footer)

    # Plot altitude data
    plt.plot(altitudes)
    plt.xlabel('Time')
    plt.ylabel('Altitude')
    plt.title('Altitude vs Time')
    plt.savefig(gpx_file + "png") 
    plt.close()

# Example usage


for csv_file in os.listdir("."):
    if csv_file.endswith(".csv"):
        folder_name = os.path.splitext(csv_file)[0]
        outputFile = os.path.join(folder_name,csv_file.replace("csv",""))
        
        os.makedirs(folder_name, exist_ok=True)  # Create folder for each CSV file
        convert_to_gpx(csv_file, outputFile)
        

#convert_to_gpx("input.csv", "output.gpx")

