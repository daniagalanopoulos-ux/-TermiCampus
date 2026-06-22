#include "Professor.hpp"
#include "Student.hpp"
#include "Course.hpp"
#include "Foithtologio.hpp"

#include <iostream>
#include <ncurses.h> // !!
#include <locale.h>
#include <string>
#include <exception>
#include <sstream>
#include <vector>

using namespace std;

// Διαβάζει ένα αλφαριθμητικό (string) από το χρήστη με ασφάλεια
// Ενεργοποιεί προσωρινά την ηχώ (echo) ώστε ο χρήστης να βλέπει τι πληκτρολογεί.
string get_string_ncurses(WINDOW* win, int y, int x, const string& prompt) {
    mvwprintw(win, y, x, "%s", prompt.c_str());
    echo();

    string input(255, '\0');

    wgetnstr(win, &input[0], 255);

    noecho();
    return string(input.c_str());
}

// Διαβάζει κείμενο και το μετατρέπει με ασφάλεια σε ακέραιο
int get_int_ncurses(WINDOW* win, int y, int x, const string& prompt) {
    string s=get_string_ncurses(win, y, x, prompt);
    try { return stoi(s); } catch(...) { return 0; }
}

// Διαβάζει κείμενο και το μετατρέπει με ασφάλεια σε δεκαδικό
float get_float_ncurses(WINDOW* win, int y, int x, const string& prompt) {
    string s=get_string_ncurses(win, y, x, prompt);
    try { return stof(s); } catch(...) { return 0.0f; }
}

