import os
import subprocess
from pathlib import Path

input_dir = "DIMACS_cliques"
output_dir = "Dimacs_results"

Path(output_dir).mkdir(parents=True, exist_ok=True)

def run_program(input_file):
    output_file = os.path.join(output_dir, f"{Path(input_file).stem}_result.txt")
    
    try:
        # Run the program with a timeout of 2 hours (7200 seconds)
        result = subprocess.run(
            ["./gae", input_file], 
            stdout=open(output_file, "w"), 
            stderr=subprocess.STDOUT, 
            timeout=3600  # 2 hours
        )
        print(f"Completed {input_file}")
    except subprocess.TimeoutExpired:
        print(f"Timeout expired for {input_file}. The program took too long.")
    except Exception as e:
        print(f"Error running {input_file}: {e}")

# Compile the C++ program
subprocess.run(["g++", "gae.cpp", "-o", "gae"])

# Get the list of .clq files in the input directory
clq_files = [os.path.join(input_dir, file) for file in os.listdir(input_dir) if file.endswith(".clq")]

# Run the program for each .clq file sequentially
resume = 0
for input_file in clq_files:
    if input_file.endswith("MANN_a45.clq"):
        resume = 1
    if resume == 0:
        continue
    print(input_file + " started.")
    run_program(input_file)
