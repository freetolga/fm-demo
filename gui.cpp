#include "wx/brush.h"
#include "wx/event.h"
#include "wx/gdicmn.h"
#include "wx/sizer.h"
#include <valarray>
#include <wx/wx.h>
//#include <matplot/matplot.h>
#include <mathplot.h>

#include "block1.h"
#include "wx/toplevel.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnPlot(wxCommandEvent& event);
    mpWindow * m_plot = new mpWindow(this, -1, wxDefaultPosition, wxSize(200, 200), wxSUNKEN_BORDER);
    mpFXYVector *vectorLayer = new mpFXYVector("Vector");
};

enum
{
    ID_Hello = 1,
    BUTTON_plot = wxID_HIGHEST + 1,
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Hello World")
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    wxBoxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxHORIZONTAL);

    wxButton *plotButton = new wxButton(this, BUTTON_plot, "Hello World", wxDefaultPosition, wxSize(100,60));
    buttons_sizer->Add(plotButton, 0, wxEXPAND, 0);
    main_sizer->Add(buttons_sizer, 1, wxSHAPED, 10);
    main_sizer->Add(m_plot, 10, wxSHAPED, 10);
    SetSizer(main_sizer); 
    SetAutoLayout(TRUE);

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnPlot, this, BUTTON_plot);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::OnPlot(wxCommandEvent& event) {
    double fm1 = 20;
    double fm2 = 200;
    double fm3 = 220;
    double fc = 1000;
    std::valarray<double> time;
    double t1 = 0;
    double t2 = 0.1;
    double dt = 0.001;
    int size = (t2-t1)/dt;
    time.resize((t2-t1)/dt, 0);
    for(int i = 0; i < size; ++i) {
        time[i] = dt*i;
    }
    message_signal m1(fm1, fm2, fm3, fc, time);
    std::vector<double> x;
    std::vector<double> y;
    for(double d: m1.time) {
        x.push_back(d);
    }
    for(double d: m1.original) {
        y.push_back(d);
    }
    vectorLayer->SetData(std::move(x), std::move(y));
    vectorLayer->SetContinuity(true);
    vectorLayer->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer->SetDrawOutsideMargins(false);
    m_plot->AddLayer(vectorLayer);
    m_plot->Fit();
}