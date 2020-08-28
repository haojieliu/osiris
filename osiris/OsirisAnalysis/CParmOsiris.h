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
* ===========================================================================
*
*  FileName: CParmOsiris.h
*  Author:   this file was generated by CParmOsiris.pl
*            a perl script written by Douglas Hoffman
*
*/
#ifndef __C_PARM_OSIRIS_H__
#define __C_PARM_OSIRIS_H__

#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxGlobalObject.h"
#include "ConfigDir.h"


#include "CParmGridAttributes.h"

class CParmOsiris : public nwxXmlPersist
{
public:
  CParmOsiris()
  {
    _Init();
  }
  CParmOsiris(const wxString &sFileName, bool bAutoSave = false)
  {
    m_sFileName = sFileName;
    _Init();
    m_bAutoSave = bAutoSave;
  }
  CParmOsiris(bool bAutoSave)
  {
    _Init();
    m_bAutoSave = bAutoSave;
  }
  CParmOsiris(const CParmOsiris &x)
  {
    (*this) = x;
    RegisterAll(true);
  }

  virtual ~CParmOsiris();

  const wxString &GetFileName() const
  {
    return m_sFileName;
  }

  void SetAutoSave(bool b = true)
  {
    m_bAutoSave = b;
  }
  bool IsAutoSave()
  {
    return m_bAutoSave;
  }
  bool IsModified()
  {
    return m_bModified;
  }
  CParmOsiris &operator = (const CParmOsiris &x);
  bool IsEqual(const CParmOsiris &x) const;
  bool operator == (const CParmOsiris &x) const
  {
    bool bEQ = IsEqual(x);
    return bEQ;
  }
  bool operator != (const CParmOsiris &x) const
  {
    bool bEQ = IsEqual(x);
    return !bEQ;
  }
  virtual bool Load()
  {
    bool b = LoadFile(m_sFileName);
    return b;
  }
  virtual bool Save();
  bool SaveIfModified()
  {
    bool bRtn = m_bModified;
    if(m_bModified)
    {
      bRtn = Save();
    }
    return bRtn;
  }

  // begin generated Get

  const wxString &GetLastExportXSLsearch() const
  {
    return m_sLastExportXSLsearch;
  }
  const wxString &GetLastExportDirectory() const
  {
    return m_sLastExportDirectory;
  }
  const wxString &GetInputDirectory() const
  {
    return m_sInputDirectory;
  }
  const wxString &GetOutputDirectory() const
  {
    return m_sOutputDirectory;
  }

  //   m_sKitName is obsolete, use volume name

  const wxString &GetKitName() const
  {
    return m_sKitName;
  }
  const wxString &GetVolumeName() const
  {
    return m_sVolumeName;
  }
  const wxString &GetLsName() const
  {
    return m_sLsName;
  }
  int GetMinRFU_Sample() const
  {
    return m_nMinRFU_Sample;
  }
  int GetMinRFU_Interlocus() const
  {
    return m_nMinRFU_Interlocus;
  }
  int GetMinRFU_ILS() const
  {
    return m_nMinRFU_ILS;
  }
  int GetMinRFU_Ladder() const
  {
    return m_nMinRFU_Ladder;
  }
  int GetMinLadderInterlocusRFU() const
  {
    return m_nMinLadderInterlocusRFU;
  }
  int GetSampleDetectionThreshold() const
  {
    return m_nSampleDetectionThreshold;
  }
  const wxString &GetAnalysisOverride() const
  {
    return m_sAnalysisOverride;
  }
  const vector<int> &GetChannelRFU() const
  {
    return m_anChannelRFU;
  }
  const vector<int> &GetChannelDetection() const
  {
    return m_anChannelDetection;
  }
  bool GetTimeStampSubDir() const
  {
    return m_bTimeStampSubDir;
  }
  bool GetDataAnalyzed() const
  {
    return m_bDataAnalyzed;
  }

  //  CMF settings

  const wxString &GetCMFsourceLab() const
  {
    return m_sCMFsourceLab;
  }
  const wxString &GetCMFdestLab() const
  {
    return m_sCMFdestLab;
  }
  const wxString &GetCMFdefaultSample() const
  {
    return m_sCMFdefaultSample;
  }
  const wxString &GetCMFbatchFormat() const
  {
    return m_sCMFbatchFormat;
  }
  const wxString &GetCMFuserID() const
  {
    return m_sCMFuserID;
  }
  bool GetCMFviewLocation() const
  {
    return m_bCMFviewLocation;
  }
  bool GetShowFileLocationDir() const
  {
    return m_bShowFileLocationDir;
  }

  //  PNG Export Settings

  double GetPNGwidth() const
  {
    return m_dPNGwidth;
  }
  double GetPNGheight() const
  {
    return m_dPNGheight;
  }
  int GetPNGunits() const
  {
    return m_nPNGunits;
  }
  int GetPNGusage() const
  {
    return m_nPNGusage;
  }
  bool GetPNGviewLocation() const
  {
    return m_bPNGviewLocation;
  }
  int GetShowAlerts() const
  {
    return m_nShowAlerts;
  }
  bool GetShowPreview() const
  {
    return m_bShowPreview;
  }
  bool GetHideGraphicToolbar() const
  {
    return m_bHideGraphicToolbar;
  }
  bool GetHideGraphicScrollbar() const
  {
    return m_bHideGraphicScrollbar;
  }
  bool GetHideTextToolbar() const
  {
    return m_bHideTextToolbar;
  }
  bool GetHideSampleToolbar() const
  {
    return m_bHideSampleToolbar;
  }
  bool GetStartupMRU() const
  {
    return m_bStartupMRU;
  }
  bool GetCheckBeforeExit() const
  {
    return m_bCheckBeforeExit;
  }
  bool GetWarnOnHistory() const
  {
    return m_bWarnOnHistory;
  }
  double GetZoomLocusMargin() const
  {
    return m_dZoomLocusMargin;
  }

