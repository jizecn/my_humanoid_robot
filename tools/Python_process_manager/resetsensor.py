import wmi
import os, signal
import subprocess
import time

def restartSensor():

	c = wmi.WMI()
	count = 0;

	icubpath = os.getenv('ICUB_DIR')
	if icubpath == None:
		icubpath = 'D:\\yarp_libraries\\iCub'
	command = [icubpath+'\\main\\build\\bin\\Release\\iCubInterface.exe', '--config', icubpath+'\\app\\kaspar\\kaspar.ini']	

	for process in c.Win32_Process(name="iCubInterface.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		#time.sleep(3)

	print 'Start iCubInterface.exe now'
	skinSV = subprocess.Popen(command)
	
	return skinSV
	
	
def main():
	a = restartSensor()
	
if __name__ == "__main__":
	main()