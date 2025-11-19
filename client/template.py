import subprocess
import os

# The name of the executable file in the current directory
EXECUTABLE_NAME = './bronze'

# The specific string parameter: 'A' repeated 16 times
PARAMETER_STRING = 'A' * 16

def run_program_with_parameter():
    """
    Launches the target executable with the specified string parameter.
    """

    command = [EXECUTABLE_NAME, PARAMETER_STRING]
    
    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=False 
    )

    # Display the results
    if result.stdout:
        print(result.stdout)

    if result.returncode < 0:
        if result.returncode == -11:
            print("SEGMENT FAULT!")
            exit(-11)
        print(f"Error!")

if __name__ == "__main__":
    run_program_with_parameter()