  //  grid

  int GetTableDisplayPeak() const
  {
    return m_nTableDisplayPeak;
  }
  int GetTableSortBy() const
  {
    return m_nTableSortBy;
  }
  bool GetTableControlsAtTop() const
  {
    return m_bTableControlsAtTop;
  }
  int GetTableShowSampleDisplayNames() const
  {
    return m_nTableShowSampleDisplayNames;
  }

  //  preview settings

  bool GetPreviewDataAnalyzed() const
  {
    return m_bPreviewDataAnalyzed;
  }
  bool GetPreviewDataRaw() const
  {
    return m_bPreviewDataRaw;
  }
  bool GetPreviewDataLadder() const
  {
    return m_bPreviewDataLadder;
  }
  bool GetPreviewDataBaseline() const
  {
    return m_bPreviewDataBaseline;
  }
  bool GetPreviewShowILS() const
  {
    return m_bPreviewShowILS;
  }
  bool GetPreviewShowRFU() const
  {
    return m_bPreviewShowRFU;
  }
  bool GetPreviewShowLadderBars() const
  {
    return m_bPreviewShowLadderBars;
  }
  bool GetPreviewShowLadderLabels() const
  {
    return m_bPreviewShowLadderLabels;
  }
  bool GetPreviewXBPS() const
  {
    return m_bPreviewXBPS;
  }
  int GetPreviewShowArtifact() const
  {
    return m_nPreviewShowArtifact;
  }

  //  plot settings

  bool GetPlotDataAnalyzed() const
  {
    return m_bPlotDataAnalyzed;
  }
  bool GetPlotDataRaw() const
  {
    return m_bPlotDataRaw;
  }
  bool GetPlotDataLadder() const
  {
    return m_bPlotDataLadder;
  }
  bool GetPlotDataBaseline() const
  {
    return m_bPlotDataBaseline;
  }
  bool GetPlotDataXBPS() const
  {
    return m_bPlotDataXBPS;
  }
  bool GetPlotShowILS() const
  {
    return m_bPlotShowILS;
  }
  bool GetPlotShowRFU() const
  {
    return m_bPlotShowRFU;
  }
  bool GetPlotShowLadderLabels() const
  {
    return m_bPlotShowLadderLabels;
  }
  bool GetPlotShowLadderBars() const
  {
    return m_bPlotShowLadderBars;
  }
  bool GetPlotResizable() const
  {
    return m_bPlotResizable;
  }
  int GetPlotMinHeight() const
  {
    return m_nPlotMinHeight;
  }
  int GetPlotShowArtifact() const
  {
    return m_nPlotShowArtifact;
  }
  const vector<unsigned int> &GetPlotDisplayPeak() const
  {
    return m_anPlotDisplayPeak;
  }
  int GetPlotMaxLadderLabels() const
  {
    return m_nPlotMaxLadderLabels;
  }

  //  plot print settings for analysis printout

  bool GetPrintCurveAnalyzed() const
  {
    return m_bPrintCurveAnalyzed;
  }
  bool GetPrintCurveRaw() const
  {
    return m_bPrintCurveRaw;
  }
  bool GetPrintCurveLadder() const
  {
    return m_bPrintCurveLadder;
  }
  bool GetPrintCurveLadderLabels() const
  {
    return m_bPrintCurveLadderLabels;
  }
  bool GetPrintCurveLadderBars() const
  {
    return m_bPrintCurveLadderBars;
  }
  bool GetPrintCurveBaseline() const
  {
    return m_bPrintCurveBaseline;
  }
  bool GetPrintCurveILSvertical() const
  {
    return m_bPrintCurveILSvertical;
  }
  bool GetPrintCurveMinRFU() const
  {
    return m_bPrintCurveMinRFU;
  }
  bool GetPrintXaxisILSBPS() const
  {
    return m_bPrintXaxisILSBPS;
  }
  int GetPrintXscale() const
  {
    return m_nPrintXscale;
  }
  int GetPrintXscaleMin() const
  {
    return m_nPrintXscaleMin;
  }
  int GetPrintXscaleMax() const
  {
    return m_nPrintXscaleMax;
  }
  int GetPrintXscaleMinBPS() const
  {
    return m_nPrintXscaleMinBPS;
  }
  int GetPrintXscaleMaxBPS() const
  {
    return m_nPrintXscaleMaxBPS;
  }
  int GetPrintYscale() const
  {
    return m_nPrintYscale;
  }
  int GetPrintYscaleMin() const
  {
    return m_nPrintYscaleMin;
  }
  int GetPrintYscaleMax() const
  {
    return m_nPrintYscaleMax;
  }
  int GetPrintYcaleNegCtrl() const
  {
    return m_nPrintYcaleNegCtrl;
  }
  const vector<unsigned int> &GetPrintLabelsPeak() const
  {
    return m_anPrintLabelsPeak;
  }
  int GetPrintArtifact() const
  {
    return m_nPrintArtifact;
  }
  int GetPrintHeading() const
  {
    return m_bPrintHeading;
  }
  const wxString &GetPrintHeadingNotes() const
  {
    return m_sPrintHeadingNotes;
  }
  int GetPrintChannelsSamples() const
  {
    return m_nPrintChannelsSamples;
  }
  int GetPrintChannelsLadders() const
  {
    return m_nPrintChannelsLadders;
  }
  int GetPrintChannelsNegCtrl() const
  {
    return m_nPrintChannelsNegCtrl;
  }
  bool GetPrintChannelsOmitILS() const
  {
    return m_bPrintChannelsOmitILS;
  }
  bool GetPrintSamplesLadders() const
  {
    return m_bPrintSamplesLadders;
  }
  bool GetPrintSamplesPosCtrl() const
  {
    return m_bPrintSamplesPosCtrl;
  }
  bool GetPrintSamplesNegCtrl() const
  {
    return m_bPrintSamplesNegCtrl;
  }
  bool GetPrintSamplesDisabled() const
  {
    return m_bPrintSamplesDisabled;
  }
  int GetPrintColorRed() const
  {
    return m_nPrintColorRed;
  }
  int GetPrintColorGreen() const
  {
    return m_nPrintColorGreen;
  }
  int GetPrintColorBlue() const
  {
    return m_nPrintColorBlue;
  }
  int GetPrintColorYellow() const
  {
    return m_nPrintColorYellow;
  }
  int GetPrintColorOrange() const
  {
    return m_nPrintColorOrange;
  }
  int GetPrintColorPurple() const
  {
    return m_nPrintColorPurple;
  }
  int GetPrintColorGray() const
  {
    return m_nPrintColorGray;
  }

