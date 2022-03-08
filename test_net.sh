#!/bin/bash

for (( ; ;))
do
	clear
	code="netstat -an | awk -f test.awk |sort|uniq -c"
	dest=$(eval $code)
	netstat -an | awk -f test.awk | sort | uniq -c
	if [[ "$dest" == '' ]]; then
		echo "Aucune connexion"
	fi
	sleep 1
done
