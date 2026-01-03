/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose: rower Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Mike Rossiter                                   *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include <stdio.h>

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <wx/progdlg.h>
#include <wx/textfile.h>
#include <wx/wx.h>
#include "wx/tglbtn.h"

#include "qtstylesheet.h"
#include "rower_gui_impl.h"
#include "rower_pi.h"

#ifdef __ANDROID__
wxWindow* g_Window;
#endif

#include <wx/listimpl.cpp>
WX_DEFINE_LIST(Plugin_WaypointExList);

class GribRecordSet;

void assign(char* dest, char* arrTest2) { strcpy(dest, arrTest2); }

#define BUFSIZE 0x10000

Dlg::Dlg(wxWindow* parent, wxWindowID id, const wxString& title,
         const wxPoint& pos, const wxSize& size, long style)
    :rowerBase(parent, id, title, pos, size, style) {
  this->Fit();
  dbg = false;  // for debug output set to true
  initLat = 0;
  initLon = 0;
  m_interval = 500;
  m_bUseSetTime = false;
  m_bUseStop = true;
  m_bUsePause = false;
  m_sNmeaTime = wxEmptyString;

  m_bUsingFollow = false;
  m_browerHasStarted = false;

  m_bGotAPB = false;
  row_vtg = "0";

#ifdef __ANDROID__

  m_binResize = false;

  g_Window = this;
  GetHandle()->setStyleSheet(qtStyleSheet);
  Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(Dlg::OnMouseEvent));
  Connect(wxEVT_LEFT_UP, wxMouseEventHandler(Dlg::OnMouseEvent));

  Connect(wxEVT_MOTION, wxMouseEventHandler(Dlg::OnMouseEvent));

#endif

  wxFileConfig* pConf = GetOCPNConfigObject();

  if (pConf) {
    pConf->SetPath("/PlugIns/rower_pi");

    pConf->Read("rowerUseAis", &m_bUseAis, 0);
    pConf->Read("rowerUseFile", &m_bUseFile, 0);
    pConf->Read("rowerMMSI", &m_tMMSI, "123456789");
    pConf->Read("rowerUseNMEA", &m_bUseNMEA, 0);
  }
}

#ifdef __ANDROID__
wxPoint g_startPos;
wxPoint g_startMouse;
wxPoint g_mouse_pos_screen;

void Dlg::OnPopupClick(wxCommandEvent& evt) {
  switch (evt.GetId()) {
    case ID_SOMETHING:
      m_binResize = true;
      break;
      // case ID_SOMETHING_ELSE:
      //   break;
  }
}

void Dlg::OnDLeftClick(wxMouseEvent& event) {
  wxMenu mnu;
  mnu.Append(ID_SOMETHING, "Resize...");
  // mnu.Append(ID_SOMETHING_ELSE, "Do something else");
  mnu.Connect(wxEVT_COMMAND_MENU_SELECTED,
              wxCommandEventHandler(Dlg::OnPopupClick), NULL, this);
  PopupMenu(&mnu);
}

