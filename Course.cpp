#include "Course.hpp"
#include "Professor.hpp"

using namespace std;

Course::Course(const string& courseCode, const string& courseDesc, unsigned int courseSemester, Professor* courseProf) : code(courseCode), description(courseDesc), semester(courseSemester), prof(courseProf) {}

string Course::getCode() const
{
    return code;
} 

string Course::getDescription() const
{
    return description;
}

unsigned int Course::getSemester() const
{
    return semester;
}

Professor* Course::getProf() const
{
    return prof;
}

void Course::setProf(Professor* newProf)
{
    // σε περίπτωση που αλλάξει ο υπεύθυνος μαθήματος
    prof=newProf;
}