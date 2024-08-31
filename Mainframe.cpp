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
    Setupsizers();
    BindEventHandlers();
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

    //assegno userSizer come sizer principale del mio pannello di utenti
    UserPanel->SetSizer(userSizer);

    // Add panels to the main sizer
    mainSizer->Add(UserPanel, 1, wxEXPAND | wxALL, 5);

    mainPanel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    // Update the layout of the frame
    mainSizer->Layout();
}

void MainFrame::BindEventHandlers() {
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

    saveUsersToFile(tasks, "users.txt");
    evt.Skip();
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