#include "Professor.hpp"
#include "Course.hpp"
#include <cstring>

using namespace std;

// Constructor
Professor::Professor(const char* profId, const string& profName, const string& profGender, const std::string& profSpecialty) : Person(profId, profName, profGender), specialty(profSpecialty){}
// Destructor
Professor::~Professor(){}

string Professor::getSpecialty() const
{
    return specialty;
}
void Professor::setSpecialty(const string& newSpecialty)
{
    specialty=newSpecialty;
}   

ostream& operator<<(ostream& os, const Professor& prof) {
    string profName = prof.getName();
    const char* c_name = profName.c_str();
    size_t name_length = strlen(c_name);

    os << "Κωδικός: " << prof.getId() << " | Όνομα: " << profName << " (" << name_length << ") " << "| Φύλο: " << prof.getGender() << " | Ειδικότητα: " << prof.getSpecialty();
    return os;
}

// Προσθήκη μαθήματος στον καθηγητή
void Professor::assignCourse(Course *c)
{
    if (c!=nullptr){
        taughtCourses.push_back(c); 
    }
}