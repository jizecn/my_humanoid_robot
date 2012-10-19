import wmi
import os, signal
import subprocess
import time

def startYarpServer():

	c = wmi.WMI()
	count = 0;

	yarppath = os.getenv('YARP_DIR')
	if yarppath == None:
		yarppath = 'D:\\yarp_libraries\\yarp2'

	for process in c.Win32_Process(name="yarpserver.exe"):
		print process.ProcessId, process.Name, "is Running already..\n"
		print 'try to kill and restart now'
		process.Terminate()
		time.sleep(3);
		
	print 'Start yarp server now'
	yarpSV = subprocess.Popen(yarppath+'\\bin\\Release\\yarpserver.exe')
	print 'process ID is => ', yarpSV.pid
	time.sleep(3)
	return yarpSV

def startICubInterface():

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
		time.sleep(5)

	print 'Start iCubInterface.exe now'
	skinSV = subprocess.Popen(command)
	time.sleep(5)
	return skinSV

def startFakeSkin():
	c = wmi.WMI()
	command = ['D:\\roboskin\\coding\\roboskin\\bin\\Release\\fakeSkin.exe']
	
	for process in c.Win32_Process(name="fakeSkin.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start fakeSkin.exe now'
	fake = subprocess.Popen(command)
	time.sleep(1)
	return fake
	
def startLogging(filename):
	c = wmi.WMI()
	command = ['D:\\roboskin\\coding\\roboskin\\bin\\Release\\dataLogger.exe', filename]

	for process in c.Win32_Process(name="dataLogger.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start dataLogger.exe now'
	datalogger = subprocess.Popen(command)
	time.sleep(1)
	return datalogger

def startKaspar():
	c = wmi.WMI()
	command = ['D:\\roboskin\\coding\\roboskin\\bin\\Release\\kasparBehaviour.exe', 'D:\\roboskin\\coding\\roboskin\\app\\KasparRoboSkinConf\\config_laptop.ini']

	for process in c.Win32_Process(name="kasparBehaviour.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start kasparBehaviour.exe now'
	kaspar = subprocess.Popen(command)
	time.sleep(1)
	return kaspar

def main():
	a = startYarpServer()
	b = startICubInterface()
	#d = startFakeSkin()
	#c = startLogging('test')
	e = startKaspar()
	#time.sleep(5);
	#print '2'
	#a = startYarpServer()
	
if __name__ == "__main__":
	main()