  //  plot printout -- margins are in millimeters

  unsigned int GetPrintPlotMarginTop() const
  {
    return m_nPrintPlotMarginTop;
  }
  unsigned int GetPrintPlotMarginBottom() const
  {
    return m_nPrintPlotMarginBottom;
  }
  unsigned int GetPrintPlotMarginLeft() const
  {
    return m_nPrintPlotMarginLeft;
  }
  unsigned int GetPrintPlotMarginRight() const
  {
    return m_nPrintPlotMarginRight;
  }
  int GetPrintPlotPaperType() const
  {
    return m_nPrintPlotPaperType;
  }
  int GetPrintPlotPaperWidth() const
  {
    return m_nPrintPlotPaperWidth;
  }
  int GetPrintPlotPaperHeight() const
  {
    return m_nPrintPlotPaperHeight;
  }
  bool GetPrintPlotLandscape() const
  {
    return m_bPrintPlotLandscape;
  }
  int GetPrintPreviewZoom() const
  {
    return m_nPrintPreviewZoom;
  }

  //  XSLT saved parameter info

  const wxString &GetLastXSLInputFileDir() const
  {
    return m_sLastXSLInputFileDir;
  }
  bool GetPrivacySeen() const
  {
    return m_bPrivacySeen;
  }


  // end generated Get


  int GetMaxLadderLabels() const
  {
    return GetPlotMaxLadderLabels(); // backward compatibility
  }
  bool WarnOnHistory() const
  {
    return GetWarnOnHistory(); // backward compatibility
  }
  bool CheckBeforeExit() const
  {
    return GetCheckBeforeExit(); // backward compatibility
  }
  wxString GetVolumeOrKit() const
  {
    wxString sRtn(m_sVolumeName);
    if(sRtn.IsEmpty() && !m_sKitName.IsEmpty())
    {
      sRtn = "[";
      sRtn.Append(m_sKitName);
      sRtn.Append("]");
    }
    return sRtn;
  }
  int GetPrintColorByName(const wxString &sName) const;
  static const wxString NO_INIT;
  // end static/global stuff

private:

  // SET VALUES

  void __SET_VALUE(wxString &s1, const wxString &s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }

  void __SET_VALUE(double &s1, double s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }

  void __SET_VALUE(bool &s1, bool s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }

  void __SET_VALUE(int &s1, int s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }

  void __SET_VALUE(unsigned int &s1, unsigned int s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }

  void __SET_VALUE(vector<int> &s1, const vector<int> &s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }

  void __SET_VALUE(vector<unsigned int> &s1, const vector<unsigned int> &s2)
  {
    if(!(s1 == s2))
    {
      s1 = s2;
      m_bModified = true;
    }
  }


public:

  // begin set

  void SetLastExportXSLsearch(const wxString &s)
  {
    __SET_VALUE(m_sLastExportXSLsearch,s);
  }
  void SetLastExportDirectory(const wxString &s)
  {
    __SET_VALUE(m_sLastExportDirectory,s);
  }
  void SetInputDirectory(const wxString &s)
  {
    __SET_VALUE(m_sInputDirectory,s);
  }
  void SetOutputDirectory(const wxString &s)
  {
    __SET_VALUE(m_sOutputDirectory,s);
  }

  //   m_sKitName is obsolete, use volume name

