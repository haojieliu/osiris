/*
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
*  OSIRIS is a desktop tool working on your computer with your own data.
*  Your sample profile data is processed on your computer and is not sent
*  over the internet.
*
*  For quality monitoring, OSIRIS sends some information about usage
*  statistics  back to NCBI.  This information is limited to use of the
*  tool, without any sample, profile or batch data that would reveal the
*  context of your analysis.  For more details and instructions on opting
*  out, see the Privacy Information section of the OSIRIS User's Guide.
*
* ===========================================================================
*
*
*  FileName: CFramePlot.cpp
*  Author:   Douglas Hoffman
*
*/
#include "mainFrame.h"
#include "mainApp.h"
#include "CFramePlot.h"
#include "CFrameAnalysis.h"
#include "nwx/stdb.h"
#include <map>
#include "nwx/stde.h"
#include "nwx/nwxHoldScrollPos.h"
#include "OsirisFileTypes.h"
#include <wx/wupdlock.h>
#include <wx/scrolwin.h>
#include <wx/file.h>
#include <wx/settings.h>
#include <wx/filename.h>
#include <wx/dcmemory.h>
#include "nwx/CPointerHold.h"
#include "nwx/CIncrementer.h"
#include "nwx/nwxString.h"
#include "CDialogExportPlot.h"
#include "COsirisIcon.h"
#include "CKitColors.h"
#include "CPlotData.h"
#include "COARfile.h"
#include "CMenuBarGraph.h"
#include "CMenuFileGraph.h"
#include "CMenuEdit.h"
#include "CParmOsiris.h"
#include "CPanelHistoryMenu.h"
#include "CNotebookEditSample.h"
#include "CPrintOutPlot.h"
#include "CGridAnalysisDisplay.h"
#include "CDialogWarnHistory.h"
#include "nwx/vectorptr.h"

#if FP_SCROLL_EVENT
DEFINE_EVENT_TYPE(wxEVT_SCROLL_PLOT)
#endif
DEFINE_EVENT_TYPE(wxEVT_SIZE_DELAY_PLOT)

#define SCROLL_UNITS 4

// round DOWN to the nearest multiple of 4, if n >= 0
#define PIXEL_TO_SCROLL_UNITS(n)  ((n) >> 2)
#define SCROLL_UNITS_TO_PIXELS(n) ((n) << 2)

//**************************************************** CFramePlotMenu
class CFramePlotMenu : public wxMenu
{
public:
  CFramePlotMenu();
  virtual ~CFramePlotMenu();
  CFramePlotMenu *Clone();
  void RemoveAllPlots();
  void InsertPlot(wxMenuItem *pItem, int nOffset);
  void UpdateLabels();
  void SetToolbarLabel(bool bShow = true);
  CMenuPlot *FindMenuPlot(int nID);
  int PlotCount()
  {
    int n = (int)GetMenuItemCount() - (int)m_nBaseSize;
    if(n < 0) { n = 0; }
    return n;
  }
  void AddPlot(wxMenuItem *pItem)
  {
    InsertPlot(pItem,PlotCount());
  }
  void BeginBatch()
  {
    m_nBatchCount++;
  }
  void EndBatch()
  {
    m_nBatchCount--;
    _CheckUpdate();
  }
  void CopySettings(CFramePlotMenu &menuFrom, bool bDeep = false);
  void CopyPlotSettings(CFramePlotMenu &menuFrom);
  void SetHistorySubMenu(wxMenu *p);
  CMenuHistory *GetHistorySubMenu()
  {
    CMenuHistory *pRtn = 
      (m_pMenuItemHistory == NULL)
      ? NULL
      : wxDynamicCast(m_pMenuItemHistory->GetSubMenu(),CMenuHistory);
    return pRtn;
  }
private:
  void _CheckUpdate()
  {
    if(m_bUpdate && !m_nBatchCount)
    {
      UpdateLabels();
    }
  }
  int m_nBatchCount;
  size_t m_nBaseSize;
  bool m_bUpdate;
  bool m_bClone;
  wxMenuItem *m_pMenuItemHistory;
};

typedef TnwxBatch<CFramePlotMenu> CBatchPlotMenu;


CFramePlotMenu::CFramePlotMenu()
  : wxMenu(0L),
    m_nBatchCount(0),
    m_bUpdate(false),
    m_bClone(false),
    m_pMenuItemHistory(NULL)
{
  AppendSeparator();
  Append(IDmenuParameters,"Parameters...",
    "Show parameters used to create this data");
  Append(
    IDExportGraphic,
    CMenuFileGraph::EXPORT_GRAPH,
    CMenuFileGraph::EXPORT_GRAPH_HELP);
  Append(IDmenuShowHideToolbar,CMDIFrame::HIDE_TOOLBARS);
  AppendCheckItem(IDmenuShowXBPS, "Show ILS BPS X-axis");
  Append(IDmenuShowHidePlotScrollbars,"Hide Plot Scrollbars");
  AppendCheckItem(IDmenuShowHideWindowScrollbar,"Resizable Plots");
  Append(IDmenuDisplaySample,"&Edit Sample");
  Append(IDmenuSampleTile,"Split Screen with Sample\tCtrl+G");
  Append(IDmenuTable,"Show T&able");
  SetHistorySubMenu(NULL);
  Append(IDMaxLadderLabels,
    CMenuPlot::LABEL_MAX_PEAK_LABELS,
    CMenuPlot::STATUS_MAX_PEAK_LABELS);

  m_nBaseSize = GetMenuItemCount();
};
CFramePlotMenu::~CFramePlotMenu()
{
  // plot menu items must be removed because
  // they are destroyed by their corresponding
  // CPanelPlot instances
  if(!m_bClone)
  {
    RemoveAllPlots();
  }
}

CFramePlotMenu *CFramePlotMenu::Clone()
{
  CFramePlotMenu *pRtn = new CFramePlotMenu();
  pRtn->m_bClone = true;
  pRtn->CopySettings(*this,true);
  return pRtn;
}
void CFramePlotMenu::CopyPlotSettings(CFramePlotMenu &menuFrom)
{
  size_t nPlotCount = menuFrom.PlotCount();
  CBatchPlotMenu xxx(this);
  if(m_bClone)
  {
    RemoveAllPlots();
    wxMenuItemList &list(menuFrom.GetMenuItems());
    wxMenuItemList::iterator itr = list.begin();
    wxMenuItem *pItem;
    wxMenuItem *pNewItem;
    CMenuPlot *pMenuPlot;
    size_t nPlotCurrent;
    for(nPlotCurrent = 0; nPlotCurrent < nPlotCount; nPlotCurrent++)
    {
      pItem = *itr;
      itr++;
      pMenuPlot = wxDynamicCast(pItem->GetSubMenu(),CMenuPlot);
      if(pMenuPlot == NULL)
      {
        wxASSERT_MSG(0,
          "CFramePlotMenu::CopySettings - CMenuPlot expected");
      }
      else
      {
        pNewItem = new wxMenuItem(
          this,
          pItem->GetId(),
          pItem->GetItemLabel(),
          pItem->GetHelp(),
          wxITEM_NORMAL,
          pMenuPlot->Clone()
          );
        InsertPlot(pNewItem,(int)nPlotCurrent);
      }
    }
  }
  else if(PlotCount() == (int)nPlotCount)
  {
    wxMenuItemList::iterator itrFrom = menuFrom.GetMenuItems().begin();
    wxMenuItemList::iterator itrTo = GetMenuItems().begin();
    CMenuPlot *pMenuPlotFrom;
    CMenuPlot *pMenuPlotTo;
    size_t i;
    for(i = 0; i < nPlotCount; i++)
    {
      pMenuPlotFrom = wxDynamicCast((*itrFrom)->GetSubMenu(),CMenuPlot);
      pMenuPlotTo = wxDynamicCast((*itrTo)->GetSubMenu(),CMenuPlot);
      if(pMenuPlotFrom == NULL || pMenuPlotTo == NULL)
      {
        wxString s;
        wxASSERT_MSG(0,"CFramePlotMenu::CopySettings() "
            "problem with CMenuPlot submenu");
      }
      else
      {
        pMenuPlotTo->CopySettings(*pMenuPlotFrom);
      }
      ++itrFrom;
      ++itrTo;
    }
  }
  else
  {
    wxASSERT_MSG(0,"CFramePlotMenu::CopyPlotSettings() "
          "plot count does not match");
  }
}

