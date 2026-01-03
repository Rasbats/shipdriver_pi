/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose: rower Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2017 by Mike Rossiter                                *
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

#include "wx/wxprec.h"

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "config.h"
#include "icons.h"
#include "rower_pi.h"
#include "rower_gui.h"
#include "rower_gui_impl.h"
#include "ocpn_plugin.h"
#include "plug_utils.h"

class Dlg;

using namespace std;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void* ppimgr) {
  return new rowerPi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p) { delete p; }

//---------------------------------------------------------------------------------------------------------
//
//   rower PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

rowerPi::rowerPi(void* ppimgr)
    : opencpn_plugin_118(ppimgr),
      m_hr_dialog_x(0),
      m_hr_dialog_y(8),
      m_tr_spd(0),
      m_tr_dir(0),
      m_cursor_lat(0),
      m_cursor_lon(0),
      m_position_menu_id(-1),
      m_gui_scale_factor(1),
      plugin(nullptr),
      m_dialog(nullptr),
      m_config(nullptr),
      m_parent_window(nullptr),
      m_hr_dialog_width(0),
      m_hr_dialog_height(0),
      m_hr_dialog_sx(0),
      m_hr_dialog_sy(0),
      m_display_width(0),
      m_display_height(0),
      m_leftclick_tool_id(-1),
      m_show_rower_icon(false),
      m_copy_use_ais(false),
      m_copy_use_file(false), 
      m_copy_use_nmea(false) 

{
  // Create the PlugIn icons
  initialize_images();
  auto icon_path = GetPluginIcon("rower_panel_icon", PKG_NAME);
  if (icon_path.type == IconPath::Type::Svg)
    m_panel_bitmap = LoadSvgIcon(icon_path.path.c_str());
  else if (icon_path.type == IconPath::Type::Png)
    m_panel_bitmap = LoadPngIcon(icon_path.path.c_str());
  else  // icon_path.type == NotFound
    wxLogWarning("Cannot find icon for basename: %s", "rower_panel_icon");
  if (m_panel_bitmap.IsOk())
    wxLogDebug("rowerPi::, bitmap OK");
  else
    wxLogDebug("rowerPi::, bitmap fail");
  m_show_rower = false;
}

rowerPi::~rowerPi() {
  delete _img_rowerIcon;

  if (m_dialog) {
    wxFileConfig* pConf = GetOCPNConfigObject();

    if (pConf) {
      pConf->SetPath("/PlugIns/rower_pi");
      pConf->Write("rowerUseAis", m_copy_use_ais);
      pConf->Write("rowerUseFile", m_copy_use_file);
      pConf->Write("rowerMMSI", m_copy_mmsi);
      pConf->Write("rowerUseNMEA", m_copy_use_nmea);
    }
  }
}

int rowerPi::Init() {
  AddLocaleCatalog("opencpn-rower_pi");

  // Set some default private member parameters
  m_hr_dialog_x = 40;
  m_hr_dialog_y = 80;
  m_hr_dialog_sx = 400;
  m_hr_dialog_sy = 300;
  ::wxDisplaySize(&m_display_width, &m_display_height);

  //    Get a pointer to the opencpn display canvas, to use as a parent for
  //    the POI Manager dialog
  m_parent_window = GetOCPNCanvasWindow();

  //    Get a pointer to the opencpn configuration object
  m_config = GetOCPNConfigObject();

  //    And load the configuration items
  LoadConfig();
  auto icon = GetPluginIcon("rower_pi", PKG_NAME);
  auto toggled_icon = GetPluginIcon("rower_pi_toggled", PKG_NAME);
  //    This PlugIn needs a toolbar icon, so request its insertion
  if (m_show_rower_icon) {
    if (icon.type == IconPath::Type::Svg)
      m_leftclick_tool_id = InsertPlugInToolSVG(
          "rower", icon.path, icon.path, toggled_icon.path, wxITEM_CHECK,
          "rower", "", nullptr,rower_TOOL_POSITION, 0, this);
    else if (icon.type == IconPath::Type::Png) {
      auto bitmap = LoadPngIcon(icon.path.c_str());
      m_leftclick_tool_id =
          InsertPlugInTool("", &bitmap, &bitmap, wxITEM_CHECK, "rower", "",
                           nullptr,rower_TOOL_POSITION, 0, this);
    }
  }
  m_dialog = nullptr;

  return (WANTS_OVERLAY_CALLBACK | WANTS_OPENGL_OVERLAY_CALLBACK |
          WANTS_TOOLBAR_CALLBACK | INSTALLS_TOOLBAR_TOOL | WANTS_CURSOR_LATLON |
          WANTS_NMEA_SENTENCES | WANTS_AIS_SENTENCES | WANTS_PREFERENCES |
          WANTS_PLUGIN_MESSAGING | WANTS_CONFIG);
}

