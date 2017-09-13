#include "unit.h"
#include <QThread> //for sleep while comparing
#include "QBrush"
#include "QDebug"
Unit::Unit() : value_{0}, column_rect{nullptr} {}

Unit::Unit(Unit&& other)
{
    value_ = std::move(other.value_);
}

Unit& Unit::operator=(Unit&& other)
{
    value_ = other.value_;
    emit operatorEQ(column_rect, column_value,  value_/100.0);
    //static bool one
           // static bool one_in_two = false;
           // if(one_in_two)
          //  {

    // }
    // else
      //   one_in_two = true;
    QThread::currentThread()->msleep(sleepTime);
    return *this;
}

bool Unit::operator<(const Unit& other)
{

   emit operatorCOMP(this->column_rect , this->column_rect);


    return this->value_ < other.value_;
}

Unit& Unit::operator=(int val)
{
    value_ = val;
    return *this;
}