void CFramePlotMenu::CopySettings(CFramePlotMenu &menuFrom, bool bDeep)
{
  if(m_bClone || bDeep)
  {
    CopyPlotSettings(menuFrom);
  }
  if(bDeep)
  {
    // set up history menu

    CMenuHistory *pMenuHistFrom = menuFrom.GetHistorySubMenu();
    CMenuHistory *pMenuHistTo = GetHistorySubMenu();
    if(pMenuHistTo != NULL)
    {
      if(pMenuHistFrom != NULL)
      {
        pMenuHistFrom->CopyState(pMenuHistTo);
      }
      else
      {
        SetHistorySubMenu(NULL);
      }
    }
    else if(pMenuHistFrom != NULL)
    {
      if(m_bClone)
      {
        SetHistorySubMenu(pMenuHistFrom->Clone());
      }
      else
      {
        wxASSERT_MSG(0,"CFramePlotMenu::CopySettings() "
          "inconsistent history menus");
      }
    }
  }
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowHideToolbar);
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowHidePlotScrollbars);
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowHideWindowScrollbar);
  CMenuEdit::CopyState(this,&menuFrom,IDmenuShowXBPS);
}
CMenuPlot *CFramePlotMenu::FindMenuPlot(int nID)
{
  size_t n = PlotCount();
  size_t i;
  wxMenuItemList::iterator itr = GetMenuItems().begin();
  CMenuPlot *pRtn = NULL;
  CMenuPlot *p;
  for(i = 0; i < n; i++)
  {
    p = wxDynamicCast((*itr)->GetSubMenu(),CMenuPlot);
    if((p != NULL) && p->HasID(nID))
    {
      pRtn = p;
      i = n; // loop exit
    }
    ++itr;
  }
  return pRtn;
}
void CFramePlotMenu::SetHistorySubMenu(wxMenu *p)
{
  wxString sLabel("&History");
  if(m_pMenuItemHistory != NULL)
  {
    sLabel = m_pMenuItemHistory->GetItemLabel();
    Destroy(m_pMenuItemHistory);
  }
  if(p != NULL)
  {
    m_pMenuItemHistory = AppendSubMenu(p,sLabel);
  }
  else
  {
    m_pMenuItemHistory = Append(IDmenuHistoryTop,sLabel);
  }
}
void CFramePlotMenu::RemoveAllPlots()
{
  wxMenuItem *pItem;
  while(GetMenuItemCount() > m_nBaseSize)
  {
    wxMenuItemList &lst(GetMenuItems());
    pItem = *lst.begin();
    if(m_bClone)
    {
      Destroy(pItem);
    }
    else
    {
      Remove(pItem);
    }
  }
}
void CFramePlotMenu::InsertPlot(wxMenuItem *pItem, int nOffset)
{
  if(nOffset < 0)
  {
    nOffset = 0;
  }
  else if(nOffset)
  {
    int nMax = PlotCount();
    if(nOffset > nMax) { nOffset = nMax;}
  }
  Insert((size_t)nOffset,pItem);
  UpdateLabels();
}

void CFramePlotMenu::UpdateLabels()
{
  if(m_nBatchCount)
  {
    m_bUpdate = true;
  }
  else
  {
    wxMenuItemList &lst(GetMenuItems());
    wxMenuItemList::iterator itr = lst.begin();
    int nCount = PlotCount();
    int i;
    if(!nCount) {;}
    else if(nCount == 1)
    {
      (*itr)->SetItemLabel("Plot");
    }
    else
    {
      wxString s;
      (*itr)->SetItemLabel("Top Plot");
      ++itr;
      for(i = 2; i < nCount; i++)
      {
        s.Printf("Plot %d",i);
        (*itr)->SetItemLabel(s);
        ++itr;
      }
      (*itr)->SetItemLabel("Bottom Plot");
    }
  }
}

//**************************************************** CFramePlot
#define PING_WINDOW_TYPE "FramePlot"

void CFramePlot::_SetupTitle()
{
  wxString sFileName(m_pData->GetLastFileName());
  if(sFileName.Len())
  {
    wxFileName fn(sFileName);
    sFileName = fn.GetFullName();
    if(m_pOARfile != NULL)
    {
      wxString sOAR = m_pOARfile->GetLastFileName();
      wxFileName fno(sOAR);
      sOAR = fno.GetFullName();
      if(!sOAR.IsEmpty())
      {
        sFileName.Append("; ");
        sFileName.Append(sOAR);
      }
    }
  }
  else
  {
    sFileName = "error...";
  }
  SetTitle(mainApp::FormatWindowTitle(
    wxT("Graph"),
    sFileName,false,&m_pData->GetParameters(), GetSelectedTime()));
}

CFramePlot::~CFramePlot()
{
  mainApp::Ping2(PING_EVENT, PING_WINDOW_CLOSE PING_WINDOW_TYPE, PING_WINDOW_NUMBER, GetFrameNumber());
  _CleanupMenuHistoryPopup();
  _CleanupMenuPopup();
  delete m_pData;
////  delete m_pMenu;
  _CleanupExportDialog();
}

CFramePlot::CFramePlot(
  mainFrame *pOwner,
  const wxSize &size,
  CPlotData *pData,
  COARfile *pFile,
  CKitColors *pColors,
  bool bSingle,
  unsigned int nChannel,
  bool bUseExternalTimer
  ) :
    CMDIFrame(pOwner, IDframePlot, "",
      wxDefaultPosition,
      GET_PERSISTENT_SIZE_DEFAULT(CFramePlot,size)
      ),
    m_pPanel(NULL),
    m_pDialogPlot(NULL),
    m_pData(pData),
    m_pOARfile(NULL),
    m_pSample(NULL),
    m_pColors(pColors),
    m_pMenu(new CFramePlotMenu),
    m_pMenuHistory(NULL),
    m_pMenuPopup(NULL),
    m_pMenuHistoryPopup(NULL),
    m_TimeLastRebuild(NULL),
    m_nState(FP_NO_STATE),
    m_pPlotSyncTo(NULL),
    m_pPlotForBitmap(NULL),
#if DELAY_PLOT_AREA_SYNC
    m_pPlotSyncThisTo(NULL),
#endif
    m_nInSync(0),
    m_nMinHeight(-1),
    m_nMenuUp(0),
    m_nBatchCount(0),
    m_nScrollOnTimer(-1),
    m_nScrollOnTimerOffset(0),
    m_nDelayViewState(0),
#if DELAY_PLOT_AREA_SYNC
    m_nSyncThisTimer(0),
#endif
    m_bUpdateMenu(false),
    m_bUseExternalTimer(bUseExternalTimer),
    m_bFixed(true),
    m_bXBPS(false)
{
  CBatchPlot BATCH(this);
  CParmOsirisGlobal parm;
  const wxString sPingType(PING_WINDOW_OPEN PING_WINDOW_TYPE);
  const wxString &sFrameNumber(GetFrameNumber());
  const wxChar * const psT = wxT("true");
  const wxChar * const psF = wxT("false");
  const wxChar * plist[] =
  {
    wxT(PING_EVENT),
    (const wxChar *)sPingType,
    wxT(PING_WINDOW_NUMBER),
    (const wxChar *)sFrameNumber,
    wxT("ShowDisabledAllele"),
    parm->GetPlotShowDisabledAlleles() ? psT : psF,
    wxT("ShowBins"),
    parm->GetPlotShowLadderBins() ? psT : psF,
    wxT("NoOAR"),
    (pFile == NULL) ? psT : psF,
    NULL
  };
  mainApp::PingList(plist);
  m_pPanel = new wxScrolledWindow(this,wxID_ANY,wxDefaultPosition, wxDefaultSize,wxVSCROLL);
  {
    m_bShowToolbar = !parm->GetHideGraphicToolbar();
    m_bShowScrollbars = !parm->GetHideGraphicScrollbar();
    m_bFixed = !parm->GetPlotResizable();
    m_nMinHeight = m_bFixed ? -1 : parm->GetPlotMinHeight();
    bool bXBPS = parm->GetPlotDataXBPS();
    SetXBPSValue(bXBPS);
  }
  _UpdateToolbarMenuLabel();
  _UpdateScrollbarMenuLabel();
  SetWindowScrollbarMenuLabel();
  m_pMenu->Check(IDmenuShowXBPS, m_bXBPS);

  SetOARfile(pFile);
  COsirisIcon x;
  SetIcon(x);
  m_pSizer = new wxBoxSizer(wxVERTICAL);
  m_pPanel->SetSizer(m_pSizer);

  CPanelPlot *p = GetPanelPlot(false);
//  p->ResetDefaults();
  m_pSizer->Add(p,1,wxEXPAND);
  if(bSingle)
  {
    p->EnableDelete(false);
    if(nChannel)
    {
      p->ShowOneChannel(nChannel);
    }
    else
    {
      p->ShowAllChannels();
    }
    p->RebuildCurves();
    _UpdateMenu();
  }
  else
  {
    SetMultiple(p);
  }
  SetMenuBar(new CMenuBarGraph(m_pMenu));
  wxBoxSizer *pSizerAll = new wxBoxSizer(wxVERTICAL);
  pSizerAll->Add(m_pPanel,1,wxEXPAND);
  SetSizer(pSizerAll);
  RE_RENDER;
}