bool rowerPi::DeInit() {
  //    Record the dialog position
  if (m_dialog) {
    // Capture dialog position
    wxPoint p = m_dialog->GetPosition();
    wxRect r = m_dialog->GetRect();
    SetrowerDialogX(p.x);
    SetrowerDialogY(p.y);
    SetrowerDialogSizeX(r.GetWidth());
    SetrowerDialogSizeY(r.GetHeight());
    if (m_copy_use_nmea) {
      if (m_dialog->nmeastream->IsOpened()) {
        m_dialog->nmeastream->Write();
        m_dialog->nmeastream->Close();
      }
    }
  

    if ((m_dialog->m_timer) && (m_dialog->m_timer->IsRunning())) {
      // need to stop the timer or crash on exit
      m_dialog->m_timer->Stop();
    }
    m_dialog->Close();
    delete m_dialog;
    m_dialog = nullptr;

    m_show_rower = false;
    SetToolbarItemState(m_leftclick_tool_id, m_show_rower);
  }

  SaveConfig();

  RequestRefresh(m_parent_window);  // refresh main window

  return true;
}

int rowerPi::GetAPIVersionMajor() { return atoi(API_VERSION); }

int rowerPi::GetAPIVersionMinor() {
  std::string v(API_VERSION);
  size_t dotpos = v.find('.');
  return atoi(v.substr(dotpos + 1).c_str());
}

int rowerPi::GetPlugInVersionMajor() { return PLUGIN_VERSION_MAJOR; }

int rowerPi::GetPlugInVersionMinor() { return PLUGIN_VERSION_MINOR; }

int rowerPi::GetPlugInVersionPatch() { return PLUGIN_VERSION_PATCH; }

int rowerPi::GetPlugInVersionPost() { return PLUGIN_VERSION_TWEAK; };

const char*rowerPi::GetPlugInVersionPre() { return PKG_PRERELEASE; }

const char*rowerPi::GetPlugInVersionBuild() { return PKG_BUILD_INFO; }

wxBitmap*rowerPi::GetPlugInBitmap() { return &m_panel_bitmap; }

wxString rowerPi::GetCommonName() { return PLUGIN_API_NAME; }

wxString rowerPi::GetShortDescription() { return PKG_SUMMARY; }

wxString rowerPi::GetLongDescription() { return PKG_DESCRIPTION; }

int rowerPi::GetToolbarToolCount() { return 1; }

void rowerPi::SetColorScheme(PI_ColorScheme cs) {
  if (!m_dialog) return;
  DimeWindow(m_dialog);
}

void rowerPi::ShowPreferencesDialog(wxWindow* parent) {
  auto* pref = new rowerPreferences(parent);

  pref->m_cbTransmitAis->SetValue(m_copy_use_ais);
  pref->m_cbAisToFile->SetValue(m_copy_use_file);
  pref->m_textCtrlMMSI->SetValue(m_copy_mmsi);
  pref->m_cbNMEAToFile->SetValue(m_copy_use_nmea);

  if (pref->ShowModal() == wxID_OK) {
    bool copy_ais = pref->m_cbTransmitAis->GetValue();
    bool copy_file = pref->m_cbAisToFile->GetValue();
    wxString copyMMSI = pref->m_textCtrlMMSI->GetValue();
    bool copy_nmea = pref->m_cbNMEAToFile->GetValue();


    if (m_copy_use_ais != copy_ais || m_copy_use_file != copy_file ||
        m_copy_mmsi != copyMMSI) {
      m_copy_use_ais = copy_ais;
      m_copy_use_file = copy_file;
      m_copy_mmsi = copyMMSI;
    }

    if (m_copy_use_nmea != copy_nmea) {
      m_copy_use_nmea = copy_nmea;
    }

    if (m_dialog) {
      m_dialog->m_bUseAis = m_copy_use_ais;
      m_dialog->m_bUseFile = m_copy_use_file;
      m_dialog->m_tMMSI = m_copy_mmsi;
      m_dialog->m_bUseNMEA = m_copy_use_nmea;
    }

    SaveConfig();

    RequestRefresh(m_parent_window);  // refresh main window
  }

  delete pref;
  pref = nullptr;
}

void rowerPi::OnToolbarToolCallback(int id) {
  if (!m_dialog) {
    m_dialog = new Dlg(m_parent_window);
    m_dialog->plugin = this;
    m_dialog->m_timer = new wxTimer(m_dialog);
    m_dialog->Move(wxPoint(m_hr_dialog_x, m_hr_dialog_y));
    m_dialog->SetSize(m_hr_dialog_sx, m_hr_dialog_sy);

    wxMenu dummy_menu;
    m_position_menu_id = AddCanvasContextMenuItem(
        new wxMenuItem(&dummy_menu, -1, _("Select Vessel Start Position")),
        this);
    SetCanvasContextMenuItemViz(m_position_menu_id, true);
  }

  // m_pDialog->Fit();
  // Toggle
  m_show_rower = !m_show_rower;

  //    Toggle dialog?
  if (m_show_rower) {
    m_dialog->Move(wxPoint(m_hr_dialog_x, m_hr_dialog_y));
    m_dialog->SetSize(m_hr_dialog_sx, m_hr_dialog_sy);
    m_dialog->Show();

  } else {
    m_dialog->Hide();
  }

  // Toggle is handled by the toolbar but we must keep plugin manager b_toggle
  // updated to actual status to ensure correct status upon toolbar rebuild
  SetToolbarItemState(m_leftclick_tool_id, m_show_rower);

  // Capture dialog position
  wxPoint p = m_dialog->GetPosition();
  wxRect r = m_dialog->GetRect();
  SetrowerDialogX(p.x);
  SetrowerDialogY(p.y);
  SetrowerDialogSizeX(r.GetWidth());
  SetrowerDialogSizeY(r.GetHeight());

  RequestRefresh(m_parent_window);  // refresh main window
}

