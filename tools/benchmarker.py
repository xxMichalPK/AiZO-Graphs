import subprocess
import time

executable_path = "./AiZO-Graphs"
results_file = "benchmark_results.csv"

# Use 50 iterations for each test case
iterations = 2

# Original sets
# 5 sizes to test
# size_set = [100, 250, 400, 550, 700]

# # 3 densities
# density_set = [25, 50, 75]

# # 3 problems
# problem_set = {
#     "MST": 0,
#     "SP": 1,
#     "MF": 2
# }



# Remaining sets
# 5 sizes to test
size_set = [700]

# 3 densities
density_set = [75]

# 3 problems
problem_set = {
    "MF": 2
}

def benchmark():
    for size in size_set:
        for density in density_set:
            for problem_name, problem_id in problem_set.items():
                print(f"[ INFO ] Running {problem_name} with size {size} and density {density}")
                
                # Run the executable with the appropriate arguments
                # command = f"{executable_path} -b -p {problem_id} -a 0 -s 0 -l {size} -d {density} -n {iterations} -r {results_file}"
                # Run max flow only for the adjacency list representation
                command = f"{executable_path} -b -p {problem_id} -a 0 -s 0 -l {size} -d {density} -n {iterations} -r {results_file}"
                subprocess.run(command, shell=True)
                # Sleep for one second for the generator seed to change
                time.sleep(1)

def main():
    benchmark()

if __name__ == "__main__":
    main()