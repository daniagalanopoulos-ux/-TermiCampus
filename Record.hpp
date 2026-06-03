#ifndef RECORD_HPP
#define RECORD_HPP

class Course;

class Record
{
    private:
        Course* course;
        float grade;

    public:
        Record(Course* c, float g = -1.0f); // η παράμετροσ g είναι προαιρετική ώστε ο φοιτητής να μπορεί να εγγραφεί στο μάθημα χωρίς να απαιτείται βαθμός αφού δεν έχει ακόμα και να μην πετάξει σφάλμα

        // δεν χρειαζόμαστε destructor, χρησιμοποιείται ο default της c++, χρειαζόμαστε όταν έχουμε δεσμεύσει μνήμη με  new
        Course* getCourse() const;
        float getGrade() const;
        void setGrade(float newGrade);

        bool hasGrade() const; // ελέγχει αν έχει καταχωρηθεί βαθμός με True/False
} ;

#endif