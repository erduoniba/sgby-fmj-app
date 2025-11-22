import sys
fname = sys.argv[1]

fin = open(fname)
fout = open(fname+'.c', 'w')
count = 0
fout.write('{')

while 1:
    c = fin.read(1)
    if not c:
        break
    if count % 16 == 0:
        fout.write('\n')
    fout.write('0x%02X, ' % ord(c))
    count += 1
fout.write('\n}')
fout.close()
