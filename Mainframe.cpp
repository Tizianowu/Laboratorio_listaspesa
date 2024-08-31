//
// Created by Tiziano Wu on 31/08/24.
//

#include "Mainframe.h"
MainFrame::MainFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title) {
    CreateControls();
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