void CFramePlot::ReInitialize(const wxString &sLocus, bool bSingle)
{
  TnwxBatchColl<CPanelPlot> batch(m_setPlots);
  CPanelPlot *pPlot = *m_setPlots.begin();
  pPlot->ResetDefaults();
  unsigned int nChannel = 0;
  int nScroll = 0;
  if(bSingle)
  {
    RemoveAllPlotsExcept(pPlot);
    if(!sLocus.IsEmpty())
    {
      nChannel = m_pData->GetChannelFromLocus(sLocus);
      if( (!nChannel) && (m_pOARfile != NULL) )
      {
        // OS-966, plt file may not have locus to channel nr
        // especially if it is a no-ladder analysis
        nChannel = m_pOARfile->GetChannelNrFromLocus(sLocus);
      }
    }
    if(nChannel > 0)
    {
      pPlot->ShowOneChannel(nChannel);
    }
    else
    {
      pPlot->ShowAllChannels();
    }
    pPlot->RebuildCurves(true);
    pPlot->ZoomToLocus(sLocus,0);
  }
  else
  {
    SetMultiple(pPlot);
    ZoomToLocus(sLocus,0);
    if( (m_nState == FP_VARIABLE_MANY_PLOTS) && (!sLocus.IsEmpty()) )
    {
      nChannel = m_pData->GetChannelFromLocus(sLocus);
      if( (!nChannel) && (m_pOARfile != NULL) )
      {
        // OS-966 - for ladder free, the .plt file may not have channel 
        // number from locus
        nChannel = m_pOARfile->GetChannelNrFromLocus(sLocus);
      }
      nScroll = int(nChannel) - 1;
    }
  }
  SetFocusPlot(nScroll);
#if FP_SCROLL_EVENT
//  SendScrollPlotEvent(nScroll);
#endif

}
wxString CFramePlot::GetFileName()
{
  wxString s(m_pData->GetLastFileName());
  return s;
}
int CFramePlot::GetType()
{
  return FRAME_PLOT;
}
wxMenu *CFramePlot::GetMenu()
{
  return m_pMenu;
}
wxMenu *CFramePlot::GetGraphMenu()
{
  return GetMenu();
}
void CFramePlot::OnTimer(wxTimerEvent &e)
{
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->OnTimer(e);
  }
  if(m_nScrollOnTimer >= 0)
  {
    _ScrollPlot();
  }
#if DELAY_PLOT_AREA_SYNC
  if(m_nSyncThisTimer > 0)
  {
    m_nSyncThisTimer--;
    if((!m_nSyncThisTimer) && (m_pPlotSyncThisTo != NULL))
    {
      this->_SyncTo(m_pPlotSyncThisTo);
      m_pPlotSyncThisTo = NULL;
    }
  }
#endif
}
void CFramePlot::UpdateOARfile(const wxString &sSampleFileName)
{
  // call this when the OAR file has been edited
  if( (m_pOARfile != NULL) &&
      nwxString::FileNameStringEqual(
      m_pData->GetFilename(),sSampleFileName) &&
      (GetSelectedTime() == NULL)
      )
  {
    _RebuildLabels(true);
  }
/*
  _SetupTitle();
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->RebuildLabels();
  }
*/
}
void CFramePlot::RebuildAll()
{
  CBatchPlot BATCH(this);
  m_pPanel->Show(false);
  _UpdateMenu();
  map<unsigned int, CPanelPlot *> mapNrPlot;
  typedef std::map<CPanelPlot *, wxRect2DDouble> SAVE_RECT;
  SAVE_RECT mapRect;
  set<CPanelPlot *>::iterator itr;
  map<unsigned int, CPanelPlot *>::iterator itrm;
  bool bEnableAppend = false;
  bool bEnableDelete = false;
  unsigned int nChannelCount = m_pData->GetChannelCount();
  unsigned int nMapSize;

  for (itr = m_setPlots.begin(); itr != m_setPlots.end(); ++itr)
  {
    if (mapNrPlot.size() < nChannelCount)
    {
      mapNrPlot.insert(
        map<unsigned int, CPanelPlot *>::value_type(
        (*itr)->GetPlotNumber(), *itr));
    }
    else
    {
      m_setPlotsHidden.insert(*itr);
    }
    m_pSizer->Detach(*itr);
    (*itr)->Show(false);
  }

  // kill all hidden plots

  for (itr = m_setPlotsHidden.begin();
    itr != m_setPlotsHidden.end();
    ++itr)
  {
    (*itr)->Destroy();
  }
  m_setPlotsHidden.clear();
  m_vpPlotsByMenuNumber.clear();
  m_setPlots.clear();

  nMapSize = (unsigned int)mapNrPlot.size();
  bEnableAppend = nMapSize < nChannelCount;
  bEnableDelete = nMapSize > 1;

  CPanelPlot *pplot = NULL;
  CPanelPlot *pold = NULL;

  vectorptr< TnwxBatch<CPanelPlot> > listBatch;
  listBatch.reserve(mapNrPlot.size());

  for (itrm = mapNrPlot.begin();
    itrm != mapNrPlot.end();
    ++itrm)
  {
    pplot = GetPanelPlot(false, itrm->first);
    listBatch.push_back(new TnwxBatch<CPanelPlot>(pplot));
    pold = itrm->second;
    pplot->CopySettings(*pold);
    // save view rect because it gets corrupted somewhere
    mapRect.insert(SAVE_RECT::value_type(pplot, pplot->GetViewRect()));
    pplot->EnableAppend(bEnableAppend);
    pplot->EnableDelete(bEnableDelete);
    pold->Destroy();
    m_pSizer->Add(pplot, 1, wxEXPAND);
  }
  _SetupTitle();
  _UpdateViewState(true);
  // restore view rects saved above
  for (SAVE_RECT::iterator itrr = mapRect.begin();
    itrr != mapRect.end();
    ++itrr)
  {
    pplot = itrr->first;
    pplot->SetViewRect(itrr->second, false, 1);
  }
}
void CFramePlot::SetOARfile(COARfile *pFile)
{
  if(pFile != m_pOARfile)
  {
    m_pOARfile = pFile;
    _SetupHistoryMenu();
    m_pParent->InsertWindow(this,m_pOARfile);
    _SetupTitle(); // change oar file in title
    set<CPanelPlot *>::iterator itr;
    set<CPanelPlot *> *pSet;
    set<CPanelPlot *> *pAllPanels[] =
    {
      &m_setPlots, &m_setPlotsHidden
    };
    size_t COUNT = sizeof(pAllPanels) / sizeof(pAllPanels[0]);
    for(size_t i = 0; i < COUNT; ++i)
    {
      pSet = pAllPanels[i];
      for(itr = pSet->begin(); itr != pSet->end(); ++itr)
      {
        (*itr)->SetOARfile(m_pOARfile);
      }
    }
    _RebuildLabels(true);
    //  need to send a resize event because
    //  the combo box does not resize when
    //  peak area is added
    Refresh();
  }
  else if(pFile == NULL)
  {
    _SetupTitle();
  }
}
void CFramePlot::_SetupHistoryMenu()
{
  if(m_pOARfile != NULL)
  {
    set<CPanelPlot *>::iterator itr;
    set<CPanelPlot *> *psets[] = { &m_setPlots, &m_setPlotsHidden };

    m_pMenuHistory = new CMenuHistory(m_pOARfile,false);
    _CleanupMenuHistoryPopup();
    CMenuHistory *pMenuPopup = _GetMenuHistoryPopup();
    m_pMenu->SetHistorySubMenu(m_pMenuHistory);

    for(size_t ndx = 0; ndx < 2; ++ndx)
    {
      set<CPanelPlot *> *pSet = psets[ndx];
      for(itr = pSet->begin(); itr != pSet->end(); ++itr)
      {
        (*itr)->SetHistoryMenu(pMenuPopup);
      }
    }
  }
}
void CFramePlot::_RebuildLabels(bool bForce)
{
  const wxDateTime *pTime = m_pMenuHistory->GetSelected();
  if( bForce || (!m_TimeLastRebuild.IsEqualTo(pTime)) )
  {
    m_TimeLastRebuild = pTime;
    set<CPanelPlot *>::iterator itr;
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
//      (*itr)->RebuildLabels(true);
      (*itr)->LabelTypeChanged();
      (*itr)->UpdateHistoryButtons();
    }
    _SetupTitle();
  }
}
bool CFramePlot::SetScrollbarMenuLabel(bool bShow)
{
  wxMenu *pMenu = GetMenu();
  bool bRtn = false;
  if(pMenu != NULL)
  {
    wxMenuItem *pItem = pMenu->FindItem(IDmenuShowHidePlotScrollbars);
    if(pItem != NULL)
    {
      const char *psLabel = bShow ? SHOW_PLOT_SCROLLBARS : HIDE_PLOT_SCROLLBARS;
      pItem->SetItemLabel(psLabel);
      bRtn = true;
    }
  }
  return bRtn;
}

bool CFramePlot::SetWindowScrollbarMenuLabel()
{
  wxMenu *pMenu = GetMenu();
  bool bRtn = false;
  if(pMenu != NULL)
  {
    wxMenuItem *pItem = pMenu->FindItem(IDmenuShowHideWindowScrollbar);
    if(pItem != NULL)
    {
      pItem->Check(!m_bFixed);
      bRtn = true;
    }
  }
  return bRtn;
}


void CFramePlot::_CleanupExportDialog()
{
  if(m_pDialogPlot != NULL)
  {
    delete m_pDialogPlot;
    m_pDialogPlot = NULL;
  }
}
CDialogExportPlot *CFramePlot::_GetExportDialog()
{
  if(m_pDialogPlot == NULL)
  {
    const wxString &sFile(m_pData->GetLastFileName());
    m_pDialogPlot = new CDialogExportPlot(this,wxID_ANY,sFile);
  }
  return m_pDialogPlot;
}