  void SetKitName(const wxString &s)
  {
    __SET_VALUE(m_sKitName,s);
  }
  void SetVolumeName(const wxString &s)
  {
    __SET_VALUE(m_sVolumeName,s);
  }
  void SetLsName(const wxString &s)
  {
    __SET_VALUE(m_sLsName,s);
  }
  void SetMinRFU_Sample(int n)
  {
    __SET_VALUE(m_nMinRFU_Sample,n);
  }
  void SetMinRFU_Interlocus(int n)
  {
    __SET_VALUE(m_nMinRFU_Interlocus,n);
  }
  void SetMinRFU_ILS(int n)
  {
    __SET_VALUE(m_nMinRFU_ILS,n);
  }
  void SetMinRFU_Ladder(int n)
  {
    __SET_VALUE(m_nMinRFU_Ladder,n);
  }
  void SetMinLadderInterlocusRFU(int n)
  {
    __SET_VALUE(m_nMinLadderInterlocusRFU,n);
  }
  void SetSampleDetectionThreshold(int n)
  {
    __SET_VALUE(m_nSampleDetectionThreshold,n);
  }
  void SetAnalysisOverride(const wxString &s)
  {
    __SET_VALUE(m_sAnalysisOverride,s);
  }
  void SetChannelRFU(const vector<int> &an)
  {
    __SET_VALUE(m_anChannelRFU,an);
  }
  void SetChannelDetection(const vector<int> &an)
  {
    __SET_VALUE(m_anChannelDetection,an);
  }
  void SetTimeStampSubDir(bool b)
  {
    __SET_VALUE(m_bTimeStampSubDir,b);
  }
  void SetDataAnalyzed(bool b)
  {
    __SET_VALUE(m_bDataAnalyzed,b);
  }

  //  CMF settings

  void SetCMFsourceLab(const wxString &s)
  {
    __SET_VALUE(m_sCMFsourceLab,s);
  }
  void SetCMFdestLab(const wxString &s)
  {
    __SET_VALUE(m_sCMFdestLab,s);
  }
  void SetCMFdefaultSample(const wxString &s)
  {
    __SET_VALUE(m_sCMFdefaultSample,s);
  }
  void SetCMFbatchFormat(const wxString &s)
  {
    __SET_VALUE(m_sCMFbatchFormat,s);
  }
  void SetCMFuserID(const wxString &s)
  {
    __SET_VALUE(m_sCMFuserID,s);
  }
  void SetCMFviewLocation(bool b)
  {
    __SET_VALUE(m_bCMFviewLocation,b);
  }
  void SetShowFileLocationDir(bool b)
  {
    __SET_VALUE(m_bShowFileLocationDir,b);
  }

  //  PNG Export Settings

  void SetPNGwidth(double d)
  {
    __SET_VALUE(m_dPNGwidth,d);
  }
  void SetPNGheight(double d)
  {
    __SET_VALUE(m_dPNGheight,d);
  }
  void SetPNGunits(int n)
  {
    __SET_VALUE(m_nPNGunits,n);
  }
  void SetPNGusage(int n)
  {
    __SET_VALUE(m_nPNGusage,n);
  }
  void SetPNGviewLocation(bool b)
  {
    __SET_VALUE(m_bPNGviewLocation,b);
  }
  void SetShowAlerts(int n)
  {
    __SET_VALUE(m_nShowAlerts,n);
  }
  void SetShowPreview(bool b)
  {
    __SET_VALUE(m_bShowPreview,b);
  }
  void SetHideGraphicToolbar(bool b)
  {
    __SET_VALUE(m_bHideGraphicToolbar,b);
  }
  void SetHideGraphicScrollbar(bool b)
  {
    __SET_VALUE(m_bHideGraphicScrollbar,b);
  }
  void SetHideTextToolbar(bool b)
  {
    __SET_VALUE(m_bHideTextToolbar,b);
  }
  void SetHideSampleToolbar(bool b)
  {
    __SET_VALUE(m_bHideSampleToolbar,b);
  }
  void SetStartupMRU(bool b)
  {
    __SET_VALUE(m_bStartupMRU,b);
  }
  void SetCheckBeforeExit(bool b)
  {
    __SET_VALUE(m_bCheckBeforeExit,b);
  }
  void SetWarnOnHistory(bool b)
  {
    __SET_VALUE(m_bWarnOnHistory,b);
  }
  void SetZoomLocusMargin(double d)
  {
    __SET_VALUE(m_dZoomLocusMargin,d);
  }

  //  grid

  void SetTableDisplayPeak(int n)
  {
    __SET_VALUE(m_nTableDisplayPeak,n);
  }
  void SetTableSortBy(int n)
  {
    __SET_VALUE(m_nTableSortBy,n);
  }
  void SetTableControlsAtTop(bool b)
  {
    __SET_VALUE(m_bTableControlsAtTop,b);
  }
  void SetTableShowSampleDisplayNames(int n)
  {
    __SET_VALUE(m_nTableShowSampleDisplayNames,n);
  }

  //  preview settings

  void SetPreviewDataAnalyzed(bool b)
  {
    __SET_VALUE(m_bPreviewDataAnalyzed,b);
  }
  void SetPreviewDataRaw(bool b)
  {
    __SET_VALUE(m_bPreviewDataRaw,b);
  }
  void SetPreviewDataLadder(bool b)
  {
    __SET_VALUE(m_bPreviewDataLadder,b);
  }
  void SetPreviewDataBaseline(bool b)
  {
    __SET_VALUE(m_bPreviewDataBaseline,b);
  }
  void SetPreviewShowILS(bool b)
  {
    __SET_VALUE(m_bPreviewShowILS,b);
  }
  void SetPreviewShowRFU(bool b)
  {
    __SET_VALUE(m_bPreviewShowRFU,b);
  }
  void SetPreviewShowLadderBars(bool b)
  {
    __SET_VALUE(m_bPreviewShowLadderBars,b);
  }
  void SetPreviewShowLadderLabels(bool b)
  {
    __SET_VALUE(m_bPreviewShowLadderLabels,b);
  }
  void SetPreviewXBPS(bool b)
  {
    __SET_VALUE(m_bPreviewXBPS,b);
  }
  void SetPreviewShowArtifact(int n)
  {
    __SET_VALUE(m_nPreviewShowArtifact,n);
  }

