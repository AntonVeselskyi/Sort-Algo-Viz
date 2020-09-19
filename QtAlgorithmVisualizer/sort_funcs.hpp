#pragma once
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QDateTime>

#include "rectfiller.hpp"
#include "sortthread.hpp"
#include "unit.h"

//sorting functions

//BUBBLE
template<class T> inline
void bubble_sort(T first, T last)
{
    bool sort_needed = true;
    int j = 0;
    while(sort_needed)
    {
        sort_needed = false;

        for(T i = first  ; i != last -1-j; ++i)
        if(i[1]<i[0])
        {
            sort_needed=true;
            qSwap(i[0],i[1]);
        }

        j++;

    }
}

//SELECTION SORT
template<class T> inline
void selection_sort(T first, T last)
{
    T min = first;

    int head_element = 0;
    while (min != last)
    {
        for(T i = min+1 /*next after min*/  ; i != last; ++i)
        {
            if(*i < *min)
                min = i;
        }

        qSwap(first[head_element], *min);
        head_element++;
        min = (first + head_element);
    }
}

//INSERTION SORT
template<class T> inline
    void insert_sort(T first, T last)
{
    for(T i = first+1; i != last-1; ++i)
    {
        T tmp = i;
        while(i>=first)
        {
            if(i[1]<i[0])
                qSwap(i[1],i[0]);
            i--;
        }
        i=tmp;
    }
}

//SHELL SORT
template<class T> inline
void shell_sort(T first, T last)
{
    int N = (last-first); //size
    qDebug() <<"size: " << N;
    for (int d = N/2; d >= 1; d /= 2) //set step
        for (int i = d; i < N; i++) //from d to N
            for (int j = i; j >= d; j -= d)// compare j with j-d
                if(first[j] < first[j-d])
                    qSwap(first[j-d], first[j]);
}

#include <cstring>
//for memcpy

//MERGE SORT
template<class T> inline
void merge_sort(T first, T last)
{
    const int N = (last-first); //size

    qDebug() <<"N: -->  " << N << "L:" <<(last-1)->value() <<" M_L: " << first[N-1].value();
    if (N <= 1)
        return;

    T left_start_iterator = first;
    T right_start_iterator = &first[N/2];

    //separating
    merge_sort(left_start_iterator, right_start_iterator); //left half
    merge_sort(right_start_iterator , last); //right half

    //at this point two upper funcs will return sorted halfs
    qDebug() <<"after N: -->  " << N;

    Unit* tmp_elements = new Unit[N];
    qDebug() <<"TMP array: -->  " << &(tmp_elements[N-1])  - tmp_elements;

    //the merging itself
    while(true)
    {
        if(*left_start_iterator < *right_start_iterator) //compare right and left half by elements
        {
            *tmp_elements = ((Unit*)left_start_iterator)->value();
            tmp_elements++; left_start_iterator++;

            if(left_start_iterator == &first[N/2])
            {
                //copy what`s right
                while(right_start_iterator != last)
                {
                    tmp_elements[0] =  ((Unit*)right_start_iterator)->value();
                    tmp_elements++; right_start_iterator++;
                }
                qDebug() << "copied whats left";
                break;
            }
        }
        else
        {
            *tmp_elements = ((Unit*)right_start_iterator)->value();

            tmp_elements++; right_start_iterator++;
            if(right_start_iterator == last) //copy part that has`nt been copied
            {
                //copy what`s left
                while(left_start_iterator != &first[N/2])
                {
                  // qDebug() <<"~2";
                    tmp_elements[0] =  ((Unit*)left_start_iterator)->value();
                    tmp_elements++; left_start_iterator++;
                }
                qDebug() << "copied whats right";
                break;
            }
        }
    }

    tmp_elements-=N;
    for(int i = 0; i < N; ++i)
    {
        qDebug() << tmp_elements[i].value();
        qSwap(first[i], tmp_elements[i]);
    }
    qDebug() << "___________________________";
    delete [] tmp_elements;
}


int mpartition(Unit *arr, const int left, const int right);
void mquicksort(Unit *arr, const int left, const int right, const int sz);
template<class T> inline
void mq_sort(T first, T last);

int mpartition(Unit *arr, const int left, const int right)
{
    const int mid = left + (right - left) / 2;
    const int pivot = arr[mid];
    // move the mid point value to the front.
    std::swap(arr[mid],arr[left]);
    int i = left + 1;
    int j = right;
    while (i <= j)
    {
        while(i <= j && arr[i] <= pivot)
            i++;

        while(i <= j && arr[j] > pivot)
            j--;

        if (i < j)
            std::swap(arr[i], arr[j]);

    }
    std::swap(arr[i - 1],arr[left]);
    return i - 1;
}

void mquicksort(Unit *arr, const int left, const int right, const int sz)
{
    if (left >= right)
        return;

    int part = mpartition(arr, left, right);

    mquicksort(arr, left, part - 1, sz);
    mquicksort(arr, part + 1, right, sz);
}

//QUICK SORT
template<class T> inline
void mq_sort(T first, T last)
{
    int N = (last-first); //size
    mquicksort((Unit*)first, 0, N-1, N);
}

#define GAP 100
template<class T> inline
void count_sort(T first, T last)
{
    int count_array[GAP] = {0};
    const int N = (last-first); //size

   for(int i = 0; i<N; i++, first++)
        count_array[ ( (Unit*)first )->value() ] ++;

    Unit* tmp_elements = new Unit[N];
    for(int i = 0; i < GAP; i++)
        for(int j = 0; j < count_array[i]; j++, tmp_elements++)
            *tmp_elements = i;

    first-=N;
    tmp_elements-=N;
    for(int i = 0; i<N; i++, first++, tmp_elements++)
        qSwap(*first, *tmp_elements);

    delete [] (tmp_elements-N);
}