bool CFramePlot::MenuEvent(wxCommandEvent &e)
{
  int nID = e.GetId();
  bool bRtn = true;
/*
  long nLong = e.GetExtraLong();
  if( (nLong < 6) && (m_nMenuUp > 0) )
  {
    wxCommandEvent ee(e);
    ee.SetExtraLong(++nLong);
    GetEventHandler()->AddPendingEvent(ee);
  }
  else
*/
  if(ID_IS_HISTORY(nID))
  {
    m_pMenuHistory->Select(nID);
    if(m_pMenuHistoryPopup != NULL)
    {
      m_pMenuHistoryPopup->Select(nID);
    }
    _RebuildLabels(true);
  }
  else if(nID == IDmenuHistoryTop)
  {
    if(m_pOARfile != NULL)
    {
      wxASSERT_MSG(
        0,"Received IDhistoryTop menu when OAR file is available");
    }
    else
    {
      _FindOARfile(CDialogPlotMessageFind::MSG_TYPE_HISTORY, true);
    }

  }
  else if(nID == IDmenuTable || nID == IDmenuDisplaySample || nID == IDmenuSampleTile)
  {
#ifdef __WXMAC__
    if(!(  (nID == IDmenuSampleTile) && 
          CheckCannotTile(this,true)  ))
#endif
    {
      OnTableButton(e);
    }
  }
  else if(nID == IDmenuParameters)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    if(itr != m_setPlots.end())
    {
      (*itr)->OnBtnDetails(e);
      bRtn = true;
    }
  }
  else if(nID == IDmenuShowHideToolbar)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    bool bWasShown = (*itr)->IsToolbarShown();
    for( ;itr != m_setPlots.end(); ++itr)
    {
      (*itr)->ShowToolbar(!bWasShown);
    }
    m_bShowToolbar = !bWasShown;
    _UpdateToolbarMenuLabel();
    CParmOsirisGlobal parm;
    parm->SetHideGraphicToolbar(bWasShown);
  }
  else if(nID == IDmenuShowHideWindowScrollbar)
  {
    _ToggleFixed();
  }
  else if(nID == IDmenuShowHidePlotScrollbars)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    bool bWasShown = (*itr)->AreScrollbarsShown();
    for( ;itr != m_setPlots.end(); ++itr)
    {
      (*itr)->ShowScrollbars(!bWasShown);
    }
    m_bShowScrollbars = !bWasShown;
    _UpdateScrollbarMenuLabel();
    CParmOsirisGlobal parm;
    parm->SetHideGraphicScrollbar(bWasShown);
    //RebuildAll();
    RebuildCurves();
  }
  else if(nID == IDmenuShowXBPS)
  {
    CPointerHold<CPanelPlot> xxp(m_pPlotSyncTo);
    CIncrementer xxx(m_nInSync);
    bool bXBPS = m_pMenu->IsChecked(nID);
    CParmOsirisGlobal parm;
    parm->SetPlotDataXBPS(bXBPS);
    SetXBPSValue(bXBPS);
    RebuildCurves();
  }
  else if(nID == IDExportGraphic)
  {
    CDialogExportPlot *pDlg = _GetExportDialog();
    int n = pDlg->ShowModal();
    if(n == wxID_OK)
    {
      _CleanupExportDialog();
    }
  }
  else
  {
    int nPlot = ID_GET_PLOT_NUMBER(nID);
    if(nPlot >= 0 && nPlot < (int)m_vpPlotsByMenuNumber.size())
    {
      CPanelPlot *pPanel = m_vpPlotsByMenuNumber.at(nPlot);
      if( (m_pMenuPopup != NULL) &&
          ((wxMenu *)m_pMenuPopup == GetLastMenuShown()) )
      {
        CMenuPlot *pMenuPlot = pPanel->GetMenuPlot();
        CMenuPlot *pMenuPlotFrom = m_pMenuPopup->FindMenuPlot(nID);
        if(pMenuPlot == NULL || pMenuPlotFrom == NULL)
        {
          wxASSERT_MSG(0,
            "CFramePlot::MenuEvent() "
            "cannot find plot menu from popup");
        }
        else
        {
          pMenuPlot->CopySettings(*pMenuPlotFrom);
        }
      }
      bRtn = pPanel->MenuEvent(e);
    }
    else
    {
      bRtn = false;
    }
  }
  return bRtn;
}

void CFramePlot::_FindOARfile(int nType, bool bOpenTable)
{
  CDialogPlotMessageFind dlg(this,nType);
  int n = dlg.ShowModal();
  if(n == wxID_YES)
  {
    wxString sError;
    wxString sTypes = FILE_TYPE_REPORT;
    wxString sFile = m_pData->FindAnalysisFile();
    wxString sDir(sFile);
    bool bNotFound = sFile.IsEmpty();
    bool bDone = false;
    bool bNew = false;
    bool bShowTable = false;
    COARfile *pFile(NULL);
    if(bNotFound)
    {
      sDir = m_pData->GetLastFileName();
    }
    wxFileName fn(sDir);
    sDir = fn.GetPath();
    if(!bNotFound)
    {
      sFile = fn.GetFullName();
    }
    wxFileDialog fileDlg(
        this,"Find Analysis File",
        sDir,sFile,sTypes,
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    while(!bDone)
    {
      bNew = false;
      if(fileDlg.ShowModal() != wxID_OK)
      {
        bDone = true;
        pFile = NULL;
      }
      else
      {
        sFile = fileDlg.GetPath();
        pFile = m_pParent->FindOARfile(sFile);
        if(pFile != NULL)
        {
          if(_CheckAnalysisFile(pFile))
          {
            bDone = true;
          }
          else
          {
            pFile = NULL;
          }
        }
        else if(m_pParent->FileInProgress(sFile,true))
        {;}
        else
        {
          const char *psBadFile =
              "This is not a valid OSIRIS Analysis Report,\n"
              "possibly from an older version of OSIRIS";
          bool bShowError = false;
          if(_CheckIsXML(sFile))
          {
            pFile = new COARfile();
            if(!pFile->LoadFile(sFile))
            {
              bShowError = true;
            }
            else
            {
              bDone = _CheckAnalysisFile(pFile);
            }
            if(!bDone)
            {
              delete pFile;
              pFile = NULL;
            }
            else
            {
              bShowTable = bOpenTable;
            }
          }
          else
          {
            bShowError = true;
          }
          if(bShowError)
          {
            mainApp::ShowError(psBadFile,this);
          }
        }
      }
    }
    if(pFile != NULL)
    {
      SetOARfile(pFile);
      if (bShowTable)
      {
        wxCommandEvent e;
        e.SetId(-1);
        OnTableButton(e);
        Raise();
      }
      RebuildCurves();
    }
  }
}

bool CFramePlot::FindOARforType(int nType)
{
  if (m_pOARfile == NULL)
  {
    _FindOARfile(nType, true);
  }
  return (m_pOARfile != NULL);
}

bool CFramePlot::_CheckAnalysisFile(COARfile *pFile)
{
  wxString sError;
  if(pFile->GetKitName() != m_pData->GetKitName())
  {
    sError = "This analysis file does not use\n"
                "the same kit as the plot data.";
  }
  else if(pFile->GetSampleByName(m_pData->GetFilename()) == NULL)
  {
    sError = "This sample, ";
    sError.Append(m_pData->GetFilename());
    sError.Append(", is not found in this analysis file");
  }
  bool bRtn = sError.IsEmpty();
  if(!bRtn)
  {
    mainApp::ShowError(sError,this);
  }
  return bRtn;
}
bool CFramePlot::_CheckIsXML(const wxString &sFileName)
{
  wxFile file(sFileName);
  const size_t BUFSIZE = 12;
  char sBuf[BUFSIZE + 1];
  size_t nRead = 0;
  bool bRtn = false;
  if(file.IsOpened())
  {
    nRead = file.Read((void *)sBuf,BUFSIZE);
    file.Close();
    bRtn = (nRead == BUFSIZE) && (sBuf[0] == '<');
  }
  return bRtn;
}

void CFramePlot::OnClose(wxCloseEvent &)
{
  Destroy();
}
void CFramePlot::OnHistoryUpdate(wxCommandEvent &e)
{
  CPanelHistoryMenu *pPanel = (CPanelHistoryMenu *)e.GetEventObject();
  if(pPanel != NULL)
  {
    m_pMenuHistory->SelectTime(pPanel->GetSelectedTime());
  }
  _RebuildLabels(false);
}
void CFramePlot::UpdateHistory()
{
  //  call this when a file is saved and therefore
  //  the history list has a new entry and call _SetupTitle
  //  because the file name may have changed
  //
  //  also call this if a file was modified, the analysis window
  //  was closed and changes were discarded, therefore the
  //  OAR file was reloaded.
  if(m_pMenuHistory != NULL)
  {
    _SetupHistoryMenu(); // rebuild history menu
    _SetupTitle();
    _RebuildLabels(true);
  }
}


void CFramePlot::CheckFileModification()
{
  if(!DialogIsShowingOrNoFocus())
  {
    wxString sOARfile;
    wxString sPLTfile;
    vector<wxString> vsFiles;
    bool bReload = false;
    bool bError = false;
    bool bReloadPlot = false;
    bool bCancel = false;
    size_t nSize;
    CFrameAnalysis *pFrameAnalysis(NULL);
    vsFiles.reserve(3);
    if( (m_pOARfile != NULL) &&
      (!m_pParent->FileInProgress(m_pOARfile->GetLastFileName())) &&
      m_pOARfile->CheckFileModification() )
    {
      sOARfile = m_pOARfile->GetLastFileName();
      vsFiles.push_back(sOARfile);
      if(m_pOARfile->IsModified())
      {
        pFrameAnalysis = m_pParent->FindAnalysisFrame(m_pOARfile);
      }
    }
    if(  (!m_pParent->FileInProgress(m_pData->GetLastFileName())) &&
      m_pData->CheckFileModification() )
    {
      sPLTfile = m_pData->GetLastFileName();
      vsFiles.push_back(sPLTfile);
    }
    nSize = vsFiles.size();
    if(nSize)
    {
      if(nSize == 1)
      {
        wxString sEmpty;
        vsFiles.push_back(sEmpty);
      }
      if(PromptReload(vsFiles.at(0),vsFiles.at(1)))
      {
        if(pFrameAnalysis != NULL)
        {
          if(pFrameAnalysis->CheckSaveOnCloseFile())
          {
            // user was prompted to save changes
            // and did not cancel
            bReload = m_pOARfile->LoadFile(sOARfile);
            bError = !bReload;
          }
          else
          {
            bCancel = true;
          }
        }
        else if(!sOARfile.IsEmpty())
        {
          bReload = m_pOARfile->ReloadFile();
          bError = !bReload;
        }
        if(bError)
        {
          mainApp::ShowError(
            "A problem occurred when reading an OAR file\n",this);
          m_pParent->KillOARfile(m_pOARfile);
        }
        else if(!bCancel)
        {
          if(!sPLTfile.IsEmpty())
          {
            unique_ptr<CPlotData> pPlotDataHole(m_pData);
            m_pData = new CPlotData();
            bReloadPlot = m_pData->LoadFile(sPLTfile);
            if(bReloadPlot)
            {
              RebuildAll();
            }
            else
            {
  //            m_pParent->RemoveWindow(this);
              _Kill();
            }
          }
          if(bReload)
          {
            m_pParent->UpdateOARfile(m_pOARfile);
          }
        }
      }
    }
  }
}
void CFramePlot::UpdateLadderLabels()
{
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->UpdateLadderLabels();
  }
}
void CFramePlot::_CleanupBins()
{
  set<CPanelPlot *>::iterator itr;
  for (itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->CleanupBins();
  }
  for (itr = m_setPlotsHidden.begin();
    itr != m_setPlotsHidden.end();
    ++itr)
  {
    (*itr)->CleanupBins();
  }
}

