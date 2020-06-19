import os # File process
import csv # csv process

print("Start booting...\nWarm OK.") # Given a Start message
print('Starting working on files:')
process_folder = 'test_data_set' # Set bas
with os.scandir(process_folder) as entries:
    for entry in entries:
        if entry.is_file():
            print(entry.name)
            #遍历/Traverse could be fulfilled by os.walk()
            with open(entry,'r',encoding='UTF-8') as cur_f: #Default data written in SHIFT-JIS, encoding necessary
                data = csv.reader(cur_f)
                #header = next(data) #Skip header file
                for row in data:
                    #print(row[4]) # row(4) is defined as "Material No."
                    print(row[17])

print('File processed.')
