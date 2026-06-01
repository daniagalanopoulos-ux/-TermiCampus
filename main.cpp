#include "Professor.hpp"
#include "Student.hpp"
#include "Course.hpp"
#include "Foithtologio.hpp"

#include <iostream>
#include <ncurses.h> // !!
#include <locale.h>
#include <string>
#include <exception>

using namespace std;

void suspend_ncurses()
{
    def_prog_mode(); // αποθήκευση της τρέχουσας κατάστασης του τερματικού
    endwin(); // αναστολή ncurses και επαναφορά τερματικού
    cout << "\n===============================================\n";
}

void resume_ncurses() 
{
    cout << "\nΠατήστε Enter για επιστροφή στο μενού";
    cin.clear(); // καθαρίζει τυχόν περιπτώσεις σφάλματος
    cin.ignore(10000, '\n'); // αγνοεί και "διαγράφει" οτιδήποτε έχει μείνει στο buffer 
    cin.get(); // σταματάει το πρόγραμμα
    reset_prog_mode(); // επαναφορά του ncurses         
    refresh(); // ανανέωση οθόνης
}

void print_menu(WINDOW *menu_win, int highlight, string choices[], int new_choices) 
{
    int x=2, y=2;
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, 2, "Σύστημα Διαχείρισης Πανεπιστημίου");
    for (int i=0; i<new_choices; ++i)
    {
        if (highlight == i+1) {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
            wattroff(menu_win, A_REVERSE); 
        } else {
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str()); // (σε ποιο παράθυρο θα γίνει η  εκτύπωση, y, x, string, μετατροπή γιατί η ncurses είναι σε C)
        }
        y+=2;
    }
    wrefresh(menu_win); 
}

int main()
{
    setlocale(LC_ALL, "");

    Foithtologio uni;

    initscr(); // αρχικοποιείται η ncurses 
    clear();
    noecho(); // κλέινει η ηχώ των πλήκτρων
    cbreak();
    keypad(stdscr, TRUE); // επιτρέπεται η χρήση ειδικών πλήκτρω
    curs_set(0); // κρύβεται ο κέρσορας

    int startx=0, starty=0;
    string choices[] = 
    {
        "1. Δημιουργία / Φόρτωση Δοκιμαστικών Τελεστών",
        "2. Αποστολή email σε καθηγητές",
        "3. Αποστολή email σε φοιτητές",
        "4. Αποθήκευση σε CSV αρχείο",
        "5. Φόρτωση από CSV αρχείο",
        "6. Έξοδος",
    } ;
    int new_choices=sizeof(choices)/sizeof(string); // υπολογίζεται το πλήθος των επιλογών 

    WINDOW *menu_win=newwin(15, 60, starty, startx); // δημιουργείται παράθυρο
    keypad(menu_win, TRUE);

    int choice=0;
    int highlight=1;
    bool running=true;

    while(running)
    {
        print_menu(menu_win, highlight, choices, new_choices); // εμφανίζεται το μενού με την τρέχουσα επιλογή
        int c=wgetch(menu_win);

        switch(c) {
            case KEY_UP: 
                if (highlight==1)
                    highlight=new_choices;
                else 
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight==new_choices)
                    highlight=1;
                else 
                    ++highlight;
                break;
            case KEY_ENTER: // case 10
                choice=highlight;

                suspend_ncurses();

                try { // σε περίπτωση σφάλματος να μην κρασάρει το πρόγραμμα
                    switch(choice) {
                        case 1:
                            cout << "Δημιουργία δοκιμαστικών δεδομένων";
                            {
                                Professor* prof1=new Professor("25390001", "Γιώργος Μελετίου", "Άρρεν", "Προγραμματισμός");
                                Student* s1=new Student("25390003", "Μαρία Παπαδοπούλου", "Θήλυ", 8);
                                Course* c1=new Course("CS101", "Aντικειμενοστραφής Προγραμματισμός", 2, prof1);
                                prof1->assignCourse(c1);
                                s1->enrollCourse(c1);
                                uni.addMember(prof1);
                                uni.addMember(s1);
                                uni.addCourse(c1);
                                cout << "Τα μέλη προστέθηκαν επιτυχώς";
                            }
                            break;
                        case 2:
                            uni.sendEmailsToProfessors();
                            break;
                        case 3:
                            uni.sendEmailsToStudents();
                            break;
                        case 4:
                            uni.saveToCSV();
                            cout << "Τα αρχεία δημιουργήθηκαν επιτυχώς";
                            break;
                        case 5:
                            uni.loadFromCSV();
                            cout << "Τα δεδομένα φορτώθηκαν επιτυχώς";
                            break;
                        case 6:
                            cout << "Κλείσιμο εφαρμογής";
                            running=false;
                            break;
                    }
                } catch (const exception& e) {
                    cout << "Υπήρξε σφάλμα. \n" << e.what() << endl;
                }
                if (running) {
                    resume_ncurses(); // αν το πρόγραμμα δεν έχει τερματίσει πατάει ο χρήστης Enter και ξαναεμφανίζεται το μενού
                }
                break;
            default:
                break;
        }
    }

    endwin();
    return 0;
}