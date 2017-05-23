#ifndef ICT_ITEM_H__
#define ICT_ITEM_H__
// include PosIO and POS header files
#include "POS.h"
#include "PosIO.h"

namespace ict{
  // class Item
  class Item : public PosIO
  {
  public:
    Item();
    Item(const char* sku, const char* name, double price, bool taxed = true);
    Item(const Item& I);
    Item& operator=(Item& I);
    virtual ~Item();
    void sku(const char* sku);
    void price(double price);
    void name(const char* name);
    void taxed(bool taxed);
    void quantity(int quantity);
    const char* sku() const;
    double price() const;
    const char* name() const;
    bool taxed() const;
    int quantity() const;
    double cost() const;
    bool isEmpty() const;
    bool operator==(const char* sku) const;
    int operator+=(int quantity);
    int operator-=(int quantity);

  private:
    char m_sku[MAX_SKU_LEN+1];
    char* m_name;
    double m_price;
    bool m_taxed;
    int m_quantity;
  };

  // end class Item
  // operator +=
  double operator+=(double& d, const Item& I);
  // operator << and operator >>
  std::ostream& operator << (std::ostream& ostr, const Item& I);
  std::istream& operator >> (std::istream& istr, Item& I);
}


#endif