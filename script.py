from random import uniform
from os import system, listdir, remove
import subprocess
import time
from regex import search

fileInputName = "input.txt"
with open(fileInputName, 'w') as file:
    for i in range(100000):
        file.write(str(uniform(0, 10000)) + " ")
    file.write("-23.5e-3")

for file in listdir('./'):
    if "output" in file:
        remove(file)


cmd = "cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Release -H./ -B./build -G Ninja;\
    cmake --build ./build --config Release --target all -- -j 14"
system(cmd)

for i in range(1, 7):
    mini = float("inf")
    fileOutputName = f"output{i}.txt"
    cmd = f"./build/Program {i} {fileInputName} {fileOutputName}"

    for trial in range(1000):
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
        f"The miniaml time for {i} method is {mini}mcs\n==========================================>\n\n")
    # rc = process.poll()
    # system(cmd)
