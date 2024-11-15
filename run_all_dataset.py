import os
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

input_dir = "DIMACS_cliques"
output_dir = "Dimacs_results"

Path(output_dir).mkdir(parents=True, exist_ok=True)

def run_program(input_file):
    output_file = os.path.join(output_dir, f"{Path(input_file).stem}_result.txt")
    
    with open(output_file, "w") as out:
        try:
            subprocess.run(["./gae", input_file], stdout=out, stderr=subprocess.STDOUT)
            print(f"Completed {input_file}")
        except Exception as e:
            print(f"Error running {input_file}: {e}")

subprocess.run(["g++", "gae.cpp", "-o", "gae"])

clq_files = [os.path.join(input_dir, file) for file in os.listdir(input_dir) if file.endswith(".clq")]

with ThreadPoolExecutor() as executor:
    executor.map(run_program, clq_files)
