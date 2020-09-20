#pragma once
#include <QThread>
#include <QTime>
#include <functional>
#include <algorithm>
#include "unit.h"


// A thread which sorts the Unit array with the sorting function provided in ctor
class SortThread : public QThread
{
    Q_OBJECT
public:
    using sortFuncType = std::function<void(Unit*,Unit*)>;

private:
    Unit *array_;
    unsigned long arraysize_;
    sortFuncType sortfunc_;

public:
    SortThread(Unit *array, unsigned long arraysize, sortFuncType func) :
               array_{array}, arraysize_{arraysize}, sortfunc_{func}
    {}
    virtual void run() override
    {
        QDateTime start = QDateTime::currentDateTime();
        sortfunc_(array_, array_+arraysize_);
        QDateTime finish = QDateTime::currentDateTime();
        qint64 ms = start.msecsTo(finish);

        emit end_of_run(ms);
    }

signals:
    void end_of_run(qint64 ms);
};
