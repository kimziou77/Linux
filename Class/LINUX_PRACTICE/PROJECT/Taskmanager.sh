#!/bin/bash

#set val########
declare -i userCount=(`awk -F':' '{if($3>=1000)print $1}' /etc/passwd |wc -l`);
declare -i MAXLINE=20;
declare -i SELECT_NUM=0;
declare -i SELECT_ETC=0;
declare -i COLOR=1;
declare -i SCROLL=0;
declare -i extra_score1=0;
login_USER=`id -un`;


Table() {
	UC=(`expr ${userCount} + 1`);
	if [ $SELECT_ETC -eq -1 ];then if [ $SCROLL -gt 0 ];then ((SCROLL--));fi;
		SELECT_ETC=0;fi;
	if [ $SELECT_NUM -eq -1 ];then SELECT_NUM=0; fi;
	if [ $SELECT_ETC -eq 20 ];then if [ $SCROLL -lt 19 ];then ((SCROLL++));fi;
		SELECT_ETC=19;fi;
	if [ $SELECT_NUM -eq $UC ];then SELECT_NUM=$userCount; fi;

#call userName from /etc/passwd#########
IFS=$'\n' userName=(root `awk -F':' '{if($3>=1000)print $1}' /etc/passwd | sort -d`);
ps -ef|grep ${userName[$SELECT_NUM]}|awk '{print $1,$8,$2,$5}'|sort>user2.txt

## re-sort PID reverse
cat user2.txt|sort -k 3 -g -r>user3.txt;

#call CMd&PID&STIME from ps-ef
userCMD=(`cut -d " " -f 2 user3.txt`);
userPID=(`cut -d " " -f 3 user3.txt`);
userSTIME=(`cut -d " " -f 4 user3.txt`);
	echo " _ __                        "
	echo "( /  )           _/_o        "
	echo " /--'_   __,  _, / ,  _, _   "
	echo "/   / (_(_/(_(__(__(_(__(/_  "
	echo "            __                   "
	echo " o         ( /   o               "
	echo ",  _ _      /   ,  _ _   , , _., "
	echo "(_/ / /_  (/___/(_/ / /_(_/_/ /\_"

	echo "-NAME-----------------CMD--------------------PID-----STIME----";

	# controll out of range colorizing green
	for((i=0;i<$MAXLINE;i++))
	do
	if [ -z ${userCMD[i]} ];then if [ ${SELECT_ETC} -eq $i ];then ((SELECT_ETC--));fi;fi;
	done

	for((i=0;i<$MAXLINE;i++))
	do
	echo -n "|"
	#   NAME   ###################
	declare -i nameLength=${#userName[i]};
		for((j=0;j<20-$nameLength;j++))
		do
			if [ $i -eq $SELECT_NUM ];then echo -n "[41m";fi
			echo -n " ";
		done
	## if nameLen > 20 rewrite userName len 20
	if [ $nameLength -gt 20 ];then TmpUserName[i]=${userName[i]:0:20-$nameLength};echo -n "$TmpUserName[0m|";
	else
		echo -n "${userName[i]}[0m|"
	fi


	#   CMD   ###################
	if [ -z ${userPID[i+$SCROLL]} ]; then echo -n "";
	else BF_Test=(`ps -eo pid,stat |grep ${userPID[i + $SCROLL]} |awk '{print $2}'`);
	fi;
	
	###if COLOR is TURE => colorlizing
	if [ $COLOR -eq 0 ]; then
		if [ $i -eq $SELECT_ETC ];then echo -n "[42m";
			if [ ${extra_score1} -eq 3 ];then echo -n "[44m";fi;killPID=${userPID[i + $SCROLL]};
		fi;
		#echo -n "extra score : ${extra_score1}";
	fi;
	
	##Background & foreground
	if [ $i -ge ${#userPID[@]} ];then echo -n "  ";
	elif [[ "${BF_Test}" =~ "R" || "${BF_Test}" =~ "+" ||-z ${BF_Test} ]];then echo -n "F "
	else echo -n "B ";
	fi;

	declare -i CMDLength=${#userCMD[i + $SCROLL]};
	for((j=0;j<20-$CMDLength;j++))
	do
		echo -n " ";
	done

	if [ $CMDLength -gt 20 ];then TmpUserCMD=${userCMD[i + $SCROLL]:0:20-$CMDLength};echo -n "$TmpUserCMD|";
	else
		echo -n "${userCMD[i + $SCROLL]}|";	
	fi
	

	#  PID   #################
	declare -i PIDLength=${#userPID[i + $SCROLL]};
	for((j=0;j<7-$PIDLength;j++))
	do
		echo -n " ";
	done
	if [ $PIDLength -gt 20 ];then TmpUserPID=${userPID[i + $SCROLL]:0:20-$PIDLength};echo -n "$TmpUserPID|";
	else
		echo -n "${userPID[i + $SCROLL]}|";
	fi

	#  STIME  #################
	declare -i STIMELength=${#userSTIME[i + $SCROLL]};
	for((j=0;j<8-$STIMELength;j++))
	do
		echo -n " ";
	done
	if [ $STIMELength -gt 20 ];then TmpUserSTIME=${userSTIME[i + $SCROLL]:0:20-$STIMELength} echo -n "${TmpUserSTIME}|";
	else
		echo -n "${userSTIME[i + $SCROLL]}[0m|";
	fi;

	echo " ";
	done

	echo "--------------------------------------------------------------"
	echo "If you want to exit , please Type ‘q’ or ‘Q’";
}


no_permission(){
clear
echo "                        .-. .-. .----.                          "
echo "                        |  \`| |/  {}  \                         "
echo "                        | |\  |\      /                         "
echo "	                \`-' \`-' \`----'                          "
echo ".----. .----..----. .-.   .-..-. .----. .----..-. .----. .-. .-."
echo "| {}  }| {_  | {}  }|  \`.'  || |{ {__  { {__  | |/  {}  \|  \`| |"
echo "| .--' | {__ | .-. \| |\ /| || |.-._} }.-._} }| |\      /| |\  |"
echo "\`-'    \`----'\`-' \`-'\`-' \` \`-'\`-'\`----' \`----' \`-' \`----' \`-' \`-'"

}


UP="A";DOWN="B";RIGHT="C";LEFT="D";
Input_Key() {
	#selected user==login_USER && press enter => then kill -9 [selected ETC pid]
	if [ -z $1 ]; then if [ $COLOR -eq 0 ];then if [ ${userName[$SELECT_NUM]} == $login_USER ];then kill -9 $killPID;Table;
			else no_permission ;fi;fi;
	elif [ $1 =  ]; then read -sn 1 second; read -sn 1 third;
		if [ "$third" = $UP ];then if [ $COLOR -eq 1 ];then ((SELECT_NUM--));else ((SELECT_ETC--));fi;Table;
       		elif [ "$third" = $RIGHT ];then COLOR=0;Table;
	       	elif [ "$third" = $DOWN ];then if [ $COLOR -eq 1 ];then ((SELECT_NUM++));else ((SELECT_ETC++));fi;Table;
       		elif [ "$third" = $LEFT ];then COLOR=1;SELECT_ETC=0;Table;
       		fi
	elif [ $1 = c ]; then extra_score1=3; #change selected CMD color blue
	elif [ $1 = b ]; then extra_score1=0; #reset color to green
	elif [ $1 = s ]; then SCROLL=0; #scroll reset
	elif [ $1 = q ]; then break;
	elif [ $1 = Q ]; then break;
	else
		echo "not arrow you press $1"
	fi
}

Table

while [ "$key" != "q" ];
do
	if read -t 3 -s -n 1 key; then Input_Key "$key" #if enter killPID
	else Table; #nothing 3sec reload table
	fi
done