void CFramePlot::SyncToolbars(CPanelPlot *p)
{
  set<CPanelPlot *>::iterator itr;
  for(itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->SyncToolbar(p);
  }
}

void CFramePlot::AddPlot(CPanelPlot *pPreceed, bool bUpdateView)
{
  CBatchPlot BATCH(this);
  CPanelPlot *p;
  {
    CPanelPlotToolbarSaveState xxx(bUpdateView ? pPreceed : NULL);
    wxRect2DDouble rect = pPreceed->GetViewRect();
    unsigned int nr = pPreceed->GetPlotNumber() + 1;
    size_t nSize = m_setPlots.size();
    if(nSize == 1)
    {
      pPreceed->EnableDelete(true);
    }
    p = GetPanelPlot(false,nr);
    if(nr == (unsigned int)nSize)
    {
      m_pSizer->Add(p,1,wxEXPAND);
    }
    else
    {
      m_pSizer->Insert(nr,p,1,wxEXPAND);
      UpdatePlotNumbers();
    }
    pPreceed->SetViewRect(rect, false, 1);
    p->CopySettings(*pPreceed);
    if(bUpdateView)
    {
      _UpdateViewState();
    }
  } // calls destructor to xxx() above
  p->SyncToolbar(pPreceed);
  _UpdateMenu();
}
void CFramePlot::_RebuildMenu()
{
  CBatchPlotMenu BATCH(m_pMenu);
  wxSizerItem *pItem;
  CPanelPlot *pCheck;
  size_t nSize = m_setPlots.size();
  m_pMenu->RemoveAllPlots();
  for(size_t i = 0; i < nSize; i++)
  {
    pItem = m_pSizer->GetItem(i);
    pCheck = (pItem == NULL)
      ? NULL : (CPanelPlot *)(pItem->GetWindow());
    if(pCheck != NULL)
    {
      wxMenuItem *pItem2 = pCheck->GetMenuItem(m_pMenu);
      m_pMenu->AddPlot(pItem2);
    }
  }
}
void CFramePlot::UpdatePlotNumbers()
{
  wxSizerItem *pItem;
  CPanelPlot *pCheck;
  size_t nSize = m_setPlots.size();
  bool bAppend = (nSize < (size_t)(m_pData->GetChannelCount()));
  for(size_t i = 0; i < nSize; i++)
  {
    pItem = m_pSizer->GetItem(i);
    pCheck = (pItem == NULL)
      ? NULL : (CPanelPlot *)(pItem->GetWindow());
    if(pCheck != NULL)
    {
      pCheck->SetPlotNumber((unsigned int)i);
      pCheck->EnableAppend(bAppend);
    }
  }
}
void CFramePlot::RemovePlot(CPanelPlot *pRemove, bool bRefresh)
{
  CBatchPlot BATCH(this);
  size_t nSize = m_setPlots.size();
  if(nSize > 1)
  {
    set<CPanelPlot *>::iterator itr = m_setPlots.find(pRemove);
    if(itr != m_setPlots.end())
    {
      nwxHoldScrollPos xx(m_pPanel);
      m_setPlots.erase(itr);
      m_setPlotsHidden.insert(pRemove);
      m_pSizer->Detach(pRemove);
      pRemove->Show(false);
      if(nSize == 2)
      {
        // we are now down to one plot
        itr = m_setPlots.begin();
        (*itr)->EnableDelete(false);
      }
      if(bRefresh)
      {
        UpdatePlotNumbers();
        _UpdateMenu();
        _UpdateViewState();
      }
    }
  }
}

void CFramePlot::RemoveAllPlotsExcept(CPanelPlot *pRemove)
{
  CBatchPlot BATCH(this);
  set<CPanelPlot *>::iterator itr;
  CPanelPlot *pPanel;
  for(size_t nSize = m_setPlots.size();
      nSize > 1;
      nSize = m_setPlots.size())
  {
    itr = m_setPlots.begin();
    pPanel = *itr;
    if(pPanel == pRemove)
    {
      ++itr;
      pPanel = *itr;
    }
    RemovePlot(pPanel, nSize == 2);
  }
}

void CFramePlot::SetMultiple(CPanelPlot *p, bool bCurrentView)
{
  CBatchPlot BATCH(this);
  CPanelPlotToolbarSaveState tbState(p);
  wxWindowUpdateLocker x(this);
  set<CPanelPlot *>::iterator itr;
  wxRect2DDouble rect = p->GetViewRect();
  size_t nSize = m_setPlots.size();
  size_t nNeeded = m_pData->GetChannelCount() - nSize;
  CPanelPlot *pCheck;
  CIncrementer incr(m_nInSync);
  if(nSize > 1)
  {
    for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pCheck = *itr;
      if(pCheck != p)
      {
        pCheck->CopySettings(*p);
      }
    }
  }
  for(size_t n = 0; n < nNeeded; n++)
  {
    AddPlot(p,false);
  }
  for(itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
  {
    pCheck = *itr;
    pCheck->ShowOneChannel(pCheck->GetPlotNumber() + 1);
    pCheck->RebuildCurves(true);
  }
  if(bCurrentView)
  {
    ZoomAll(rect);
  }
  _UpdateViewState();
  tbState.RestoreState(true);
  SyncToolbars(p);
}

wxRect2DDouble CFramePlot::GetZoomOutRect(bool bAll)
{
  // begin with a unit rectangle,
  // then expand to include all plots where sync is on
  wxRect2DDouble rtn(0.0,0.0,1.0,1.0);
  CParmOsirisGlobal parm;
  bool bXBPS = parm->GetPlotDataXBPS()
    ? m_pData->CanSetBPS() : false;
  if(!bAll)
  {
    double dx = (double)(m_pData->GetBegin());
    if(bXBPS)
    {
      dx = m_pData->TimeToILSBps(dx);
    }
    rtn.m_x = dx;
  }
  for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    CPanelPlot *pPlot = *itr;
    if(pPlot->SyncValue())
    {
      rtn.Union(pPlot->GetZoomOutRect(bAll));
    }
  }
  return rtn;
}
CMenuHistory *CFramePlot::_GetMenuHistoryPopup()
{
  if(m_pMenuHistory != NULL && m_pMenuHistoryPopup == NULL)
  {
    m_pMenuHistoryPopup = m_pMenuHistory->Clone();
  }
  return m_pMenuHistoryPopup;
}
CFramePlotMenu *CFramePlot::_GetMenuPopup()
{
  _CleanupMenuPopup();
  m_pMenuPopup = (m_pMenu == NULL) ? NULL : m_pMenu->Clone();
  return m_pMenuPopup;
}
CPanelPlot *CFramePlot::GetPanelPlot(bool bFirst, unsigned int nr)
{
  CPanelPlot *pRtn(NULL);
  set<CPanelPlot *>::iterator itr = m_setPlotsHidden.begin();
  if(itr == m_setPlotsHidden.end())
  {
    int nMenu = (int)m_vpPlotsByMenuNumber.size();
    CMenuHistory *pMenuHist = _GetMenuHistoryPopup();
    pRtn = new CPanelPlot(this, m_pData, m_pOARfile, pMenuHist, m_pColors, nMenu, bFirst, nr); // , true);  // EXT TIMER
    m_vpPlotsByMenuNumber.push_back(pRtn);
    if(bFirst)
    {
      TnwxBatch<CPanelPlot> x(pRtn);
      pRtn->RebuildCurves(true);
      pRtn->ZoomOut();
    }
  }
  else
  {
    pRtn = *itr;
    m_setPlotsHidden.erase(itr);
    pRtn->SetPlotNumber(nr);
    pRtn->Show(true);
  }
  m_setPlots.insert(pRtn);
  pRtn->ShowScrollbars(m_bShowScrollbars);
  pRtn->ShowToolbar(m_bShowToolbar);
  return pRtn;
}

