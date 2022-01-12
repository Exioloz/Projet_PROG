#!/bin/bash

echo "RESULTATS:" > resultat_tests.txt
echo >> resultat_tests.txt

for test in Examples_loader/example*.o
do 
  
	#1.1

	readelf -h $test > .original
	./readelf -h $test | tail -n +2 > .custom

  echo "1.1" >> resultat_tests.txt
  echo "" >> resultat_tests.txt
	if diff .original .custom > .temp_res
	then
	echo "" >> resultat_tests.txt
	echo "$test : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "" >> resultat_tests.txt
	echo "$test : NOK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi

  echo "" >> resultat_tests.txt

	#1.2
	
	readelf -e $test | tr -s ' ' | head -n -1 > .original
	./readelf -e $test | tr -s ' ' | tail -n +2 > .custom

  echo "1.2" >> resultat_tests.txt
  echo "" >> resultat_tests.txt
	if diff .original .custom > .temp_res
	then
	echo "" >> resultat_tests.txt
	echo "$test : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "" >> resultat_tests.txt
	echo "$test : NOK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi

  #1.3

	cat .custom | sed -n '/Section Headers:/,/Key to Flags:/p' | tr -s " " | sed '1,3d' | head -n -1 | sed -e 's/\[[^][]*]//g' | awk '{print $1}' > .temp_sec

  echo "1.3" >> resultat_tests.txt
  echo "" >> resultat_tests.txt
	while IFS= read -r line; do
		readelf -x $line $test | sed '1,2d' | head -n -2 > .original
		./readelf -x $line $test | sed '1,3d' | head -n -2 > .custom

		if diff .original .custom > .temp_res
		then
		echo "" >> resultat_tests.txt
		echo "$test, section $line: OK" >> resultat_tests.txt
		echo "" >> resultat_tests.txt
		else
		echo "" >> resultat_tests.txt
		echo "$test, section $line : NOK" >> resultat_tests.txt
		echo "" >> resultat_tests.txt
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

  echo "1.4" >> resultat_tests.txt
  echo "" >> resultat_tests.txt
	if diff .original .custom > .temp_res
	then
	echo "" >> resultat_tests.txt
	echo "$test : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "" >> resultat_tests.txt
	echo "$test : NOK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi 


	#1.5

	readelf -r $test | tr -s ' ' > .original
	./readelf -r $test | head -n -1 | tail -n +2 | tr -s ' ' > .custom

  #remove extra spaces at the end of each line
  sed -i 's/ *$//' .custom

  echo "1.5" >> resultat_tests.txt
  echo "" >> resultat_tests.txt
	if diff .original .custom > .temp_res
	then
	echo "" >> resultat_tests.txt
	echo "$test : OK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	else
	echo "" >> resultat_tests.txt
	echo "$test : NOK" >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	cat .temp_res >> resultat_tests.txt
	echo "" >> resultat_tests.txt
	fi

done
printf '\n'

echo "DONE"
#rm .temp_res
#rm .temp_sec
#rm .original
#rm .custom
chmod u+rwx resultat_tests.txt 


