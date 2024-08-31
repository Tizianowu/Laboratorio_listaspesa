//
// Created by Tiziano Wu on 31/08/24.
//
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm>
#include <wx/string.h>
#include "Mainframe.h"
MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {
    CreateControls();
    CreateShopControls();
    CreateItemsControls();
    Setupsizers();
    SetupShopSizers();
    SetupItemSizers();
    BindEventHandlers();
    BindShopEvents();
    BindItemEvents();
    Addfromsaved();
}

void MainFrame::CreateControls() {
    wxFont headlineFont(wxFontInfo(wxSize(0,36)).Bold());
    wxFont mainFont(wxFontInfo(wxSize(0,16)));

    mainPanel = new wxPanel(this);
    mainPanel->SetFont(mainFont);

    // wxWidgets controls for UserPanel
    UserPanel = new wxPanel(mainPanel);
    addButton = new wxButton(UserPanel,wxID_ANY,"Add");
    headlineText = new wxStaticText(UserPanel,wxID_ANY,"LISTA UTENTI");
    headlineText->SetFont(headlineFont);
    inputField = new wxTextCtrl(UserPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,wxTE_PROCESS_ENTER);
    clearButton = new wxButton(UserPanel, wxID_ANY, "Clear", wxDefaultPosition, wxDefaultSize);
    userList = new wxListBox(UserPanel, wxID_ANY);
}

