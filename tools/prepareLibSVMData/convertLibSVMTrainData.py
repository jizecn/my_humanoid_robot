import os
import getopt
import numpy as np, sys, getopt
import tokenize

def prepareSingleLine(data, classlabel):
    l = len(data)
    if classlabel > 0:
        ret = '+' + str(classlabel)  ## not necessary. just to comply with the original author's style
    else:
        ret = str(classlabel)
        
    for i in range(l):
        ret = ret + ' ' + str(i) + ':' + str(data[i]);

    #ret = ret + '\n'; # handled outside
    return ret

def convertFileFormat(inputFile, outFile, col, classlabel):
    """
    Load data and return it as a numpy array
    col: columns / dimension of data
    """
    print 'Load data and return it as a numpy array'
    print 'Dimension is : ' + str(col)
    f = open(inputFile, 'r')
    out = open(outFile, 'w')
    
    a = f.readlines()
    data = []
    
    for line in a:
        #print line
        tokens = line.split()
        
        #print tokens
        try:
            vals = map(float, tokens[0:col])
            tempLine = prepareSingleLine(vals, classlabel)
            out.write(tempLine)
            out.write('\n')
            data.append(vals)
        except:
            print 'wrong format: ' + line + ' --> ignore'

    dataArray = np.array(data)
    dataArray.reshape(int(dataArray.size/col), col)
    
    f.close()
    out.close()
    return dataArray

def main():
    """
    
    """
    print """
    Load skin data into memory or convert to recognisable file format for libsvm
    """
    opts, args = getopt.getopt(sys.argv[1:], "i:o:d:c:", ["input=", "output=", "dims=", "classlabel="])
    
    infile = None
    outfile = None
    #dims = 2
    col = None
    #n = 100
    classlabel = None
    
    for opt, arg in opts:
        if opt in ("-i", "--input"):
            infile = arg
        elif opt in ("-o", "--output"):
            outfile = arg
        elif opt in ("-d", "--dimension"):
            'd: number of columns'
            col = int(arg)
        elif opt in ("-c", "--classlabel"):
            classlabel = int(arg)

    if (not infile) or (not outfile) or (not col) or (not classlabel):
        print """
        Usage: python loaddata -f <infile> -o <outfile> [-d <dims> -a <auto: exclude time in the last column> -c <class label for libsvm, 1, -1 etc> ]
        e.g.
        righthand_bottom: -d 109 or -d 108, depending on whether the last time item is included or not
        """
        sys.exit()
    
    data = convertFileFormat(infile, outfile, col, classlabel)
    """
    print data.size
    print data.shape
    
    pl.imshow(data[:,0:108])
    pl.axis('tight')

    pl.colorbar()
    pl.show()
    """
    
if __name__== "__main__":
    main()
