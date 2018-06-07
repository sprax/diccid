#!/bin/bash

nom=${1:-1}

a=2
echo "$a + 2"
echo "$(($a + 2))"
echo "$a + $nom"
echo "$(($a + $nom))"

echo "now string-wise...."
if [ $nom == 1 ]; then
    echo "nom $nom is 1, bro."
else
    echo "nom $nom is not 1, bra."
fi

echo "but numbers-wise...."
if (($nom > 1)); then
    echo "nom $nom > 1, bra."
elif (($nom == 1)); then
    echo "nom $nom == 1, bro."
else
    echo "nom $nom <= 1, bru."
fi


exit 1