void MainFrame::Setupsizers() {
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* userSizer = new wxBoxSizer(wxVERTICAL);
    userSizer->Add(headlineText,wxSizerFlags().CenterHorizontal());
    userSizer->AddSpacer(25);

    wxBoxSizer* usersubSizer = new wxBoxSizer(wxHORIZONTAL);
    usersubSizer->Add(inputField,wxSizerFlags().Proportion(1));
    usersubSizer->AddSpacer(5);
    usersubSizer->Add(addButton);

    userSizer->Add(usersubSizer,wxSizerFlags().Expand());
    usersubSizer->AddSpacer(5);
    userSizer->Add(userList,wxSizerFlags().Expand().Proportion(1));
    usersubSizer->AddSpacer(5);
    userSizer->Add(clearButton);

    wxGridSizer* outSizer = new wxGridSizer(1);
    outSizer->Add(userSizer,wxSizerFlags().Border(wxALL,25).Expand());

    UserPanel->SetSizer(userSizer);

    // Add panels to the main sizer
    mainSizer->Add(UserPanel, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(listPanel, 1, wxEXPAND | wxALL, 5);
    mainSizer->Add(ItemPanel, 1, wxEXPAND | wxALL, 5);

    mainPanel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    // Update the layout of the frame
    mainSizer->Layout();
}

void MainFrame::BindEventHandlers() {
    userList->Bind(wxEVT_LEFT_DCLICK, &MainFrame::ShowListPanel, this);
    addButton->Bind(wxEVT_BUTTON,&MainFrame::AddUserButtonClicked,this);
    inputField->Bind(wxEVT_TEXT_ENTER,&MainFrame::UserInputEnter,this);
    userList->Bind(wxEVT_KEY_DOWN,&MainFrame::OnListKeyDown,this);
    clearButton->Bind(wxEVT_BUTTON,&MainFrame::ClearButtonClicked,this);
    this->Bind(wxEVT_CLOSE_WINDOW,&MainFrame::OnWindowClosed,this);

}



void MainFrame::AddUserButtonClicked(wxCommandEvent &evt) {
    AddUser();
}

void MainFrame::UserInputEnter(wxCommandEvent &evt) {
    AddUser();
}

void MainFrame::OnListKeyDown(wxKeyEvent &evt) {
    int keyCode = evt.GetKeyCode();
    int selectedIndex = userList->GetSelection();

    if ((keyCode == WXK_DELETE || keyCode == WXK_BACK) && selectedIndex != wxNOT_FOUND) {
        DeleteSelectedUser();
    }
}

void MainFrame::ClearButtonClicked(wxCommandEvent &evt) {
    if(userList->IsEmpty())
        return;
    wxMessageDialog dialog(this,"are you sure you want to clear all?","clear",wxYES_NO | wxCANCEL);
    int result = dialog.ShowModal();
    if(result == wxID_YES){
        for (int i = 0; i < userList->GetCount(); i++) {
            wxString tmpShopName = userList->GetString(i);
            std::string filename = std::string(tmpShopName.mb_str()) + "_" + "shops.txt";
            if (std::filesystem::exists(filename)) {
                std::filesystem::remove(filename);
            }
        }
        userList->Clear();
    }
}

void MainFrame::OnWindowClosed(wxCloseEvent &evt) {
    std::vector<wxString> tasks;
    for (int i = 0; i < userList->GetCount(); i++) {
        wxString s = userList->GetString(i);
        tasks.push_back(userList->GetString(i));
    }
    saveUsersToFile(tasks,"users.txt");

    if (!currentUser.IsEmpty()) {

        std::vector<wxString> shops;
        for (int i = 0; i < shopList->GetCount(); i++) {
            shops.push_back(shopList->GetString(i));
        }
        std::string filename = std::string(currentUser.mb_str()) + "_shops.txt";
        saveShopsToUser(shops, filename);
    }
    if (!currentShop.IsEmpty()) {
        if(ItemList->GetCount()>0 && ItemQuantityList->GetCount() > 0) {

            std::vector<Item> shopItems;
            for (int i = 0; i < ItemList->GetCount(); i++) {
                wxString itemName = ItemList->GetString(i);
                int itemQuantity = wxAtoi(ItemQuantityList->GetString(i)); // Converte wxString a int
                shopItems.push_back(Item(itemName, itemQuantity));
            }

            // Salva gli items in un file associato all'utente
            std::string itemsFilename =std::string(currentUser.mb_str()) + "_" + std::string(currentShop.mb_str()) + "item.txt";
            saveItemstoShops(shopItems, itemsFilename);
        }
    }
    evt.Skip();
}

void MainFrame::ShowListPanel(wxMouseEvent &evt) {
    int selectedUserIndex = userList->GetSelection();
    if (selectedUserIndex == wxNOT_FOUND) {
        wxMessageBox("No user selected!");
        return;
    }

    wxString selectedUser = userList->GetString(selectedUserIndex);
    currentUser = selectedUser;  // Salva l'utente corrente

    // Costruisci il nome del file per l'utente selezionato
    std::string filename = std::string(selectedUser.mb_str()) + "_shops.txt";

    // Carica le liste di spesa dell'utente
    std::vector<wxString> shopItems = loadShopstoUser(filename);

    shopList->Clear();
    for (const wxString& item : shopItems) {
        shopList->Insert(item, shopList->GetCount());
    }
    UserPanel->Hide();  // Hide the shop panel
    listPanel->Show();  // Show the list panel
    UserPanel->GetParent()->Layout();  // Recalculate layout for the parent panel
}



void MainFrame::AddUser() {
    wxString description = inputField->GetValue();
    if(!description.IsEmpty()){
        userList->Insert(description,userList->GetCount());
        inputField->Clear();
    }
    inputField->SetFocus();
}

void MainFrame::DeleteSelectedUser() {
    int selectedIndex = userList->GetSelection();
    if (selectedIndex == wxNOT_FOUND)
        return;
    {
        wxString selectedUser = userList->GetString(selectedIndex);
        std::string filename = std::string(selectedUser.mb_str()) + "_shops.txt";
        if (std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
        userList->Delete(selectedIndex);
    }
}

void MainFrame::Addfromsaved() {
    std::vector<wxString>tasks = loadUserstoFile("users.txt");
    for (const wxString& task : tasks) {
        int index = userList->GetCount();
        userList->Insert(task, index);
    }
}

void MainFrame::updatelist() {
    if (!currentUser.IsEmpty()) {
        std::vector<wxString> shopItems;
        for (int i = 0; i < shopList->GetCount(); i++) {
            shopItems.push_back(shopList->GetString(i));
        }
        std::string filename = std::string(currentUser.mb_str()) + "_shops.txt";
        saveShopsToUser(shopItems, filename);
    }
}



void MainFrame::saveUsersToFile(const std::vector<wxString> &lists, const std::string &filename) {
    std::ofstream ostream(filename);
    ostream << lists.size();

    for(const wxString& list : lists) {
        wxString description = list;
        std::replace(description.begin(), description.end(), ' ', '_');

        ostream << '\n' << description;
    }
}



std::vector<wxString> MainFrame::loadUserstoFile(const std::string &filename) {
    if (!std::filesystem::exists(filename)){
        return std::vector<wxString>();
    }
    std::vector<wxString> lists;
    std::ifstream istream(filename);
    int n;
    istream>>n;
    for (int i=0;i<n;i++){
        std::string description;

        istream>>description;
        replace(description.begin(),description.end(),'_',' ');
        lists.push_back(description);
    }
    return lists;
}






void MainFrame::CreateShopControls() {
    wxFont headlineFont(wxFontInfo(wxSize(0,36)).Bold());

    listPanel = new wxPanel(mainPanel);
    shopAddButton = new wxButton(listPanel, wxID_ANY, "Add");
    shopTitle = new wxStaticText(listPanel, wxID_ANY, "LISTE DELLA SPESA");
    shopTitle->SetFont(headlineFont);
    shopField = new wxTextCtrl(listPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    clearShopsButton = new wxButton(listPanel, wxID_ANY, "Clear", wxDefaultPosition, wxDefaultSize);
    shopList = new wxListBox(listPanel, wxID_ANY);
    shopbackButton = new wxButton(listPanel, wxID_ANY, "Go Back", wxDefaultPosition, wxDefaultSize);
    listPanel->Hide(); // Initially hide the list panel
}

void MainFrame::SetupShopSizers() {
    wxBoxSizer* shopSizer = new wxBoxSizer(wxVERTICAL);
    shopSizer->Add(shopTitle,wxSizerFlags().CenterHorizontal());
    shopSizer->AddSpacer(25);

    wxBoxSizer* shopsubSizer = new wxBoxSizer(wxHORIZONTAL);
    shopsubSizer->Add(shopField,wxSizerFlags().Proportion(1));
    shopsubSizer->AddSpacer(5);
    shopsubSizer->Add(shopAddButton);

    shopSizer->Add(shopsubSizer,wxSizerFlags().Expand());
    shopsubSizer->AddSpacer(5);
    shopSizer->Add(shopList,wxSizerFlags().Expand().Proportion(1));
    shopsubSizer->AddSpacer(5);
    wxBoxSizer* shopsubSizer2 = new wxBoxSizer(wxHORIZONTAL);
    shopsubSizer2->Add(clearShopsButton);
    shopsubSizer2->AddSpacer(10);
    shopsubSizer2->Add(shopbackButton);
    shopSizer->Add(shopsubSizer2);


    wxGridSizer* outSizer = new wxGridSizer(1);
    outSizer->Add(shopSizer,wxSizerFlags().Border(wxALL,25).Expand());

    listPanel->SetSizer(shopSizer);
}

void MainFrame::BindShopEvents() {
    shopList->Bind(wxEVT_KEY_DOWN,&MainFrame::shopListKeyDown,this);
    shopList->Bind(wxEVT_LEFT_DCLICK, &MainFrame::ShowItemPanel, this);
    shopAddButton->Bind(wxEVT_BUTTON,&MainFrame::AddShopButtonClicked,this);
    shopField->Bind(wxEVT_TEXT_ENTER,&MainFrame::shopInputEnter,this);
    clearShopsButton->Bind(wxEVT_BUTTON,&MainFrame::shopClearButton,this);
    shopbackButton->Bind(wxEVT_BUTTON,&MainFrame::Goback,this);
}

void MainFrame::AddShopButtonClicked(wxCommandEvent &evt) {
    AddShop();
}
void MainFrame::shopInputEnter(wxCommandEvent &evt) {
    AddShop();
}
void MainFrame::shopClearButton(wxCommandEvent &evt) {
    if(shopList->IsEmpty())
        return;
    wxMessageDialog dialog(this,"are you sure you want to clear all?","clear",wxYES_NO | wxCANCEL);
    int result = dialog.ShowModal();
    if(result == wxID_YES){
        for(int i=0;i<shopList->GetCount();i++){
            wxString tmpShopName = shopList->GetString(i);
            std::string filename =std::string(currentUser.mb_str()) + "_" + std::string(tmpShopName.mb_str()) + "item.txt";
            if(std::filesystem::exists(filename)) {
                std::filesystem::remove(filename);
            }
        }
        shopList->Clear();
    }
}
void MainFrame::shopListKeyDown(wxKeyEvent &evt) {
    int keyCode = evt.GetKeyCode();

    if (keyCode == WXK_DELETE || keyCode == WXK_BACK) {
        DeleteselectedList();
    }
}

void MainFrame::ShowItemPanel(wxMouseEvent &evt) {
    int selectedShopIndex = shopList->GetSelection();
    if (selectedShopIndex == wxNOT_FOUND) {
        wxMessageBox("No Shop selected!");
        return;
    }

    currentShop = shopList->GetString(shopList->GetSelection());
    wxString selectedList = currentShop;

    // Costruisci il nome del file per la spesa selezionato
    std::string filename = std::string(currentUser.mb_str()) + "_" + std::string(currentShop.mb_str()) + "item.txt";

    // Carica le liste di spesa dell'utente
    std::vector<Item> Items = loadItemstoShop(filename);

    ItemList->Clear();
    ItemQuantityList->Clear();
    for (const Item& item : Items) {
        ItemList->Insert(item.name, ItemList->GetCount());
        wxString updatedQuantityStr = wxString::Format(wxT("%d"), item.quantity);
        ItemQuantityList->Insert(updatedQuantityStr, ItemQuantityList->GetCount());  // Insert the quantity string
    }

    listPanel->Hide();  // Hide the shop panel
    ItemPanel->Show();  // Show the list panel
    ItemPanel->GetParent()->Layout();  // Recalculate layout for the parent panel

}

void MainFrame::Goback(wxCommandEvent &evt) {
    updatelist();
    UserPanel->Show();
    listPanel->Hide();
    UserPanel->GetParent()->Layout();
}



void MainFrame::AddShop() {
    wxString description = shopField->GetValue();
    if(!description.IsEmpty()){
        shopList->Insert(description,shopList->GetCount());
        shopField->Clear();
    }
    shopField->SetFocus();
}

void MainFrame::DeleteselectedList() {
    int selectedIndex = shopList->GetSelection();
    if(selectedIndex == wxNOT_FOUND)
        return;
    wxString selectedShop = shopList->GetString(selectedIndex);
    std::string filename = std::string(currentUser.mb_str()) + "_" + std::string(selectedShop.mb_str()) + "item.txt";
    if(std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
    shopList->Delete(selectedIndex);
}



void MainFrame::saveShopsToUser(const std::vector<wxString> &items, const std::string &filename) {
    std::ofstream ostream(filename);
    ostream << items.size();

    for (const wxString& item : items) {
        wxString description = item;
        std::replace(description.begin(), description.end(), ' ', '_');
        ostream << '\n' << description;
    }
}

std::vector<wxString> MainFrame::loadShopstoUser(const std::string &filename) {
    if (!std::filesystem::exists(filename)){
        return std::vector<wxString>();
    }
    std::vector<wxString> lists;
    std::ifstream istream(filename);
    int n;
    istream>>n;
    for (int i=0;i<n;i++){
        std::string description;

        istream>>description;
        replace(description.begin(),description.end(),'_',' ');
        lists.push_back(description);
    }
    return lists;
}





void MainFrame::CreateItemsControls() {
    wxFont headlineFont(wxFontInfo(wxSize(0,36)).Bold());

    ItemPanel = new wxPanel(mainPanel);
    AddItemButton = new wxButton(ItemPanel,wxID_ANY, "Add");
    ItemTitle = new wxStaticText(ItemPanel,wxID_ANY,"Lista della spesa");
    ItemTitle->SetFont(headlineFont);
    ItemField = new wxTextCtrl(ItemPanel,wxID_ANY, "",wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
    clearItemsButton = new wxButton(ItemPanel,wxID_ANY,"Clear",wxDefaultPosition,wxDefaultSize);
    ItemList = new wxListBox(ItemPanel,wxID_ANY);
    ItemQuantityList = new wxListBox(ItemPanel,wxID_ANY);
    ItemBackButton = new wxButton(ItemPanel,wxID_ANY,"Go back");
    ItemSpin = new wxSpinCtrl(ItemPanel,wxID_ANY," ",wxDefaultPosition,wxDefaultSize,wxSP_WRAP,0,100,0);
    ShareButton = new wxButton(ItemPanel,wxID_ANY,"Share");
    ItemPanel->Hide();
}

void MainFrame::SetupItemSizers() {
    wxBoxSizer* ItemSizer = new wxBoxSizer(wxVERTICAL);
    ItemSizer->Add(ItemTitle,wxSizerFlags().CenterHorizontal());
    ItemSizer->AddSpacer(25);

    wxBoxSizer* ItemsubSizer = new wxBoxSizer(wxHORIZONTAL);
    ItemsubSizer->Add(ItemField,wxSizerFlags().Proportion(1));
    ItemsubSizer->AddSpacer(5);
    ItemsubSizer->Add(ItemSpin);
    ItemsubSizer->AddSpacer(5);
    ItemsubSizer->Add(AddItemButton);
    ItemSizer->Add(ItemsubSizer,wxSizerFlags().Expand());
    ItemSizer->AddSpacer(5);

    wxBoxSizer* ItemsubSizer1 = new wxBoxSizer(wxHORIZONTAL);
    ItemsubSizer1->Add(ItemList, wxSizerFlags().Proportion(10).Expand());
    ItemsubSizer1->AddSpacer(5);
    ItemsubSizer1->Add(ItemQuantityList, wxSizerFlags().Proportion(1).Expand());
    ItemSizer->Add(ItemsubSizer1, wxSizerFlags().Expand().Proportion(1));

    wxBoxSizer* ItemsubSizer2 = new wxBoxSizer(wxHORIZONTAL);
    ItemsubSizer2->Add(clearItemsButton);
    ItemsubSizer2->AddSpacer(10);
    ItemsubSizer2->Add(ItemBackButton);
    ItemsubSizer2->AddSpacer(10);
    ItemsubSizer2->Add(ShareButton);
    ItemSizer->Add(ItemsubSizer2);

    wxGridSizer* outSizer = new wxGridSizer(1);
    outSizer->Add(ItemSizer,wxSizerFlags().Border(wxALL,25).Expand());

    ItemPanel->SetSizer(ItemSizer);
}

void MainFrame::BindItemEvents() {
    AddItemButton->Bind(wxEVT_BUTTON, &MainFrame::AddItemButtonClicked, this);
    ItemField->Bind(wxEVT_TEXT_ENTER, &MainFrame::ItemListKeyDown, this);
    clearItemsButton->Bind(wxEVT_BUTTON, &MainFrame::ItemClearButton, this);
    ItemList->Bind(wxEVT_KEY_DOWN, &MainFrame::ItemKeyDown, this);
    ItemBackButton->Bind(wxEVT_BUTTON,&MainFrame::ItemGoback,this);
}



void MainFrame::AddItemButtonClicked(wxCommandEvent& evt) {
    AddItem();
}

void MainFrame::ItemListKeyDown(wxCommandEvent& evt) {
    AddItem();
}

void MainFrame::ItemKeyDown(wxKeyEvent &evt) {
    int keyCode = evt.GetKeyCode();

    if (keyCode == WXK_DELETE || keyCode == WXK_BACK) {
        DeleteSelectedItem();
    }
}

void MainFrame::ItemClearButton(wxCommandEvent &evt) {
    if(ItemList->IsEmpty())
        return;
    wxMessageDialog dialog(this,"are you sure you want to clear all?","clear",wxYES_NO | wxCANCEL);
    int result = dialog.ShowModal();
    if(result == wxID_YES){
        ItemList->Clear();
        ItemQuantityList->Clear();
        std::string filename = std::string(currentUser.mb_str()) + "_" + std::string(currentShop.mb_str()) + "item.txt";
        if(std::filesystem::exists(filename)) {
            std::filesystem::remove(filename);
        }
    }
}

void MainFrame::ItemGoback(wxCommandEvent &evt) {
    updateShop();
    ItemPanel->Hide();
    listPanel->Show();
    ItemPanel->GetParent()->Layout();
}



void MainFrame::AddItem() {
    wxString description = ItemField->GetValue();
    int number = ItemSpin->GetValue();

    if (!description.IsEmpty() && number > 0) {
        // Converti la descrizione dell'elemento in minuscolo
        description.MakeLower();

        // Controlla se l'elemento è già presente nella lista
        int itemCount = ItemList->GetCount();
        bool itemFound = false;

        for (int i = 0; i < itemCount; ++i) {
            // Ottieni e converti la descrizione esistente in minuscolo
            wxString existingDescription = ItemList->GetString(i);
            existingDescription.MakeLower();

            if (existingDescription == description) {
                // Se l'elemento è già presente, aggiorna solo la quantità
                wxString existingQuantityStr = ItemQuantityList->GetString(i);
                int existingQuantity = wxAtoi(existingQuantityStr);
                existingQuantity += number;
                wxString updatedQuantityStr = wxString::Format(wxT("%d"), existingQuantity);
                ItemQuantityList->SetString(i, updatedQuantityStr);
                itemFound = true;
                break;
            }
        }

        if (!itemFound) {
            // Se l'elemento non è stato trovato, aggiungilo come nuovo elemento
            ItemList->Insert(description, itemCount);
            wxString numberStr = wxString::Format(wxT("%d"), number);
            ItemQuantityList->Append(numberStr);
        }

        // Pulisce il campo di input e resetta il valore dello spinner
        ItemField->Clear();
        ItemSpin->SetValue(0);
    }

    ItemField->SetFocus();
}

void MainFrame::DeleteSelectedItem() {
    int selectedIndex = ItemList->GetSelection();
    if(selectedIndex == wxNOT_FOUND)
        return;
    else {
        ItemList->Delete(selectedIndex);
        ItemQuantityList->Delete(selectedIndex);
    }
}

void MainFrame::updateShop() {
    if (!currentShop.IsEmpty()) {
        if (ItemList->GetCount() != ItemQuantityList->GetCount()) {
            wxLogError("ItemList and ItemQuantityList do not have the same number of elements.");
            return;
        }

        std::vector<Item> shopItems;
        for (int i = 0; i < ItemList->GetCount(); i++) {
            wxString itemName = ItemList->GetString(i);
            int itemQuantity = wxAtoi(ItemQuantityList->GetString(i)); // Converte wxString a int
            shopItems.push_back(Item(itemName, itemQuantity));
        }

        // Salva gli items in un file associato all'utente
        std::string itemsFilename = std::string(currentUser.mb_str()) + "_" + std::string(currentShop.mb_str()) + "item.txt";
        saveItemstoShops(shopItems, itemsFilename);
    }
}



std::vector<Item> MainFrame::loadItemstoShop(const std::string &filename) {
    if (!std::filesystem::exists(filename)) {
        return std::vector<Item>();
    }

    std::vector<Item> items;
    std::ifstream istream(filename);
    int n;
    istream >> n;  // Leggi la dimensione della lista

    for (int i = 0; i < n; ++i) {
        std::string name;
        int quantity;
        istream >> name >> quantity;  // Leggi il nome e la quantità

        std::replace(name.begin(), name.end(), '_', ' ');  // Ripristina gli spazi nel nome
        items.push_back({wxString(name), quantity});  // Aggiungi l'elemento alla lista
    }

    return items;
}

void MainFrame::saveItemstoShops(const std::vector<Item> &items, const std::string &filename) {
    std::ofstream ostream(filename);
    ostream << items.size();

    for (const Item& item : items) {
        std::string name = std::string(item.name.mb_str());  // Converti wxString in std::string
        std::replace(name.begin(), name.end(), ' ', '_');  // Sostituisci gli spazi con underscore

        ostream << '\n' << name << ' ' << item.quantity;
    }
}