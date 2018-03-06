def ArithCodage ( tab, lettre):
	tabrep =[0,0,0,0,0,0]
	index=0
	for k in delta.keys():
		if(k==lettre):
			break
		else:
			index = index +1
	print index
	tabrep[0] = tab[index]
	tabrep[5] = tab[index+1]
	for i in xrange(1,5):
		tabrep[i] = tabrep[i-1]+delta[i-1]*(tab[index+1]-tab[index])
	return tabrep

def ArithDecode (tab,delta,code):
	tabrep=[]
	#tabrep.append()

tab = [0, 0.3, 0.6, 0.8, 0.9, 1]
delta = {"e":0.3,"n": 0.3, "t":0.2, "v":0.1, ".":0.1}
e=0;
n=1;
t=2;
v=3;
point=4;
mot = ["v","e","n","t","."]
#tab = Arith(tab,v)
for j in mot:
	tab = ArithCodage(tab,j)
	print tab

result = "Resultat entre [{0};{1}]".format(tab[0],tab[len(tab)-1])
print(result)



