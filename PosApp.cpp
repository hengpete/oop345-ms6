#include <ostream>
#include <cstring>
#include <iomanip>
#include <ctime>
#include <istream>
#include <ostream>
#include <fstream>
#include <typeinfo>

#include "PosApp.h"
#include "Item.h"
using namespace std;

namespace ict {
  PosApp::PosApp(const char * filename, const char * billfname) {
    strncpy(m_filename, filename, 127);
    m_filename[127] = '\0';
    strncpy(m_billfname, billfname, 127);
    m_billfname[127] = '\0';
    loadRecs();
  }

  void PosApp::run() {
    int selection, index;
    bool ON = true;
    char sku[MAX_SKU_LEN];
    do {
      selection = menu();
      switch (selection) {
      case LIST_ITEMS:
        listItems();
        break;
      case ADD_PERISHABLE:
        addItem(true);
        break;
      case ADD_NONPERISHABLE:
        addItem(false);
        break;
      case UPDATE_ITEM:
        updateQty();
        break;
      case SHOW_ITEM:
        cout << "Please enter the SKU: ";
        cin >> sku;
        index = searchItems(sku);
        if (index == -1)
          cout << "Not found!" << endl << endl;
        else {
          cout << "v-----------------------------------v" << endl;
          m_items[index]->write(cout, false);
          cout << "^-----------------------------------^" << endl << endl;
        }
        break;
      case POS_MENU:
        POS();
        break;
      case SORT:
        runSort();
        break;
      case EXIT_PROGRAM:
        cout << "Goodbye!" << endl;
        ON = false;
        break;
      default:
        cout << "===Invalid Selection, try again===" << endl << endl;
        break;
      }// end of switch
    } while (ON);
    return;
  }

  int PosApp::menu() {
    int selection;
    cout << "The Sene-Store" << endl
      << "1- List items" << endl
      << "2- Add Perishable item" << endl
      << "3- Add Non-Perishable item" << endl
      << "4- Update item quantity" << endl
      << "5- Show Item" << endl
      << "6- POS" << endl
      << "7- Sort" << endl
      << "0- exit program" << endl
      << "> ";
    cin >> selection;                         //need to validate for integer entry
    if (cin.fail() || cin.peek() != '\n') {
      cin.clear();
      cin.ignore(256, '\n');
      selection = -1;
    }
    cout << endl;
    return (selection >= EXIT_PROGRAM && selection <= SORT) ? selection : -1;
  }

  int PosApp::sortMenu() {
    int selection;
    cout << "Sort on:" << endl
      << "1- Sku" << endl
      << "2- Item Name" << endl
      << "3- Price" << endl
      << "4- Quantity" << endl
      << "0- exit sort" << endl
      << "> ";
    cin >> selection;                         //need to validate for integer entry
    if (cin.fail() || cin.peek() != '\n') {
      cin.clear();
      cin.ignore(256, '\n');
      selection = -1;
    }
    cout << endl;
    return (selection >= EXIT_SORT && selection <= QUANTITY_SORT) ? selection : -1;
  }

