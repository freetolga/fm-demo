#include <wx/brush.h>
#include <wx/event.h>
#include <wx/gdicmn.h>
#include <wx/print.h>
#include <wx/sizer.h>
#include <valarray>
#include <wx/wx.h>
#include <mathplot.h>

#include "block1.hpp"
#include "block2.hpp"
#include "wx/arrstr.h"
#include "wx/filedlg.h"
#include "wx/string.h"

enum
{
    ID_Hello = 1,
    BUTTON_plot = wxID_HIGHEST + 1,
    BUTTON_load,
    BUTTON_save,
    BUTTON_save2,
    BUTTON_demodulate,
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
    void OnLoadButton(wxCommandEvent& event);
    void OnSaveButton(wxCommandEvent& event);
    void OnDemodulateButton(wxCommandEvent& event);
    void CleanAllPlots(void);
    void OnSave2Button(wxCommandEvent &e);
    wxBoxSizer *main_sizer = new wxBoxSizer(wxHORIZONTAL);
    wxGridSizer *plots_sizer = new wxGridSizer(3, 2, 0 ,0);
    wxBoxSizer *controls_sizer = new wxBoxSizer(wxVERTICAL);
    wxButton *plotButton = new wxButton(this, BUTTON_plot, "Generate Controlled");
    wxButton *loadButton = new wxButton(this, BUTTON_load, "Load audio file");
    wxButton *saveButton = new wxButton(this, BUTTON_save, "Save original signal");
    wxButton *save2Button = new wxButton(this, BUTTON_save2, "Save modulated signal");
    wxButton *demodulateButton = new wxButton(this, BUTTON_demodulate, "Demodulate Workspace");
    mpWindow *m_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *mf_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *s_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *sf_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *d_plot = new mpWindow(this, -1, wxDefaultPosition);
    mpWindow *df_plot = new mpWindow(this, -1, wxDefaultPosition);
    wxSlider *fm1_slider = new wxSlider(this, SLIDER_FM1, 20, 1, 2000);
    wxSlider *fm2_slider =  new wxSlider(this, SLIDER_FM2, 200, 1, 2000);
    wxSlider *fm3_slider = new wxSlider(this, SLIDER_FM3, 2000, 1, 2000);
    wxCheckBox *addNoise = new wxCheckBox(this, BUTTON_ADD_NOISE, "Add Noise?");
    double fm1 = 20;
    double fm2 = 200;
    double fm3 = 2000;
    double fc = 10000;
    bool add_noise = false;
    message_signal m1 = message_signal(this->fm1, this->fm2, this->fm3, this->fc);
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