  //  plot settings

  void SetPlotDataAnalyzed(bool b)
  {
    __SET_VALUE(m_bPlotDataAnalyzed,b);
  }
  void SetPlotDataRaw(bool b)
  {
    __SET_VALUE(m_bPlotDataRaw,b);
  }
  void SetPlotDataLadder(bool b)
  {
    __SET_VALUE(m_bPlotDataLadder,b);
  }
  void SetPlotDataBaseline(bool b)
  {
    __SET_VALUE(m_bPlotDataBaseline,b);
  }
  void SetPlotDataXBPS(bool b)
  {
    __SET_VALUE(m_bPlotDataXBPS,b);
  }
  void SetPlotShowILS(bool b)
  {
    __SET_VALUE(m_bPlotShowILS,b);
  }
  void SetPlotShowRFU(bool b)
  {
    __SET_VALUE(m_bPlotShowRFU,b);
  }
  void SetPlotShowLadderLabels(bool b)
  {
    __SET_VALUE(m_bPlotShowLadderLabels,b);
  }
  void SetPlotShowLadderBars(bool b)
  {
    __SET_VALUE(m_bPlotShowLadderBars,b);
  }
  void SetPlotResizable(bool b)
  {
    __SET_VALUE(m_bPlotResizable,b);
  }
  void SetPlotMinHeight(int n)
  {
    __SET_VALUE(m_nPlotMinHeight,n);
  }
  void SetPlotShowArtifact(int n)
  {
    __SET_VALUE(m_nPlotShowArtifact,n);
  }
  void SetPlotDisplayPeak(const vector<unsigned int> &an)
  {
    __SET_VALUE(m_anPlotDisplayPeak,an);
  }
  void SetPlotMaxLadderLabels(int n)
  {
    __SET_VALUE(m_nPlotMaxLadderLabels,n);
  }

  //  plot print settings for analysis printout

  void SetPrintCurveAnalyzed(bool b)
  {
    __SET_VALUE(m_bPrintCurveAnalyzed,b);
  }
  void SetPrintCurveRaw(bool b)
  {
    __SET_VALUE(m_bPrintCurveRaw,b);
  }
  void SetPrintCurveLadder(bool b)
  {
    __SET_VALUE(m_bPrintCurveLadder,b);
  }
  void SetPrintCurveLadderLabels(bool b)
  {
    __SET_VALUE(m_bPrintCurveLadderLabels,b);
  }
  void SetPrintCurveLadderBars(bool b)
  {
    __SET_VALUE(m_bPrintCurveLadderBars,b);
  }
  void SetPrintCurveBaseline(bool b)
  {
    __SET_VALUE(m_bPrintCurveBaseline,b);
  }
  void SetPrintCurveILSvertical(bool b)
  {
    __SET_VALUE(m_bPrintCurveILSvertical,b);
  }
  void SetPrintCurveMinRFU(bool b)
  {
    __SET_VALUE(m_bPrintCurveMinRFU,b);
  }
  void SetPrintXaxisILSBPS(bool b)
  {
    __SET_VALUE(m_bPrintXaxisILSBPS,b);
  }
  void SetPrintXscale(int n)
  {
    __SET_VALUE(m_nPrintXscale,n);
  }
  void SetPrintXscaleMin(int n)
  {
    __SET_VALUE(m_nPrintXscaleMin,n);
  }
  void SetPrintXscaleMax(int n)
  {
    __SET_VALUE(m_nPrintXscaleMax,n);
  }
  void SetPrintXscaleMinBPS(int n)
  {
    __SET_VALUE(m_nPrintXscaleMinBPS,n);
  }
  void SetPrintXscaleMaxBPS(int n)
  {
    __SET_VALUE(m_nPrintXscaleMaxBPS,n);
  }
  void SetPrintYscale(int n)
  {
    __SET_VALUE(m_nPrintYscale,n);
  }
  void SetPrintYscaleMin(int n)
  {
    __SET_VALUE(m_nPrintYscaleMin,n);
  }
  void SetPrintYscaleMax(int n)
  {
    __SET_VALUE(m_nPrintYscaleMax,n);
  }
  void SetPrintYcaleNegCtrl(int n)
  {
    __SET_VALUE(m_nPrintYcaleNegCtrl,n);
  }
  void SetPrintLabelsPeak(const vector<unsigned int> &an)
  {
    __SET_VALUE(m_anPrintLabelsPeak,an);
  }
  void SetPrintArtifact(int n)
  {
    __SET_VALUE(m_nPrintArtifact,n);
  }
  void SetPrintHeading(int n)
  {
    __SET_VALUE(m_bPrintHeading,n);
  }
  void SetPrintHeadingNotes(const wxString &s)
  {
    __SET_VALUE(m_sPrintHeadingNotes,s);
  }
  void SetPrintChannelsSamples(int n)
  {
    __SET_VALUE(m_nPrintChannelsSamples,n);
  }
  void SetPrintChannelsLadders(int n)
  {
    __SET_VALUE(m_nPrintChannelsLadders,n);
  }
  void SetPrintChannelsNegCtrl(int n)
  {
    __SET_VALUE(m_nPrintChannelsNegCtrl,n);
  }
  void SetPrintChannelsOmitILS(bool b)
  {
    __SET_VALUE(m_bPrintChannelsOmitILS,b);
  }
  void SetPrintSamplesLadders(bool b)
  {
    __SET_VALUE(m_bPrintSamplesLadders,b);
  }
  void SetPrintSamplesPosCtrl(bool b)
  {
    __SET_VALUE(m_bPrintSamplesPosCtrl,b);
  }
  void SetPrintSamplesNegCtrl(bool b)
  {
    __SET_VALUE(m_bPrintSamplesNegCtrl,b);
  }
  void SetPrintSamplesDisabled(bool b)
  {
    __SET_VALUE(m_bPrintSamplesDisabled,b);
  }
  void SetPrintColorRed(int n)
  {
    __SET_VALUE(m_nPrintColorRed,n);
  }
  void SetPrintColorGreen(int n)
  {
    __SET_VALUE(m_nPrintColorGreen,n);
  }
  void SetPrintColorBlue(int n)
  {
    __SET_VALUE(m_nPrintColorBlue,n);
  }
  void SetPrintColorYellow(int n)
  {
    __SET_VALUE(m_nPrintColorYellow,n);
  }
  void SetPrintColorOrange(int n)
  {
    __SET_VALUE(m_nPrintColorOrange,n);
  }
  void SetPrintColorPurple(int n)
  {
    __SET_VALUE(m_nPrintColorPurple,n);
  }
  void SetPrintColorGray(int n)
  {
    __SET_VALUE(m_nPrintColorGray,n);
  }

