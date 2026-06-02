#ifndef ADMIN_STAFF_HPP
#define ADMIN_STAFF_HPP

#include "Person.hpp"
#include <string>
#include <iostream>

using namespace std;

class AdminStaff : public Person
{
    private:
    string officeNumber;
    string role;

    public:
    AdminStaff(const char* staffId, const string& staffName, const string& staffGender, const string& staffOffice, const string& staffRole) ;
    virtual ~AdminStaff();

    string getOfficeNumber() const;
    void setOfficeNumber(const string& newOffice);
    string getRole() const;
    void setRole(const string& newRole);
} ;

ostream& operator<<(ostream& os, const AdminStaff& staff);
#endif