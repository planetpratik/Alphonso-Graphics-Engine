if not defined in_subprocess (cmd /k set in_subprocess=y ^& %0 %*) & exit )
echo Making 'Alphonso Engine' Project
mkdir build
cd build
cmake ..