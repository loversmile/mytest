
dict1 = {
	"1" : "a" ,
	"2" : "b",
	"3" : "c"
}

print dict1

if dict1.has_key("1"):
    print 1
if dict1.has_key("6"):
    print 2


dict2 = {
	"4" : "d",
	"5" : "e",
	"6" : "f"
}

print dict1.get("1")
print dict1.items()


print dict1.values()

dict1.update(dict2)
print dict1
