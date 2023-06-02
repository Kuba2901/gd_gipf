echo "Podaj numer testu: "
read testNum

# Uruchamianie programu
g++-12 *.cpp -fsanitize=address && ./a.out < tests_GIPF/$testNum.txt > result_1.txt

# Otwieraniu wyniku
# open "result_1.txt"
# open "tests_GIPF/$testNum.out"

# Uruchamianie automatycznego testera
g++-12 *.cpp -o gipf.exe
./tester.sh gipf tests_GIPF