#ifndef COURSE_HPP
#define COURSE_HPP
#include <string>

using namespace std;
// Η Course αντιπροσωπεύει ένα μάθημα, π.χ. Θεωρία Κυκλωμάτων, και δεν ξέρει ποιοι φοιτητές το παρακολουθούν, 
// αλλά μόνο τα δικά της στοιχεία και ποιος είναι ο υπεύθυνος καθηγητής

class Professor;
 
class Course
{
    private:
        string code;
        string description;
        unsigned int semester;
        Professor* prof;

    public:
        Course(const string& courseCode, const string& courseDesc, unsigned int courseSemester, Professor* courseProf);

        string getCode() const;
        string getDescription() const;
        unsigned int getSemester() const;
        Professor* getProf() const;

        void setProf(Professor* newProf);
};
#endif