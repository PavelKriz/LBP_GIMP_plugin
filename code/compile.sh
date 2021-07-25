#!/bin/bash

#argmunets
if [ "$#" -ne 1 ]; then
	echo "Wrong count of arguments. Arguments is [gimptool-2.0 output]"
fi

#gimptool output = GO
GO1="$1"

#aditional library
GO2=$(echo $GO1 | awk '{ gsub("gcc","g++"); str1 = $0;
									str2 = str1 " -lm"; print str2}')
#all cpps
GO3=$(echo $GO2 | awk '{ gsub("PavelsLBPGimpPlugin.cpp", "PavelsLBPGimpPlugin.cpp CAbstractLBP.cpp CLBP.cpp CMLBP.cpp CULBP.cpp CCLBP.cpp CTile.cpp CTileManager.cpp CProcessHandler.cpp"); print $0}')
echo  
echo "Building and installing plugin"  
echo
eval "$GO3"
echo 
echo "Plugin is now installed"

exit 0


