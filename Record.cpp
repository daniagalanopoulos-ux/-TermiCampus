#include "Record.hpp"
#include "Course.hpp"

Record::Record(Course* c, float g) : course(c), grade(g) {}

Course* Record::getCourse() const
{
    return course;
}

float Record::getGrade() const
{
    return grade;
}

void Record::setGrade(float newGrade) 
{
    if (newGrade >= 0.0f && newGrade <= 10.0f) {
        grade=newGrade;
    } else {
        grade=-1.0f;
    }
}

bool Record::hasGrade() const
{
    return grade>=0.0f;
}