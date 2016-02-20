import os
import time

def offsetsToList():
    offsetList = []
    offsets = open('offsets.txt','r')
    for line in offsets:
        offsetList.append((int(line[0:8]),int(line[9:17])))
    offsets.close()
    return offsetList

offsets = offsetsToList()
if os.path.realpath(__file__)[len(os.path.realpath(__file__))-23:] != 'RivalsOfAetherModder.py':
    print 'SCRIPT SHOULD BE NAMED RivalsOfAetherModder.py, REENAMING IT MAY CAUSE ISSUES'
path = os.path.realpath(__file__)[:len(os.path.realpath(__file__))-23]
ripType = raw_input('\'rip\' or \'replace\'? ')
if ripType == 'rip':
    print 'Beginning sprite rip...'
    rivalsEXE = open('RivalsofAether.exe','rb')
    currentRip = 0
    for start,end in offsets:
        print 'start - '+str(start)
        print 'end - '+str(end)
        currentRip += 1
        f = open(path+'sprites\\RIP_'+str(currentRip)+'.png','wb')
        rivalsEXE.seek(start)
        f.write(rivalsEXE.read((start-end)-1))
        f.close()
        
    rivalsEXE.close()
elif ripType == 'replace':
    print 'Beginning sprite replacement...'
    rivalsEXE = open('RivalsofAether.exe','r+b')
    currentRip = 0
    for start,end in offsets:
        print 'start - '+str(start)
        print 'end - '+str(end)
        currentRip += 1
        f = open(path+'sprites\\RIP_'+str(currentRip)+'.png','rb')
        rivalsEXE.seek(start)
        rivalsEXE.write(f.read((start-end)-1))
        f.close()
    rivalsEXE.close()
else:
    print 'Not a specified type.'