  //  plot printout -- margins are in millimeters

  void SetPrintPlotMarginTop(unsigned int n)
  {
    __SET_VALUE(m_nPrintPlotMarginTop,n);
  }
  void SetPrintPlotMarginBottom(unsigned int n)
  {
    __SET_VALUE(m_nPrintPlotMarginBottom,n);
  }
  void SetPrintPlotMarginLeft(unsigned int n)
  {
    __SET_VALUE(m_nPrintPlotMarginLeft,n);
  }
  void SetPrintPlotMarginRight(unsigned int n)
  {
    __SET_VALUE(m_nPrintPlotMarginRight,n);
  }
  void SetPrintPlotPaperType(int n)
  {
    __SET_VALUE(m_nPrintPlotPaperType,n);
  }
  void SetPrintPlotPaperWidth(int n)
  {
    __SET_VALUE(m_nPrintPlotPaperWidth,n);
  }
  void SetPrintPlotPaperHeight(int n)
  {
    __SET_VALUE(m_nPrintPlotPaperHeight,n);
  }
  void SetPrintPlotLandscape(bool b)
  {
    __SET_VALUE(m_bPrintPlotLandscape,b);
  }
  void SetPrintPreviewZoom(int n)
  {
    __SET_VALUE(m_nPrintPreviewZoom,n);
  }

  //  XSLT saved parameter info

  void SetLastXSLInputFileDir(const wxString &s)
  {
    __SET_VALUE(m_sLastXSLInputFileDir,s);
  }
  void SetPrivacySeen(bool b)
  {
    __SET_VALUE(m_bPrivacySeen,b);
  }


  // end set

  void SetPlotFixed(bool b)
  {
    SetPlotResizable(!b);
    if(b)
    {
      SetPlotMinHeight(-1);
    }
  }
  void SetMaxLadderLabels(int n)
  {
    SetPlotMaxLadderLabels(n); // backward compatibility
  }
private:
  void _CheckMod(bool b)
  {
    if(b) { m_bModified = true; }
  }
public:
  static bool IsAttrBold(int n)
  {
    return !!(n & (int)GRID_BOLD);
  }
  static bool IsAttrItalic(int n)
  {
    return !!(n & (int)GRID_ITALIC);
  }
  static bool IsAttrReverse(int n)
  {
    return !!(n & (int)GRID_REVERSE);
  }
  bool SetAttrNeedsAttn(int n)
  {
    bool bRtn = m_gridAttr.SetAttrNeedsAttn(n);
    _CheckMod(bRtn);
    return bRtn;
  }
  bool SetAttrEdited(int n)
  {
    bool bRtn = m_gridAttr.SetAttrEdited(n);
    _CheckMod(bRtn);
    return bRtn;
  }
  bool SetDefaultColours()
  {
    bool bRtn = m_gridAttr.SetDefaultColours();
    _CheckMod(bRtn);
    return bRtn;
  }
  void SetBackground(int nGRID_FLAG, const wxColour &c)
  {
    bool bRtn = m_gridAttr.SetBackground(nGRID_FLAG,c);
    _CheckMod(bRtn);
  }
  void SetForeground(int nGRID_FLAG, const wxColour &c)
  {
    bool bRtn = m_gridAttr.SetForeground(nGRID_FLAG,c);
    _CheckMod(bRtn);
  }
  void SetNormalForeground(const wxColour &c)
  {
    SetForeground(GRID_NORMAL,c);
  }
  void SetAlertForeground(const wxColour &c)
  {
    SetForeground(GRID_ALERT_CELL,c);
  }
  void SetDisabledForeground(const wxColour &c)
  {
    SetForeground(GRID_DISABLED,c);
  }
  void SetStatusForeground(const wxColour &c, bool bNeedsAttn)
  {
    bool bRtn = m_gridAttr.SetStatusForeground(c,bNeedsAttn);
    _CheckMod(bRtn);
  }
  void SetStatusBackground(const wxColour &c, bool bNeedsAttn)
  {
    bool bRtn = m_gridAttr.SetStatusBackground(c,bNeedsAttn);
    _CheckMod(bRtn);
  }
  const wxColour &GetBackground(int nGRID_FLAG) const
  {
    return m_gridAttr.GetBackground(nGRID_FLAG);
  }
  const wxColour &GetForeground(int nGRID_FLAG) const
  {
    return m_gridAttr.GetForeground(nGRID_FLAG);
  }
  const wxColour &GetStatusForeground(bool bNeedsAttn) const
  {
    return m_gridAttr.GetStatusForeground(bNeedsAttn);
  }
  const wxColour &GetStatusBackground(bool bNeedsAttn) const
  {
    return m_gridAttr.GetStatusBackground(bNeedsAttn);
  }