void CFramePlot::SyncState(CPanelPlot *p, int nID)
{
  CPanelPlot *pPlot;
  for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    pPlot = *itr;
    if(pPlot != p)
    {
      pPlot->SyncState(p,nID);
    }
  }
}
void CFramePlot::SyncThis(CPanelPlot *p)
{
  if( p->SyncValue() && (m_setPlots.size() > 1) )
  {
    CPanelPlot *pPlot;
    for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      if( (pPlot != p) && pPlot->SyncValue() )
      {
        TnwxBatch<CPanelPlot> xxx(p);
        p->SetViewRect(pPlot->GetViewRect());
        break;
      }
    }
    Refresh();
  }
}
bool CFramePlot::_SyncTo(CPanelPlot *p)
{
  bool bRtn = false;
  if( p->SyncValue() && (m_setPlots.size() > 1) && !m_nInSync)
  {
    TnwxBatchColl<CPanelPlot> batch(m_setPlots);
    CPointerHold<CPanelPlot> xxx(m_pPlotSyncTo,p);
    CIncrementer incr(m_nInSync);
    wxRect2DDouble r = p->GetViewRect();
    wxRect2DDouble rect;
    CPanelPlot *pPlot;
    for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      if( (pPlot != p) && pPlot->SyncValue())
      {
        TnwxBatch<CPanelPlot> xxx2(pPlot);
        pPlot->SetViewRect(r, false, 0);
        bRtn = true;
      }
    }
    Refresh();
  }
  return bRtn;
}


class DelayZoomToLocus : public nwxTimerTaskCount
{
public:
  // delay a call to CFramePlot::ZoomToLocus() by 'nDelay' timer intervals
  // nwxTimerTaskCount is in ../nwx/nwxTimerReceiver.h, cpp
  DelayZoomToLocus(CFramePlot *pPlot, const wxString &sLocus, unsigned int nDelay) :
    nwxTimerTaskCount(nDelay),
    m_sLocus(sLocus),
    m_pFrame(pPlot)
  {}
  virtual ~DelayZoomToLocus() {}
  virtual bool Run(wxTimerEvent &)
  {
    bool b = m_pFrame->IsShown();
    if (b)
    {
      m_pFrame->ZoomToLocus(m_sLocus, 0);
    }
    return b;
  }
private:
  const wxString m_sLocus;
  CFramePlot *m_pFrame;
};

class DelayZoomOut : public nwxTimerTaskCount
{
public:
  // delay a call to CFramePlot::ZoomOut() by 'nDelay' timer intervals
  // nwxTimerTaskCount is in ../nwx/nwxTimerReceiver.h, cpp
  DelayZoomOut(CFramePlot *pPlot, bool bAll, unsigned int nDelay) :
    nwxTimerTaskCount(nDelay),
    m_pFrame(pPlot),
    m_bAll(bAll)
  {}
  virtual ~DelayZoomOut() {}
  virtual bool Run(wxTimerEvent &)
  {
    bool b = m_pFrame->IsShown();
    if (b)
    {
      m_pFrame->ZoomOut(m_bAll, 0);
    }
    return b;
  }
private:
  CFramePlot *m_pFrame;
  bool m_bAll;
};

void CFramePlot::ZoomOut(bool bAll,unsigned int nDelay )
{
  if (nDelay)
  {
    AddTask(new DelayZoomOut(this, bAll, nDelay));
  }
  else
  {
    wxRect2DDouble r = GetZoomOutRect(bAll);
    ZoomAll(r, nDelay);
  }
}

void CFramePlot::ZoomToLocus(const wxString &sLocus,unsigned int nDelay )
{
  if (nDelay)
  {
    AddTask(new DelayZoomToLocus(this, sLocus, nDelay));
  }
  else if(!m_setPlots.size()) {} // do nothing
  else if(sLocus.IsEmpty())
  {
    // no locus specified
    ZoomOut(false, nDelay);
  }
  else
  {
    // locus specified
    set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    wxRect2DDouble rect = (*itr)->GetZoomLocus(sLocus);
    ZoomAll(rect,nDelay);
  }
}

void CFramePlot::ZoomAll(const wxRect2DDouble &rect,unsigned int nDelay )
{
  CPanelPlot *pFirstSync = NULL;
  {
    CParmOsirisGlobal parm;
    TnwxBatchColl< CPanelPlot > BATCH(m_setPlots);
    for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
        itr != m_setPlots.end();
        ++itr)
    {
      (*itr)->SetViewRect(rect,false,0);
      if(!(*itr)->SyncValue())
      {
        // not sync'd
        if(nDelay)
        {
          (*itr)->SetViewRect(rect,false,nDelay);
        }
      }
      else if(pFirstSync == NULL)
      {
        pFirstSync = *itr;
      }
    }

    if(pFirstSync != NULL)
    {
      SyncTo(pFirstSync);
      if(nDelay)
      {
        pFirstSync->SetViewRect(rect,true,nDelay);
      }
    }
  }
}
void CFramePlot::_CleanupMenuPopup()
{
  if(m_pMenuPopup != NULL)
  {
    delete m_pMenuPopup;
    m_pMenuPopup = NULL;
  }
}
void CFramePlot::OnContextMenu(wxContextMenuEvent &)
{
  if((!m_nMenuUp) && (m_pMenu != NULL))
  {
    // there have been problem with this event
    // being triggered when the menu is already up.
    // This is being prevented by making sure
    // that m_nMenuUp is zero

    CIncrementer x(m_nMenuUp);
    CFramePlotMenu *pMenu = _GetMenuPopup();
    PopupMenu_(pMenu);
    _CleanupMenuPopup();
  }
}

void CFramePlot::EditPeak(COARpeakAny *pPeak)
{
  if (pPeak != NULL)
  {
    CFrameAnalysis *pFrame = NULL;
    if (m_pOARfile == NULL)
    {
      _FindOARfile(CDialogPlotMessageFind::MSG_TYPE_EDIT, true);
    }
    if (m_pOARfile != NULL)
    {
      pFrame = m_pParent->FindAnalysisFrame(m_pOARfile);
      if (pFrame == NULL)
      {
        wxCommandEvent e;
        e.SetId(0);
        OnTableButton(e);
        pFrame = m_pParent->FindAnalysisFrame(m_pOARfile);
        RaiseWindow();
      }
    }
    if (pFrame != NULL)
    {
      pFrame->EditPeak(pPeak, GetSample(), this);
    }
  }
}
CFrameAnalysis *CFramePlot::FindAnalysisFrame()
{
  CFrameAnalysis *pRtn = (m_pOARfile == NULL)
    ? NULL : m_pParent->FindAnalysisFrame(m_pOARfile);
  return pRtn;
}

COARsample *CFramePlot::GetSample()
{
  if ((m_pOARfile != NULL) && (m_pSample == NULL))
  {
    m_pSample = m_pOARfile->GetSampleByName(m_pData->GetFilename());
  }
  return m_pSample;
}

void CFramePlot::OnHistoryButton(wxCommandEvent &e)
{
  if(m_pOARfile != NULL)
  {
    e.Skip();
  }
  else
  {
    CPanelPlot *p = *m_setPlots.begin();
    CPanelPlotToolbarSaveState xxx(p);
    _FindOARfile(CDialogPlotMessageFind::MSG_TYPE_HISTORY, true);
    xxx.RestoreState(true);
    SyncToolbars(p);
  }
}
void CFramePlot::OnTableButton(wxCommandEvent &e)
{
  //  callback function for the table or sample button

  if(m_pOARfile == NULL)
  {
    _FindOARfile(CDialogPlotMessageFind::MSG_TYPE_TABLE);
    if (m_pOARfile == NULL)
    {
    }
    else if (e.GetId() > 0)
    {
      // if this is a menu event,change to button
      // because processing is faster
      wxCommandEvent ee(e);
      ee.SetEventType(wxEVT_COMMAND_BUTTON_CLICKED);
      ee.SetEventObject(this);
      GetEventHandler()->AddPendingEvent(e);
    }
    else
    {
      // call recursively to force analysis 
      //  frame to show immediately
      OnTableButton(e);
    }
  }
  else
  {
    m_pParent->OpenFile(m_pOARfile->GetLastFileName(),wxEmptyString,m_pOARfile);
    CFrameAnalysis *pFrame = m_pParent->FindAnalysisFrame(m_pOARfile);
    if(pFrame != NULL)
    {
      const wxString &sFile = m_pData->GetFilename();
      pFrame->SelectSample(sFile);
      int nID = e.GetId();
      if(nID == IDmenuDisplaySample || nID == IDmenuSampleTile)
      {
        COARsample *pSample = m_pOARfile->GetSampleByName(sFile);
        if(pSample != NULL)
        {
          RaiseWindow();
          pFrame->ShowSampleFrame(pSample,wxEmptyString, SA_NDX_SAMPLE,nID);
        }
      }
    }
  }
}

