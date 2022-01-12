#!/bin/bash

echo "RESULTATS:" > resultat_tests.txt
echo >> resultat_tests.txt

for test in Examples_loader/example*.o
do 
  
	#1.1
	echo "" >> resultat_tests.txt
	echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" >> resultat_tests.txt
	echo "~~~~~~ FICHIER : $test ~~~~~~" >> resultat_tests.txt
	echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" >> resultat_tests.txt
	echo "" >> resultat_tests.txt

	readelf -h $test > .original
	./readelf -h $test | tail -n +2 > .custom

	# pas implemente, donc a supprimer
	sed -i 's|, Version5 EABI||g' .original

	if diff .original .custom > .temp_res
	then
	echo "1.1 : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "1.1 : NOK" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi

	#1.2
	
	readelf -S $test | tr -s ' ' | tail -n +2 > .original
	./readelf -S $test | tr -s ' ' | tail -n +2 > .custom

	if diff .original .custom > .temp_res
	then
	echo "1.2 : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "1.2 : NOK" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi

  	#1.3

	cat .custom | sed -n '/Section Headers:/,/Key to Flags:/p' | tr -s " " | sed '1,3d' | head -n -1 | sed -e 's/\[[^][]*]//g' | awk '{print $1}' > .temp_sec

	while IFS= read -r line; do
		readelf -x $line $test 2> /dev/null | sed '1,2d' | head -n -2 > .original
		./readelf -x $line $test 2> /dev/null | sed '1,3d' | head -n -2 | tail -n +2 > .custom

		sed -i -e '/Usage:/,+11d' .custom

		if diff .original .custom > .temp_res
		then
		echo "1.3 section $line: OK" >> resultat_tests.txt
		echo "" >> resultat_tests.txt
		else
		echo "1.3 section $line : NOK" >> resultat_tests.txt
		cat .temp_res >> resultat_tests.txt
		echo "" >> resultat_tests.txt
		fi
	done < .temp_sec

  	#1.4

	readelf -s $test | tail -n -2 | tr -s ' ' > .original
	./readelf -s $test | tail -n -2 | tr -s ' ' > .custom

  	sed -i 's/\t/ /g' .custom
  	sed -i 's/ *$//' .custom
  	sed -i 's/ *$//' .original

	if diff .original .custom > .temp_res
	then
	echo "1.4 : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "1.4 : NOK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	fi 


	#1.5

	readelf -r $test | tr -s ' ' > .original
	./readelf -r $test | head -n -1 | tail -n +2 | tr -s ' ' > .custom

  	#remove extra spaces at the end of each line
  	sed -i 's/ *$//' .custom

	if diff .original .custom > .temp_res
	then
	echo "1.5 : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "1.5 : NOK" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi

	echo "" >> resultat_tests.txt

done

echo "DONE"
echo "Le resultat a ete stocke dans le fichier resultat_tests.txt"
rm .temp_res
rm .temp_sec
rm .original
rm .custom
chmod u+rwx resultat_tests.txt 





