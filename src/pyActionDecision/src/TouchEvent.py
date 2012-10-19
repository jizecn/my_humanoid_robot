
try:
    import yarp
except ImportError:
    print 'import error, add yarp_folder: ./pyYarp'
    yarp_folder = './pyYarp'
    sys.path.append(yarp_folder)
    import yarp

class TouchEvent:
    def __init__(self):
        
