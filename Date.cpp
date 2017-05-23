#include <iomanip>
#include <iostream>
#include <ctime>
using namespace std;
#include "Date.h"
#include "POS.h"


namespace ict {
  void Date::set() {
    time_t t = time(NULL);
    tm lt = *localtime(&t);
    m_day = lt.tm_mday;
    m_mon = lt.tm_mon + 1;
    m_year = lt.tm_year + 1900;
    if (dateOnly()) {
      m_hour = m_min = 0;
    }
    else {
      m_hour = lt.tm_hour;
      m_min = lt.tm_min;
    }
  }

  int Date::value()const {
    return m_year * 535680 + m_mon * 44640 + m_day * 1440 + m_hour * 60 + m_min;
  }

  int Date::mdays()const {
    int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, -1 };
    int mon = m_mon >= 1 && m_mon <= 12 ? m_mon : 13;
    mon--;
    return days[mon] +
      int((mon == 1)*((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0)); // 0 or 1
  }

  void Date::errCode(int errorCode) {
    if (errorCode >= 0 && errorCode <= 6)
      m_readErrorCode = errorCode;
  }

  void Date::set(int year, int mon, int day, int hour, int min) {
    m_year = year;
    m_mon = mon;
    m_day = day;
    m_hour = hour;
    m_min = min;
    m_readErrorCode = NO_ERROR;
  }
  Date::Date() {
    m_dateOnly = false;
    set();
  }
  Date::Date(int year, int mon, int day) {
    m_dateOnly = true;
    set(year, mon, day, 0, 0);
  }
  Date::Date(int year, int mon, int day, int hour, int min) {
    m_dateOnly = false;
    set(year, mon, day, hour, min);
  }
  bool Date::operator==(const Date& D)const {
    return this->value() == D.value();
  }
  bool Date::operator!=(const Date& D)const {
    return this->value() != D.value();
  }
  bool Date::operator<(const Date& D)const {
    return this->value() < D.value();
  }
  bool Date::operator>(const Date& D)const {
    return this->value() > D.value();
  }
  bool Date::operator<=(const Date& D)const {
    return this->value() <= D.value();
  }
  bool Date::operator>=(const Date& D)const {
    return this->value() >= D.value();
  }
  int Date::errCode() const {
    return m_readErrorCode;
  }
  void Date::errCodeInsert(int year, int mon, int day, int hour, int min) {
    if (year < MIN_YEAR || year > MAX_YEAR) errCode(YEAR_ERROR);
    else if (mon < MIN_MONTH || mon > MAX_MONTH) errCode(MON_ERROR);
    else if (day < MIN_DAY || day > mdays()) errCode(DAY_ERROR);
    else if (hour < MIN_HOUR || hour > MAX_HOUR) errCode(HOUR_ERROR);
    else if (min < MIN_MIN || min > MAX_MIN) errCode(MIN_ERROR);
    else errCode(NO_ERROR);
  }
  bool Date::bad() const {
    return (m_readErrorCode != 0);
  }
  bool Date::dateOnly() const {
    return m_dateOnly;
  }
  void Date::dateOnly(bool value) {
    m_dateOnly = value;
    if (value)
      m_hour = 0, m_min = 0;
  }
  std::istream& Date::read(std::istream& is) {
    errCode(NO_ERROR);
    is >> m_year;
    if (is.get() != '/') errCode(CIN_FAILED);
    is >> m_mon;
    if (is.get() != '/') errCode(CIN_FAILED);
    is >> m_day;
    if (!m_dateOnly) {
      if (is.get() != ',') errCode(CIN_FAILED);
      if (is.get() != ' ') errCode(CIN_FAILED);
      is >> m_hour;
      if (is.get() != ':') errCode(CIN_FAILED);
      is >> m_min;
    }
    if (is.fail() || m_readErrorCode == CIN_FAILED)
      errCode(CIN_FAILED);
    else
      errCodeInsert(m_year, m_mon, m_day, m_hour, m_min);
    return is;
  }
  ostream& Date::write(ostream& ostr) const {
    if (m_dateOnly)
      ostr << m_year << "/" << setfill('0') << setw(2) << right << m_mon << "/" << setfill('0') << right << setw(2) << m_day;
    else
      ostr << m_year << "/" << setfill('0') << setw(2) << right << m_mon << "/" << setfill('0') << right << setw(2) << m_day << ", " << m_hour << ":" << m_min;
    return ostr;
  }

  std::ostream& operator << (std::ostream& ostr, const Date& D) {
    return D.write(ostr);
  }
  std::istream& operator >> (std::istream& istr, Date& D) {
    return D.read(istr);
  }
}
