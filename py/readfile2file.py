#!/sh/bin/python
ifn = 'in.conf'
ofn = 'out.conf'

infile = open(ifn, 'rb')
outfile = open(ofn, 'wb')
outfile.write('[general]\n')
outfile.write(infile.read())
infile.close()
outfile.close()