bool rowerPi::LoadConfig() {
  auto* conf = (wxFileConfig*)m_config;

  if (conf) {
    if (conf->HasGroup(_T("/Settings/rower_pi"))) {
      // Read the existing settings

      conf->SetPath("/Settings/rower_pi");
      conf->Read("ShowrowerIcon", &m_show_rower_icon, true);
      conf->Read("rowerUseAis", &m_copy_use_ais, false);
      conf->Read("rowerUseNMEA", &m_copy_use_nmea, false);
      conf->Read("rowerUseFile", &m_copy_use_file, false);
      m_copy_mmsi = conf->Read("rowerMMSI", "123456789");

      m_hr_dialog_x = conf->Read("DialogPosX", 40L);
      m_hr_dialog_y = conf->Read("DialogPosY", 140L);
      m_hr_dialog_sx = conf->Read("DialogSizeX", 330L);
#ifdef __WXOSX__
      m_hr_dialog_sy = conf->Read("DialogSizeY", 250L);
#else
      m_hr_dialog_sy = conf->Read("DialogSizeY", 300L);
#endif
      conf->DeleteGroup(_T("/Settings/rower_pi"));
    } else {
      conf->SetPath("/PlugIns/rower_pi");
      conf->Read("ShowrowerIcon", &m_show_rower_icon, true);
      conf->Read("rowerUseAis", &m_copy_use_ais, false);
      conf->Read("rowerUseNMEA", &m_copy_use_nmea, false);
      conf->Read("rowerUseFile", &m_copy_use_file, false);
      m_copy_mmsi = conf->Read("rowerMMSI", "123456789");

      m_hr_dialog_x = conf->Read("DialogPosX", 40L);
      m_hr_dialog_y = conf->Read("DialogPosY", 140L);
      m_hr_dialog_sx = conf->Read("DialogSizeX", 330L);
#ifdef __WXOSX__
      m_hr_dialog_sy = conf->Read("DialogSizeY", 250L);
#else
      m_hr_dialog_sy = conf->Read("DialogSizeY", 300L);
#endif
    }
    if ((m_hr_dialog_x < 0) || (m_hr_dialog_x > m_display_width))
      m_hr_dialog_x = 40;
    if ((m_hr_dialog_y < 0) || (m_hr_dialog_y > m_display_height))
      m_hr_dialog_y = 140;

    return true;
  } else
    return false;
}

bool rowerPi::SaveConfig() {
  auto* conf = (wxFileConfig*)m_config;

  if (conf) {
    bool is_digits = m_copy_mmsi.IsNumber();
    if (m_copy_mmsi.length() < 9 || !is_digits) {
      wxMessageBox(_("MMSI must be 9 digits.\nEdit using Preferences"));
      return false;
    }

    conf->SetPath("/PlugIns/rower_pi");
    conf->Write("ShowrowerIcon", m_show_rower_icon);
    conf->Write("rowerUseAis", m_copy_use_ais);
    conf->Write("rowerUseNMEA", m_copy_use_nmea);
    conf->Write("rowerUseFile", m_copy_use_file);
    conf->Write("rowerMMSI", m_copy_mmsi);

    conf->Write("DialogPosX", m_hr_dialog_x);
    conf->Write("DialogPosY", m_hr_dialog_y);
    conf->Write("DialogSizeX", m_hr_dialog_sx);
    conf->Write("DialogSizeY", m_hr_dialog_sy);

    return true;
  } else
    return false;
}

void rowerPi::OnrowerDialogClose() {
  m_show_rower = false;
  SetToolbarItemState(m_leftclick_tool_id, m_show_rower);
  m_dialog->Hide();
  SaveConfig();

  RequestRefresh(m_parent_window);  // refresh main window
}

void rowerPi::OnContextMenuItemCallback(int id) {
  if (!m_dialog) return;

  if (id == m_position_menu_id) {
    m_cursor_lat = GetCursorLat();
    m_cursor_lon = GetCursorLon();

    m_dialog->OnContextMenu(m_cursor_lat, m_cursor_lon);
  }
}

void rowerPi::SetCursorLatLon(double lat, double lon) {
  m_cursor_lat = lat;
  m_cursor_lon = lon;
}

void rowerPi::SetNMEASentence(wxString& sentence) {
  if (m_dialog) {
    m_dialog->parseNMEASentence(sentence);
  }
}
