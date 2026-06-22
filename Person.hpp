#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>
#include <iostream>

using namespace std;

class Person 
{
    protected:
        char *id;
        string name;
        string gender;

    public: 
        // βασικός constructor
        Person(const char* personId, const string& personName, const string& personGender) ;

        virtual ~Person();
        Person(const Person& other);
        Person& operator=(const Person& other);

        const char* getId() const;
        string getName() const;
        string getGender() const; 

        void setGender(const string& newGender);
        void removeCourseReference(Course* c);
} ; 
#endif
