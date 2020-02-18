from random import uniform
from os import system, listdir, remove, name
import subprocess
import time
from regex import search

fileInputName = "input.txt"
with open(fileInputName, 'w') as file:
    for i in range(100000):
        file.write(str(uniform(-10000, 10000)) + " ")
    file.write("-23.5e-3")

for file in listdir('./'):
    if "output" in file:
        remove(file)

cmd = f"cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -H./ -B./build -G \"{'Visual Studio 15 2017' if name == 'nt' else 'Unix Makefiles'}\" && cmake --build ./build --config Release --target all"
system(cmd)

timeByMethod, methods = {}, [
    '"stringstream"', '"to_string"', '"sprintf"', '"custom"', '"lexical_cast"', '"QString"']

for i in range(6):
    mini = float("inf")
    fileOutputName = f"output{i+1}.txt"
    cmd = f"./build/Program {i+1} {fileInputName} {fileOutputName}"

    # change range when ready
    for trial in range(10):
        process = subprocess.Popen(
            cmd.split(), stdout=subprocess.PIPE)
        while True:
            output = process.stdout.readline()
            if process.poll() is not None:
                break
            if output:
                s = str(output.strip())
                timed_res = int(search("(\d+)(?=mcs)", s).group(1))
                if mini > timed_res:
                    mini = timed_res
                print(s)

    print(
        f"The miniaml time for {methods[i]} method is {mini}mcs\n======================================================>\n\n")
    timeByMethod[i] = mini

print("\n\n======================================================>\n")
for (key, value) in timeByMethod.items():
    print(f"The minimal time for {methods[key]} method is {value}mcs")

results = []
for i in range(6):
    fileInputName = f"output{i+1}.txt"
    with open(fileInputName) as file:
        lines = file.readlines()
        for j in range(1, len(lines)):
            if lines[j] != lines[j-1]:
                print(f"!!! {methods[i]} is not working correctly !!!")
        results.append(lines[0].split()[1])
print("\n------------------------------------------------------\n")

fl = True
for i in range(len(results)):
    for j in range(i, len(results)):
        if abs(float(results[i])-float(results[j])) > 0.01:
            print(
                f" ! Average length of {methods[i]} and {methods[j]} differs significantly (by {abs(float(results[i])-float(results[j]))})")
            fl = False
if fl:
    print("Results coincide")

print("\n======================================================>\n")
