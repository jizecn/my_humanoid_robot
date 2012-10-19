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
	icubroot = os.getenv('ICUB_ROOT')
	print icubroot, icubpath
	if icubpath == None:
		icubpath = 'D:\\yarp_libraries\\iCub'
	command = [icubpath+'\\bin\\Release\\iCubInterface.exe', '--config', icubroot+'\\app\\kaspar\\conf\\kaspar.ini']	

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

	
def startSkinDriftComp():
	c = wmi.WMI()
	count = 0;

	icubpath = os.getenv('ICUB_DIR')
	if icubpath == None:
		icubpath = 'D:\\yarp_libraries\\iCub'
	command = [icubpath+'\\bin\\Release\\skinDriftCompensation.exe', '--context', 'skinGui/conf', '--from', 'driftKaspar.ini']	

	for process in c.Win32_Process(name="skinDriftCompensation.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(5)

	print 'Start skinDriftCompensation.exe now'
	skinSV = subprocess.Popen(command)
	time.sleep(15)
	return skinSV

	
def startSkinDriftCompGui():
	c = wmi.WMI()
	count = 0;

	icubpath = os.getenv('ICUB_DIR')
	if icubpath == None:
		icubpath = 'D:\\yarp_libraries\\iCub'
	command = [icubpath+'\\bin\\Release\\skinDriftCompensationGui.exe', '--context', 'skinGui/conf', '--from', 'driftCompGuiLeftHand.ini']	

	for process in c.Win32_Process(name="skinDriftCompensationGui.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(5)

	print 'Start skinDriftCompensationGui.exe now'
	skinSV = subprocess.Popen(command)
	time.sleep(15)
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
	command = ['D:\\roboskin\\coding\\roboskin\\bin\\Release\\kasparBehaviour.exe', 'D:\\roboskin\\coding\\roboskin\\app\\KasparRoboSkinConf\\config.ini']

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

def startTactilePreprocessing():
	c = wmi.WMI()
	kasparRoot = os.environ.get('KASPAR_ROOT');
	kasparBin = kasparRoot + '\\bin\\Release';
	command = [kasparBin + '\\TactilePreprocessing.exe', '--context', 'TactilePreprocessing']

	for process in c.Win32_Process(name="TactilePreprocessing.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start TactilePreprocessing.exe now'
	kaspar = subprocess.Popen(command)
	time.sleep(1)
	return kaspar

def startActionDecision():
	c = wmi.WMI()
	kasparRoot = os.environ.get('KASPAR_ROOT');
	kasparBin = kasparRoot + '\\bin\\Release';
	command = [kasparBin + '\\ActionDecision.exe', '--context', 'ActionDecision']

	for process in c.Win32_Process(name="ActionDecision.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start ActionDecision.exe now'
	kaspar = subprocess.Popen(command)
	time.sleep(1)
	return kaspar

def startActionExecutor():
	c = wmi.WMI()
	kasparRoot = os.environ.get('KASPAR_ROOT');
	kasparBin = kasparRoot + '\\bin\\Release';
	command = [kasparBin + '\\ActionExecutor.exe', '--context', 'ActionExecutor']

	for process in c.Win32_Process(name="ActionExecutor.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start ActionExecutor.exe now'
	kaspar = subprocess.Popen(command)
	time.sleep(1)
	return kaspar

def startGuiManager():
	c = wmi.WMI()
	kasparRoot = os.environ.get('KASPAR_ROOT');
	kasparBin = kasparRoot + '\\bin\\Release';
	command = [kasparBin + '\\guiManager.exe', '--context', 'GuiManager']

	for process in c.Win32_Process(name="guiManager.exe"):
		#t = process.Name.encode('utf8')
		#if t.find('iCubInterface') != -1:
		#	... 	
		print process.ProcessId, process.Name, "is Running already\n"
		process.Terminate()
		time.sleep(3)

	print 'Start guiManager.exe now'
	kaspar = subprocess.Popen(command)
	time.sleep(1)
	return kaspar

def connectAll():
	c = wmi.WMI()
	count = 0;

	yarppath = os.getenv('YARP_DIR')
	if yarppath == None:
		yarppath = 'D:\\yarp_libraries\\yarp2'
		
	print 'Start yarp server now'
	yarpSV = subprocess.Popen(yarppath+'\\bin\\Release\\yarp.exe connect /action_decision/outport /executor')
	yarpSV = subprocess.Popen(yarppath+'\\bin\\Release\\yarp.exe connect /driftCompGuiLeft/rpc:o /leftSkinDriftComp/rpc')
	yarpSV = subprocess.Popen(yarppath+'\\bin\\Release\\yarp.exe connect /leftSkinDriftComp/monitor:o /driftCompGuiLeft/monitor:i')	
	yarpSV = subprocess.Popen(yarppath+'\\bin\\Release\\yarp.exe connect /leftSkinDriftComp/info:o /driftCompGuiLeft/info:i')	
	
	print 'process ID is => ', yarpSV.pid
	time.sleep(3)
	return yarpSV	
	
	
def main():
	a = startYarpServer()
	b = startICubInterface()
	startSkinDriftComp()
	startSkinDriftCompGui()
	#d = startFakeSkin()
	#c = startLogging('test')
	#e = startKaspar()
	startTactilePreprocessing()
	startActionDecision()
	startActionExecutor()
	startGuiManager()
	connectAll()
	#time.sleep(5);
	#print '2'
	#a = startYarpServer()
	
if __name__ == "__main__":
	main()
