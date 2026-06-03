#ifndef STUDENT_HPP
#define STUDENT_HPP

#include "Person.hpp"
#include "Record.hpp"
#include <vector>
#include <iostream>

using namespace std;

class Course; // Forward declaration, για να μπορέσει να κάνει compile το πρόγραμμα και να μην δημιουργηθεί ένας "ατέρμων βρόχος"

class Student : public Person
{
    private:
        unsigned int semester;
        vector<Record> enrolledCourses; // το αντικείμενο περιέχει και το μάθημα και τον βαθμό του
    
    public:
        Student(const char* studentId, const string& studentName);
        Student(const char* studentId, const string studentName, const string& studentGender, unsigned int studentSemester);
        Student(const Student& other);

        unsigned int getSemester() const;
        void setSemester(unsigned int newSemester);

        Student &operator++(); 
        Student operator++(int);
        Student &operator+=(unsigned int value);
        Student &operator-=(unsigned int value);
        Student &operator-();

        void enrollCourse(Course* c);
        void assignGrade(Course* c, float grade);

        void print(ostream& os) const;
}; 

ostream& operator<<(ostream& os, const Student& student);
#endif