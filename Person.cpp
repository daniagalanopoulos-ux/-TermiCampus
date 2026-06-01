#include "Person.hpp"
#include <cstring>

using namespace std;

// βασικός constructor
Person::Person(const char* personId, const string& personName, const string& personGender) : name(personName), gender(personGender) 
{
    if (personId!=nullptr){
        id=new char[strlen(personId)+1];
        strcpy(id, personId);
    } else {
        id=nullptr;
    } 
}

// destructor
Person::~Person()
{
    delete[] id;
}

Person::Person(const Person& other) : name(other.name), gender(other.gender)
{
    if (other.id!=nullptr){
        id=new char[strlen(other.id)+1];
        strcpy(id, other.id);
    } else {
        id=nullptr;
    }
}

Person& Person::operator=(const Person& other) 
{
    if (this==&other){
        return *this;
    }

    delete[] id;
    name=other.name;
    gender=other.gender;
    if (other.id!=nullptr){
        id=new char[strlen(other.id)+1];
        strcpy(id, other.id);
    } else {
        id=nullptr;
    }

    return *this;
}

const char* Person::getId() const 
{
    return id;
}

string Person::getName() const 
{
    return name;
}

string Person::getGender() const 
{
    return gender;
}

void Person::setGender(const string& newGender) 
{
    gender=newGender;
}