#include "AdminStaff.hpp"
using namespace std;

AdminStaff::AdminStaff(const char* staffId, const string& staffName, const string& staffGender, const string& staffOffice, const string& staffRole)
    : Person(staffId, staffName, staffGender), officeNumber(staffOffice), role(staffRole) {}

AdminStaff::~AdminStaff() {}

string AdminStaff::getOfficeNumber() const 
{
    return officeNumber;
}

void AdminStaff::setOfficeNumber(const string& newOffice)
{
    officeNumber=newOffice;
}

string AdminStaff::getRole() const
{
    return role;
}

void AdminStaff::setRole(const string& newRole)
{
    role=newRole;
}

ostream& operator<<(ostream& os, const AdminStaff& staff) {

    os << "AM: " << staff.getId() 
    << "| Ονοματεπώνυμο: " << staff.getName()  
    << "| Φύλο: " << staff.getGender() 
    << "| Γραφείο: " << staff.getOfficeNumber() 
    << "| Ρόλος: " << staff.getRole();
    
    return os;
}