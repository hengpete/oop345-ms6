// Lib includes
#include <iostream>
#include <cstring>
// inlcude Item and POS header files
#include "Item.h"
#include "POS.h"
#include "PosIO.h"
using namespace std;
namespace ict {
  // class Item implementaion
  Item::Item() {
    m_sku[0] = '\0';
    m_name = nullptr;
    m_price = 0;
    m_taxed = true;
    m_quantity = 0;
  }
  Item::Item(const char* sku, const char* name, double price, bool taxed) {
    this->sku(sku);
    this->name(name);
    quantity(0);
    this->price(price);
    this->taxed(taxed);
  }
  Item::Item(const Item& I) {
    sku(I.m_sku);
    name(I.m_name);
    quantity(I.m_quantity);
    price(I.m_price);
    taxed(I.m_taxed);
  }
  Item::~Item() {
    if (m_name != nullptr) delete[] m_name;
  }
  Item& Item::operator=(Item& I) {
    if (this != &I && I.isEmpty() == false) {
      sku(I.m_sku);
      name(I.m_name);
      quantity(I.m_quantity);
      price(I.m_price);
      taxed(I.m_taxed);
    }
    return *this;
  }
  void Item::sku(const char* sku) {
    m_sku[0] = '\0';
    if (strlen(sku) > MAX_SKU_LEN && sku != nullptr) {
      strncpy(this->m_sku, sku, MAX_SKU_LEN);
      this->m_sku[MAX_SKU_LEN] = '\0';
    }
    else if (sku != nullptr)
      strcpy(this->m_sku, sku);
    else
      this->m_sku[0] = '\0';
  }
  void Item::price(double price) {
    this->m_price = price;
  }
  void Item::name(const char* name) {
    if (name != nullptr) {
      if (m_name != nullptr)
        delete[] m_name;
      this->m_name = new char[strlen(name) + 1];
      strcpy(this->m_name, name);
    }
    else
      this->m_name = nullptr;
  }
  void Item::taxed(bool taxed) {
    this->m_taxed = taxed;
  }
  void Item::quantity(int quantity) {
    this->m_quantity = quantity;
  }
  const char* Item::sku() const {
    return m_sku;
  }
  double Item::price() const {
    return m_price;
  }
  const char* Item::name() const {
    return m_name;
  }
  bool Item::taxed() const {
    return m_taxed;
  }
  int Item::quantity() const {
    return m_quantity;
  }
  double Item::cost() const {
    return (m_taxed) ? m_price * (1 + TAX) : m_price;
  }
  bool Item::isEmpty() const {
    return (m_sku[0] == '\0'
      && m_name == nullptr
      && m_price == 0
      && m_taxed == false
      && m_quantity == 0);
  }
  bool Item::operator==(const char *sku) const {
    return (strcmp(m_sku, sku) == 0) ? true : false;
  }
  int Item::operator+=(int quantity) {
    return m_quantity += quantity;
  }
  int Item::operator-=(int quantity) {
    return m_quantity -= quantity;
  }
  double operator+=(double& d, const Item& I) {
    d += I.cost() * I.quantity();
    return d;
  }
  std::ostream& operator << (std::ostream& ostr, const Item& I) {
    return I.write(ostr, true); //return I.write(ostr, true);
  }
  std::istream& operator >> (std::istream& istr, Item& I) {
    return I.read(istr);
  }

}