void CFramePlot::OnSashDragged(wxSashEvent &e)
{
  CPanelPlot *pw = (CPanelPlot *) e.GetEventObject();
  wxRect r = e.GetDragRect();
  wxPoint pt = pw->GetPosition();
  wxSize szC = m_pPanel->GetClientSize();
  int nH = r.GetHeight();
  int nHC = szC.GetHeight();
  if(nH > nHC)
  {
    nH = nHC;
  }
  _UpdateMinHeight(nH);
  _SendSizeAction();
  pw->SetFocusPlot();
  _SetPlotScroll(pw->GetPlotNumber(),pt.y);
}
void CFramePlot::_UpdateViewState(bool bForce)
{
  //
  // this is called when the number of plots
  // has changed, including when the window
  // is created
  const int DELAY = 1;
  const int WITH_FORCE = 2;
  const int DELAY_WITH_FORCE = 3;
  if(IsInBatch())
  {
    m_nDelayViewState |= (bForce ? DELAY_WITH_FORCE : DELAY);
  }
  else
  {
    bool bReRender = false;
    CFramePlotState nState =
      m_bFixed 
      ? FP_FIXED
      : ( 
          (m_setPlots.size() > 1)
          ? FP_VARIABLE_MANY_PLOTS
          : FP_VARIABLE_1PLOT
        );
    if(bForce || 
      (m_nDelayViewState & WITH_FORCE) || 
      (nState != m_nState))
    {
      m_nState = nState;
      if(m_nState != FP_VARIABLE_MANY_PLOTS)
      {
        m_nMinHeight = -1;
        m_pPanel->SetScrollbars(0,0,0,0,0,0,true);
      }
      else
      {
        m_pPanel->SetScrollRate(0,SCROLL_UNITS);
      }
      bReRender = true;
    }
    if(m_nState == FP_VARIABLE_MANY_PLOTS)
    {
      _UpdateMinHeight();
      _UpdateVirtualWidth();
    }
    m_nDelayViewState = 0;
    if (bReRender)
    {
      mainApp::ReRender(m_pPanel);
    }
  }
}
void CFramePlot::_UpdateVirtualWidth()
{
  wxSize szC = m_pPanel->GetClientSize();
  wxSize szV = m_pPanel->GetVirtualSize();
  if(szC.GetWidth() != szV.GetWidth())
  {
    szV.SetWidth(szC.GetWidth());
    m_pPanel->SetVirtualSize(szV);
  }
}
void CFramePlot::_UpdateMinHeight()
{
  if(m_bFixed || (m_nMinHeight > 0))
  {
    CParmOsirisGlobal parm;
    parm->SetPlotMinHeight(m_nMinHeight);
  }
  bool bShowSash = _ShowSash();
  int nMinHeight = bShowSash ? m_nMinHeight : -1;
  for(set<CPanelPlot *>::iterator itr = m_setPlots.begin();
    itr != m_setPlots.end();
    ++itr)
  {
    (*itr)->SetSashAndMinHeight(bShowSash,nMinHeight);
  }
  bool bSync = true;
  if(nMinHeight > 0)
  {
    int nTotalHeight = 0;
    int nClientHeight = 0;
    wxSize szC = m_pPanel->GetClientSize();
#ifdef _DEBUG
      wxSize szV = m_pPanel->GetVirtualSize();
#endif
    nTotalHeight = 
      m_nMinHeight * (int)m_setPlots.size();
    nClientHeight = szC.GetHeight();
    if(nTotalHeight >= nClientHeight)
    {
      szC.SetHeight(nTotalHeight);
      m_pPanel->SetVirtualSize(szC);
      bSync = false;
    }
  }
  if(bSync)
  {
    _SyncSize();
  }
}
void CFramePlot::_SyncSize()
{
  wxSize szC = m_pPanel->GetClientSize();
  wxSize szV = m_pPanel->GetVirtualSize();
  if(szV != szC)
  {
    m_pPanel->SetVirtualSize(szC);
#ifdef _DEBUG
    szV = m_pPanel->GetVirtualSize();
#endif
  }
}
void CFramePlot::_SetFixed(bool bFixed)
{
  if(bFixed != m_bFixed)
  {
    m_bFixed = bFixed;
    {
      CParmOsirisGlobal parm;
      parm->SetPlotFixed(m_bFixed);
    }
    RebuildAll();
  }
  else
  {
    _SendSizeAction();
  }
  SetWindowScrollbarMenuLabel();
}
void CFramePlot::SetFocusPlot(int nPlot)
{
  // nPlot is the plot number (0 based)
  if( (nPlot >= (int)m_setPlots.size()) || (nPlot < 0) )
  {
    nPlot = 0;
  }
  wxSizerItem *pItem = m_pSizer->GetItem(nPlot);
  CPanelPlot *pWin = (pItem != NULL) ? (CPanelPlot *)pItem->GetWindow() : NULL;
  if(pWin != NULL)
  {
    pWin->SetFocusPlot();
  }
#if FP_SCROLL_EVENT
  SendScrollPlotEvent(nPlot,0);
#else
  _SetPlotScroll(nPlot,0);
#endif
}

void CFramePlot::_ScrollPlot()
{
  if( (m_nState == FP_VARIABLE_MANY_PLOTS) && IsShown())
  {
    int nScrollY = 0;
    bool bCancel = false;
    wxSize szC = m_pPanel->GetClientSize();
    if(szC.GetHeight() < 100)
    {
      bCancel = true; // window not sized and layed out yet
    }
    else if(m_nScrollOnTimer < 1) 
    {
      // nScrollY = 0; // scrolling to the top, already set to 0
    }
    else if(m_nScrollOnTimer >= (int)(m_setPlots.size() - 1))
    {
      // last plot, scrolling to the bottom
      nScrollY = 0x7fffffff; // need a number > max scroll units
    }
#if 1
    else
    {
      wxSize sz;
      wxSizerItem *pItem;
      for(int i = 0; i < m_nScrollOnTimer; i++)
      {
        pItem = m_pSizer->GetItem(i);
        if(pItem != NULL)
        {
          sz = pItem->GetSize();
          nScrollY += sz.GetHeight();
        }
      }
    }
#else
    else
    {
      // scroll such that plot following this one
      // starts at the bottom of the window
      // n is 1 based and m_pSizerItem->GetItem() is zero based,
      // so get the position of Item(n) and scroll it to the bottom
      // which is the position - client height
      //
      int nX;
      int nY;
      wxSizerItem *pItem = m_pSizer->GetItem(m_nScrollOnTimer);
      m_pPanel->GetViewStart(&nX,&nY);
      wxPoint posStart(SCROLL_UNITS_TO_PIXELS(nX),SCROLL_UNITS_TO_PIXELS(nY));
      wxPoint pos = pItem->GetPosition();
      wxSize szC = m_pPanel->GetClientSize();
      if(szC.GetHeight() < 100)
      {
        bCancel = true; // window not sized and layed out yet
      }
      else if(pos.y < posStart.y)
      {
        // need to scroll up
        nScrollY = pos.y;
      }
      else
      {
        wxSize szItem = pItem->GetSize();
        int nYWindowBottom = posStart.y + szC.GetHeight();
        int nYPlotBottom = pos.y + szItem.GetHeight();
        if(nYPlotBottom <= nYWindowBottom)
        {
          bCancel = true; // no scrolling needed
        }
        else if(szC.GetHeight() < (szItem.GetHeight() + SCROLL_UNITS))
        {
          nScrollY = pos.y;
        }
        else
        {
          // scroll bottom of window into view
          // add (SCROLL_UNITS - 1) to round scroll units up
          nScrollY = nYPlotBottom - szC.GetHeight() + SCROLL_UNITS - 1;
        }
      }
    }
#endif
    if(!bCancel)
    {
      nScrollY -= m_nScrollOnTimerOffset;
      if(nScrollY > 0)
      {
        nScrollY = PIXEL_TO_SCROLL_UNITS(nScrollY);
      }
      else
      {
        nScrollY = 0;
      }
      m_pPanel->Scroll(0,nScrollY);
      _ResetPlotScroll();
    }
  }
}
#if FP_SCROLL_EVENT

void CFramePlot::OnScrollPlot(wxCommandEvent &e)
{
  int nInt = e.GetInt();
  int nDelay;
  int nPlot;
  _ExtractScrollEventInt(nInt, &nPlot,&nDelay);
  if(nDelay > 1)
  {
    SendScrollPlotEvent(nPlot,nDelay - 1);
  }
  else
  {
    ScrollPlot(nPlot);
  }
}
#endif
void CFramePlot::OnSizeAction(wxCommandEvent &e)
{
  int nInt = e.GetInt();
  if(!(IsShown() && m_pPanel->IsShown()))
  {} // do nothing
  else if(nInt > 1)
  {
    _SendSizeAction(nInt - 1);
  }
  else if(m_nState ==  FP_VARIABLE_MANY_PLOTS)
  {
    wxSize szC = m_pPanel->GetClientSize();
    int nHC = szC.GetHeight();
    if(nHC < m_nMinHeight)
    {
      // size of window is smaller than one 
      // plot, so make the min plot height smaller
      _UpdateMinHeight(nHC);
    }
    else
    {
      // when scrollbar appears, client width
      // becomes smaller and virtual width
      // needs to be updated 
      _UpdateVirtualWidth();
    }
  }
  else
  {
    _SyncSize();
  }
}
bool CFramePlot::Show(bool show)
{
  bool bRtn = CMDIFrame::Show(show);
  if(bRtn && show)
  {
    TnwxBatchColl<CPanelPlot> batch(m_setPlots);
    _UpdateViewState(true);
    m_pPanel->Scroll(0,0);
  }
  return bRtn;
}
void CFramePlot::OnSize(wxSizeEvent &e)
{
  _SendSizeAction();
  e.Skip(true);
}