void Dlg::OnMouseEvent(wxMouseEvent& event) {
  if (m_binResize) {
    wxSize currentSize = g_Window->GetSize();
    wxSize par_size = GetOCPNCanvasWindow()->GetClientSize();
    wxPoint par_pos = g_Window->GetPosition();
    if (event.LeftDown()) {
      m_resizeStartPoint = event.GetPosition();
      m_resizeStartSize = currentSize;
      m_binResize2 = true;
    }

    if (m_binResize2) {
      if (event.Dragging()) {
        wxPoint p = event.GetPosition();

        wxSize dragSize = m_resizeStartSize;

        dragSize.y = p.y;  //  - m_resizeStartPoint.y;
        dragSize.x = p.x;  //  - m_resizeStartPoint.x;
        ;
        /*
        if ((par_pos.y + dragSize.y) > par_size.y)
            dragSize.y = par_size.y - par_pos.y;

        if ((par_pos.x + dragSize.x) > par_size.x)
            dragSize.x = par_size.x - par_pos.x;
*/
        // not too small
        dragSize.x = wxMax(dragSize.x, 150);
        dragSize.y = wxMax(dragSize.y, 150);

        int x = wxMax(0, m_resizeStartPoint.x);
        int y = wxMax(0, m_resizeStartPoint.y);
        int xmax = ::wxGetDisplaySize().x - GetSize().x;
        x = wxMin(x, xmax);
        int ymax =
            ::wxGetDisplaySize().y - (GetSize().y);  // Some fluff at the bottom
        y = wxMin(y, ymax);

        g_Window->Move(x, y);
      }
      if (event.LeftUp()) {
        wxPoint p = event.GetPosition();

        wxSize dragSize = m_resizeStartSize;

        dragSize.y = p.y;
        dragSize.x = p.x;

        // not too small
        dragSize.x = wxMax(dragSize.x, 150);
        dragSize.y = wxMax(dragSize.y, 150);

        g_Window->SetSize(dragSize);

        m_binResize = false;
        m_binResize2 = false;
      }
    }
  } else {
    if (event.Dragging()) {
      m_resizeStartPoint = event.GetPosition();
      int x = wxMax(0, m_resizeStartPoint.x);
      int y = wxMax(0, m_resizeStartPoint.y);
      int xmax = ::wxGetDisplaySize().x - GetSize().x;
      x = wxMin(x, xmax);
      int ymax =
          ::wxGetDisplaySize().y - (GetSize().y);  // Some fluff at the bottom
      y = wxMin(y, ymax);

      g_Window->Move(x, y);
    }
  }
}

#endif  // End of Android functions for move/resize

void Dlg::OnTimer(wxTimerEvent& event) { Notify(); }

void Dlg::SetNextStep(double inLat, double inLon, double inDir, double inSpd,
                      double& outLat, double& outLon) {
  PositionBearingDistanceMercator_Plugin(inLat, inLon, inDir, inSpd, &stepLat,
                                         &stepLon);
}

void Dlg::SetFollowStep(double inLat, double inLon, double inDir, double inSpd,
                        double& outLat, double& outLon) {
  double myBrg;
  PlugIn_Waypoint_Ex* myWaypoint;

  PositionBearingDistanceMercator_Plugin(inLat, inLon, inDir, inSpd, &stepLat,
                                         &stepLon);
  DistanceBearingMercator_Plugin(nextLat, nextLon, stepLat, stepLon, &myBrg,
                                 &myDist);

  if (myDist <= initSpd / 7200) {
    stepLat = nextLat;
    stepLon = nextLon;

    nextRoutePointIndex++;

    if (nextRoutePointIndex > (countRoutePoints - 1)) {
      SetStop();
      return;
    }

    for (size_t n = 0; n < theWaypoints.size(); n++) {
      if (static_cast<int>(n) == nextRoutePointIndex) {
        myWaypoint = theWaypoints[n];
        nextLat = myWaypoint->m_lat;
        nextLon = myWaypoint->m_lon;
      }
    }

    DistanceBearingMercator_Plugin(nextLat, nextLon, stepLat, stepLon,
                                   &followDir, &myDist);

    PositionBearingDistanceMercator_Plugin(stepLat, stepLon, followDir, inSpd,
                                           &stepLat, &stepLon);

    myDir = followDir;
  }
}

void Dlg::OnStart(wxCommandEvent& event) { StartDriving(); }

