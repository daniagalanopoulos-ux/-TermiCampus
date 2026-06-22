#include "Foithtologio.hpp"
#include "Student.hpp"
#include "Professor.hpp"

#include <iostream> 
#include <fstream>
#include <sstream>
#include <cstring>
#include <stdexcept>

using namespace std;

static string sanitizeCSVField(const string& field) {
    string result=field;
    for (char& ch : result) {
        if (ch==',') {
            ch=';';
        }
    }
    return result;
}

Foithtologio::Foithtologio() {}

void Foithtologio::sendEmailsToStudents(ostream& os) const  
{
    os << "Αποστολή email σε φοιτητές" << endl;
    for (Person* p : members) {
        Student* studentPtr = dynamic_cast<Student*>(p);
        
        if (studentPtr != nullptr) {
            os << "Στάλθηκε ενημερωτικό μήνυμα στον/στην φοιτητή/φοιτήτρια: " << studentPtr->getName() << " (ΑΜ: " << studentPtr->getId() << ")" << " | Θέμα: Ενημέρωση Εξαμήνου" << " | Κείμενο: Βρίσκεστε στο " << studentPtr->getSemester() << "ο εξάμηνο."<< "\n";
        }
    }
}

void Foithtologio::sendEmailsToProfessors(ostream& os) const
{
    os << "Aποστολή email σε καθηγητές/τριες: " << endl;
    for (Person* p : members) {
        Professor* profPtr=dynamic_cast<Professor*>(p);
        if (profPtr!=nullptr) {
            os << "Στάλθηκε ενημερωτικό μήνυμα στον/στην καθηγητή/καθηγήτρια" << profPtr->getName() << " (Ειδικότητα: " << profPtr->getSpecialty() << ")"<< "\n";
        }
    }
}

// destructor
Foithtologio::~Foithtologio() 
{
    for (Person* p: members) {
        delete p;
    }
    for (Course* c : courses) {
        delete c;
    }
}

Person* Foithtologio::findMember(const char* id) 
{
    for (Person* p : members) {
        if (strcmp((*p).getId(), id)==0) {
            return p; // βρέθηκε
        }
    }
    return nullptr;
}

void Foithtologio::addMember(Person *p)
{
    if (p==nullptr) {
        throw invalid_argument("Σφάλμα: προσπάθεια εισαγωγής κενού δείκτη μέλους");
    }
    if (findMember((*p).getId())!=nullptr) {
        throw runtime_error("Σφάλμα: το μέλος με αυτό τον ΑΜ υπάρχει ήδη στο σύστημα");
    }
    members.push_back(p);
}

void Foithtologio::removeMember(const char* id)
{
    for (size_t i=0; i<members.size(); i++) {
        if (strcmp(members[i]->getId(), id)==0) {
            Professor* profPtr=dynamic_cast<Professor*>(members[i]);
            if (profPtr!=nullptr){
                for (Course* c : courses) {
                    if (c->getProf() == profPtr) {
                        c->setProf(nullptr);
                    }
                }
            }
            delete members[i];
            members.erase(members.begin()+i);
            return;
        }
    }
    throw runtime_error("Σφάλμα: το μέλος δεν βρέθηκε για διαγραφή");
}

Course* Foithtologio::findCourse(const string& code) const
{
    for (Course* c : courses) {
        if (c->getCode()==code) {
            return c;
        }
    }
    return nullptr;
} 

void Foithtologio::addCourse(Course* c)
{
    if (c==nullptr) {
        throw invalid_argument("Σφάλμα: προσπάθεια εισαγωγής μη αναγνωρίσιμου μαθήματος");
    }
    if (findCourse(c->getCode())!=nullptr) {
        throw runtime_error("To μάθημα υπάρχει ήδη");
    }
    courses.push_back(c);
}
 
void Foithtologio::removeCourse(const string& code) 
{
    for (size_t i=0; i<courses.size(); i++) {
        if (courses[i]->getCode()==code) {
            Course* c=courses[i];
            for (Person* p : members) {
                Student* s = dynamic_cast<Student*>(p);
                if (s!=nullptr) {
                    s->removeCourseReference(c);
                    continue;
                }
                Professor* prof = dynamic_cast<Professor*>(p);
                if (prof!=nullptr) {
                    prof->removeCourseReference(c);
                }
            }
            delete c;
            courses.erase(courses.begin()+i);
            return;
        }
    }
    throw runtime_error("Σφάλμα: δεν βρέθηκε μάθημα για διαγραφή");
}

