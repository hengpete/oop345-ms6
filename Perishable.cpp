// lib includes
#include <iostream>
#include <fstream>
#include "Perishable.h"
#include "PosIO.h"

using namespace std;
namespace ict {
  char Perishable::signature() const {
    return 'P';
  }
  Perishable::Perishable() : NonPerishable() {
    m_expiry.dateOnly(true);
  }
  std::fstream & Perishable::save(std::fstream & file) const {
    // TODO: insert return statement here
    NonPerishable::save(file);
    file << ",";
    file << m_expiry;
    return file;
  }
  std::fstream & Perishable::load(std::fstream & file) {
    // TODO: insert return statement here
    NonPerishable::load(file);
    file.ignore();
    file >> m_expiry;
    return file;
  }
  std::ostream & Perishable::write(std::ostream & os, bool linear) const {
    // TODO: insert return statement here
    NonPerishable::write(os, linear);
    if (ok() && linear == false)
      os << "Expiry date: " << m_expiry << endl;
    return os;
  }
  std::istream & Perishable::read(std::istream & istr) {
    // TODO: insert return statement here
    cout << "Perishable ";
    NonPerishable::read(istr);
    if (NonPerishable::ok()) {
      cout << "Expiry date (YYYY/MM/DD): ";
      istr >> m_expiry;
      if (m_expiry.errCode() != NO_ERROR/*!istr.fail()*/) {
        switch (m_expiry.errCode()) {
        case CIN_FAILED:
          error("Invalid Date Entry");
          break;
        case YEAR_ERROR:
          error("Invalid Year in Date Entry");
          break;
        case MON_ERROR:
          error("Invalid Month in Date Entry");
          break;
        case DAY_ERROR:
          error("Invalid Day in Date Entry");
          break;
        }// end of switch
        istr.setstate(ios::failbit);
      }
      else {
        NonPerishable::ok();
        istr.clear();
      }// end of expiry fail
    }// end of NonPerisable error
    return istr;
  }
}
