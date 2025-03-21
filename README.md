# ParallelProgram1

Задание - посчитать сумму значений синуса на одной периоде, количество элементов 10^7. 

При типе double программа выдает значение -0.000779278, при типе float 0.0771715

Для компиляции программы с типом double на make

git checkout make

make

Для компиляции программы с типом float на make

git checkout make

make option=float

Для компиляции программы с типом double на cmake 

git checkout cmake

cmake -S . -B build 

cmake --build build

Для компиляции программы с типом float на cmake

git checkout cmake

cmake -S . -B build2 -D float=ON

cmake --build build2

# ParallelProgram2

Нужно перейти на ветку master, чтобы иметь доступ ко второй лаборатной работе по Параллельному программированию
