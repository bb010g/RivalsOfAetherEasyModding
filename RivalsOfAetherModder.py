import os
import time

def offsetsToList():
    offsetList = []
    offsets = open('offsets.txt','r')
    for line in offsets:
        offsetList.append((int(line[0:8]),int(line[9:17])))
    offsets.close()
    return offsetList

########PNG FORMAT###############
#Hex - 89 50 4E 47
#Dec - 137 80 78 71

#through

#Hex - 49 45 4E 44 AE 42 60 82
#Dec - 73 69 78 68 174 66 96 130
#################################

def newOffsetsToList():
    offsetList = []
    lastEight = []
    currentTuple = [0,0]
    currentOffset = -1
    currentValue = ' '
    rivals = open('RivalsofAether.exe','rb')
    while currentValue != '':
        currentOffset += 1
        currentValue = rivals.read(1)
        if currentValue != '':
            lastEight.append(ord(currentValue))
        #print 'Offset - '+str(currentOffset)+' | Last Eight - '+str(lastEight)
        if len(lastEight) > 8:
            del lastEight[0]
        if lastEight[len(lastEight)-4:] == [137,80,78,71]:
            currentTuple[0] = currentOffset - 3
            print 'Start - '+str(currentTuple[0])
        if lastEight == [73,69,78,68,174,66,96,130]:
            currentTuple[1] = currentOffset + 1
            print 'End - '+str(currentTuple[1])
            offsetList.append(tuple(currentTuple))
        
    rivals.close()
    print offsetList
    return offsetList

def saveOffsetsFromList(offsets):
    offsetFile = open('offsets.txt','w')
    for start,end in offsets:
        offsetFile.write(str(start)+'-'+str(end)+'\n')
    offsetFile.close()

if os.path.realpath(__file__)[len(os.path.realpath(__file__))-23:] != 'RivalsOfAetherModder.py':
    print 'SCRIPT SHOULD BE NAMED RivalsOfAetherModder.py, REENAMING IT MAY CAUSE ISSUES'
path = os.path.realpath(__file__)[:len(os.path.realpath(__file__))-23]
ripType = 'help'
while ripType == 'help':
    ripType = raw_input('\'rip\', \'replace\', \'update\' or \'help\'? ')
    if ripType == 'help':
        print '* rip - rip the sprites from the exe to your sprites folder'
        print '* replace - take the sprites from your sprites folder and put them in your exe'
        print '* update - update your offsets.txt, use whenever a new patch releases'
        

if ripType == 'update':
    print 'Updating sprite offsets this may take a while...'
    saveOffsetsFromList(newOffsetsToList())
    
offsets = offsetsToList()
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
elif ripType != 'update':
    print 'Not a specified type.'

