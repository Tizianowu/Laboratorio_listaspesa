//
// Created by Tiziano Wu on 31/08/24.
//

#ifndef LABORATORIO_LISTASPESA_MAINFRAME_H
#define LABORATORIO_LISTASPESA_MAINFRAME_H
#include <wx/wx.h>

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
    void OnWindowClosed(wxCloseEvent& evt);  // keep the panel when we close it

    std::vector<wxString> loadUserstoFile(const std::string& filename);
    void saveUsersToFile(const std::vector<wxString> &tasks, const std::string &filename);

    void AddUser();
    void DeleteSelectedUser();
    void Addfromsaved();

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

    wxPanel* listPanel;
    wxButton* shopAddButton;
    wxStaticText* shopTitle;
    wxTextCtrl* shopField;
    wxButton* clearShopsButton;
    wxListBox* shopList;
    wxButton* shopbackButton;
};

#endif //LABORATORIO_LISTASPESA_MAINFRAME_H
