#!/bin/bash

if [[ "$1" == '' ]]; then
	listen="127.0.0.1:3000"
else
	listen=$1
fi

for (( ; ;))
do
	clear
	echo -e "***  sniff of \e[34m$listen\e[0m ***"
	code="netstat -an | awk '{if(\$1 == \"tcp\" && \$4 == \"$listen\") print \$6 }' |sort|uniq -c"
	dest=$(eval $code)
	if [[ "$dest" == '' ]]; then
		echo -e "\e[31mNothing...\e[0m"
	else
		IFS=$(echo -en "\n\b")
		for kv in ${dest}
		do
			key=$(echo $kv | awk '{print $2}')
			value=$(echo $kv | awk '{print $1}')
			echo -n "$key : "
			if [ $value -lt 10 ]; then
				echo -ne "\e[32m"
			elif [ $value -lt 50 ]; then
				echo -ne "\e[33m"
			else
				echo -ne "\e[31m"
			fi
			echo -e "$value\e[0m"
		done
		IFS=' '
	fi
	sleep 1
done
