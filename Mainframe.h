//
// Created by Tiziano Wu on 31/08/24.
//

#ifndef LABORATORIO_LISTASPESA_MAINFRAME_H
#define LABORATORIO_LISTASPESA_MAINFRAME_H
#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Item.h"


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString &title);
private:
    void CreateControls();
    void Setupsizers();
    void BindEventHandlers();

    void AddUserButtonClicked(wxCommandEvent& evt);
    void UserInputEnter(wxCommandEvent&evt);
    void OnListKeyDown(wxKeyEvent &evt);   //serve per delete elemento selezionato
    void ClearButtonClicked(wxCommandEvent& evt);  //server per clear all
    void ShowListPanel(wxMouseEvent& evt);
    void OnWindowClosed(wxCloseEvent& evt);  // keep the panel when we close it

    std::vector<wxString> loadUserstoFile(const std::string& filename);
    void saveUsersToFile(const std::vector<wxString> &tasks, const std::string &filename);

    void AddUser();
    void DeleteSelectedUser();
    void Addfromsaved();

    wxString currentUser;
    wxPanel* mainPanel;
    wxPanel* UserPanel;
    wxButton* addButton;
    wxStaticText* headlineText;
    wxTextCtrl* inputField;
    wxButton* clearButton;
    wxListBox* userList;

    //liste della spesa
    void CreateShopControls();
    void SetupShopSizers();
    void BindShopEvents();

    void AddShopButtonClicked(wxCommandEvent& evt);
    void shopInputEnter(wxCommandEvent& evt);
    void shopClearButton(wxCommandEvent& evt);
    void shopListKeyDown(wxKeyEvent &evt);
    void Goback(wxCommandEvent& evt);

    void AddShop();
    void DeleteselectedList();
    void updatelist();

    std::vector<wxString> loadShopstoUser(const std::string& filename);
    void saveShopsToUser(const std::vector<wxString> &items, const std::string &filename);


    wxPanel* listPanel;
    wxButton* shopAddButton;
    wxStaticText* shopTitle;
    wxTextCtrl* shopField;
    wxButton* clearShopsButton;
    wxListBox* shopList;
    wxButton* shopbackButton;

    //items di una lista
    void CreateItemsControls();
    void SetupItemSizers();
    void BindItemEvents();

    void AddItemButtonClicked(wxCommandEvent& evt);
    void ItemListKeyDown(wxCommandEvent& evt);
    void ItemClearButton(wxCommandEvent& evt);
    void ItemKeyDown(wxKeyEvent& evt);

    void AddItem();
    void DeleteSelectedItem();

    std::vector<Item> loadItemstoShop(const std::string& filename);
    void saveItemstoShops(const std::vector<Item> &items, const std::string &filename);

    wxString currentShop;
    wxPanel* ItemPanel;
    wxButton* AddItemButton;
    wxStaticText* ItemTitle;
    wxTextCtrl* ItemField;
    wxSpinCtrl* ItemSpin;
    wxListBox* ItemList;
    wxListBox* ItemQuantityList;
    wxButton* clearItemsButton;
    wxButton* ItemBackButton;
    wxButton* ShareButton;
};


#endif //LABORATORIO_LISTASPESA_MAINFRAME_H
