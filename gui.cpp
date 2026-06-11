#include <wx/brush.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/print.h>
#include <wx/sizer.h>
#include <valarray>
#include <wx/wx.h>
#include <mathplot.h>

#include "block1.hpp"

enum
{
    ID_Hello = 1,
    BUTTON_plot = wxID_HIGHEST + 1,
    SLIDER_FM1,
    SLIDER_FM2,
    SLIDER_FM3,
    LABEL_FM1,
    LABEL_FM2,
    LABEL_FM3,
    BUTTON_ADD_NOISE,
};

class MyApp final : public wxApp
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
    void OnSliderChange(wxCommandEvent& event);
    void OnCheckBoxChange(wxCommandEvent& event);
    void CleanAllPlots(void);
    mpWindow *m_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *mf_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *s_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *sf_plot = new mpWindow(this, -1, wxDefaultPosition);
    wxSlider *fm1_slider = new wxSlider(this, SLIDER_FM1, 20, 1, 2000);
    wxSlider *fm2_slider =  new wxSlider(this, SLIDER_FM2, 200, 1, 2000);
    wxSlider *fm3_slider = new wxSlider(this, SLIDER_FM3, 2000, 1, 2000);
    wxCheckBox *addNoise = new wxCheckBox(this, BUTTON_ADD_NOISE, "Add Noise?");
    double fm1 = 20;
    double fm2 = 200;
    double fm3 = 2000;
    double fc = 10000;
    bool add_noise = false;
};


wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Block1 GUI")
{
    wxBoxSizer *main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *plots_sizer = new wxGridSizer(2, 2, 10 ,0);
    wxBoxSizer *controls_sizer = new wxBoxSizer(wxVERTICAL);

    wxButton *plotButton = new wxButton(this, BUTTON_plot, "Generate and Plot");
    wxButton *saveButton = new wxButton(this, BUTTON_plot, "Save result to file");
    controls_sizer->Add(plotButton, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(fm1_slider, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(fm2_slider, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(fm3_slider, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(saveButton, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(addNoise, 0, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(m_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(mf_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(s_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(sf_plot, 1, wxSHAPED | wxCENTER, 0);
    main_sizer->Add(controls_sizer, 1, wxEXPAND | wxCENTER, 0);
    main_sizer->Add(plots_sizer, 10, wxEXPAND | wxCENTER, 0);
    SetSizer(main_sizer); 
    SetAutoLayout(TRUE);

    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnPlot, this, BUTTON_plot);
    Bind(wxEVT_CHECKBOX, &MyFrame::OnCheckBoxChange, this, BUTTON_ADD_NOISE);
    Bind(wxEVT_SLIDER, &MyFrame::OnSliderChange, this);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnPlot(wxCommandEvent& event) {
    this->CleanAllPlots();

    message_signal m1(this->fm1, this->fm2, this->fm3, this->fc);
    std::vector<double> x;
    std::vector<double> y;
    for(double d: m1.time) {
        x.push_back(d);
    }
    for(double d: m1.original) {
        y.push_back(d);
    }
    mpFXYVector *vectorLayer1 = new mpFXYVector("Original signal(time)");
    vectorLayer1->SetData(std::move(x), std::move(y));
    vectorLayer1->SetContinuity(true);
    vectorLayer1->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer1->SetDrawOutsideMargins(false);
    m_plot->AddLayer(vectorLayer1);
    m_plot->Fit();

    m1.take_fft_message();

    std::vector<double> x2;
    std::vector<double> y2;
    // sidenote: how for-range works in c++11 and later
    // for(typename iteration_variable : iterable  )
    // read as: for(typename variable in iterable)
    // : means "in keyword" in python
    for(double d: m1.freq) {
        std::cout << d;
        x2.push_back(d);
    }
    for(std::complex<double> d: m1.original_f) {
        std::cout << d;
        y2.push_back(abs(d));
    }


    mpFXYVector *vectorLayer2 = new mpFXYVector("Original signal(freq)");
    vectorLayer2->SetData(std::move(x2), std::move(y2));
    vectorLayer2->SetContinuity(true);
    vectorLayer2->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer2->SetDrawOutsideMargins(false);
    mf_plot->AddLayer(vectorLayer2);
    mf_plot->Fit();

    m1.modulate();
    if(this->add_noise) m1.add_noise();

    std::vector<double> x3;
    std::vector<double> y3;
    for(double d: m1.time) {
        x3.push_back(d);
    }
    for(double d: m1.modulated) {
        y3.push_back(d);
    }


    mpFXYVector *vectorLayer3 = new mpFXYVector("Modulated signal(time)");
    vectorLayer3->SetData(std::move(x3), std::move(y3));
    vectorLayer3->SetContinuity(true);
    vectorLayer3->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer3->SetDrawOutsideMargins(false);
    s_plot->AddLayer(vectorLayer3);
    s_plot->Fit();

    m1.take_fft_modulated();

    std::vector<double> x4;
    std::vector<double> y4;
    for(double d: m1.freq) {
        x4.push_back(d);
    }
    for(auto d: m1.modulated_f) {
        y4.push_back(abs(d));
    }



    mpFXYVector *vectorLayer4 = new mpFXYVector("Modulated signal(freq)");
    vectorLayer4->SetData(std::move(x4),std::move( y4));
    vectorLayer4->SetContinuity(true);
    vectorLayer4->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer4->SetDrawOutsideMargins(false);
    sf_plot->AddLayer(vectorLayer4);
    sf_plot->Fit();


}

void MyFrame::OnSliderChange(wxCommandEvent& event) {
    switch(event.GetId()) {
        case SLIDER_FM1:
            this->fm1 = this->fm1_slider->GetValue();
            break;
        case SLIDER_FM2:
            this->fm2 = this->fm2_slider->GetValue();
            break;
        case SLIDER_FM3:
            this->fm3 = this->fm3_slider->GetValue();
            break;

    }
}

void MyFrame::CleanAllPlots() {
    m_plot->DelAllPlot(mpDeleteAction::mpYesDelete);
    mf_plot->DelAllPlot(mpDeleteAction::mpYesDelete);
    s_plot->DelAllPlot(mpDeleteAction::mpYesDelete);
    sf_plot->DelAllPlot(mpDeleteAction::mpYesDelete);
}

void MyFrame::OnCheckBoxChange(wxCommandEvent &e) {
    this->add_noise = this->addNoise->GetValue();
}