void Dlg::StartDriving() {
  if (initLat == 0.0) {
    wxMessageBox(_("Please right-click and choose vessel start position"));
    return;
  }

  bool bIsDigits = m_tMMSI.IsNumber();

  if (m_tMMSI.Len() != 9 || !bIsDigits) {
    wxMessageBox(_("MMSI must be nine digits\nEdit using Preferences"));
    return;
  }

  if (m_bUseFile) {
    wxString caption = wxT("Choose a file");
    wxString wildcard = wxT("Text files (*.txt)|*.txt|All files (*.*)|*.*");

    wxString s = "/";
    const char* pName = "rower_pi";
    wxString defaultDir = GetPluginDataDir(pName) + s + "data" + s;

    wxString defaultFilename = wxEmptyString;
    wxFileDialog filedlg(this->m_parent, caption, defaultDir, defaultFilename,
                         wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (filedlg.ShowModal() != wxID_OK) {
      wxMessageBox(_("rower has been stopped"));
      return;
    } else {
      nmeafile = new wxTextFile(filedlg.GetPath());
      nmeafile->Open();
      nmeafile->Clear();
    }
  }

  if (m_bUseNMEA && !m_browerHasStarted) {
    wxString caption = wxT("Choose a file");
    wxString wildcard = wxT("Text files (*.txt)|*.txt|All files (*.*)|*.*");

    wxString s = "/";
    const char* pName = "rower_pi";
    wxString defaultDir = GetPluginDataDir(pName) + s + "data" + s;

    wxString defaultFilename = wxEmptyString;
    wxFileDialog filedlg2(this->m_parent, caption, defaultDir, defaultFilename,
                          wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (filedlg2.ShowModal() != wxID_OK) {
      wxMessageBox(_("rower has been stopped"));
      return;
    } else {
      nmeastream = new wxTextFile(filedlg2.GetPath());
      nmeastream->Open();
      nmeastream->Clear();
    }
  }

  m_browerHasStarted = true;

  m_textCtrlRudderStbd->SetValue("");
  m_textCtrlRudderPort->SetValue("");
  initSpd = 0; // 5 knots

  if (!m_bUsingFollow) {
    wxString myHeading = m_stHeading->GetLabel();
    myHeading.ToDouble(&initDir);
    myDir = initDir;

  } else {
    myDir = followDir;
  }

  dt = dt.Now();
  VTG = createVTGSentence(initSpd, initDir);
  m_interval = 500;
  m_timer->Start(m_interval, wxTIMER_CONTINUOUS);  // start timer
  m_bAuto = false;

  myAIS = new AisMaker();

}

void Dlg::OnStop(wxCommandEvent& event) { SetStop(); }

void Dlg::SetStop() {
  if (m_timer->IsRunning()) m_timer->Stop();

  wxMessageBox(_("Vessel stopped"));

  m_SliderSpeed->SetValue(0);
  m_SliderRudder->SetValue(30);
  m_textCtrlRudderStbd->SetValue("");
  m_textCtrlRudderPort->SetValue("");

  m_interval = m_timer->GetInterval();
  m_bUseSetTime = false;
  m_bUseStop = true;
  m_bUsingFollow = false;
  m_browerHasStarted = false;
  m_buttonStandby->SetBackgroundColour(wxColour(0, 255, 0));

  if (m_bUseFile) {
    nmeafile->Write();
    nmeafile->Close();
  }
  if (m_bUseNMEA) {
    nmeastream->Write();
    nmeastream->Close();
  }
  initSpd = 0.0;
  m_stSpeed->SetLabel(wxString::Format("%3.1f", initSpd));

  Refresh();
}

void Dlg::OnMidships(wxCommandEvent& event) { m_SliderRudder->SetValue(30); }

void Dlg::OnMinus10(wxCommandEvent& event) {
  m_bAuto = false;
  GoToStandby();
  myDir -= 10;
  wxString mystring = wxString::Format("%03.0f", myDir);
  m_stHeading->SetLabel(mystring);
}

void Dlg::OnPlus10(wxCommandEvent& event) {
  m_bAuto = false;
  GoToStandby();
  myDir += 10;
  wxString mystring = wxString::Format("%03.0f", myDir);
  m_stHeading->SetLabel(mystring);
}

void Dlg::OnMinus1(wxCommandEvent& event) {
  m_bAuto = false;
  GoToStandby();
  myDir -= 1;
  wxString mystring = wxString::Format("%03.0f", myDir);
  m_stHeading->SetLabel(mystring);
}

void Dlg::OnPlus1(wxCommandEvent& event) {
  m_bAuto = false;
  GoToStandby();
  myDir += 1;
  wxString mystring = wxString::Format("%03.0f", myDir);
  m_stHeading->SetLabel(mystring);
}

void Dlg::SetNMEAMessage(wxString sentence) {
  // $GPAPB,A,A,0.10,R,N,V,V,011,M,DEST,011,M,011,M*3C
  wxString token[40];
  wxString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
  token[0] = "";

  wxStringTokenizer tokenizer(sentence, ",");
  int i = 0;
  while (tokenizer.HasMoreTokens()) {
    token[i] = tokenizer.GetNextToken();
    i++;
  }

  if (token[0].Right(3) == "APB") {
    s11 = token[11];

    m_bGotAPB = true;
    if (m_bAuto) {
      double value;
      s11.ToDouble(&value);
      myDir = value;
    }
  }
}

void Dlg::parseNMEASentence(wxString& sentence) {
  wxString token[40];
  wxString s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
  token[0] = _T("");

  wxStringTokenizer tokenizer(sentence, wxT(","));
  int i = 0;

  while (tokenizer.HasMoreTokens()) {
    token[i] = tokenizer.GetNextToken();
    i++;
  }
  if (token[0].Right(3) == "VTG") {
    s5 = token[5];

    row_vtg = s5;
    //wxMessageBox(row_vtg);
  }

  //row_vtg = "16";

}


void Dlg::OnStandby(wxCommandEvent& event) { GoToStandby(); }

void Dlg::GoToStandby() {
  m_bAuto = false;

  m_buttonStandby->SetBackgroundColour(wxColour(0, 255, 0));
  Refresh();
}

void Dlg::OnPause(wxCommandEvent& event) {
  bool active = m_buttonPause->GetValue();
  if (active) {
    m_timer->Stop();
    m_buttonPause->SetLabel("Resume");
    m_buttonPause->SetBackgroundColour(wxColour(255, 0, 0));
  } else {
    StartDriving();
    m_buttonPause->SetLabel("Pause");
    m_buttonPause->SetBackgroundColour(wxColour(0, 255, 0));
  }
}

void Dlg::ResetPauseButton() {
  m_buttonPause->SetValue(0);
  m_buttonPause->SetLabel("Pause");
  m_buttonPause->SetBackgroundColour(wxColour(0, 255, 0));
}

void Dlg::OnClose(wxCloseEvent& event) {
  if (m_timer->IsRunning()) m_timer->Stop();
  plugin->OnrowerDialogClose();
}

void Dlg::Notify() {

  wxString mySentence;
  plugin->SetNMEASentence(mySentence);
  initSpd = wxAtof(row_vtg);
//m_SliderSpeed->GetValue();
  initRudder = m_SliderRudder->GetValue();

  double myRudder = initRudder - 30;
  if (myRudder < 0) {
    initRudder -= 30.0;
    myRudder = std::abs(initRudder);
    myDir -= myRudder;
    double myPortRudder = 30 - std::abs(myRudder);
    m_gaugeRudderPort->SetValue(myPortRudder);
    m_textCtrlRudderPort->SetValue(wxString::Format("%.0f", myRudder) + " P");
    m_gaugeRudderStbd->SetValue(0);
    m_textCtrlRudderStbd->SetValue("");
  } else if (myRudder >= 0) {
    initRudder -= 30;
    myDir += initRudder;
    m_gaugeRudderStbd->SetValue(myRudder);
    if (myRudder == 0) {
      m_textCtrlRudderStbd->SetValue("");
    } else {
      m_textCtrlRudderStbd->SetValue(wxString::Format("%.0f", myRudder) + " S");
    }
    m_gaugeRudderPort->SetValue(0);
    m_textCtrlRudderPort->SetValue("");
  }

  if (myDir < 0) {
    myDir += 360;
  } else if (myDir > 360) {
    myDir -= 360;
  }

  wxString mystring = wxString::Format("%03.0f", myDir);
  m_stHeading->SetLabel(mystring);


  m_stSpeed->SetLabel(wxString::Format("%3.1f", initSpd));

  if (!m_bUsingFollow) {
    SetNextStep(initLat, initLon, myDir, initSpd / 7200, stepLat, stepLon);

  } else {
    SetFollowStep(initLat, initLon, myDir, initSpd / 7200, stepLat, stepLon);
  }

  wxString timeStamp = wxString::Format("%i", wxGetUTCTime());

  wxString myNMEAais = myAIS->nmeaEncode("18", m_iMMSI, "5", initSpd, initLat,
                                         initLon, myDir, myDir, "B", timeStamp);

  wxString notMID = m_tMMSI.Mid(3);

  
  if (m_bUseFile) nmeafile->AddLine(myNMEAais);

  wxTimeSpan myTimeInterval = wxTimeSpan::Milliseconds(m_interval);
  wxDateTime mdt = dt.Add(myTimeInterval);

  RMC = createRMCSentence(mdt, initLat, initLon, initSpd, myDir);
  VTG = createVTGSentence(initSpd, myDir);

  PushNMEABuffer(RMC + "\r\n");
  PushNMEABuffer(VTG + "\r\n");

  if (m_bUseAis) {
    PushNMEABuffer(myNMEAais + "\r\n");
  }

  wxString rn = "\r\n";

  if (m_bUseAis && m_bUseNMEA) {
    nmeastream->AddLine(myNMEAais + rn);
  }
  if (m_bUseNMEA) {
    nmeastream->AddLine(RMC + "\r\n");
    nmeastream->AddLine(VTG + "\r\n");
  }

  initLat = stepLat;
  initLon = stepLon;

  dt = mdt;

  GetParent()->Refresh();
}

void Dlg::SetInterval(int interval) {
  m_interval = interval;
  if (m_timer->IsRunning())  // Timer started?
    m_timer->Start(m_interval,
                   wxTIMER_CONTINUOUS);  // restart timer with new interval
}

wxString Dlg::createRMCSentence(wxDateTime myDateTime, double myLat,
                                double myLon, double mySpd, double myDir) {
  //$GPRMC, 110858.989, A, 4549.9135, N, 00612.2671, E, 003.7, 207.5, 050513,
  //, , A * 60
  //$GPRMC,110858.989,A,4549.9135,N,00612.2671,E,003.7,207.5,050513,,,A*60

  wxString nlat;
  wxString nlon;
  wxString nRMC;
  wxString nNS;
  wxString nEW;
  wxString nSpd;
  wxString nDir;
  wxString nTime;
  wxString nDate;
  wxString nValid;
  wxString nForCheckSum;
  wxString nFinal;
  wxString nC = ",";
  wxString nA = "A,";
  nRMC = "GPRMC,";
  nValid = "A,A";
  wxString ndlr = "$";
  wxString nast = "*";

  nTime = DateTimeToTimeString(myDateTime);
  nNS = LatitudeToString(myLat);
  nEW = LongitudeToString(myLon);
  nSpd = wxString::Format("%f", mySpd);
  nDir = wxString::Format("%f", myDir);
  nDate = DateTimeToDateString(myDateTime);

  nForCheckSum = nRMC + nTime + nC + nA + nNS + nEW + nSpd + nC + nDir + nC +
                 nDate + ",,,A";
  nFinal = ndlr + nForCheckSum + nast + makeCheckSum(nForCheckSum);
  
  return nFinal;
}


wxString Dlg::createVTGSentence(double mySpd, double myDir) {
  //$GPVTG, 054.7, T, 034.4, M, 005.5, N, 010.2, K * 48
  //$IIVTG, 307., T, , M, 08.5, N, 15.8, K, A * 2F
  wxString nSpdN;
  wxString nSpdK;
  wxString nDir;
  wxString nTime;
  wxString nDate;
  wxString nValid;
  wxString nForCheckSum;
  wxString nFinal;
  wxString nC = ",";
  wxString nA = "A";
  wxString nT = "T,";
  wxString nM = "M,";
  wxString nN = "N,";
  wxString nK = "K,";

  wxString nVTG = "IIVTG,";
  nValid = "A,A";
  wxString ndlr = "$";
  wxString nast = "*";

  nSpdN = wxString::Format("%f", mySpd);
  nSpdK = wxString::Format("%f", mySpd*KNOT_2_KPH);
  nDir = wxString::Format("%f", myDir);

  nForCheckSum =
      nVTG + nDir + nC + nT + nC + nM + nSpdN + nC + nN + nSpdK + nC + nK + nA;

  nFinal = ndlr + nForCheckSum + nast + makeCheckSum(nForCheckSum);
  // wxMessageBox(nFinal);
  return nFinal;
}

wxString Dlg::makeCheckSum(wxString mySentence) {
  size_t i;
  unsigned char XOR;

  wxString s(mySentence);
  wxCharBuffer buffer = s.ToUTF8();
  char* Buff = buffer.data();  // data() returns const char *
  size_t iLen = strlen(Buff);
  for (XOR = 0, i = 0; i < iLen; i++) XOR ^= (unsigned char)Buff[i];
  stringstream tmpss;
  tmpss << hex << (int)XOR;
  wxString mystr = tmpss.str();
  return mystr;
}

double StringToLatitude(wxString mLat) {
  // 495054
  double returnLat;
  wxString mBitLat = mLat(0, 2);
  double degLat;
  mBitLat.ToDouble(&degLat);
  wxString mDecLat = mLat(2, mLat.length());
  double decValue;
  mDecLat.ToDouble(&decValue);

  returnLat = degLat + decValue / 100 / 60;

  return returnLat;
}

wxString Dlg::LatitudeToString(double mLat) {
  wxString singlezero = "0";
  wxString mDegLat;

  int degLat = std::abs(mLat);
  wxString finalDegLat = wxString::Format("%i", degLat);

  int myL = finalDegLat.length();
  switch (myL) {
    case (1): {
      mDegLat = singlezero + finalDegLat;
      break;
    }
    case (2): {
      mDegLat = finalDegLat;
      break;
    }
  }

  double minLat = std::abs(mLat) - degLat;
  double decLat = minLat * 60;

  wxString returnLat;

  if (mLat >= 0) {
    if (decLat < 10) {
      returnLat = mDegLat + "0" + wxString::Format("%.6f", decLat) + ",N,";
    } else {
      returnLat = mDegLat + wxString::Format("%.6f", decLat) + ",N,";
    }

  } else if (mLat < 0) {
    if (decLat < 10) {
      returnLat = mDegLat + "0" + wxString::Format("%.6f", decLat) + ",S,";
    } else {
      returnLat = mDegLat + wxString::Format("%.6f", decLat) + ",S,";
    }
  }

  return returnLat;
}

double StringToLongitude(wxString mLon) {
  wxString mBitLon = "";
  wxString mDecLon;
  double value1;
  double decValue1;

  double returnLon;

  int m_len = mLon.length();

  if (m_len == 7) {
    mBitLon = mLon(0, 3);
  }

  if (m_len == 6) {
    mBitLon = mLon(0, 2);
  }

  if (m_len == 5) {
    mBitLon = mLon(0, 1);
  }

  if (m_len == 4) {
    mBitLon = "00.00";
  }

  if (mBitLon == "-") {
    value1 = -0.00001;
  } else {
    mBitLon.ToDouble(&value1);
  }

  mDecLon = mLon(mLon.length() - 4, mLon.length());
  mDecLon.ToDouble(&decValue1);

  if (value1 < 0) {
    returnLon = value1 - decValue1 / 100 / 60;
  } else {
    returnLon = value1 + decValue1 / 100 / 60;
  }

  return returnLon;
}

wxString Dlg::LongitudeToString(double mLon) {
  wxString mDecLon;
  wxString mDegLon;
  double decValue;
  wxString returnLon;
  wxString doublezero = "00";
  wxString singlezero = "0";

  int degLon = fabs(mLon);
  wxString inLon = wxString::Format("%i", degLon);

  int myL = inLon.length();
  switch (myL) {
    case (1): {
      mDegLon = doublezero + inLon;
      break;
    }
    case (2): {
      mDegLon = singlezero + inLon;
      break;
    }
    case (3): {
      mDegLon = inLon;
      break;
    }
  }
  decValue = std::abs(mLon) - degLon;
  double decLon = decValue * 60;

  if (mLon >= 0) {
    if (decLon < 10) {
      returnLon = mDegLon + "0" + wxString::Format("%.6f", decLon) + ",E,";
    } else {
      returnLon = mDegLon + wxString::Format("%.6f", decLon) + ",E,";
    }

  } else {
    if (decLon < 10) {
      returnLon = mDegLon + "0" + wxString::Format("%.6f", decLon) + ",W,";
    } else {
      returnLon = mDegLon + wxString::Format("%.6f", decLon) + ",W,";
    }
  }
  return returnLon;
}

wxString Dlg::DateTimeToTimeString(wxDateTime myDT) {
  wxString sHours, sMinutes, sSecs;
  sHours = myDT.Format("%H");
  sMinutes = myDT.Format("%M");
  sSecs = myDT.Format("%S");
  wxString dtss = sHours + sMinutes + sSecs;
  return dtss;
}

wxString Dlg::DateTimeToDateString(wxDateTime myDT) {
  wxString sDay, sMonth, sYear;
  sDay = myDT.Format("%d");
  sMonth = myDT.Format("%m");
  sYear = myDT.Format("%y");

  return sDay + sMonth + sYear;
}

void Dlg::OnContextMenu(double m_lat, double m_lon) {

  initLat = m_lat;
  initLon = m_lon;
}

double Dlg::AttributeDouble(TiXmlElement* e, const char* name, double def) {
  const char* attr = e->Attribute(name);
  if (!attr) return def;
  char* end;
  double d = strtod(attr, &end);
  if (end == attr) return def;
  return d;
}

void Dlg::OnFollow(wxCommandEvent& event) {
  std::vector<std::unique_ptr<PlugIn_Route_Ex>> routes;
  auto uids = GetRouteGUIDArray();
  for (size_t i = 0; i < uids.size(); i++) {
    routes.push_back(std::move(GetRouteEx_Plugin(uids[i])));
  }

  GetRouteDialog RouteDialog(this, -1, _("Select the route to follow"),
                             wxPoint(200, 200), wxSize(300, 200),
                             wxCAPTION | wxRESIZE_BORDER);

  RouteDialog.dialogText->InsertColumn(0, "", 0, wxLIST_AUTOSIZE);
  RouteDialog.dialogText->SetColumnWidth(0, 290);
  RouteDialog.dialogText->InsertColumn(1, "", 0, wxLIST_AUTOSIZE);
  RouteDialog.dialogText->SetColumnWidth(1, 0);
  RouteDialog.dialogText->DeleteAllItems();

  int in = 0;
  std::vector<std::string> names;
  for (const auto& r : routes) names.push_back(r->m_NameString.ToStdString());

  for (size_t n = 0; n < names.size(); n++) {
    wxString routeName = names[in];

    RouteDialog.dialogText->InsertItem(in, "", -1);
    RouteDialog.dialogText->SetItem(in, 0, routeName);
    in++;
  }

  // ReadNavobj();
  long si = -1;
  long itemIndex = -1;
  // int f = 0;

  wxListItem row_info;
  wxString cell_contents_string = wxEmptyString;
  bool foundRoute = false;

  if (RouteDialog.ShowModal() != wxID_OK) {
    m_bUsingFollow = false;
  } else {
    ResetPauseButton();
    for (;;) {
      itemIndex = RouteDialog.dialogText->GetNextItem(
          itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

      if (itemIndex == -1) break;

      // Got the selected item index
      if (RouteDialog.dialogText->IsSelected(itemIndex)) {
        si = itemIndex;
        foundRoute = true;
        break;
      }
    }

    if (foundRoute) {
      // Set what row it is (m_itemId is a member of the regular
      // wxListCtrl class)
      row_info.m_itemId = si;
      // Set what column of that row we want to query for information.
      row_info.m_col = 0;
      // Set text mask
      row_info.m_mask = wxLIST_MASK_TEXT;

      // Get the info and store it in row_info variable.
      RouteDialog.dialogText->GetItem(row_info);
      // Extract the text out that cell
      cell_contents_string = row_info.m_text;
      rtept initPoint;
      nextRoutePointIndex = 0;
      bool foundRoute = false;

      for (size_t i = 0; i < uids.size(); i++) {
        thisRoute = GetRouteEx_Plugin(uids[i]);

        if (thisRoute->m_NameString == cell_contents_string) {
          foundRoute = true;
          break;
        }
      }
      if (foundRoute) {
        m_bUsingFollow = true;
        countRoutePoints = thisRoute->pWaypointList->size();
        myList = thisRoute->pWaypointList;

        PlugIn_Waypoint_Ex* myWaypoint;
        theWaypoints.clear();

        wxPlugin_WaypointExListNode* pwpnode = myList->GetFirst();
        while (pwpnode) {
          myWaypoint = pwpnode->GetData();

          theWaypoints.push_back(myWaypoint);

          pwpnode = pwpnode->GetNext();
        }

        for (size_t n = 0; n < theWaypoints.size(); n++) {
          if (n == 0) {
            double dlat = theWaypoints[n]->m_lat;
            double dlon = theWaypoints[n]->m_lon;

            initLat = dlat;
            initLon = dlon;

            nextRoutePointIndex = 1;
          }

          if (n == 1) {
            double dlat1 = theWaypoints[n]->m_lat;
            double dlon1 = theWaypoints[n]->m_lon;

            nextLat = dlat1;
            nextLon = dlon1;

            DistanceBearingMercator_Plugin(nextLat, nextLon, initLat, initLon,
                                           &followDir, &myDist);
          }
        }

      } else
        wxMessageBox("Route not found");
    }

    double scale_factor = GetOCPNGUIToolScaleFactor_PlugIn();
    JumpToPosition(initLat, initLon, scale_factor);
    StartDriving();
  }
}

wxString Dlg::StandardPath() {
  wxStandardPathsBase& std_path = wxStandardPathsBase::Get();
  wxString s = wxFileName::GetPathSeparator();

#if defined(__WXMSW__)
  wxString stdPath = std_path.GetConfigDir();
#elif defined(__WXGTK__) || defined(__WXQT__)
  wxString stdPath = std_path.GetUserDataDir();
#elif defined(__WXOSX__)
  wxString stdPath = (std_path.GetUserConfigDir() + s + "opencpn");
#endif

#ifdef __WXOSX__
  // Compatibility with pre-OCPN-4.2; move config dir to
  // ~/Library/Preferences/opencpn if it exists
  wxString oldPath = (std_path.GetUserConfigDir() + s);
  if (wxDirExists(oldPath) && !wxDirExists(stdPath)) {
    wxLogMessage("rower_pi: moving config dir %s to %s", oldPath, stdPath);
    wxRenameFile(oldPath, stdPath);
  }
#endif

  stdPath += s;  // is this necessary?
  return stdPath;
}

GetRouteDialog::GetRouteDialog(wxWindow* parent, wxWindowID id,
                               const wxString& title, const wxPoint& position,
                               const wxSize& size, long style)
    : wxDialog(parent, id, title, position, size, style) {
  wxPoint p;
  wxSize sz;

  sz.SetWidth(size.GetWidth() - 20);
  sz.SetHeight(size.GetHeight() - 70);

  p.x = 6;
  p.y = 2;

  dialogText = new wxListView(this, wxID_ANY, p, sz,
                              wxLC_NO_HEADER | wxLC_REPORT | wxLC_SINGLE_SEL,
                              wxDefaultValidator, wxT(""));
  wxFont pVLFont(wxFontInfo(12).FaceName("Arial"));
  dialogText->SetFont(pVLFont);

  auto sizerlist = new wxBoxSizer(wxVERTICAL);
  sizerlist->Add(-1, -1, 100, wxEXPAND);
  sizerlist->Add(dialogText);

  auto sizer = new wxBoxSizer(wxHORIZONTAL);
  auto flags = wxSizerFlags().Bottom().Border();
  sizer->Add(1, 1, 100, wxEXPAND);  // Expanding spacer
  auto cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));
  sizer->Add(cancel, flags);
  auto m_ok = new wxButton(this, wxID_OK, _("OK"));
  m_ok->Enable(true);
  sizer->Add(m_ok, flags);
  sizerlist->Add(sizer);
  SetSizer(sizerlist);
  Fit();
  SetFocus();
};