void show_text_viewer(const string& title, const string& text) { // pop-up παράθυρο

    // Δυναμικός υπολογισμός μεγέθους για να χωράει πάντα στο τερματικό
    int height=(LINES<20) ? LINES-2 : 20;
    int width=(COLS<90) ? COLS-2 : 90;

    int starty=(LINES-height) / 2;
    int startx=(COLS-width) / 2;

    vector<string> lines;
    stringstream ss(text);
    string line;
    while (getline(ss, line)) {
        lines.push_back(line);
    }
    // Αποφυγή διαίρεσης με το μηδέν 
    int max_lines_per_page=height-4;
    int total_pages=(lines.size() + max_lines_per_page-1) / max_lines_per_page;
    if (total_pages==0) total_pages=1;

    for (int page=0; page<total_pages; page++) {
        WINDOW* win=newwin(height, width, starty, startx);
	if (win==nullptr) break; // Αν η ncurses αποτύχει, σταματάμε
        box(win, 0, 0);
        mvwprintw(win, 0, 2, "%s Σελίδα %d/%d", title.c_str(), page+1, total_pages);
        
        int y=2;
        int start_idx=page * max_lines_per_page;
        int end_idx=start_idx + max_lines_per_page;
        if (end_idx > (int)lines.size()) {
            end_idx=(int)lines.size();
        }

        for (int i=start_idx; i<end_idx; i++) {
            mvwprintw(win, y++, 2, "%s", lines[i].c_str());
        }
        if (page < total_pages-1) {
            mvwprintw(win, height-2, 2, "Πιέστε enter για την επόμενη σελίδα");
        } else {
            mvwprintw(win, height-2, 2, "Τέλος κειμένου, πιέστε enter για έξοδο");
        }

        wrefresh(win);
        wgetch(win);
        delwin(win);
        clear();
        refresh();
    }
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
        "2. Εισαγωγή νέου φοιτητή",
        "3. Εισαγωγή νέου καθηγητή",
        "4. Δημιουργία μαθήματος",
        "5. Αναζήτηση μέλους με βάση το ΑΜ",
        "6. Αποστολή email σε καθηγητές",
        "7. Αποστολή email σε φοιτητές",
        "8. Αποθήκευση σε CSV αρχείο",
        "9. Φόρτωση από CSV αρχείο",
        "10. Προβολή όλων φοιτητών/καθηγητών",
        "11. Καταχώρηση βαθμολογίας",
        "12. Έξοδος",
    } ;
    int new_choices=sizeof(choices)/sizeof(string); // υπολογίζεται το πλήθος των επιλογών 

    WINDOW *menu_win=newwin(25, 70, starty, startx); // δημιουργείται παράθυρο
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
		case 10:
                choice=highlight;

                try { // σε περίπτωση σφάλματος να μην κρασάρει το πρόγραμμα
                    switch(choice) {
                        case 1:
                            {
                                stringstream ss;
                                Professor* prof1=new Professor("25390001", "Γιώργος Μελετίου", "Άρρεν", "Προγραμματισμός");
                                Student* s1=new Student("25390003", "Μαρία Παπαδοπούλου", "Θήλυ", 8);
                                Course* c1=new Course("CS101", "Aντικειμενοστραφής Προγραμματισμός", 2, prof1);
                                prof1->assignCourse(c1);
                                s1->enrollCourse(c1);
                                uni.addMember(prof1);
                                uni.addMember(s1);
                                uni.addCourse(c1);
                                ss << "Δημιουργία δοκιμαστικών δεδομένων \nΤα μέλη προστέθηκαν στο σύστημα";
                                show_text_viewer("Δοκιμαστικά δεδομένα", ss.str());
                            } break;
                        case 2:
                        {
                            int h=14, w=65;
                            WINDOW* form=newwin(h, w, (LINES-h)/2, (COLS-w)/2);
                            box(form, 0, 0);
                            mvwprintw(form, 0, 2, "Εισαγωγή νέου φοιτητή");

                            string id=get_string_ncurses(form, 2, 2, "Δώστε ΑΜ: ");
                            string name=get_string_ncurses(form, 4, 2, "Δώστε ονοματεπώνυμο: ");
                            string gender=get_string_ncurses(form, 6, 2, "Δώστε φύλο: ");
                            unsigned int semester=get_int_ncurses(form, 8, 2, "Δώστε εξάμηνο: ");

                            uni.addMember(new Student(id.c_str(), name, gender, semester));

                            mvwprintw(form, 11, 2, "Ο φοιτητής προστέθηκε επιττχώς");
                            mvwprintw(form, 12, 2, "Πιέστε ένα πλήκτρο για επιστροφή");
                            wrefresh(form);
                            wgetch(form);
                            delwin(form);
                            clear();
                            refresh();
                        } break;
                        case 3:
                        {
                            int h=14, w=65;
                            WINDOW* form=newwin(h, w, (LINES-h)/2, (COLS-w)/2);
                            box(form, 0, 0);
                            mvwprintw(form, 0, 2, "Εισαγωγή νέου καθηγητή");

                            string id=get_string_ncurses(form, 2, 2, "Δώστε ΑΜ: ");
                            string name=get_string_ncurses(form, 4, 2, "Δώστε ονοματεπώνυμο: ");
                            string gender=get_string_ncurses(form, 6, 2, "Δώστε φύλο: ");
                            string specialty=get_string_ncurses(form, 8, 2, "Δώστε ειδικότητα: ");

                            uni.addMember(new Professor(id.c_str(), name, gender, specialty));

                            mvwprintw(form, 11, 2, "Ο καθηγητής προστέθηκε επιττχώς");
                            mvwprintw(form, 12, 2, "Πιέστε ένα πλήκτρο για επιστροφή");
                            wrefresh(form);
                            wgetch(form);
                            delwin(form);
                            clear();
                            refresh();
                        } break;
                        case 4:
                        {
                            int h=14, w=65;
                            WINDOW* form=newwin(h, w, (LINES-h)/2, (COLS-w)/2);
                            box(form, 0, 0);
                            mvwprintw(form, 0, 2, "Εισαγωγή νέου μαθηματός");

                            string code=get_string_ncurses(form, 2, 2, "Δώστε κωβδικό μαθήματος: ");
                            string desc=get_string_ncurses(form, 4, 2, "Δώστε τίτλο/περιγραφή: ");
                            unsigned int semester=get_int_ncurses(form, 6, 2, "Δώστε εξάμηνο διδασκαλίας: ");
                            string profId=get_string_ncurses(form, 8, 2, "Δώστε ΑΜ υπεύθυνου καθηγητή: ");

                            Professor* assignedProf=nullptr;
                            if (profId!="NONE") {
                                Person* p=uni.findMember(profId.c_str());
                                assignedProf=dynamic_cast<Professor*>(p);
                            }

                            Course* newCourse=new Course(code, desc, semester, assignedProf);
                            if (assignedProf!=nullptr) {
                                assignedProf->assignCourse(newCourse);
                            }
                            uni.addCourse(newCourse);

                            mvwprintw(form, 11, 2, "To μάθημα προστέθηκε επιττχώς");
                            mvwprintw(form, 12, 2, "Πιέστε ένα πλήκτρο για επιστροφή");
                            wrefresh(form);
                            wgetch(form);
                            delwin(form);
                            clear();
                            refresh();
                        } break;
                        case 5:
                        {
                            int h=8, w=65;
                            WINDOW* form=newwin(h, w, (LINES-h)/2, (COLS-w)/2);
                            box(form, 0, 0);
                            mvwprintw(form, 0, 2, "Αναζήτηση μέλους");
                            string searchId=get_string_ncurses(form, 2, 2, "Δώστε ΑΜ μέλους: ");
                            delwin(form);
                            
                            Person* p=uni.findMember(searchId.c_str());
                            stringstream ss;
                            if (p!=nullptr) {
                                ss << "Το μέλος βρέθηκε.\nΣτοιχεία: \n";
                                if (Student* s=dynamic_cast<Student*>(p)) {
                                    ss << *s;
                                } else if (Professor* prof=dynamic_cast<Professor*>(p)) {
                                    ss << *prof;
                                }
                            } else {
                                ss << "Δεν βρέθηκε το μέλος με ΑΜ: " << searchId;
                            }
                            show_text_viewer("Αποτέλεσμα αναζήτησης: ", ss.str());
                        } break;
                        case 6: {
                            stringstream ss;
                            uni.sendEmailsToProfessors(ss);
                            show_text_viewer("Emails καθηγητών: ", ss.str());
                        
                        } break;
                        case 7: {
                            stringstream ss;
                            uni.sendEmailsToStudents(ss);
                            show_text_viewer("Emails φοιτητών: ", ss.str());
                        } break;
                        case 8:
                            uni.saveToCSV();
                            show_text_viewer("Aποθήκευση", "τα αρχεία δημιουργήθηκαν επιτυχώς");
                        break;
                        case 9: {
                            uni.loadFromCSV();

			    stringstream ss; // Δημιουργούμε ένα stream για να μαζέψουμε το κείμενο
			    ss << "Τα δεδομένα φορτώθηκαν επιτυχώς από τα CSV αρχεία.\n";
			    uni.printAllMembers(ss);
                            show_text_viewer("Φόρτωση", ss.str());
                        } break;
                        case 10: {
                            stringstream ss;
                            uni.printAllMembers(ss);
                            show_text_viewer("Προβολή μελών", ss.str());
                        } break;
                        case 11: {
                            int h=12, w=65;
                            WINDOW* form=newwin(h, w, (LINES-h)/2, (COLS-w)/2);
                            box(form, 0, 0);
                            mvwprintw(form, 0, 2, "Καταχώρηση βαθμολογίας");

                            string studentId=get_string_ncurses(form, 2, 2, "Δώστε ΑΜ φοιτητή: ");
                            Person* p=uni.findMember(studentId.c_str());
                            Student* studentPtr=dynamic_cast<Student*>(p);

                            if (studentPtr==nullptr) {
                                mvwprintw(form, 9, 2, "Ο φοιτητής δεν βρέθηκε");
                                wrefresh(form);
                                wgetch(form);
                                delwin(form);
                                clear();
                                refresh();
                                break;
                            }
                            
                            string courseCode=get_string_ncurses(form, 4, 2, "Δώστε κωδικό μαθήματος");
                            Course* coursePtr=uni.findCourse(courseCode);

                            if (coursePtr==nullptr) {
                                mvwprintw(form, 9, 2, "Το μάθημα δεν βρέθηκε");
                                wrefresh(form);
                                wgetch(form);
                                delwin(form);
                                clear();
                                refresh();
                                break;
                            }

                            float grade=get_float_ncurses(form, 6, 2, "Δώστε βαθμό (0.0-10.0): ");
                            try {
                                bool isEnrolled=false;
                                for (const auto& r : studentPtr->getEnrolledCourses()) {
                                    if (r.getCourse()==coursePtr) { isEnrolled=true; break; }
                                }
                                if (!isEnrolled) {
                                    studentPtr->enrollCourse(coursePtr);
                                }
                                studentPtr->assignGrade(coursePtr, grade);
                                mvwprintw(form, 9, 2, "Η βαθμολογία καταχωρήθηκε επιττχώς");
                            } catch (const exception& e) {
                                mvwprintw(form, 9, 2, "Σφάλμα: %s", e.what());
                            }

                            wrefresh(form);
                            wgetch(form);
                            delwin(form);
                            clear();
                            refresh();
                        } break;
                        case 12:
                            show_text_viewer("Έξοδος", "κλείσιμο εφαρμογής");
                            running=false;
                            break;
                    }
                } catch (const exception& e) {
                    stringstream ss;
                    ss << "Υπήρξε σφάλμα\n" << e.what();
                    show_text_viewer("Σφάλμα", ss.str());
                }
            
                break;
            default:
                break;
        }
    }

    endwin();
    return 0;
}