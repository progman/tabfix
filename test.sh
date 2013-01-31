#!/bin/bash
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
function test_it()
{
	TMP1="$(mktemp)";
	TMP2="$(mktemp)";
	TMP3="$(mktemp)";


	echo -en "${MSG1}" > "${TMP1}";
	echo -en "${MSG1}" > "${TMP2}";
	echo -en "${MSG2}" > "${TMP3}";
	./bin/tabfix -c -m -kb -s "${TMP2}";


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
if [ ! -e ./bin/tabfix ];
then
	echo "ERROR: make it";
	exit 1;
fi


MSG1="hello\nworld!";
MSG2="hello\nworld!";
test_it;

MSG1="    for(;;)";
MSG2="	for(;;)";
test_it;

MSG1="        for(;;)";
MSG2="		for(;;)";
test_it;

MSG1="	        for(;;)";
MSG2="			for(;;)";
test_it;

MSG1="    	    for(;;)";
MSG2="			for(;;)";
test_it;

MSG1="//    	    for(;;)";
MSG2="//			for(;;)";
test_it;

MSG1="<--><--><-->for(;;)";
MSG2="			for(;;)";
test_it;

MSG1="//<--><--><-->for(;;)";
MSG2="//			for(;;)";
test_it;

MSG1="                break; //                is break;";
MSG2="				break; //                is break;";
test_it;

MSG1="//                break; //                is break;";
MSG2="//				break; //                is break;";
test_it;

MSG1="//<--><--><--><-->break; //                is break;";
MSG2="//				break; //                is break;";
test_it;



echo "ok, test passed";

exit 0;
#-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
