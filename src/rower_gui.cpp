///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "rower_gui.h"

///////////////////////////////////////////////////////////////////////////

rowerBase::rowerBase(wxWindow* parent, wxWindowID id, const wxString& title,
                     const wxPoint& pos, const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxSize(-1, -1), wxSize(-1, -1));
  this->SetFont(wxFont(10, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                       wxFONTWEIGHT_BOLD, false, wxT("Arial")));
  this->SetBackgroundColour(wxColour(201, 201, 201));

  wxBoxSizer* bSizer10;
  bSizer10 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* bSizer14;
  bSizer14 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* bSizer92;
  bSizer92 = new wxBoxSizer(wxHORIZONTAL);

  m_gaugeRudderPort = new wxGauge(this, wxID_ANY, 30, wxDefaultPosition,
                                  wxSize(-1, -1), wxGA_HORIZONTAL);
  m_gaugeRudderPort->SetValue(0);
  m_gaugeRudderPort->SetForegroundColour(wxColour(255, 0, 0));
  m_gaugeRudderPort->SetBackgroundColour(wxColour(255, 0, 0));

  bSizer92->Add(m_gaugeRudderPort, 1,
                wxALIGN_RIGHT | wxBOTTOM | wxEXPAND | wxTOP, 5);

  m_gaugeRudderStbd = new wxGauge(this, wxID_ANY, 30, wxDefaultPosition,
                                  wxSize(-1, -1), wxGA_HORIZONTAL);
  m_gaugeRudderStbd->SetValue(0);
  m_gaugeRudderStbd->SetForegroundColour(wxColour(0, 255, 0));
  m_gaugeRudderStbd->SetBackgroundColour(wxColour(0, 255, 0));

  bSizer92->Add(m_gaugeRudderStbd, 1, wxBOTTOM | wxEXPAND | wxTOP, 5);

  bSizer14->Add(bSizer92, 0, wxEXPAND, 5);

  wxBoxSizer* bSizer13;
  bSizer13 = new wxBoxSizer(wxHORIZONTAL);

  m_SliderRudder = new wxSlider(this, wxID_ANY, 30, 0, 60, wxDefaultPosition,
                                wxSize(300, -1),
                                wxSL_AUTOTICKS | wxSL_BOTH | wxSL_HORIZONTAL);
  m_SliderRudder->SetForegroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
  m_SliderRudder->SetBackgroundColour(wxColour(201, 201, 201));
  m_SliderRudder->SetToolTip(_("Tiller Control"));

  bSizer13->Add(m_SliderRudder, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  bSizer14->Add(bSizer13, 0, wxEXPAND, 5);

  wxBoxSizer* bSizer6;
  bSizer6 = new wxBoxSizer(wxVERTICAL);

  wxBoxSizer* bSizer131;
  bSizer131 = new wxBoxSizer(wxHORIZONTAL);

  bSizer131->Add(0, 0, 1, wxEXPAND, 5);

  m_textCtrlRudderPort =
      new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                     wxDefaultSize, wxTE_READONLY | wxTE_CENTER);
  m_textCtrlRudderPort->SetFont(wxFont(10, wxFONTFAMILY_SWISS,
                                       wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
                                       false, wxT("Arial")));

  bSizer131->Add(m_textCtrlRudderPort, 0,
                 wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 0);

  m_buttonMid =
      new wxButton(this, wxID_ANY, _("|"), wxDefaultPosition, wxDefaultSize, 0);
  m_buttonMid->SetFont(wxFont(11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_BOLD, false, wxT("Arial")));
  m_buttonMid->SetBackgroundColour(wxColour(250, 203, 107));
  m_buttonMid->SetToolTip(_("Midships"));

  bSizer131->Add(m_buttonMid, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);

  m_textCtrlRudderStbd =
      new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition,
                     wxDefaultSize, wxTE_READONLY | wxTE_CENTER);
  m_textCtrlRudderStbd->SetFont(wxFont(10, wxFONTFAMILY_SWISS,
                                       wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
                                       false, wxT("Arial")));

  bSizer131->Add(m_textCtrlRudderStbd, 0,
                 wxALIGN_BOTTOM | wxALIGN_CENTER_VERTICAL, 0);

  bSizer131->Add(0, 0, 1, wxEXPAND, 5);

  bSizer6->Add(bSizer131, 1, wxEXPAND, 5);

  wxBoxSizer* bSizer16;
  bSizer16 = new wxBoxSizer(wxHORIZONTAL);

  bSizer16->Add(0, 0, 1, wxEXPAND, 5);

  m_bpPlay = new wxButton(this, wxID_ANY, _("Start"), wxDefaultPosition,
                          wxSize(-1, -1), 0);
  m_bpPlay->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                           wxFONTWEIGHT_BOLD, false, wxT("Arial")));
  m_bpPlay->SetForegroundColour(
      wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
  m_bpPlay->SetBackgroundColour(wxColour(0, 255, 0));

  bSizer16->Add(m_bpPlay, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);

  m_bpStop = new wxButton(this, wxID_ANY, _("Stop"), wxDefaultPosition,
                          wxDefaultSize, 0);
  m_bpStop->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                           wxFONTWEIGHT_BOLD, false, wxT("Arial")));
  m_bpStop->SetBackgroundColour(wxColour(255, 0, 0));

  bSizer16->Add(m_bpStop, 0, wxALIGN_CENTER_VERTICAL | wxALL, 2);

  bSizer16->Add(25, 0, 0, wxEXPAND, 5);

  m_buttonPause = new wxToggleButton(this, wxID_ANY, _("Pause"),
                                     wxDefaultPosition, wxDefaultSize, 0);
  m_buttonPause->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_NORMAL, false, wxT("Arial")));
  m_buttonPause->SetBackgroundColour(wxColour(0, 255, 0));
  m_buttonPause->SetToolTip(_("MOB"));

  bSizer16->Add(m_buttonPause, 0, wxALL, 5);

  bSizer16->Add(0, 0, 0, wxEXPAND, 5);

  bSizer6->Add(bSizer16, 1, wxEXPAND, 5);

  wxBoxSizer* bSizer7;
  bSizer7 = new wxBoxSizer(wxHORIZONTAL);

  bSizer7->Add(0, 0, 1, wxEXPAND, 5);

  m_staticTextHeading = new wxStaticText(this, wxID_ANY, _("  Heading:  "),
                                         wxDefaultPosition, wxDefaultSize, 0);
  m_staticTextHeading->Wrap(-1);
  m_staticTextHeading->SetFont(wxFont(12, wxFONTFAMILY_SWISS,
                                      wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD,
                                      false, wxT("Arial")));

  bSizer7->Add(m_staticTextHeading, 0, wxALIGN_CENTER_VERTICAL, 5);

  m_stHeading = new wxStaticText(this, wxID_ANY, _("000"), wxDefaultPosition,
                                 wxDefaultSize, 0);
  m_stHeading->Wrap(-1);
  m_stHeading->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                              wxFONTWEIGHT_BOLD, false, wxT("Arial")));

  bSizer7->Add(m_stHeading, 0, wxALIGN_CENTER_VERTICAL, 5);

  m_staticTextKnots = new wxStaticText(this, wxID_ANY, _("       Speed: "),
                                       wxDefaultPosition, wxDefaultSize, 0);
  m_staticTextKnots->Wrap(-1);
  m_staticTextKnots->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                    wxFONTWEIGHT_BOLD, false, wxT("Arial")));

  bSizer7->Add(m_staticTextKnots, 0, wxALIGN_CENTER_VERTICAL, 5);

  m_stSpeed = new wxStaticText(this, wxID_ANY, _("00.0"), wxDefaultPosition,
                               wxDefaultSize, 0);
  m_stSpeed->Wrap(-1);
  m_stSpeed->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_BOLD, false, wxT("Arial")));

  bSizer7->Add(m_stSpeed, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_staticText81 = new wxStaticText(this, wxID_ANY, _("        Kts"),
                                    wxDefaultPosition, wxDefaultSize, 0);
  m_staticText81->Wrap(-1);
  m_staticText81->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                 wxFONTWEIGHT_BOLD, false, wxT("Arial")));

  bSizer7->Add(m_staticText81, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);

  bSizer7->Add(0, 0, 1, wxEXPAND, 5);

  bSizer6->Add(bSizer7, 1,
               wxEXPAND | wxALIGN_BOTTOM | wxALIGN_CENTER_HORIZONTAL, 5);

  bSizer14->Add(bSizer6, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

  bSizer10->Add(bSizer14, 0, wxEXPAND, 5);

  wxBoxSizer* bSizer12;
  bSizer12 = new wxBoxSizer(wxHORIZONTAL);

  m_staticText7 = new wxStaticText(this, wxID_ANY, _("0"), wxDefaultPosition,
                                   wxDefaultSize, 0);
  m_staticText7->Wrap(-1);
  m_staticText7->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_BOLD, false, wxT("Arial")));

  bSizer12->Add(m_staticText7, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  m_SliderSpeed = new wxSlider(this, wxID_ANY, 0, 0, 100, wxDefaultPosition,
                               wxSize(-1, -1), 0);
  m_SliderSpeed->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_BOLD, false, wxT("Arial")));
  m_SliderSpeed->SetForegroundColour(wxColour(0, 0, 0));
  m_SliderSpeed->SetBackgroundColour(wxColour(201, 201, 201));
  m_SliderSpeed->SetToolTip(_("Speed Control"));

  bSizer12->Add(m_SliderSpeed, 1, wxALIGN_CENTER_VERTICAL, 5);

  m_staticText8 = new wxStaticText(this, wxID_ANY, _("100"), wxDefaultPosition,
                                   wxDefaultSize, 0);
  m_staticText8->Wrap(-1);
  m_staticText8->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                wxFONTWEIGHT_BOLD, false, wxT("Arial")));

  bSizer12->Add(m_staticText8, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  bSizer10->Add(bSizer12, 0, wxEXPAND, 5);

  wxFlexGridSizer* fgSizer82;
  fgSizer82 = new wxFlexGridSizer(2, 4, 0, 0);
  fgSizer82->AddGrowableCol(0);
  fgSizer82->AddGrowableCol(1);
  fgSizer82->AddGrowableCol(2);
  fgSizer82->AddGrowableCol(3);
  fgSizer82->AddGrowableRow(0);
  fgSizer82->AddGrowableRow(1);
  fgSizer82->SetFlexibleDirection(wxVERTICAL);
  fgSizer82->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_buttonStandby = new wxButton(this, wxID_ANY, _("Stby"), wxDefaultPosition,
                                 wxDefaultSize, 0);
  m_buttonStandby->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                                  wxFONTWEIGHT_BOLD, false, wxT("Arial")));
  m_buttonStandby->SetBackgroundColour(wxColour(0, 255, 0));
  m_buttonStandby->SetToolTip(_("Hand Steering"));
  m_buttonStandby->SetMinSize(wxSize(50, -1));

  fgSizer82->Add(m_buttonStandby, 1, wxALL | wxEXPAND, 2);

  m_button7 = new wxButton(this, wxID_ANY, _("Follow"), wxDefaultPosition,
                           wxDefaultSize, 0);
  m_button7->SetFont(wxFont(12, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL,
                            wxFONTWEIGHT_NORMAL, false, wxT("Arial")));
  m_button7->SetMinSize(wxSize(50, -1));

  fgSizer82->Add(m_button7, 1, wxALL | wxEXPAND, 2);

  bSizer10->Add(fgSizer82, 0, wxEXPAND, 5);

  this->SetSizer(bSizer10);
  this->Layout();
  bSizer10->Fit(this);
  m_timer1.SetOwner(this, m_timer1.GetId());

  this->Centre(wxBOTH);

  // Connect Events
  this->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(rowerBase::OnClose));
  this->Connect(wxEVT_LEFT_DCLICK,
                wxMouseEventHandler(rowerBase::OnDLeftClick));
  m_buttonMid->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(rowerBase::OnMidships), NULL,
                       this);
  m_bpPlay->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                    wxCommandEventHandler(rowerBase::OnStart), NULL, this);
  m_bpStop->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                    wxCommandEventHandler(rowerBase::OnStop), NULL, this);
  m_buttonPause->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                         wxCommandEventHandler(rowerBase::OnPause), NULL, this);
  m_buttonStandby->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                           wxCommandEventHandler(rowerBase::OnStandby), NULL,
                           this);
  m_button7->Connect(wxEVT_COMMAND_BUTTON_CLICKED,
                     wxCommandEventHandler(rowerBase::OnFollow), NULL, this);
  this->Connect(wxID_ANY, wxEVT_TIMER, wxTimerEventHandler(rowerBase::OnTimer));
}

