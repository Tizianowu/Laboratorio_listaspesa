//
// Created by Tiziano Wu on 31/08/24.
//

#ifndef LABORATORIO_LISTASPESA_ITEM_H
#define LABORATORIO_LISTASPESA_ITEM_H

#endif //LABORATORIO_LISTASPESA_ITEM_H
#include <wx/wx.h>
struct Item{
    wxString name;
    int quantity;
    Item(const wxString& itemName, int itemQuantity): name(itemName), quantity(itemQuantity) {}

    const wxString &getName() const {
        return name;
    }

    int getQuantity() const {
        return quantity;
    }
};