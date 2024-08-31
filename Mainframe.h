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

    void AddUser();

    wxPanel* mainPanel;
    wxPanel* UserPanel;
    wxButton* addButton;
    wxStaticText* headlineText;
    wxTextCtrl* inputField;
    wxButton* clearButton;
    wxListBox* userList;
};

#endif //LABORATORIO_LISTASPESA_MAINFRAME_H
