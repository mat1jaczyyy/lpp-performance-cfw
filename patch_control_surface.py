import sys, os, signal, tkinter, tkinter.filedialog, subprocess, time
root = tkinter.Tk()
root.withdraw()
root.update()

print("[*] Upgrading pip...")
os.popen("\"" + sys.executable + "\" -m pip install --upgrade pip").read()

print("[*] Installing uncompyle6...")
os.popen("\"" + sys.executable + "\" -m pip install uncompyle6").read()

print("[!] Please select your Ableton Live application (.app or .exe) file.")
live_path = tkinter.filedialog.askopenfilename()
root.update()

if live_path[-3:] == "app":
	windows = False
elif live_path[-3:] == "exe":
	windows = True
else:
	raise ValueError("Incorrect file type selected!")

print("[*] Closing Ableton...")

if windows:
	for process in os.popen("tasklist").read().split("\n")[5:-1]:
		if ("Ableton" in process) or ("Live" in process):
			pname = process.split(".exe")[0] + ".exe"
			os.popen("taskkill /F /IM \"" + pname).read()
	script_path = "\\".join(live_path.split("/")[:-2]) + "\\Resources\\MIDI Remote Scripts\\Launchpad_Pro\\"
else:
	for process in os.popen("ps -A | grep Ableton").read().split("\n"):
		if ("Ableton" in process) or ("Live" in process):
			pid = int(process.split()[0])
			try:
				os.kill(pid, signal.SIGKILL)
			except:
				...
	script_path = live_path + "/Contents/App-Resources/MIDI Remote Scripts/Launchpad_Pro/"

print("[*] Decompiling and patching Launchpad_Pro.pyc...")
patch = os.popen("uncompyle6 \"" + script_path + "Launchpad_Pro.pyc\"").read().replace("self._send_challenge()", "self._on_handshake_successful()")

code = open(script_path + "Launchpad_Pro.py", "w")
code.write(patch)
code.close()

print("[*] Creating backup of original Launchpad_Pro.pyc...")
os.rename(script_path + "Launchpad_Pro.pyc", script_path + "Launchpad_Pro.pyc.backup")

print("[*] Waiting for Ableton to compile the patched Control Surface...")

if windows:
	subprocess.Popen([live_path])
else:
	subprocess.Popen(["open", live_path])

while not os.path.isfile(script_path + "Launchpad_Pro.pyc"):
	time.sleep(.5)

print("[*] Removing patched source code...")
os.remove(script_path + "Launchpad_Pro.py")

print("[*] Done!")