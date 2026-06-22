#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include "Person.hpp"
#include <vector>

using namespace std;

class Course;

class Professor : public Person
{
    private:
        string specialty;
        vector<Course*> taughtCourses; // επειδή αυτή η λίστα περιέχει δείκτες, πολλοί διαφορετικοί καθηγητές μπορούν να βάλουν τον ίδιο δείκττη, 
                                       // άρα αν, π.χ. 2 καθηγητές διδάσκουν ένα μάθημα τότε θα έχουν τον ίδιο δείκτη

    public:
        Professor(const char* profId, const string& profName, const string& profGender, const string& profSpecialty);
        ~Professor();
        
        string getSpecialty() const;
        void setSpecialty(const string& newSpecialty);

        void assignCourse(Course* c);
        void removeCourseReference(Course* c);
};

ostream& operator<<(ostream& os, const Professor& prof);
#endif 
 