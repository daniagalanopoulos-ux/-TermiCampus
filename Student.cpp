#include "Student.hpp"
#include "Course.hpp"
#include <iostream>
#include <cstring>

using namespace std;

// Constructors 
Student::Student(const char* studentId, const string& studentName) : Person(studentId, studentName, "Μη ορισμένο"),semester(1){}
Student::Student(const char* studentId, const string studentName, const string& studentGender, unsigned int studentSemester) : Person(studentId, studentName, studentGender), semester(studentSemester){}
Student::Student(const Student& other) : Person(other), semester(other.semester), enrolledCourses(other.enrolledCourses){}

unsigned int Student::getSemester() const
{
    return semester;
}

void Student::setSemester(unsigned int newSemester)
{
    semester=newSemester;
}

// Υπερφόρτωση τελεστών
Student& Student::operator++()
{
    ++semester; // προσαύξηση
    return *this; 
}

Student Student::operator++(int) 
{
    Student temp=*this;
    semester++; // μετααύξηση
    return temp;
}

Student& Student::operator+=(unsigned int value)
{
    semester+=value;
    return *this;
}

Student& Student::operator-=(unsigned int value)
{
    if (semester>value) {
        semester-=value;
    } else {
         semester=1;
    }
    return *this;
}

Student& Student::operator-()
{
    if (getGender()=="Male" || getGender()=="A" || getGender()=="Άρρεν") {
        setGender("Θήλυ");
    } else {
        setGender("Άρρεν");
    }

    return *this;
}

ostream& operator<<(ostream& os, const Student& student) {
    string studentName=student.getName();
    const char* c_name=studentName.c_str();
    size_t name_length=strlen(c_name);

    os << "AM: " << student.getId() << " | Ονοματεπώνυμο: " << studentName << " (" << name_length << ") " << "| Φύλο: " << student.getGender() << " | Εξάμηνο: " << student.getSemester();
    
    return os;
}

// Προσθήκη μαθήματος στη λίστα του φοιτητή
void Student::enrollCourse(Course*c)
{
    if (c!=nullptr){
        enrolledCourses.push_back(c);
    }
}