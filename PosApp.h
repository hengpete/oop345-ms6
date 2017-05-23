#ifndef ICT_POSAPP_H_
#define ICT_POSAPP_H_
#include <fstream>
#include <istream>
#include <ostream>
#include <iostream>
#include "Item.h"
#include "Date.h"
#include "Error.h"
#include "NonPerishable.h"
#include "Perishable.h"
#include "POS.h"
#include "PosIO.h"

namespace ict {
  const int LIST_ITEMS = 1;
  const int ADD_PERISHABLE = 2;
  const int ADD_NONPERISHABLE = 3;
  const int UPDATE_ITEM = 4;
  const int SHOW_ITEM = 5;
  const int POS_MENU = 6;
  const int SORT = 7;
  const int EXIT_PROGRAM = 0;

  const int SKU_SORT = 1;
  const int ITEM_NAME_SORT = 2;
  const int PRICE_SORT = 3;
  const int QUANTITY_SORT = 4;
  const int EXIT_SORT = 0;

  class PosApp {
    //data members
    char m_filename[128];
    char m_billfname[128];
    Item* m_items[MAX_NO_ITEMS];
    int m_noOfItems;
    std::fstream fs;
    //member functions
    int menu();
    // MS6 SORT
    int sortMenu();
    void runSort();
    void sortSku();
    void sortName();
    void sortQuantity();
    void sortPrice();
    //Data Management member functions
    void deallocateItems();
    void loadRecs();
    void saveRecs();
    int searchItems(const char* sku) const;
    void updateQty();
    void addItem(bool isPerishable);
    void listItems() const;
    //Point of Sale member functions
    void truncateBillFile();
    void showBill();
    void addToBill(Item& I);
    void POS();
  public:
    //constructor
    PosApp(const char* filename, const char* billfname);
    //Forbidding Copying and assignment
    PosApp(const PosApp& P) = delete;
    PosApp& operator=(const PosApp& P) = delete;
    //member function
    void run();
  };
}
#endif // !ICT_POSAPP_H_

