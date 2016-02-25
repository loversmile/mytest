

abd = '1,2,3,4,5,6,7,8,9'

c = abd.split(',')

print c[1:]

d = ''
for cc in c[1:]:
	d += cc + ','

print d



ppp = '1,1,1,1,2,3,4,5'

asd = ppp.split(',')
print "asd = %s  %s" % (asd[-1],asd[0])

q = ppp.replace('1', '9', 1)
print q
