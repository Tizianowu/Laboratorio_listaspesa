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
    bool bought;
    Item(const wxString& itemName, int itemQuantity) {
        try {
            if (itemQuantity < 0) {
                throw std::invalid_argument("Quantity cannot be negative.");
            }
            name = itemName;
            quantity = itemQuantity;
            bought = false;
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            quantity = 0; // Impostiamo un valore predefinito in caso di errore
        }
    }


    const wxString &getName() const {
        return name;
    }

    int getQuantity() const {
        return quantity;
    }
};