void Foithtologio::saveToCSV() const 
{
    ofstream fStud("Students.csv");
    ofstream fProf("Professors.csv");
    ofstream fCourse("Courses.csv");
    // myFile.open(filename, ios::out);
    
    string errorFiles="";
    if (!fStud.is_open()) {
        errorFiles+="Students.csv";
    }
    if (!fProf.is_open()) {
        errorFiles+="Professors.csv";
    }
    if (!fCourse.is_open()) {
        errorFiles+="Courses.csv";
    }
   
    if (!errorFiles.empty()) {
        string finalMessage="Σφάλμα, δεν ήταν δυνατό το άνοιγμα των παρακάτων αρχείων: " + errorFiles;
        throw runtime_error(finalMessage);
    }

    // Γράφω τους φοιτητές και τους καθηγητές
    for (int i=0; i<members.size(); i++) {
        Person* p=members[i];
        
        Student* s=dynamic_cast<Student*>(p);
        if (s!=nullptr) {
            fStud << s->getId() << "," <<sanitizeCSVField(s->getName()) << "," << s->getGender() << "," << s->getSemester() << "\n";
            continue;
        }
        Professor* prof=dynamic_cast<Professor*>(p);
        if (prof!=nullptr) {
            fProf << prof->getId() << "," << sanitizeCSVField(prof->getName()) << "," << prof->getGender() << "," << prof->getSpecialty() << "\n";
            continue;
        }
    }

    // Γράφω τα μαθήματα
    for (int i=0; i<courses.size(); i++) {
        Course* c=courses[i];

        string profId="None";
        if (c->getProf()!=nullptr) {
            profId=c->getProf()->getId();
        }

        fCourse << c->getCode() << "," << sanitizeCSVField(c->getDescription()) << "," << c->getSemester() << "," << profId << "\n";
    }
    fStud.close();
    fProf.close();

    ofstream fGrades("Grades.csv");
    for (Person* p : members) {
        Student* s=dynamic_cast<Student*>(p);
        if (s!=nullptr) {
            for (const auto& record : s->getEnrolledCourses()) {
                fGrades << s->getId() << "," << record.getCourse()->getCode() << "," << record.getGrade() << "\n" ;
            }
        }
    }
    fCourse.close();
}

void Foithtologio::loadFromCSV()
{
    string line;

    // Φόρτωση καθηγητών
    ifstream fProf("Professors.csv");
    if (!fProf.is_open()) {
        throw runtime_error("Σφάλμα: δεν βρέθηκε αρχείο 'Professors.csv'. ");
    }

    while (getline(fProf, line)) {
        stringstream ss(line);            
        string id, name, gender, specialty;

        getline(ss, id, ',');            
        getline(ss, name, ',');
        getline(ss, gender, ',');
        getline(ss, specialty, ',');
        
        addMember(new Professor(id.c_str(), name , gender, specialty));
    }
    fProf.close();

    // Φόρτωση φοιτητών
    ifstream fStud("Students.csv");
    if (!fStud.is_open()) {
        throw runtime_error("Σφάλμα: δεν βρέθηκε αρχείο 'Students.csv'. ");
    }

    while (getline(fStud, line)) {
        stringstream ss(line);
        string id, name, gender, extra;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, gender, ',');
        getline(ss, extra, ',');

        unsigned int semester=stoi(extra);
        addMember(new Student(id.c_str(), name, gender, semester));
    }
    fStud.close();

    // Φόρτωση μαθημάτων
    ifstream fCourse("Courses.csv");
    if (!fCourse.is_open()) {
        throw runtime_error("Σφάλμα: δεν βρέθηκε αρχείο 'Courses.csv'. ");
    }

    while (getline(fCourse, line)) {
        stringstream ss(line);
        string code, description, semester_str, profId;

        getline(ss, code, ',');
        getline(ss, description, ',');
        getline(ss, semester_str, ',');
        getline(ss, profId, ',');

        unsigned int semester=stoi(semester_str);
        Professor* assignedProf=nullptr;

        if (profId!="None") {
            Person* p=findMember(profId.c_str());
            assignedProf=dynamic_cast<Professor*>(p);
        }
        addCourse(new Course(code, description, semester, assignedProf));
    }
    fCourse.close();

    ifstream fGrades("Grades.csv");
    if (fGrades.is_open()) {
        while (getline(fGrades, line)) {
            stringstream ss(line);
            string studId, courseCode, gradeStr;

            getline(ss, studId, ',');
            getline(ss, courseCode, ',');
            getline(ss, gradeStr, ',');

            Person* p=findMember(studId.c_str());
            Student* s=dynamic_cast<Student*>(p);
            Course* c=findCourse(courseCode);

            if (s!=nullptr && c!=nullptr) {
                s->enrollCourse(c);
                s->assignGrade(c, stof(gradeStr));
            }
        }
    }
    fGrades.close();
}

void Foithtologio::printAllMembers(ostream& os) const
{
    os << "ΚΑΘΗΓΗΤΕΣ" << "\n";
    int countP=0;
    for (Person* p : members) {
        Professor* profPtr=dynamic_cast<Professor*>(p);
        if (profPtr!=nullptr) {
            os << *profPtr << "\n";
            countP++;
        }
    }
    if (countP==0) os << "Δεν έχουν βρεθεί καταχωρημένοι καθηγτητές\n";

    os << "ΦΟΙΤΗΤΕΣ" << "\n";
    int countS=0;
    for (Person* p : members) {
        Student* studentPtr=dynamic_cast<Student*>(p);
        if (studentPtr!=nullptr) {
            os << *studentPtr << "\n";
            countS++;
        }
    }
    if (countS==0) os << "Δεν έχουν βρεθέι καταχωρημένοι φοιτητές";
}