# ParallelProgram1

Задание - посчитать сумму значений синуса на одной периоде, количество элементов 10^7. 

При типе double программа выдает значение -0.000779278, при типе float 0.0771715

Для компиляции программы с типом double на make

make

Для компиляции программы с типом float на make

make

Для компиляции программы с типом double на cmake 

cmake -S . -B build 

cmake --build build

Для компиляции программы с типом float на cmake

cmake -S . -B build2 -D float=ON

cmake --build build2
