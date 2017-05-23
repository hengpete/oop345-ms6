// compilation safegaurds
#ifndef ICT_Date_H_
#define ICT_Date_H_

#include <iostream>
// ict namespace 
namespace ict {
  // Date defined Error values
  const int NO_ERROR = 0;    //No error - the date is valid
  const int CIN_FAILED = 1;  //istream failed on accepting information using cin
  const int YEAR_ERROR = 2;  //Year value is invalid
  const int MON_ERROR = 3;   //Month value is invalid
  const int DAY_ERROR = 4;   //Day value is invalid
  const int HOUR_ERROR = 5;  //Hour value is invalid
  const int MIN_ERROR = 6;   //Minute value is invalid

  class Date {
  private:
    // member variables
    int m_year;  //between MIN_YEAR and MAX_YEAR
    int m_mon;   //Between 1 and 12
    int m_day;   //mday(); Feb has 29 in leap year
    int m_hour;  //between 0 and 23
    int m_min;   //between 0 and 59
    int m_readErrorCode;   //error code
    bool m_dateOnly;    // flag: true if object holds only date, not time => h = 0 and min = 0.

    // private methods
    int value()const;
    void errCode(int errorCode);
    void set(int year, int mon, int day, int hour, int min);

  public:
    // constructors
    Date();   //sets current date and time m_dateOnly = False
    Date(int year, int mon, int day);
    Date(int year, int mon, int day, int hour, int min = 0);
    void set();
    // operator overloads
    bool operator==(const Date& D)const;
    bool operator!=(const Date& D)const;
    bool operator<(const Date& D)const;
    bool operator>(const Date& D)const;
    bool operator<=(const Date& D)const;
    bool operator>=(const Date& D)const;
    // methods
    int errCode() const;
    void errCodeInsert(int year, int mon, int day, int hour, int min);
    bool bad() const;
    bool dateOnly() const;
    void dateOnly(bool value);
    int mdays()const;

    // istream  and ostream read and write methods
    std::istream& read(std::istream& istr = std::cin);
    std::ostream& write(std::ostream& ostr = std::cout) const;
  };
  // operator<< and >> overload prototypes for cout and cin
  std::ostream& operator << (std::ostream& ostr, const Date& D);
  std::istream& operator >> (std::istream& istr, Date& D);
}
#endif