  void PosApp::runSort() {
    int selection;
    selection = sortMenu();
    switch (selection) {
    case SKU_SORT:
      sortSku();
      break;
    case ITEM_NAME_SORT:
      sortName();
      break;
    case PRICE_SORT:
      sortPrice();
      break;
    case QUANTITY_SORT:
      sortQuantity();
      break;
    case EXIT_SORT:
      cout << "Aborted!!" << endl << endl;
      break;
    default:
      cout << "===Invalid Selection, try again===" << endl << endl;
      break;
    }// end of switch
    return;
  }
  void PosApp::sortQuantity() {
    char ascending;
    int i, j;
    Item* temp = nullptr;
    cout << "Ascending? (y/n) ";
    cin >> ascending;
    for (i = 0; i < (m_noOfItems - 1); i++) {
      for (j = 0; j < (m_noOfItems - i - 1); j++) {
        if (ascending == 'y' || ascending == 'Y') {
          if (m_items[j]->quantity() > m_items[j + 1]->quantity()) {
            if (typeid(*m_items[j]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }
        }
        else {
          if (m_items[j]->quantity() < m_items[j + 1]->quantity()) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }
        }
      }// end of loop j
    }// end of loop i
    cout << endl;
    listItems();
  }// end of sort quantity

  void PosApp::sortName() {
    char ascending;
    int i, j;
    Item* temp = nullptr;
    cout << "Ascending? (y/n) ";
    cin >> ascending;
    for (i = m_noOfItems - 1; i > 0; i--) { // decending
      for (j = 0; j < i; j++) {
        if (ascending == 'N' || ascending == 'n') {
          if (strcmp(m_items[j]->name(), m_items[j + 1]->name()) < 0) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }// end of if
        }
        else {
          if (strcmp(m_items[j]->name(), m_items[j + 1]->name()) > 0) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }
        }
      }// end of loop j
    }// end of loop i
    cout << endl;
    listItems();
  }
  void PosApp::sortPrice() {
    char ascending;
    int i, j;
    Item* temp = nullptr;
    cout << "Ascending? (y/n) ";
    cin >> ascending;
    for (i = 0; i < (m_noOfItems - 1); i++) {
      for (j = 0; j < (m_noOfItems - i - 1); j++) {
        if (ascending == 'y' || ascending == 'Y') {
          if (m_items[j]->price() > m_items[j + 1]->price()) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }
        }
        else {
          if (m_items[j]->price() < m_items[j + 1]->price()) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }
        }
      }// end of loop j
    }// end of loop i
    cout << endl;
    listItems();
  }// end of sort price

  void PosApp::sortSku() {
    char ascending;
    int i, j;
    Item* temp = nullptr;
    cout << "Ascending? (y/n) ";
    cin >> ascending;
    for (i = m_noOfItems - 1; i > 0; i--) { // decending
      for (j = 0; j < i; j++) {
        if (ascending == 'N' || ascending == 'n') {
          if (strcmp(m_items[j]->sku(), m_items[j + 1]->sku()) < 0) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }// end of if
        }
        else {
          if (strcmp(m_items[j]->sku(), m_items[j + 1]->sku()) > 0) {
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              temp = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              temp = new NonPerishable;
            *temp = *m_items[j];
            delete m_items[j];
            if (typeid(*m_items[j + 1]).name() == typeid(Perishable).name())
              m_items[j] = new Perishable;
            if (typeid(*m_items[j + 1]).name() == typeid(NonPerishable).name())
              m_items[j] = new NonPerishable;
            *m_items[j] = *m_items[j + 1];
            delete m_items[j + 1];
            if (typeid(*temp).name() == typeid(Perishable).name())
              m_items[j + 1] = new Perishable;
            if (typeid(*temp).name() == typeid(NonPerishable).name())
              m_items[j + 1] = new NonPerishable;
            *m_items[j + 1] = *temp;
            delete temp;
          }
        }
      }
    }
    cout << endl;
    listItems();
  }

  void PosApp::deallocateItems() {
    int i;
    for (i = 0; i < m_noOfItems; i++) {
      delete m_items[i];
      m_items[i] = nullptr;
    }
    m_noOfItems = 0;
  }

  void PosApp::loadRecs() {
    int i = 0;
    char itemType;
    deallocateItems();//Deletes all the dynamically allocated Items pointed by the m_items array.
    fs.open(m_filename, ios::in); //opens file for reading
    if (fs.fail()) {            //file failed to open?
      fs.clear();               //Clears all failures
      fs.close();               //close the file
      fs.open(m_filename, ios::out); //Creates a new file with name m_filename; opens file for writing if file already exist
    }
    else {
      while (true) {
        fs >> itemType;
        if (fs.fail()) {
          break;
        }
        fs.ignore(); // ignores ',' after the signature
        if (itemType == 'P')
          m_items[m_noOfItems] = new Perishable;
        else if (itemType == 'N')
          m_items[m_noOfItems] = new NonPerishable;
        m_items[m_noOfItems]->load(fs); // will call load() based on class assigned
        fs.ignore();
        i++;
        m_noOfItems = i;
      }
      fs.clear();
      fs.close();
      return;
    }
  }
  void PosApp::saveRecs() {
    int i;
    fs.open(m_filename, ios::out); //opens file in write mode
    for (i = 0; i < m_noOfItems; i++) {
      if (m_items[i]->quantity() > 0) {
        m_items[i]->save(fs);
        fs << endl;
      }
    }
    fs.clear();
    fs.close();
    loadRecs();
  }

  int PosApp::searchItems(const char * sku) const {
    int i, index = -1;
    for (i = 0; i < m_noOfItems; i++) {
      if (*m_items[i] == sku) {
        index = i;
        break;
      }
    }
    return (index >= 0 && index <= m_noOfItems) ? index : -1;
  }

  void PosApp::updateQty() {
    char sku[MAX_SKU_LEN];
    int quantity;
    cout << "Please enter the SKU: ";
    cin >> sku;
    if (searchItems(sku) == -1)
      cout << "Not found!" << endl << endl;
    else {
      m_items[searchItems(sku)]->write(cout, false);
      cout << endl << "Please enter the number of purchased items: ";
      cin >> quantity;
      if (cin.fail() || cin.peek() != '\n') {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "Invalid Quantity value!" << endl << endl;
      }
      else {
        *m_items[searchItems(sku)] += quantity;
        saveRecs();
        cout << "Updated!" << endl << endl;
      }
    }
    return;
  }

  void PosApp::addItem(bool isPerishable) {
    if (isPerishable)
      m_items[m_noOfItems] = new Perishable;
    else
      m_items[m_noOfItems] = new NonPerishable;
    m_items[m_noOfItems]->read(cin);
    if (cin.fail()) {
      cin.clear();
      cin.ignore(2000, '\n'); //clears a line
      m_items[m_noOfItems]->write(cout, false); // print error message; with error code ok() is false
      cout << endl;
    }
    else {
      m_noOfItems++;
      saveRecs();
      cout << "Item added." << endl;
    }
    cout << endl;
  }

  void PosApp::listItems() const {
    int i;
    double totalAsset = 0;
    cout << " Row | SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
    cout << "-----|--------|--------------------|-------|---|----|---------|" << endl;
    for (i = 0; i < m_noOfItems; i++) {
      cout << setw(4) << right << setfill(' ') << i + 1 << " | ";
      m_items[i]->write(cout, true);
      totalAsset += *m_items[i];
      cout << endl;
    }
    cout << "-----^--------^--------------------^-------^---^----^---------^" << endl;
    cout << "                               Total Asset: $  |" << setw(14) << right << fixed << setprecision(2) << totalAsset << "|" << endl;
    cout << "-----------------------------------------------^--------------^" << endl << endl;
  }

  void PosApp::truncateBillFile() {
    fs.open(m_billfname, ios::out | ios::trunc);
    fs.close();
  }


  void PosApp::showBill() {
    char itemType, buffer[80];
    Item* temp[MAX_NO_ITEMS];
    int i = 0, noOfItems;
    double total = 0;
    time_t t = time(0);
    fstream fp;
    fs.open(m_billfname, ios::in); // opens file for reading
    if (fs.fail()) {            //file failed to open?
      fs.clear();               //Clears all failures
      fs.close();               //close the file
    }
    else {
      cout << "v--------------------------------------------------------v" << endl;
      strftime(buffer, 80, "%Y/%m/%d, %R", localtime(&t));
      cout << "| " << buffer << "                                      |" << endl;
      cout << "| SKU    | Item Name          | Price |TX |Qty |   Total |" << endl;
      cout << "|--------|--------------------|-------|---|----|---------|" << endl;
      while (fs >> itemType) {
        fs.ignore();
        if (itemType == 'P') //Select P or N
          temp[i] = new Perishable;
        else if (itemType == 'N')
          temp[i] = new NonPerishable;
        temp[i]->load(fs);   //loads data from file into temporary object of type P or N
        total += *temp[i];
        cout << "| ";
        temp[i]->write(cout, true);//Prints in linear from temp object
        cout << endl;
        temp[i]->quantity(0);//Delete printed item/line; set quantity to 0 and to remove with save();
        i++;
      }
      cout << "^--------^--------------------^-------^---^----^---------^" << endl;
      cout << "|                               Total: $  |" << setw(14) << right << fixed << setprecision(2) << total << "|" << endl;
      cout << "^-----------------------------------------^--------------^" << endl << endl;
      noOfItems = i;  // records total number of items
      fp.open("copy.txt", ios::out);  //Create a temporary file
      for (i = 0; i < noOfItems; i++) {
        if (temp[i]->quantity() > 0) {    //save the items with quantities were greater than 0
          temp[i]->save(fp);
        }
      }
      fp.clear();     // clear failures and close temporary file
      fp.close();

      int i;
      for (i = 0; i < noOfItems; i++) {
        delete temp[i];
        temp[i] = nullptr;
      }
      noOfItems = 0;
      fs.clear();   // clear failures and close original file
      fs.close();
      remove(m_billfname);    //delete original file
      rename("copy.txt", m_billfname);    //rename the copy file to the original file
      truncateBillFile();     //truncate file // purging data from file, why??
    }

  }
  void PosApp::addToBill(Item & I) {
    int quantity;
    fs.open(m_billfname, ios::out | ios::app); // open file in write mode, if file exist appends to the end of file.
    quantity = I.quantity();
    I.quantity(1);
    I.save(fs);
    I.quantity(quantity);     //set the quantity in I to the local variable amount
    I -= 1;                   // decrease by one using -= operator
    fs.close();
  }
  void PosApp::POS() {
    char sku[MAX_SKU_LEN];
    bool flag = true;
    while (true) {
      cout << "Sku: ";// << endl;
      if (flag == true) {
        cin.ignore(256, '\n');
        flag = false;
      }
      cin.getline(sku, MAX_SKU_LEN, '\n');
      if (sku[0] == '\0') {
        break;
      }
      if (searchItems(sku) != -1) {
        cout << "v------------------->" << endl;
        cout << "| " << m_items[searchItems(sku)]->name() << endl;
        addToBill(*m_items[searchItems(sku)]);
        cout << "^------------------->" << endl;
      }
      else {
        cout << "Not found!" << endl;
      }
    }// end of while loop
    saveRecs();
    showBill();
  }// end of POS()
}

