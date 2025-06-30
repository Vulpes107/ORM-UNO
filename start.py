import subprocess
import time

# Define the commands to run in each terminal
commands = [
    "./bin/build/server",
    "./bin/build/client",
    "./bin/build/client"
]


subprocess.run("make client", shell=True, capture_output=True, text=True)

# Open three gnome terminals, each with a specific command
for command in commands:
    subprocess.Popen(['gnome-terminal', '--', 'bash', '-c', command])

# Add a small delay before the script finishes, so the terminals remain open for a bit
time.sleep(1)
