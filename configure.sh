#!/bin/bash

dep_addr=()
dep_name=()

#dependency string_builder
dep_addr[0]=https://github.com/Oscuro1111/dependency.string-builder.git
dep_name[0]=dependency.string-builder




#---------------------------------------------------------------------
#shared dependency 

shared_dep_addr=()
shared_dep_name=()

#mpm_list
shared_dep_addr[0]=https://github.com/Oscuro1111/mpm_list.git
shared_dep_name[0]=mpm_list

shared_dep_addr[1]=https://github.com/Oscuro1111/fs_c.git
shared_dep_name[1]=fs_c

shared_dep_addr[2]=https://github.com/Oscuro1111/http_parse_utility.git
shared_dep_name[2]=http_parse_utility
#---------------------------------------------------------------------


function get_shared_dependency(){
	
	if [ -d .cache ]
	then
	     cd .cache

		 for index in ${!shared_dep_name[*]}
		 do
		     if [ -d ${shared_dep_name[$index]} ]
			 then
			     echo "checking dependency: ${shared_dep_name[$index]}..done [OK]"
             else
			     echo "downloading dependency ${shared_dep_name[$index]}..."
				 git clone ${shared_dep_addr[$index]}&&cd ${shared_dep_name[$index]}&&make build&&cp ./includes/* ../../includes/&&cp ./lib/* ../../lib/shared/&&cd ..
			  fi
		 done 
    fi

	cd ..
}

function get_dependency()
{
	mkdir .cache 
	cd .cache 

	for index in ${!dep_name[*]}
	do
		if [ -d ${dep_name[$index]} ]
		then
			echo "checking dependency: ${dep_name[$index]}...done-[OK]"
		else
			echo "downloading dependency ${dep_name[$index]}"
			git clone ${dep_addr[$index]}&&cd ${dep_name[$index]}&&make build&&cp ./includes/* ../../includes/&&cd .. 
			fi
		done

		cd ..

		cd lib/static 
		for index in ${!dep_name[*]}
		do
			for obj_file in $(ls ../../.cache/${dep_name[$index]}/objs|grep -E .o)
			do
				ar r ./lib.a ../../.cache/${dep_name[$index]}/objs/$obj_file 
			done 
		done
		cd ..
		cd ..
}


if [ -d lib ]
then
	echo "lib...[OK]"
else
	echo "creating lib..."&&mkdir -p lib/shared&&mkdir -p lib/static&&echo "Done...[ok]"&&get_dependency&&get_shared_dependency
fi


if [ -d "bin" ]
then 
		echo "checked build directory exist name: bin ...done [OK]"
else
		echo "creating building directory \"bin\""&& mkdir bin
fi


if [ -d temp ]
then
			echo "temp...[ok]"
else
			mkdir temp
			echo "temp dir is created"
fi