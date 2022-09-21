cmake -S ./ -B ./out/build -G "MinGW Makefiles"

cd ./out/build
mingw32-make 
cd ../../