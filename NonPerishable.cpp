// lib includes
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <string>
#include "NonPerishable.h"

using namespace std;

namespace ict {
  bool NonPerishable::ok() const {
    return m_err.isClear();
  }

  void NonPerishable::error(const char* message) {
    m_err.message(message);
  }

  char NonPerishable::signature() const {
    return 'N';
  }

  std::fstream & NonPerishable::save(std::fstream & file) const {
    // TODO: insert return statement here
    file << signature() << "," << sku() << "," << name() << "," << price() << "," << taxed() << "," << quantity();// << endl;
    return file;
  }// end of save function

  std::fstream & NonPerishable::load(std::fstream & file) {
    // TODO: insert return statement here
    double price;
    int quantity, taxed;
    char name[256], sku[MAX_SKU_LEN];

    file.getline(sku, MAX_SKU_LEN, ',');//',' extracted but discarded
    file.getline(name, 256, ',');
    file >> price;
    file.ignore(); // ignores next character
    file >> taxed;
    file.ignore();
    file >> quantity;

    // call setters
    this->sku(sku);
    this->price(price);
    this->name(name);
    (taxed == 1 ) ? this->taxed(true) : this->taxed(false);
    this->quantity(quantity);
    return file;
  }// end of load function

  std::ostream & NonPerishable::write(std::ostream & ostr, bool linear) const {
    // TODO: insert return statement here
    char sName[21];
    if (!ok())
      ostr << m_err;
    else if (linear) {
      ostr << left << setfill(' ') << setw(MAX_SKU_LEN) << sku() << "|";
      if (strlen(name()) > 19) {
        strncpy(sName, name(), 20);
        sName[20] = '\0';
        ostr << left << setw(20) << setfill(' ') << sName << "|";
      }
      else
        ostr << left << setw(20) << setfill(' ') << name() << "|";
      ostr << right << setw(7) << setfill (' ')<< fixed << setprecision(2) << price() << "|" << " ";
      (taxed()) ? ostr << 'T': ostr << " " ;
      ostr << signature() << "|";
      ostr << right << setw(4) <<setfill(' ') << quantity() << "|";
      ostr << right << setw(9) << setfill(' ') << fixed << setprecision(2) << cost()*quantity() << "|";
    }
    else {
      if (strlen(name()) > 20) {
        strncpy(sName, name(), 20);
        ostr << "Name:" << endl << left << sName << endl;
      }
      else
        ostr << "Name:" << endl << left << name() << endl;
      ostr << "Sku: " << left << sku() << endl;
      ostr << "Price: " << fixed << setprecision(2) << price() << endl;
      ostr << "Price after tax: ";
      (taxed()) ? ostr << cost() : ostr << "N/A";
      ostr << endl;
      ostr << "Quantity: " << quantity() << endl;
      ostr << "Total Cost: " << fixed << setprecision(2) << cost()*quantity() << endl;
    }
    return ostr;
  }// end of write function (Displays the item in one of two forms)

  std::istream & NonPerishable::read(std::istream & is) {
    //TODO: insert return statement here
    double price;
    int quantity;
    char taxed, name[256], sku[MAX_SKU_LEN];

    if (!is.fail()) {
      m_err.clear();    //ensures no preliminary error messages
      cout << "Item Entry:" << endl;
      cout << "Sku: ";
      is.ignore();
      //is >> sku;
      is.getline(sku, MAX_SKU_LEN);
      if (!is.fail()) {
        cout << "Name:" << endl;
        //is >> name;
        is.getline(name, 256);
        if (!is.fail()) {
          cout << "Price: ";
          is >> price;
          if (is.fail())
            m_err.message("Invalid Price Entry");
          else {
            cout << "Taxed: ";
            is >> taxed;
            if (is.fail() || !(taxed == 'Y' || taxed == 'y' || taxed == 'N' || taxed == 'n')) {
              m_err.message("Invalid Taxed Entry, (y)es or (n)o");
              is.setstate(ios::failbit);
            }
            else {
              cout << "Quantity: ";
              is >> quantity;
              if (is.fail())
                m_err.message("Invalid Quantity Entry");
            }//taxed not fail
          }// price not fail
        }// name not fail
      }// sku not fail
    }// first not fail
    if (m_err.isClear()) { // checks if there was error message
      this->sku(sku);
      this->price(price);
      this->name(name);
      if (taxed == 'Y' || taxed == 'y')
        this->taxed(true);
      if (taxed == 'N' || taxed == 'n')
        this->taxed(false);
      this->quantity(quantity);
    }
    return is;
  }// end of read function

}
