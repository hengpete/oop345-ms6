#include <iostream>
#include <cstring>
#include "Error.h"

namespace ict {
  Error::Error() {
    m_message = nullptr;
  }
  Error::Error(const char* errorMessage) {
    m_message = nullptr;
    message(errorMessage);
  }
  void Error::operator=(const char* errorMessage) {
    if (errorMessage != nullptr) {
      if (m_message != nullptr)
        delete[] m_message;
      m_message = new char[strlen(errorMessage) + 1];
      clear();
      message(errorMessage);
    }
    else
      m_message = nullptr;
  }
  Error::~Error() {
    if (m_message != nullptr)
      delete[] m_message;
  }
  void Error::clear() {
    if (m_message != nullptr) {
      delete[] m_message;
      m_message = nullptr;
    }
  }
  bool Error::isClear() const {
    return m_message == nullptr;
  }
  void Error::message(const char* value) {
    if (value != nullptr) {
      delete[] m_message;
      m_message = new char[strlen(value) + 1];
      strcpy(m_message, value);
    }
    else
      m_message = nullptr;
  }
  Error::operator const char*() const {
    return m_message;
  }
  Error::operator bool()const {
    return m_message == nullptr;
  }
  std::ostream& operator << (std::ostream& ostr, const Error& E) {
    return (E.isClear()) ? ostr : ostr << (const char*)E;
  }
}
