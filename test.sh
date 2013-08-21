#!/bin/bash
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
APP='./bin/tabfix';
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
# run app
function run_app()
{
	STDIN=$(cat);


	if [ "${FLAG_VALGRIND}" != "1" ];
	then
		if [ "${STDIN}" != "" ];
		then
			STDOUT=$(echo "${STDIN}" | ${APP} "${@}");
		else
			STDOUT=$(${APP} "${@}");
		fi
	else
		VAL="valgrind --tool=memcheck --leak-check=yes --leak-check=full --show-reachable=yes --log-file=valgrind.log";

		STDOUT=$(echo "${STDIN}" | ${VAL} ${APP} "${@}");

		echo '--------------------------' >> valgrind.all.log;
		cat valgrind.log >> valgrind.all.log;
		rm -rf valgrind.log;
	fi


	if [ "${STDOUT}" != "" ];
	then
		echo -n "${STDOUT}";
	fi
}
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
# test1
function test1()
{
	TMP1="$(mktemp)";
	TMP2="$(mktemp)";
	TMP3="$(mktemp)";


	echo -en "${MSG1}" > "${TMP1}";
	echo -en "${MSG1}" > "${TMP2}";
	echo -en "${MSG2}" > "${TMP3}";
	run_app -c -m -kb -s "${TMP2}" < /dev/null;


	HASH1="$(md5sum ${TMP2} | awk '{print $1}')";
	HASH2="$(md5sum ${TMP3} | awk '{print $1}')";


	if [ "${FLAG_DEBUG}" != "" ];
	then
		echo "-------------------------------";
		echo -n "data1(${TMP1}): "; cat "${TMP1}"; echo;
		echo -n "data2(${TMP2}): "; cat "${TMP2}"; echo;
		echo -n "data3(${TMP3}): "; cat "${TMP3}"; echo;
	fi


	if [ "${HASH1}" != "${HASH2}" ];
	then
		echo "ERROR: result different...";

		echo -n "data1(${TMP1}): "; cat "${TMP1}"; echo;
		echo -n "data2(${TMP2}): "; cat "${TMP2}"; echo;
		echo -n "data3(${TMP3}): "; cat "${TMP3}"; echo;
		exit 1;
	fi


	rm "${TMP1}";
	rm "${TMP2}";
	rm "${TMP3}";
}
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
# check depends
function check_prog()
{
	for i in ${CHECK_PROG_LIST};
	do
		if [ "$(which ${i})" == "" ];
		then
			echo "ERROR: you must install \"${i}\"...";
			echo;
			echo;
			exit 1;
		fi
	done
}
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
if [ ! -e "${APP}" ];
then
	echo "ERROR: make it";
	exit 1;
fi


CHECK_PROG_LIST='awk cat echo md5sum mktemp rm';
check_prog;


MSG1="hello\nworld!";
MSG2="hello\nworld!";
test1;

MSG1="    for(;;)";
MSG2="	for(;;)";
test1;

MSG1="        for(;;)";
MSG2="		for(;;)";
test1;

MSG1="	        for(;;)";
MSG2="			for(;;)";
test1;

MSG1="    	    for(;;)";
MSG2="			for(;;)";
test1;

MSG1="//    	    for(;;)";
MSG2="//			for(;;)";
test1;

MSG1="<--><--><-->for(;;)";
MSG2="			for(;;)";
test1;

MSG1="//<--><--><-->for(;;)";
MSG2="//			for(;;)";
test1;

MSG1="                break; //                is break;";
MSG2="				break; //                is break;";
test1;

MSG1="//                break; //                is break;";
MSG2="//				break; //                is break;";
test1;

MSG1="//<--><--><--><-->break; //                is break;";
MSG2="//				break; //                is break;";
test1;


echo "ok, test passed";
exit 0;
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
