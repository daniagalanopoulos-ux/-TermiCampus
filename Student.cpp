#include "Student.hpp"
#include "Course.hpp"
#include "Record.hpp"
#include <iostream>
#include <cstring>
#include <stdexcept>

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

void Student::print(ostream& os) const{
    string studentName=getName();;
    size_t name_length=studentName.length();

    os << "AM: " << getId() << " | Ονοματεπώνυμο: " << studentName << " (" << name_length << ") " << "| Φύλο: " << getGender() << " | Εξάμηνο: " << getSemester();
    if (enrolledCourses.empty()) {
        os << "Καμία εγγραφή";
    } else {
        for (const auto& record : enrolledCourses) {
            os << "\n - " << record.getCourse()->getDescription();
            if (record.hasGrade()) {
                os << " | Βαθμός: " << record.getGrade();
            } else {
                os << " | Βαθμός: - ";
            } 
        }
    }
}

ostream& operator<<(ostream& os, const Student& student) {
    student.print(os);
    return os;
}

// Προσθήκη μαθήματος στη λίστα του φοιτητή
void Student::enrollCourse(Course*c)
{
    if (c!=nullptr){
        enrolledCourses.push_back(Record(c));
    }
}

void Student::assignGrade(Course* c, float grade)
{
    for (size_t i=0; i<enrolledCourses.size(); i++)
    {
        if (enrolledCourses[i].getCourse()==c) {
            enrolledCourses[i].setGrade(grade);
            return;
        }
    }
    throw runtime_error("Σφάλμα: ο φοιτητής δεν είναι εγγεγραμμένος στο συγκεκριμένο μάθημα.");
}