  bool IsGridItalic(int nGRID_FLAG) const
  {
    return m_gridAttr.IsItalic(nGRID_FLAG);
  }
  bool IsGridBold(int nGRID_FLAG) const
  {
    return m_gridAttr.IsBold(nGRID_FLAG);
  }
  bool IsGridReverse(int nGRID_FLAG) const
  {
    return m_gridAttr.IsReverse(nGRID_FLAG);
  }

  int GetAttrEdited() const
  {
    return m_gridAttr.GetAttrEdited();
  }
  int GetAttrNeedsAttn() const
  {
    return m_gridAttr.GetAttrNeedsAttn();
  }
protected:
  CParmOsiris(int)
  {
    // inherited constructor
    m_bModified = false;
    m_bAutoSave = false;
    SetDefaults();
  }
  virtual void RegisterAll(bool bInConstructor = false);
  void _Init();
  void SetDefaults();

  CParmGridAttributes m_gridAttr;
  wxString m_sFileName;  // file name for Save/Load

  // begin generated variables

  wxString m_sLastExportXSLsearch;
  wxString m_sLastExportDirectory;
  wxString m_sInputDirectory;
  wxString m_sOutputDirectory;

  //   m_sKitName is obsolete, use volume name

  wxString m_sKitName;
  wxString m_sVolumeName;
  wxString m_sLsName;
  int m_nMinRFU_Sample;
  int m_nMinRFU_Interlocus;
  int m_nMinRFU_ILS;
  int m_nMinRFU_Ladder;
  int m_nMinLadderInterlocusRFU;
  int m_nSampleDetectionThreshold;
  wxString m_sAnalysisOverride;
  vector<int> m_anChannelRFU;
  vector<int> m_anChannelDetection;
  bool m_bTimeStampSubDir;
  bool m_bDataAnalyzed;

  //  CMF settings

  wxString m_sCMFsourceLab;
  wxString m_sCMFdestLab;
  wxString m_sCMFdefaultSample;
  wxString m_sCMFbatchFormat;
  wxString m_sCMFuserID;
  bool m_bCMFviewLocation;
  bool m_bShowFileLocationDir;

  //  PNG Export Settings

  double m_dPNGwidth;
  double m_dPNGheight;
  int m_nPNGunits;
  int m_nPNGusage;
  bool m_bPNGviewLocation;
  int m_nShowAlerts;
  bool m_bShowPreview;
  bool m_bHideGraphicToolbar;
  bool m_bHideGraphicScrollbar;
  bool m_bHideTextToolbar;
  bool m_bHideSampleToolbar;
  bool m_bStartupMRU;
  bool m_bCheckBeforeExit;
  bool m_bWarnOnHistory;
  double m_dZoomLocusMargin;

  //  grid

  int m_nTableDisplayPeak;
  int m_nTableSortBy;
  bool m_bTableControlsAtTop;
  int m_nTableShowSampleDisplayNames;

  //  preview settings

  bool m_bPreviewDataAnalyzed;
  bool m_bPreviewDataRaw;
  bool m_bPreviewDataLadder;
  bool m_bPreviewDataBaseline;
  bool m_bPreviewShowILS;
  bool m_bPreviewShowRFU;
  bool m_bPreviewShowLadderBars;
  bool m_bPreviewShowLadderLabels;
  bool m_bPreviewXBPS;
  int m_nPreviewShowArtifact;

  //  plot settings

  bool m_bPlotDataAnalyzed;
  bool m_bPlotDataRaw;
  bool m_bPlotDataLadder;
  bool m_bPlotDataBaseline;
  bool m_bPlotDataXBPS;
  bool m_bPlotShowILS;
  bool m_bPlotShowRFU;
  bool m_bPlotShowLadderLabels;
  bool m_bPlotShowLadderBars;
  bool m_bPlotResizable;
  int m_nPlotMinHeight;
  int m_nPlotShowArtifact;
  vector<unsigned int> m_anPlotDisplayPeak;
  int m_nPlotMaxLadderLabels;

  //  plot print settings for analysis printout

