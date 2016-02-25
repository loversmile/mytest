
family = 'AAUSER/6500'
key = 'DIAL'
value = 'SIP/6500'

sql = "insert or replace into astdb(key, value) value('/%s/%s', %s)" % (family, key, value)

print sql
