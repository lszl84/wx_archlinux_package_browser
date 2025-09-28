#include <wx/splitter.h>
#include <wx/listbox.h>
#include <wx/wx.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    auto frame = new MyFrame();

    frame->Show(true);

    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Package Browser", wxDefaultPosition,
          wxSize(800, 600))
{
    auto mainSplitter = new wxSplitterWindow(this);
    auto detailsSplitter = new wxSplitterWindow(mainSplitter);

    mainSplitter->SetSashGravity(0.3);
    detailsSplitter->SetSashGravity(0.5);
    
    auto list = new wxListBox(mainSplitter, wxID_ANY);

    wxArrayString packages;
    wxExecute("pacman -Qq", packages);

    list->InsertItems(packages, 0);

    auto descriptionText = new wxTextCtrl(detailsSplitter,
        wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY);

    descriptionText->SetFont(wxFontInfo(10)
        .Family(wxFONTFAMILY_TELETYPE));
    
    auto depsText = new wxTextCtrl(detailsSplitter,
        wxID_ANY, "",
        wxDefaultPosition, wxDefaultSize,
        wxTE_MULTILINE | wxTE_READONLY);

    detailsSplitter->SplitHorizontally(descriptionText, depsText);
    mainSplitter->SplitVertically(list, detailsSplitter);

    list->Bind(wxEVT_LISTBOX,
        [descriptionText, depsText](const wxCommandEvent& e) {
            auto package = e.GetString();
            auto flags = wxEXEC_NODISABLE | wxEXEC_SYNC;

            wxArrayString descriptionOutput;
            wxArrayString depsOutput;

            wxExecute("pacman -Qi " + package,
                descriptionOutput, flags);
            wxExecute("pactree -r " + package,
                depsOutput, flags);

            descriptionText->SetValue(wxJoin(descriptionOutput, '\n'));
            depsText->SetValue(wxJoin(depsOutput, '\n'));
    });
}