  bool m_bPrintCurveAnalyzed;
  bool m_bPrintCurveRaw;
  bool m_bPrintCurveLadder;
  bool m_bPrintCurveLadderLabels;
  bool m_bPrintCurveLadderBars;
  bool m_bPrintCurveBaseline;
  bool m_bPrintCurveILSvertical;
  bool m_bPrintCurveMinRFU;
  bool m_bPrintXaxisILSBPS;
  int m_nPrintXscale;
  int m_nPrintXscaleMin;
  int m_nPrintXscaleMax;
  int m_nPrintXscaleMinBPS;
  int m_nPrintXscaleMaxBPS;
  int m_nPrintYscale;
  int m_nPrintYscaleMin;
  int m_nPrintYscaleMax;
  int m_nPrintYcaleNegCtrl;
  vector<unsigned int> m_anPrintLabelsPeak;
  int m_nPrintArtifact;
  int m_bPrintHeading;
  wxString m_sPrintHeadingNotes;
  int m_nPrintChannelsSamples;
  int m_nPrintChannelsLadders;
  int m_nPrintChannelsNegCtrl;
  bool m_bPrintChannelsOmitILS;
  bool m_bPrintSamplesLadders;
  bool m_bPrintSamplesPosCtrl;
  bool m_bPrintSamplesNegCtrl;
  bool m_bPrintSamplesDisabled;
  int m_nPrintColorRed;
  int m_nPrintColorGreen;
  int m_nPrintColorBlue;
  int m_nPrintColorYellow;
  int m_nPrintColorOrange;
  int m_nPrintColorPurple;
  int m_nPrintColorGray;

  //  plot printout -- margins are in millimeters

  unsigned int m_nPrintPlotMarginTop;
  unsigned int m_nPrintPlotMarginBottom;
  unsigned int m_nPrintPlotMarginLeft;
  unsigned int m_nPrintPlotMarginRight;
  int m_nPrintPlotPaperType;
  int m_nPrintPlotPaperWidth;
  int m_nPrintPlotPaperHeight;
  bool m_bPrintPlotLandscape;
  int m_nPrintPreviewZoom;

  //  XSLT saved parameter info

  wxString m_sLastXSLInputFileDir;
  bool m_bPrivacySeen;


  // end generated variables

  bool m_bModified;
  bool m_bAutoSave;
  nwxXmlIOwxString m_ioBatchFormat;
  nwxXmlIOwxString m_ioDefaultSample;
  nwxXmlIOint m_ioIntViewPlotArtifact;
  nwxXmlIOuint m_ioUint1;
  nwxXmlIOint m_ioInt_1; // default to -1
public:
  static const wxString DEFAULT_BATCH_FORMAT;
  static const wxString LABEL_VIEW_LOCATION;
  static const wxString LABEL_ORIGIN;
  static const wxString LABEL_INPUT;
  static const wxString LABEL_OUTPUT;
  static const wxString LABEL_KIT;
  static const wxString LABEL_VOLUME;
  static const wxString LABEL_LS;
  static const wxString LABEL_RFU;
  static const wxString LABEL_RFU_SAMPLE;
  static const wxString LABEL_RFU_ILS;
  static const wxString LABEL_RFU_LADDER;
  static const wxString LABEL_RFU_INTERLOCUS;
  static const wxString LABEL_DATA;
  static const wxString LABEL_DATA_RAW;
  static const wxString LABEL_DATA_ANALYZED;
  static const wxString LABEL_ANALYSIS;
  static const wxString LABEL_DETECTION;
  static const wxString LABEL_INTERLOCUS;
  static const wxString ERROR_MSG_INTERLOCUS;


//  **************************************   static/global stuff

public:
  nwxDECLARE_GLOBAL_OBJECT_XML(CParmOsiris)

};

class CParmOsirisGlobal
{
public:
  CParmOsirisGlobal() : m_bGotten(false)
  {}
  virtual ~CParmOsirisGlobal()
  {
    if(m_bGotten)
    {
      Get()->SaveIfModified();
    }
  }
  CParmOsiris *Get()
  {
    m_bGotten = true;
    return CParmOsiris::GetGlobal();
  }
  CParmOsiris * operator ->()
  {
    return Get();
  }
private:
  bool m_bGotten;

};

class CParmOsirisLite : public CParmOsiris
{
public:
  CParmOsirisLite() : CParmOsiris(1)
  {
    RegisterAll(true);
  }
protected:
  virtual void RegisterAll(bool = false)
  {
    RegisterWxString("inputDirectory", &m_sInputDirectory);
    RegisterWxString("outputDirectory", &m_sOutputDirectory);
    RegisterWxString("kit", &m_sKitName);
    RegisterWxString("ls", &m_sLsName);
    RegisterInt("minRFUsample", &m_nMinRFU_Sample);
    RegisterInt("minRFUILS", &m_nMinRFU_ILS);
    RegisterInt("minRFUladder", &m_nMinRFU_Ladder);
    RegisterInt("minRFUinterlocus", &m_nMinRFU_Interlocus);
    RegisterIntNonZero("minRFUladderInterlocus", &m_nMinLadderInterlocusRFU);
    RegisterIntNonZero("minRFUsampleDetection", &m_nSampleDetectionThreshold);
    RegisterIntVector("ChannelRFU", &m_anChannelRFU);
    RegisterIntVector("ChannelDetection", &m_anChannelDetection);
    RegisterBoolSkipFalse("DataAnalyzed", &m_bDataAnalyzed);
  }
  virtual bool Load()
  {
    return false;
  }
  virtual bool Save()
  {
    return false;
  }
};


#endif
