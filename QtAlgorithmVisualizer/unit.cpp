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
    //this class should manage the rect on the scene
    value_ = other.value_;
    emit operatorEQ(column_rect, column_value,  value_/100.0);

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