rowerBase::~rowerBase() {
  // Disconnect Events
  this->Disconnect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(rowerBase::OnClose));
  this->Disconnect(wxEVT_LEFT_DCLICK,
                   wxMouseEventHandler(rowerBase::OnDLeftClick));
  m_buttonMid->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                          wxCommandEventHandler(rowerBase::OnMidships), NULL,
                          this);
  m_bpPlay->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(rowerBase::OnStart), NULL, this);
  m_bpStop->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                       wxCommandEventHandler(rowerBase::OnStop), NULL, this);
  m_buttonPause->Disconnect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,
                            wxCommandEventHandler(rowerBase::OnPause), NULL,
                            this);
  m_buttonStandby->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                              wxCommandEventHandler(rowerBase::OnStandby), NULL,
                              this);
  m_button7->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED,
                        wxCommandEventHandler(rowerBase::OnFollow), NULL, this);
  this->Connect(wxID_ANY, wxEVT_TIMER,
                wxTimerEventHandler(rowerBase::OnTimer));
}

rowerPreferences::rowerPreferences(wxWindow* parent, wxWindowID id,
                                   const wxString& title, const wxPoint& pos,
                                   const wxSize& size, long style)
    : wxDialog(parent, id, title, pos, size, style) {
  this->SetSizeHints(wxDefaultSize, wxDefaultSize);

  wxStaticBoxSizer* sbSizer1;
  sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("AIS")),
                                  wxVERTICAL);

  wxFlexGridSizer* fgSizer2;
  fgSizer2 = new wxFlexGridSizer(0, 1, 0, 0);
  fgSizer2->SetFlexibleDirection(wxBOTH);
  fgSizer2->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

  m_cbTransmitAis =
      new wxCheckBox(sbSizer1->GetStaticBox(), wxID_ANY, _("Transmit"),
                     wxDefaultPosition, wxDefaultSize, 0);
  fgSizer2->Add(m_cbTransmitAis, 0, wxALL, 5);

  m_cbAisToFile =
      new wxCheckBox(sbSizer1->GetStaticBox(), wxID_ANY, _("Save to file"),
                     wxDefaultPosition, wxDefaultSize, 0);
  fgSizer2->Add(m_cbAisToFile, 0, wxALL, 5);

  sbSizer1->Add(fgSizer2, 1, wxEXPAND, 5);

  wxStaticBoxSizer* sbSizer2;
  sbSizer2 = new wxStaticBoxSizer(
      new wxStaticBox(sbSizer1->GetStaticBox(), wxID_ANY, _("MMSI")),
      wxVERTICAL);

  m_textCtrlMMSI =
      new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, _("123456789"),
                     wxDefaultPosition, wxDefaultSize, 0);
  sbSizer2->Add(m_textCtrlMMSI, 0, wxALL, 5);

  sbSizer1->Add(sbSizer2, 1, wxEXPAND, 5);

  wxStaticBoxSizer* sbSizer4;
  sbSizer4 = new wxStaticBoxSizer(
      new wxStaticBox(sbSizer1->GetStaticBox(), wxID_ANY, _("NMEA")),
      wxVERTICAL);

  m_cbNMEAToFile =
      new wxCheckBox(sbSizer4->GetStaticBox(), wxID_ANY, _("Save to file"),
                     wxDefaultPosition, wxDefaultSize, 0);
  sbSizer4->Add(m_cbNMEAToFile, 0, wxALL, 5);

  sbSizer1->Add(sbSizer4, 1, wxEXPAND, 5);

  m_sdbSizer1 = new wxStdDialogButtonSizer();
  m_sdbSizer1OK = new wxButton(sbSizer1->GetStaticBox(), wxID_OK);
  m_sdbSizer1->AddButton(m_sdbSizer1OK);
  m_sdbSizer1Cancel = new wxButton(sbSizer1->GetStaticBox(), wxID_CANCEL);
  m_sdbSizer1->AddButton(m_sdbSizer1Cancel);
  m_sdbSizer1->Realize();

  sbSizer1->Add(m_sdbSizer1, 1, wxEXPAND, 5);

  this->SetSizer(sbSizer1);
  this->Layout();
  sbSizer1->Fit(this);

  this->Centre(wxBOTH);
}

rowerPreferences::~rowerPreferences() {}
