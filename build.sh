CPU_CORE_NUM=$(nproc)
if [ $CPU_CORE_NUM -lt 8];then
	CPU_CORE_NUM=8
fi
echo ">>>>>>>> parallel number is ${CPU_CORE_NUM}"

mkdir build;
cd build/;
#cmake -DCMAKE_BUILD_TYPE="Debug" ..
cmake -DCMAKE_BUILD_TYPE="" ..
make;
cd ../;