void CFramePlot::_SetupBitmapPlot()
{
  // called from CFramePlot::_GetBitmapPlot()
  if (m_pPlotForBitmap != NULL)
  {
    m_pPlotForBitmap->Destroy();
  }
  m_pPlotForBitmap = new CPanelPlot(this, m_pData, m_pOARfile, NULL, m_pColors, 0, false, 0, true);   // EXT TIMER , true);
  m_pPlotForBitmap->SetRenderingToWindow(false);
  m_pPlotForBitmap->SetSync(false);
  m_pPlotForBitmap->Show(false);
  m_pPlotForBitmap->BeginBatch();
}

wxBitmap *CFramePlot::CreateBitmap(
  int nWidth, int nHeight, int nDPI, 
  const wxString &sTitlePNG, // used only for export PNG, should be empty for printing
  int nPlotsPerPage, // this and following parameters are used for printing
  int nPageNr,
  bool bForcePrintFont)
{
  wxSize sz(nWidth, nHeight);
  wxBitmap *pBitmap = new wxBitmap(sz, 32);
  wxMemoryDC dc(*pBitmap);
  // initialize bitmap to white -- probably not necessary
  dc.SetBackground(*wxWHITE_BRUSH);
  dc.SetBackgroundMode(wxPENSTYLE_TRANSPARENT);
  dc.Clear();
  if(nPageNr > 0)
  {
    wxRect rect(sz);
    double dLabelExtension;
    double dDPI = (double)nDPI;
    int nPage = nPageNr - 1;
    int nPlotCount = (int)m_setPlots.size();
    if (nPlotsPerPage > nPlotCount)
    {
      nPlotsPerPage = nPlotCount;
    }
    int nSkip = nPage * nPlotsPerPage;
    int nPlotsOnThisPage = nPlotCount - nSkip;
    if (nPlotsOnThisPage > nPlotsPerPage)
    {
      nPlotsOnThisPage = nPlotsPerPage;
    }
    wxString sTitle(sTitlePNG);
    wxString sTitleInfo;
    CPanelPlot *pPanelPlot = _GetBitmapPlot(bForcePrintFont);

    if (bForcePrintFont && sTitlePNG.IsEmpty())
    {
      pPanelPlot->TitleStrings(&sTitleInfo, nPageNr);
      pPanelPlot->SampleTitle(&sTitle);
    }
    int nXLabelHeight = pPanelPlot->GetXAxisLabelHeight(&dc, rect, dDPI, bForcePrintFont);
    int nTitleOffset = pPanelPlot->DrawPlotTitleToDC(
      &dc, sTitle, sTitleInfo, nWidth, nHeight, dDPI, bForcePrintFont);

    int nLabelHeight = pPanelPlot->GetPlotCtrl()->GetTextHeight(wxT("-123456789.0"), &dc, rect, dDPI, bForcePrintFont);

    int nPlotAreaHeight = nHeight - nXLabelHeight - nTitleOffset;
    int nPlotHeight = nPlotAreaHeight / nPlotsPerPage;
    int nRoundingOffset = nPlotAreaHeight - (nPlotHeight * nPlotsPerPage);
    nTitleOffset += nRoundingOffset;

    if (nPlotHeight >= 20)
    {
      wxRect rectPlot(0, 0, nWidth, nPlotHeight);
      set<CPanelPlot *>::iterator itr;
      CPanelPlot *pPlot;
      CPanelPlot *vpPlots[CHANNEL_MAX];
      memset((void *)vpPlots, 0, sizeof(vpPlots));
      std::unique_ptr<wxBitmap> pBitmapPlot
      (new wxBitmap(nWidth, nPlotHeight, 32));
      wxMemoryDC dcPlot(*pBitmapPlot);
      int nY;
      unsigned int nr;
      bool bSyncDone = false;

#if 1
      // there is a axis sync bug, so here is the work around
      // added - put plots in vpPlots array in plot number order
      for (itr = m_setPlots.begin();
        itr != m_setPlots.end();
        ++itr)
      {
        pPlot = *itr;
        nr = pPlot->GetPlotNumber();
        vpPlots[nr] = pPlot;
        if ((!bSyncDone) && pPlot->SyncValue())
        {
          SyncTo(pPlot);
          bSyncDone = true;
        }
      }
#endif
      int nPlotsToRender = nPlotsOnThisPage;
      int nLast = nSkip + nPlotsToRender;
      nY = nTitleOffset;
      for (int n = nSkip; n < nLast; ++n)
      {
        pPlot = vpPlots[n];
        dcPlot.SetBackground(*wxWHITE_BRUSH);
        dcPlot.SetBackgroundMode(wxPENSTYLE_TRANSPARENT);
        dcPlot.DestroyClippingRegion();
        dcPlot.Clear();
        dLabelExtension = pPlot->GetLabelHeightExtension();
        pPanelPlot->CopySettings(*pPlot, 0, bForcePrintFont);
        pPanelPlot->AdjustLabelHeightExtension(dLabelExtension, rectPlot, nLabelHeight);
        pPanelPlot->DrawPlotToDC(&dcPlot, rectPlot, dDPI, false, bForcePrintFont);
#ifdef TMP_DEBUG
        mainApp::DumpBitmap(pBitmapPlot.get(), wxString::Format(wxT("plot_schnl_%d"), n));
#endif
        dc.Blit(0, nY, nWidth, nPlotHeight, &dcPlot, 0, 0);
        nY += nPlotHeight;
      }
    }
    int nHeightCut = (nPlotsPerPage - nPlotsOnThisPage) * nPlotHeight;
    rect.SetHeight(rect.GetHeight() - nHeightCut);
    pPanelPlot->DrawXAxisLabelToDC(&dc, rect, dDPI, bForcePrintFont);
  }
  return pBitmap;
}


#if HAS_STATUS_BAR
void CFramePlot::UpdateStatusBar()
{
  m_pParent->SetStatusText(m_pData->GetFilename());
}
#endif

void CFramePlot::OnActivateCB(wxActivateEvent &e)
{
  // when a dialog is opened, the wxThings toggle buttons
  // return a false negative for "IsEnabled" and show grey
  // text.  This hack refreshes all toggle buttons
  if(e.GetActive())
  {
    set<CPanelPlot *>::iterator itr;
    CPanelPlot *pPlot;
    for (itr = m_setPlots.begin();
      itr != m_setPlots.end();
      ++itr)
    {
      pPlot = *itr;
      if(!pPlot->IsToolbarShown())
      {
        break;
      }
      else
      {
        pPlot->GetToolbar()->RefreshChannelButtons();
      }
    }
  }
}
wxString CFramePlot::GetPrintTitle()
{
  wxString sFileName = m_pData->GetFilename();
  COARsample *pSample =
    (m_pOARfile != NULL && (CGridAnalysisDisplay::GetDisplayType() > 0))
    ? m_pOARfile->GetSampleByName(sFileName)
    : NULL;
  return (pSample == NULL) ? sFileName  : pSample->GetSampleName();
}
void CFramePlot::OnPrintPreview(wxCommandEvent &)
{
  if (m_pOARfile == NULL)
  {
    _FindOARfile(CDialogPlotMessageFind::MSG_TYPE_PRINT, true);
  }
  if (m_pOARfile != NULL)
  {
    CPrintOutPlot::DoPrintPreview(this);
  }
}
#ifdef __WXMAC__
void CFramePlot::OnPageMargins(wxCommandEvent &)
{
  CPrintOutPlot::DoPageMargins(this);
}
#endif
void CFramePlot::OnPageSetup(wxCommandEvent &)
{
  CPrintOutPlot::DoPageSetup(this);
}

IMPLEMENT_PERSISTENT_SIZE(CFramePlot)
IMPLEMENT_ABSTRACT_CLASS(CFramePlot,CMDIFrame)


BEGIN_EVENT_TABLE(CFramePlot,CMDIFrame)
EVT_PERSISTENT_SIZE(CFramePlot)

EVT_COMMAND(IDhistoryButton,CEventHistory,CFramePlot::OnHistoryUpdate)
EVT_CLOSE(CFramePlot::OnClose)
EVT_CONTEXT_MENU(CFramePlot::OnContextMenu)
EVT_BUTTON(IDhistoryButton,CFramePlot::OnHistoryButton)
EVT_BUTTON(IDgraphTable, CFramePlot::OnTableButton)
EVT_BUTTON(IDmenuDisplaySample, CFramePlot::OnTableButton)
EVT_SASH_DRAGGED(wxID_ANY,CFramePlot::OnSashDragged)
EVT_SIZE(CFramePlot::OnSize)
EVT_MENU(wxID_PRINT, CFramePlot::OnPrintPreview)
#ifdef __WXMAC__
EVT_MENU(IDpageMargins, CFramePlot::OnPageMargins)
#endif
EVT_MENU(IDpageSetup, CFramePlot::OnPageSetup)
EVT_COMMAND(IDframePlot,wxEVT_SIZE_DELAY_PLOT,CFramePlot::OnSizeAction)
#if FP_SCROLL_EVENT
EVT_COMMAND(IDframePlot,wxEVT_SCROLL_PLOT,CFramePlot::OnScrollPlot)
#endif
END_EVENT_TABLE()

