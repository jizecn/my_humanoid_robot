#!/opt/local/bin/python2.7

import os, sys

try:
    import yarp
except ImportError:
    print 'import error, add yarp_folder: ./pyYarp'
    yarp_folder = './pyYarp'
    sys.path.append(yarp_folder)
    import yarp
    
class PyActionDecision (yarp.RFModule):

    def __init__(self, name = None):
        yarp.RFModule.__init__(self)
        self.name = name

    def __del__(self):
        yarp.Network.fini()
        print 'destructor'

    def updateModule(self):
        print 'updating module'
        print self.getPeriod()
        yarp.Time.delay(1)
        return True

    def respond(self, command, reply):
        ### used to update 
        size = command.size()
        print '----> ', size
        if size != 9:
            return False
        return True

    def updateTouchTypes(self, command, reply):
        

    def getPeriod(self):
        return 0.0
    
def main():
    yarp.Network.init()
    
    ad = PyActionDecision('KasparAction')
    #ad.attachTerminal()
    ad.runModule()

if __name__ == "__main__":
    print 'Start PyActionDecision module'
    main()
