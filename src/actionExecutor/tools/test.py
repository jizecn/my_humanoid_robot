#!/opt/local/bin/python2.7

import yarp
import getopt, sys

def main():
    print int(sys.argv[1])
    numIt = int(sys.argv[1])
    
    print 'send command to test actionExecutor'
    yarp.Network.init()
    prop = yarp.Property()
    prop.put('msg_type', 'touch_action_command')

    v = yarp.Value(prop.toString())
    
    b = yarp.Bottle()
    b.add(v)

    t1 = yarp.Property()
    t1.put('ComplianceAction', 'NONE')
    t1.put('CompliancePart', 'ARM_L_4')
    t1.put('NAME', 'tickR')
    #t1.put('IFCANCELLASTCMD', 'FALSE')
    #t1.put('WAITORABORT', 'FALSE')
    
    #t1.put('SUBPARTNAME', 'PALM')
    #t1.put('TYPE', 'POK')
    #t1.put('STRENGTH', 'SOF')

    v = yarp.Value(t1.toString())
    b.add(v)
    
    pt = yarp.Port()
    pt.open('/test_actionexecutor')

    stmp = yarp.Stamp()
    
    for i in range(numIt):
        pt.setEnvelope(stmp)
        pt.write(b)
        yarp.Time.delay(1)
        stmp.update()
        print i, stmp.getTime(), stmp.getCount()

    pt.close()
    yarp.Network.fini()

if __name__ == '__main__':
    main()
