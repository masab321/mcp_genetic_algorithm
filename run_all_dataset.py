import os
import time
import subprocess
from pathlib import Path

input_dir = "datasets/dimacs_cliques"
output_dir = "dimacs_results"

Path(output_dir).mkdir(parents=True, exist_ok=True)

def run_program(input_file, time_limit_seconds):
    output_file = os.path.join(output_dir, f"{Path(input_file).stem}_result.txt")
    
    try:
        result = subprocess.run(
            [".\\gae.exe", input_file, str(time_limit_seconds)], 
            stdout=open(output_file, "w"), 
            stderr=subprocess.STDOUT
        )
    except Exception as e:
        print(f"Error running {input_file}: {e}")

# Compile the C++ program
subprocess.run(["g++", "gae.cpp", "-o", "gae.exe"])

clq_files = [os.path.join(input_dir, file) for file in os.listdir(input_dir) if file.endswith(".clq")]
left_dimacs_files = [
    "DSJC500_5.clq",
    "DSJC1000_5.clq",
    "gen200_p0.9_44.clq",
    "gen200_p0.9_55.clq",
    "gen400_p0.9_55.clq",
    "gen400_p0.9_65.clq",
    "gen400_p0.9_75.clq"
]


resume = 0
index = 0
total = len(clq_files)
time_limit = 10800
for input_file in clq_files:
    index = index + 1
    # if os.path.basename(input_file) not in left_dimacs_files:
    #     continue
    # if input_file.endswith("p_hat1000-1.clq"):
    #     resume = 1
    # if resume == 0:
    #     continue

    start_time = time.perf_counter()
    print(f"+ {index}/{total} - Started:" + input_file)

    run_program(input_file, time_limit)

    end_time = time.perf_counter()
    elapsed_time = end_time - start_time
    hours = int(elapsed_time // 3600)
    minutes = int((elapsed_time % 3600) // 60)
    seconds = elapsed_time % 60
    print(f"- Completed {input_file}. Execution time: {hours} hours, {minutes} minutes, and {seconds:.2f} seconds.")
