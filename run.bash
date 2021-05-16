printf "==== Compiling ====\n"
make

printf "\n==== Running ====\n"
./bin/main.out;

printf "\n==== Cleaning Up ====\n"
rm ./bin/main.out;
