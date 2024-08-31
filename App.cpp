//
// Created by Tiziano Wu on 31/08/24.
//

#include "App.h"
#include <wx/wx.h>

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    MainFrame* mainframe = new MainFrame("");
    mainframe->SetClientSize(1000,600);
    mainframe->Center();
    mainframe->Show();
    return true;
}