    controls_sizer->Add(loadButton, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(plotButton, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(fm1_slider, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(fm2_slider, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(fm3_slider, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(saveButton, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(save2Button, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(addNoise, 0, wxSHAPED | wxCENTER, 0);
    controls_sizer->Add(demodulateButton, 0, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(m_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(mf_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(s_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(sf_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(d_plot, 1, wxSHAPED | wxCENTER, 0);
    plots_sizer->Add(df_plot, 1, wxSHAPED | wxCENTER, 0);
    main_sizer->Add(controls_sizer, 1, wxEXPAND | wxCENTER, 0);
    main_sizer->Add(plots_sizer, 10, wxEXPAND | wxCENTER, 0);
    SetSizer(main_sizer); 
    SetAutoLayout(TRUE);

    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_BUTTON, &MyFrame::OnPlot, this, BUTTON_plot);
    Bind(wxEVT_BUTTON, &MyFrame::OnLoadButton, this, BUTTON_load);
    Bind(wxEVT_BUTTON, &MyFrame::OnSaveButton, this, BUTTON_save);
    Bind(wxEVT_BUTTON, &MyFrame::OnSave2Button, this, BUTTON_save2);
    Bind(wxEVT_BUTTON, &MyFrame::OnDemodulateButton, this, BUTTON_demodulate);
    Bind(wxEVT_CHECKBOX, &MyFrame::OnCheckBoxChange, this, BUTTON_ADD_NOISE);
    Bind(wxEVT_SLIDER, &MyFrame::OnSliderChange, this);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnPlot(wxCommandEvent& event) {
    this->CleanAllPlots();

    m1 = message_signal(this->fm1, this->fm2, this->fm3, this->fc);
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
    m_plot->AddLayer(new mpTitle("Message Signal(time)"));
    m_plot->AddLayer(new mpInfoCoords());
    m_plot->Fit();
    m_plot->SetMargins(0, 0, 0,0);

    m1.take_fft_message();

    std::vector<double> x2;
    std::vector<double> y2;
    // sidenote: how for-range works in c++11 and later
    // for(typename iteration_variable : iterable  )
    // read as: for(typename variable in iterable)
    // : means "in keyword" in python
    for(double d: m1.freq) {
        x2.push_back(d);
    }
    for(std::complex<double> d: m1.original_f) {
        y2.push_back(abs(d));
    }


    mpFXYVector *vectorLayer2 = new mpFXYVector("Original signal(freq)");
    vectorLayer2->SetData(std::move(x2), std::move(y2));
    vectorLayer2->SetContinuity(true);
    vectorLayer2->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer2->SetDrawOutsideMargins(false);
    mf_plot->AddLayer(vectorLayer2);
    mf_plot->AddLayer(new mpTitle("Message Signal(freq)"));
    mf_plot->AddLayer(new mpInfoCoords());
    mf_plot->Fit();
    mf_plot->SetMargins(0, 0, 0,0);


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
    s_plot->AddLayer(new mpTitle("Modulated Signal(time)"));
    s_plot->AddLayer(new mpInfoCoords());
    s_plot->Fit();
    s_plot->SetMargins(0, 0, 0,0);


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
    sf_plot->AddLayer(new mpTitle("Modulated Signal(freq)"));
    sf_plot->AddLayer(new mpInfoCoords());
    sf_plot->Fit();
    sf_plot->SetMargins(0, 0, 0,0);


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

void MyFrame::OnLoadButton(wxCommandEvent& e) {
    wxString filename = wxFileSelector("Choose a file to open", wxEmptyString, wxEmptyString, ".wav");
    if ( !filename.empty() ) {
           m1 = message_signal(filename.ToStdString());
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
           m_plot->AddLayer(new mpTitle("Message Signal(time)"));
           m_plot->AddLayer(new mpInfoCoords());
           m_plot->Fit();
           m_plot->SetMargins(0, 0, 0,0);

           m1.take_fft_message();

           std::vector<double> x2;
           std::vector<double> y2;
           // sidenote: how for-range works in c++11 and later
           // for(typename iteration_variable : iterable  )
           // read as: for(typename variable in iterable)
           // : means "in keyword" in python
           for(double d: m1.freq) {
               x2.push_back(d);
           }
           for(std::complex<double> d: m1.original_f) {
               y2.push_back(abs(d));
           }


           mpFXYVector *vectorLayer2 = new mpFXYVector("Original signal(freq)");
           vectorLayer2->SetData(std::move(x2), std::move(y2));
           vectorLayer2->SetContinuity(true);
           vectorLayer2->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
           vectorLayer2->SetDrawOutsideMargins(false);
           mf_plot->AddLayer(vectorLayer2);
           mf_plot->AddLayer(new mpTitle("Message Signal(freq)"));
           mf_plot->AddLayer(new mpInfoCoords());
           mf_plot->Fit();
           mf_plot->SetMargins(0, 0, 0,0);


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
           s_plot->AddLayer(new mpTitle("Modulated Signal(time)"));
           s_plot->AddLayer(new mpInfoCoords());
           s_plot->Fit();
           s_plot->SetMargins(0, 0, 0,0);


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
           sf_plot->AddLayer(new mpTitle("Modulated Signal(freq)"));
           sf_plot->AddLayer(new mpInfoCoords());
           sf_plot->Fit();
           sf_plot->SetMargins(0, 0, 0,0);

    }

}

void MyFrame::OnSaveButton(wxCommandEvent &e) {
    std::string save_path = wxFileSelector("Choose where to save the message", wxEmptyString, "orignial.wav", ".wav", "*.wav", wxFD_SAVE).ToStdString();
    m1.save_original(save_path);
}

void MyFrame::OnSave2Button(wxCommandEvent &e) {
    std::string save_path = wxFileSelector("Choose where to save the modulated signal", wxEmptyString, "modulated.wav", ".wav", "*.wav", wxFD_SAVE).ToStdString();
    m1.save_modulated(save_path);
}

void MyFrame::OnDemodulateButton(wxCommandEvent &e) {
    d_plot->DelAllLayers(mpDeleteAction::mpNoDelete);
    df_plot->DelAllLayers(mpDeleteAction::mpNoDelete);
    block2 m2(m1.modulated, m1.time, m1.kf, m1.fs, m1.fc);

    m2.hilbert_demodulate();

    std::vector<double> x5;
    std::vector<double> y5;
    for(double d: m2.time) {
        x5.push_back(d);
    }
    for(double d: m2.output) {
        y5.push_back(d);
    }
    mpFXYVector *vectorLayer5 = new mpFXYVector("Recovered Signal(time)");
    vectorLayer5->SetData(std::move(x5),std::move( y5));
    vectorLayer5->SetContinuity(true);
    vectorLayer5->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer5->SetDrawOutsideMargins(false);

    d_plot->AddLayer(vectorLayer5);
    d_plot->AddLayer(new mpTitle("Recovered Signal(time)"));
    d_plot->AddLayer(new mpInfoCoords());
    d_plot->Fit();
    d_plot->SetMargins(0, 0, 0,0);

    
    m2.take_fft_output();
    std::vector<double> x6;
    std::vector<double> y6;
    for(auto d: m2.freq) {
        x6.push_back(d);
    }
    for(auto d: m2.output_f) {
        y6.push_back(abs(d));
    }
    mpFXYVector *vectorLayer6 = new mpFXYVector("Recovered Signal(freq)");
    vectorLayer6->SetData(std::move(x6),std::move( y6));
    vectorLayer6->SetContinuity(true);
    vectorLayer6->SetPen(wxPen(*wxBLUE, 2, wxPENSTYLE_SOLID));
    vectorLayer6->SetDrawOutsideMargins(false);
    df_plot->AddLayer(vectorLayer6);
    df_plot->AddLayer(new mpTitle("Recovered Signal(freq)"));
    df_plot->AddLayer(new mpInfoCoords());
    df_plot->Fit();
    df_plot->SetMargins(0, 0, 0,0);

}