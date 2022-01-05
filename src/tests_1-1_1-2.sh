#!/bin/bash

echo "RESULTATS:" > resultat_tests.txt
echo >> resultat_tests.txt

for test in Examples_loader/example*.o
do
  readelf -hS $test > .original
  ./readelf $test > .custom
  sed -i '/Examples_loader/d' .custom
  sed -i 's/  W (write)/W (write)/' .custom
  sed -i 's/  L (link order)/L (link order)/' .custom
  sed -i 's/  C (compressed), x (unknown), o (OS specific), E (exclude),  /C (compressed), x (unknown), o (OS specific), E (exclude),/' .custom
  sed -i 's/   y (purecode)/ y (purecode)/' .custom
  sed -i 's/   y (purecode)/ y (purecode)/' .custom

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
echo "DONE"

rm .temp_res
rm .original
rm .custom
chmod u+rwx resultat_tests.txt 