/*
outputs:
-------------------------------------
update:
Please enter the SKU: 1313
Name:
Paper Tissue
Sku: 1313
Price: 1.22
Price after tax: 1.38
Quantity: 194
Total Cost: 267.45

Please enter the number of purchased items: 12
Updated!

------------------------------
Please enter the SKU: 3434
Not found!

-------------------------------------
add item:
Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 1200/10/12
Invalid Year in Date Entry

Perishable Item Entry:
Sku: 5656
Name:
Honey
Price: 12.99
Taxed: y
Quantity: 12
Expiry date (YYYY/MM/DD): 2017/5/15
Item added.

--------------------------------------------
list item:
 Row | SKU    | Item Name          | Price |TX |Qty |   Total |
-----|--------|--------------------|-------|---|----|---------|
   1 | 1234   |Milk                |   3.99|  P|   2|     7.98|
   2 | 3456   |Paper Cups          |   5.99| TN|  38|   257.21|
   3 | 4567   |Butter              |   4.56| TP|   9|    46.38|
   4 | 1212   |Salted Butter       |   5.99|  P| 111|   664.89|
   5 | 1313   |Paper Tissue        |   1.22| TN| 206|   283.99|
   6 | 5656   |Honey               |  12.99| TP|  12|   176.14|
-----^--------^--------------------^-------^---^----^---------^
                               Total Asset: $  |       1436.59|
-----------------------------------------------^--------------^


--------------------------------------
printbill:
v--------------------------------------------------------v
| 2017/04/02, 12:42                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         22.05|
^-----------------------------------------^--------------^

-------------------------------------------------------
POS:
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1212
v------------------->
| Salted Butter
^------------------->
Sku: 1313
v------------------->
| Paper Tissue
^------------------->
Sku: 1234
v------------------->
| Milk
^------------------->
Sku: 7654
Not found!
Sku: 5656
v------------------->
| Honey
^------------------->
Sku:
v--------------------------------------------------------v
| 2017/04/02, 12:58                                      |
| SKU    | Item Name          | Price |TX |Qty |   Total |
|--------|--------------------|-------|---|----|---------|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1212   |Salted Butter       |   5.99|  P|   1|     5.99|
| 1313   |Paper Tissue        |   1.22| TN|   1|     1.38|
| 1234   |Milk                |   3.99|  P|   1|     3.99|
| 5656   |Honey               |  12.99| TP|   1|    14.68|
^--------^--------------------^-------^---^----^---------^
|                               Total: $  |         32.03|
^-----------------------------------------^--------------^

------------------------------------------------------
run:
The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 5656
v-----------------------------------v
Name:
Honey
Sku: 5656
Price: 12.99
Price after tax: 14.68
Quantity: 10
Total Cost: 146.79
Expiry date: 2017/05/15
^-----------------------------------^

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 5

Please enter the SKU: 12345
Not found!

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> five

===Invalid Selection, try again===

The Sene-Store
1- List items
2- Add Perishable item
3- Add Non-Perishable item
4- Update item quantity
5- Show Item
6- POS
0- exit program
> 0

Goodbye!

*/
