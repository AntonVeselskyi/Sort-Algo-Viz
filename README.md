# Sort Algorithm Visualizer
## (QT / C++)

Sort vertical bars.
To choose sort method change 
line 80 in mainwindow.cpp
(meh. maybe I`ll refactor it someday)
```c++ 
sort_thread = new SortThread(array, array_size, count_sort<Unit*>);//bubble_sort<Unit*>);
```
options for last arg:
- mq_sort - QUICK SORT
- shell_sort
- insert_sort
- selection_sort
- bubble_sort

