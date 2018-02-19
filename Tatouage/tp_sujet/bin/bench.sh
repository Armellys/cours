#!/bin/sh 

if [ $# -ne 2 ]; then
	echo $0 " SS|QT input.pgm "
	echo "        input.pgm : image hôte"
	echo " "
	exit 0 
fi

# Quel algo de tatouage ? 
if [ "$1" = "SS" ]; then 
	wmread="bin/ss_read"
	wmwrite="bin/ss_write"
else 
	wmread="bin/qt_read"
	wmwrite="bin/qt_write"
fi

message="789753"

jpeg="bin/jpeg"
noise="bin/noise"
scale="bin/scale"

$wmwrite $2 tmpo.pgm $message 

echo " "; echo "--- Attaque JPEG --- "
qf=95 

while [ $qf -ge 5 ]; do 
	cp tmpo.pgm tmpa.pgm 
	$jpeg tmpa.pgm $qf
	
	res=$($wmread tmpa.pgm | grep "Message")
	
	if [ $(echo $res | cut -d":" -f2) = $message ]; then 
		echo "    OK | Q = $qf"
	else 
		echo "    -- | Q = $qf"
	fi 

	qf=$(expr $qf - 5) 
done 


echo " "; echo "--- Attaque NOISE --- "
n=1 

while [ $n -le 30 ]; do 
	cp tmpo.pgm tmpa.pgm 
	$noise tmpa.pgm $n
	
	res=$($wmread tmpa.pgm | grep "Message")
	
	if [ $(echo $res | cut -d":" -f2) = $message ]; then 
		echo "    OK | N = $n"
	else 
		echo "    -- | N = $n"
	fi 

	n=$(expr $n + 1) 
done

echo " "; echo "--- Attaque SCALE --- "
s=300 

while [ $s -ge 10 ]; do 
	cp tmpo.pgm tmpa.pgm 
	$scale tmpa.pgm $s
	
	res=$($wmread tmpa.pgm | grep "Message")
	
	if [ $(echo $res | cut -d":" -f2) = $message ]; then 
		echo "    OK | S = $s"
	else 
		echo "    -- | S = $s"
	fi 

	s=$(expr $s - 10) 
done

echo " "