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
    size_t name_length = profName.length(); // Υπολογισμός μήκους που πρόσθεσες

    os << " Κωδικός: " << prof.getId() << " | Όνομα: " << profName << " (" << name_length << ")\n"
       << " └ Φύλο: " << prof.getGender() << " | Ειδικότητα: " << prof.getSpecialty();
    return os;
}

// Προσθήκη μαθήματος στον καθηγητή
void Professor::assignCourse(Course *c)
{
    if (c==nullptr){ return; }
    for (Course* taught : taughtCourses) {
        if (taught==c) { return; }
    }
    taughtCourses.push_back(c);
}

void Professor::removeCourseReference(Course* c) {
    for (size_t i=0; i<taughtCourses.size(); i++) {
        if (taughtCourses[i]==c) {
            taughtCourses.erase(taughtCourses.begin()+i);
            return;
        }
    }
}