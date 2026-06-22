#ifndef FOITHTOLOGIO_HPP
#define FOITHTOLOGIO_HPP

#include <vector>
#include <string>
#include "Person.hpp"
#include "Course.hpp"

class Foithtologio 
{
    private:
        vector<Person*> members;
        vector<Course*> courses;
    
    public:
        Foithtologio();
        ~Foithtologio();

        // διαχείριση μελών
        void addMember(Person* p);
        void removeMember(const char* id);
        Person* findMember(const char* id);

        // διαχείριση μαθημάτων
        void addCourse(Course* c);
        void removeCourse(const string& code);
        Course* findCourse(const string& code) const;

        void sendEmailsToStudents(std::ostream& os) const;
        void sendEmailsToProfessors(std::ostream& os) const;

        void saveToCSV() const;
        void loadFromCSV(); 

        void printAllMembers(std::ostream& os) const;
};

#endif 