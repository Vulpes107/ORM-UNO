import subprocess
import sys
import os

def rebuild():
    print("Rebuilding project...")
    subprocess.run(["make"], check=True)

def open_terminal(command, title):
    subprocess.Popen([
        "gnome-terminal",
        "--title", title,
        "--",
        "bash", "-c", f"{command}; exec bash"
    ])

def main():
    rebuild_flag = "--rebuild" in sys.argv

    if rebuild_flag:
        rebuild()

    root = os.path.dirname(os.path.abspath(__file__))
    server_bin = os.path.join(root, "bin", "build", "server")
    client_bin = os.path.join(root, "bin", "build", "client")

    open_terminal(server_bin, "Server")
    open_terminal(client_bin, "Client 1")
    open_terminal(client_bin, "Client 2")

if __name__ == "__main__":
    main()