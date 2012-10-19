#!/opt/local/bin/python2.7

import yarp
import getopt, sys

def main():
    print int(sys.argv[1])
    numIt = int(sys.argv[1])
    
    
    print 'send command to test actionDecision'
    yarp.Network.init()
    prop = yarp.Property()
    prop.put('msg_type', 'touchClasses')
    prop.put('num_of_touch_classes', 2)

    v = yarp.Value(prop.toString())
    
    b = yarp.Bottle()
    b.add(v)

    t1 = yarp.Property()
    t1.put('PARTNAME', 'LEFTHAND')
    t1.put('SUBPARTNAME', 'PALM')
    t1.put('TYPE', 'POK')
    t1.put('STRENGTH', 'SOF')

    t2 = yarp.Property()
    t2.put('PARTNAME', 'RIGHTHAND')
    t2.put('SUBPARTNAME', 'PALM')
    t2.put('TYPE', 'POKE')
    t2.put('STRENGTH', 'SOFT')

    v = yarp.Value(t1.toString())
    b.add(v)
    v = yarp.Value(t2.toString())
    b.add(v)
    
    pt = yarp.Port()
    pt.open('/test')

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
