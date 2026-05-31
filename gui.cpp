#include "wx/brush.h"
#include "wx/event.h"
#include "wx/gdicmn.h"
#include "wx/print.h"
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
    void OnResize(wxSizeEvent& event);
    mpWindow * m_plot = new mpWindow(this, -1, wxDefaultPosition, wxSize(200, 200), wxSUNKEN_BORDER);
    mpWindow * f_plot = new mpWindow(this, -1, wxDefaultPosition, wxSize(200, 200), wxSUNKEN_BORDER);
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
    wxBoxSizer *main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *plots_sizer = new wxGridSizer(2,2, 10 ,10);
    wxBoxSizer *buttons_sizer = new wxBoxSizer(wxVERTICAL);

    wxButton *plotButton = new wxButton(this, BUTTON_plot, "Hello World", wxDefaultPosition, wxSize(100,60));
    buttons_sizer->Add(plotButton, 0, wxSHAPED, 0);
    plots_sizer->Add(m_plot, 1, wxSHAPED, 10);
    plots_sizer->Add(f_plot, 1, wxSHAPED, 10);
    main_sizer->Add(buttons_sizer, 1, wxEXPAND, 10);
    main_sizer->Add(plots_sizer, 9, wxEXPAND, 10);
    SetSizer(main_sizer); 
    SetAutoLayout(TRUE);

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnPlot, this, BUTTON_plot);
    Bind(wxEVT_SIZE, &MyFrame::OnResize, this, ID_Hello);
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
    mpFXYVector *vectorLayer1 = new mpFXYVector("Vector");
    vectorLayer1->SetData(std::move(x), std::move(y));
    vectorLayer1->SetContinuity(true);
    vectorLayer1->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer1->SetDrawOutsideMargins(false);
    m_plot->DelLayer(vectorLayer1);
    m_plot->AddLayer(vectorLayer1);
    m_plot->Fit();

    m1.modulate();
    std::vector<double> x2;
    std::vector<double> y2;
    for(double d: m1.time) {
        x2.push_back(d);
    }
    for(double d: m1.modulated) {
        y2.push_back(d);
    }

    mpFXYVector *vectorLayer2 = new mpFXYVector("Vector");
    vectorLayer2->SetData(std::move(x2), std::move(y2));
    vectorLayer2->SetContinuity(true);
    vectorLayer2->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer2->SetDrawOutsideMargins(false);
    f_plot->DelLayer(vectorLayer2);
    f_plot->AddLayer(vectorLayer2);
    f_plot->Fit();
}

void MyFrame::OnResize(wxSizeEvent& event) {
    this->Refresh();
    m_plot->Fit();
    f_plot->Fit();
}