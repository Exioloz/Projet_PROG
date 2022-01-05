#!/bin/bash

echo "RESULTATS:" > resultat_tests.txt
echo >> resultat_tests.txt

for test in Examples_loader/example*.o
do
  readelf -hS $test > .original
  ./readelf $test > .custom
  sed -i '/Examples_loader/d' .custom
  sed -i 's/Elf Header/ELF Header/' .custom
  sed -i 's/Magic:/ Magic:/' .custom
  sed -i 's/Class:/ Class:/' .custom
  sed -i 's/Data:/ Data:/' .custom
  sed -i 's/OS\/ABI:/ OS\/ABI:/' .custom
  sed -i 's/Version:/ Version:/' .custom
  sed -i 's/ABI Version / ABI Version:/' .custom
  sed -i 's/Type:/ Type:/' .custom
  sed -i 's/Machine:/ Machine:/' .custom
  sed -i 's/Version / Version:/' .custom
  sed -i 's/Entry point address / Entry point address:/' .custom
  sed -i 's/Start of program headers / Start of program headers:/' .custom
  sed -i 's/Start of section headers:/ Start of section headers:/' .custom
  sed -i 's/Flags:/ Flags:/' .custom
  sed -i 's/Size of this header:/ Size of this header:/' .custom
  sed -i 's/Size of program headers:/ Size of program headers:/' .custom
  sed -i 's/Number of program headers:/ Number of program headers:/' .custom
  sed -i 's/Size of section headers:/ Size of section headers:/' .custom
  sed -i 's/Number of section headers:/ Number of section headers:/' .custom
  sed -i 's/Section header string table index:/ Section header string table index:/' .custom
  sed -i 's/Key to /Key to/' .custom
  sed -i 's/  W (write)/W (write)/' .custom
  sed -i 's/  L (link order)/L (link order)/' .custom
  sed -i 's/  C (compressed), x (unknown), o (OS specific), E (exclude),  /C (compressed), x (unknown), o (OS specific), E (exclude),/' .custom

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
