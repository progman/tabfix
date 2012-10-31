#!/bin/bash

if [ "${1}" == "" ];
then
	echo "${0} broken_source_file";
	exit 1;
fi

if [ ! -e "${1}" ];
then
	echo "${0} broken_source_file";
	exit 1;
fi


mv "${1}" "${1}.bak";

cat "${1}.bak" | sed -e 's/^\ \ \ \ \ \ \ \ \ \ \ \ \ \ \ \ /\t/g' | sed -e 's/^\ \ \ \ \ \ \ \ \ \ \ \ /\t/g' | sed -e 's/^\ \ \ \ \ \ \ \ /\t/g' | sed -e 's/^\ \ \ \ /\t/g' > "${1}";

exit 0;
