#include <wx/brush.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/print.h>
#include <wx/sizer.h>
#include <valarray>
#include <wx/wx.h>
#include <mathplot.h>

#include "block1.h"

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
    void OnSliderChange(wxScrollEvent& event);
    mpWindow *m_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *f_plot = new mpWindow(this, -1, wxDefaultPosition);
    wxSlider *fm1_slider, *fm2_slider, *fm3_slider;
    double fm1 = 20;
    double fm2 = 200;
    double fm3 = 2000;
    double fc = 1000;
};

enum
{
    ID_Hello = 1,
    BUTTON_plot = wxID_HIGHEST + 1,
    SLIDER_FM1,
    SLIDER_FM2,
    SLIDER_FM3
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "FM Demo")
{
    wxBoxSizer *main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *plots_sizer = new wxGridSizer(2, 2, 0 ,0);
    wxBoxSizer *controls_sizer = new wxBoxSizer(wxVERTICAL);

    wxButton *plotButton = new wxButton(this, BUTTON_plot, "Generate and Plot");
    this->fm1_slider = new wxSlider(this, SLIDER_FM1, 20, 0, 2000);
    this->fm2_slider = new wxSlider(this, SLIDER_FM2, 20, 0, 2000);
    this->fm3_slider = new wxSlider(this, SLIDER_FM3, 20, 0, 2000);
    controls_sizer->Add(plotButton, 0, wxSHAPED, 0);
    controls_sizer->Add(fm1_slider, 0, wxSHAPED, 0);
    controls_sizer->Add(fm2_slider, 0, wxSHAPED, 0);
    controls_sizer->Add(fm3_slider, 0, wxSHAPED, 0);
    plots_sizer->Add(m_plot, 1, wxEXPAND, 0);
    plots_sizer->Add(f_plot, 1, wxEXPAND, 0);
    main_sizer->Add(controls_sizer, 2, wxEXPAND, 0);
    main_sizer->Add(plots_sizer, 10, wxEXPAND, 0);
    SetSizer(main_sizer); 
    SetAutoLayout(TRUE);

    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnPlot, this, BUTTON_plot);
    Bind(wxEVT_SIZE, &MyFrame::OnResize, this, ID_Hello);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnPlot(wxCommandEvent& event) {
    std::valarray<double> time;
    double t1 = 0;
    double t2 = 0.1;
    double dt = 0.001;
    int size = (t2-t1)/dt;
    time.resize((t2-t1)/dt, 0);
    for(int i = 0; i < size; ++i) {
        time[i] = dt*i;
    }
    message_signal m1(this->fm1, this->fm2, this->fm3, this->fc, time);
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
    vectorLayer1->SetDrawOutsideMargins(true);
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
    vectorLayer2->SetDrawOutsideMargins(true);
    f_plot->DelLayer(vectorLayer2);
    f_plot->AddLayer(vectorLayer2);
    f_plot->Fit();
}

void MyFrame::OnResize(wxSizeEvent& event) {
    this->Refresh();
    m_plot->UpdateAll();
    f_plot->UpdateAll();
}

void MyFrame::OnSliderChange(wxScrollEvent& event) {
    switch(event.GetId()) {
        case SLIDER_FM1:
            fm1 = this->fm1_slider->GetValue();
            break;
        case SLIDER_FM2:
            fm2 = this->fm2_slider->GetValue();
            break;
        case SLIDER_FM3:
            fm3 = this->fm3_slider->GetValue();
            break;

    }
}