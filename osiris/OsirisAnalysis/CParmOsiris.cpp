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
*  FileName: CParmOsiris.cpp
*  Author:   this file was generated by CParmOsiris.pl
*            a perl script written by Douglas Hoffman
*
*/
#include "mainApp.h"
#include "CParmOsiris.h"
#include "CLabSettings.h"
#include "CVolumes.h"
#include "wxIDS.h"
#include "nwx/nwxFileUtil.h"
#include <wx/filename.h>
#include <wx/utils.h>

nwxIMPLEMENT_GLOBAL_OBJECT(CParmOsiris)

const wxString CParmOsiris::NO_INIT(wxS(":"));

void CParmOsiris::_Init()
{
  SetNoInit(true);
  SetDefaults();
  if(m_sFileName.IsEmpty())
  {
    m_sFileName = mainApp::GetConfig()->GetConfigPath();
    m_sFileName += "osiris.xml";
  }
  m_bModified = false;
  m_bAutoSave = false;
  RegisterAll(true);
  if(!wxFileName::FileExists(m_sFileName))
  {}
  else if(!Load())
  {
    SetDefaults();
  }
}


CParmOsiris &CParmOsiris::operator =(const CParmOsiris &x)
{
#define CP(elem) elem = x.elem

  CP(m_sFileName);

  // begin generated copy

  CP(m_sLastExportXSLsearch);
  CP(m_sLastExportDirectory);
  CP(m_sInputDirectory);
  CP(m_sOutputDirectory);

  //   m_sKitName is obsolete, use volume name

  CP(m_sKitName);
  CP(m_sVolumeName);
  CP(m_sLsName);
  CP(m_nMinRFU_Sample);
  CP(m_nMinRFU_Interlocus);
  CP(m_nMinRFU_ILS);
  CP(m_nMinRFU_Ladder);
  CP(m_nMinLadderInterlocusRFU);
  CP(m_nSampleDetectionThreshold);
  CP(m_sAnalysisOverride);
  CP(m_anChannelRFU);
  CP(m_anChannelDetection);
  CP(m_bTimeStampSubDir);
  CP(m_bDataAnalyzed);

  //  CMF settings

  CP(m_sCMFsourceLab);
  CP(m_sCMFdestLab);
  CP(m_sCMFdefaultSample);
  CP(m_sCMFbatchFormat);
  CP(m_sCMFuserID);
  CP(m_bCMFviewLocation);
  CP(m_bShowFileLocationDir);

  //  PNG Export Settings

  CP(m_dPNGwidth);
  CP(m_dPNGheight);
  CP(m_nPNGunits);
  CP(m_nPNGusage);
  CP(m_bPNGviewLocation);
  CP(m_nShowAlerts);
  CP(m_bShowPreview);
  CP(m_bHideGraphicToolbar);
  CP(m_bHideGraphicScrollbar);
  CP(m_bHideTextToolbar);
  CP(m_bHideSampleToolbar);
  CP(m_bStartupMRU);
  CP(m_bCheckBeforeExit);
  CP(m_bWarnOnHistory);
  CP(m_dZoomLocusMargin);

  //  grid

  CP(m_nTableDisplayPeak);
  CP(m_nTableSortBy);
  CP(m_bTableControlsAtTop);
  CP(m_nTableShowSampleDisplayNames);

  //  preview settings

  CP(m_bPreviewDataAnalyzed);
  CP(m_bPreviewDataRaw);
  CP(m_bPreviewDataLadder);
  CP(m_bPreviewDataBaseline);
  CP(m_bPreviewShowILS);
  CP(m_bPreviewShowRFU);
  CP(m_bPreviewShowLadderLabels);
  CP(m_bPreviewXBPS);
  CP(m_nPreviewShowArtifact);

  //  plot settings

  CP(m_bPlotDataAnalyzed);
  CP(m_bPlotDataRaw);
  CP(m_bPlotDataLadder);
  CP(m_bPlotDataBaseline);
  CP(m_bPlotDataXBPS);
  CP(m_bPlotShowILS);
  CP(m_bPlotShowRFU);
  CP(m_bPlotShowLadderLabels);
  CP(m_bPlotResizable);
  CP(m_nPlotMinHeight);
  CP(m_nPlotShowArtifact);
  CP(m_anPlotDisplayPeak);
  CP(m_nPlotMaxLadderLabels);

  //  plot print settings for analysis printout

  CP(m_bPrintCurveAnalyzed);
  CP(m_bPrintCurveRaw);
  CP(m_bPrintCurveLadder);
  CP(m_bPrintCurveLadderLabels);
  CP(m_bPrintCurveBaseline);
  CP(m_bPrintCurveILSvertical);
  CP(m_bPrintCurveMinRFU);
  CP(m_bPrintXaxisILSBPS);
  CP(m_nPrintXscale);
  CP(m_nPrintXscaleMin);
  CP(m_nPrintXscaleMax);
  CP(m_nPrintYscale);
  CP(m_nPrintYscaleMin);
  CP(m_nPrintYscaleMax);
  CP(m_nPrintYcaleNegCtrl);
  CP(m_anPrintLabelsPeak);
  CP(m_nPrintArtifact);
  CP(m_bPrintHeading);
  CP(m_sPrintHeadingNotes);
  CP(m_nPrintChannelsSamples);
  CP(m_nPrintChannelsLadders);
  CP(m_nPrintChannelsNegCtrl);
  CP(m_bPrintChannelsOmitILS);
  CP(m_bPrintSamplesLadders);
  CP(m_bPrintSamplesPosCtrl);
  CP(m_bPrintSamplesNegCtrl);
  CP(m_bPrintSamplesDisabled);
  CP(m_nPrintColorRed);
  CP(m_nPrintColorGreen);
  CP(m_nPrintColorBlue);
  CP(m_nPrintColorYellow);
  CP(m_nPrintColorOrange);
  CP(m_nPrintColorPurple);
  CP(m_nPrintColorGray);

  //  plot printout -- margins are in millimeters

  CP(m_nPrintPlotMarginTop);
  CP(m_nPrintPlotMarginBottom);
  CP(m_nPrintPlotMarginLeft);
  CP(m_nPrintPlotMarginRight);
  CP(m_nPrintPlotPaperType);
  CP(m_nPrintPlotPaperWidth);
  CP(m_nPrintPlotPaperHeight);
  CP(m_bPrintPlotLandscape);
  CP(m_nPrintPreviewZoom);

  //  XSLT saved parameter info

  CP(m_sLastXSLInputFileDir);
  CP(m_bPrivacySeen);


  // end generated copy

  CP(m_bModified);
  m_bAutoSave = false;
  CP(m_gridAttr);

#undef CP
  return *this;
}

bool CParmOsiris::IsEqual(const CParmOsiris &x) const
{
#define CP(elem) else if(!(elem == x.elem)) { bRtn = false; }
  bool bRtn = true;
  if(0) {}
  // begin generated compare

  CP(m_sLastExportXSLsearch)
  CP(m_sLastExportDirectory)
  CP(m_sInputDirectory)
  CP(m_sOutputDirectory)

  //   m_sKitName is obsolete, use volume name

  CP(m_sKitName)
  CP(m_sVolumeName)
  CP(m_sLsName)
  CP(m_nMinRFU_Sample)
  CP(m_nMinRFU_Interlocus)
  CP(m_nMinRFU_ILS)
  CP(m_nMinRFU_Ladder)
  CP(m_nMinLadderInterlocusRFU)
  CP(m_nSampleDetectionThreshold)
  CP(m_sAnalysisOverride)
  CP(m_anChannelRFU)
  CP(m_anChannelDetection)
  CP(m_bTimeStampSubDir)
  CP(m_bDataAnalyzed)

  //  CMF settings

  CP(m_sCMFsourceLab)
  CP(m_sCMFdestLab)
  CP(m_sCMFdefaultSample)
  CP(m_sCMFbatchFormat)
  CP(m_sCMFuserID)
  CP(m_bCMFviewLocation)
  CP(m_bShowFileLocationDir)

  //  PNG Export Settings

  CP(m_dPNGwidth)
  CP(m_dPNGheight)
  CP(m_nPNGunits)
  CP(m_nPNGusage)
  CP(m_bPNGviewLocation)
  CP(m_nShowAlerts)
  CP(m_bShowPreview)
  CP(m_bHideGraphicToolbar)
  CP(m_bHideGraphicScrollbar)
  CP(m_bHideTextToolbar)
  CP(m_bHideSampleToolbar)
  CP(m_bStartupMRU)
  CP(m_bCheckBeforeExit)
  CP(m_bWarnOnHistory)
  CP(m_dZoomLocusMargin)

  //  grid

  CP(m_nTableDisplayPeak)
  CP(m_nTableSortBy)
  CP(m_bTableControlsAtTop)
  CP(m_nTableShowSampleDisplayNames)

  //  preview settings

  CP(m_bPreviewDataAnalyzed)
  CP(m_bPreviewDataRaw)
  CP(m_bPreviewDataLadder)
  CP(m_bPreviewDataBaseline)
  CP(m_bPreviewShowILS)
  CP(m_bPreviewShowRFU)
  CP(m_bPreviewShowLadderLabels)
  CP(m_bPreviewXBPS)
  CP(m_nPreviewShowArtifact)

  //  plot settings

  CP(m_bPlotDataAnalyzed)
  CP(m_bPlotDataRaw)
  CP(m_bPlotDataLadder)
  CP(m_bPlotDataBaseline)
  CP(m_bPlotDataXBPS)
  CP(m_bPlotShowILS)
  CP(m_bPlotShowRFU)
  CP(m_bPlotShowLadderLabels)
  CP(m_bPlotResizable)
  CP(m_nPlotMinHeight)
  CP(m_nPlotShowArtifact)
  CP(m_anPlotDisplayPeak)
  CP(m_nPlotMaxLadderLabels)

  //  plot print settings for analysis printout

  CP(m_bPrintCurveAnalyzed)
  CP(m_bPrintCurveRaw)
  CP(m_bPrintCurveLadder)
  CP(m_bPrintCurveLadderLabels)
  CP(m_bPrintCurveBaseline)
  CP(m_bPrintCurveILSvertical)
  CP(m_bPrintCurveMinRFU)
  CP(m_bPrintXaxisILSBPS)
  CP(m_nPrintXscale)
  CP(m_nPrintXscaleMin)
  CP(m_nPrintXscaleMax)
  CP(m_nPrintYscale)
  CP(m_nPrintYscaleMin)
  CP(m_nPrintYscaleMax)
  CP(m_nPrintYcaleNegCtrl)
  CP(m_anPrintLabelsPeak)
  CP(m_nPrintArtifact)
  CP(m_bPrintHeading)
  CP(m_sPrintHeadingNotes)
  CP(m_nPrintChannelsSamples)
  CP(m_nPrintChannelsLadders)
  CP(m_nPrintChannelsNegCtrl)
  CP(m_bPrintChannelsOmitILS)
  CP(m_bPrintSamplesLadders)
  CP(m_bPrintSamplesPosCtrl)
  CP(m_bPrintSamplesNegCtrl)
  CP(m_bPrintSamplesDisabled)
  CP(m_nPrintColorRed)
  CP(m_nPrintColorGreen)
  CP(m_nPrintColorBlue)
  CP(m_nPrintColorYellow)
  CP(m_nPrintColorOrange)
  CP(m_nPrintColorPurple)
  CP(m_nPrintColorGray)

  //  plot printout -- margins are in millimeters

  CP(m_nPrintPlotMarginTop)
  CP(m_nPrintPlotMarginBottom)
  CP(m_nPrintPlotMarginLeft)
  CP(m_nPrintPlotMarginRight)
  CP(m_nPrintPlotPaperType)
  CP(m_nPrintPlotPaperWidth)
  CP(m_nPrintPlotPaperHeight)
  CP(m_bPrintPlotLandscape)
  CP(m_nPrintPreviewZoom)

  //  XSLT saved parameter info

  CP(m_sLastXSLInputFileDir)
  CP(m_bPrivacySeen)


  // end generated compare


  CP(m_gridAttr)
//  CP(m_bModified)
//  CP(m_bAutoSave)

#undef CP
  return bRtn;
}


void CParmOsiris::RegisterAll(bool bInConstructor)
{
  if(bInConstructor)
  {
    m_ioDefaultSample.SetDefault(
      CLabNameStrings::DEFAULT_SPECIMEN_CATEGORY);
    m_ioBatchFormat.SetDefault(DEFAULT_BATCH_FORMAT);
    m_ioIntViewPlotArtifact.SetDefault(ARTIFACT_CRITICAL);
    m_ioUint1.SetDefault(1);
    m_ioInt_1.SetDefault(-1);
  }

  // begin generated register

  RegisterWxString("LastExportXSLsearch", &m_sLastExportXSLsearch);
  RegisterWxString("LastExportDirectory", &m_sLastExportDirectory);
  RegisterWxString("inputDirectory", &m_sInputDirectory);
  RegisterWxString("outputDirectory", &m_sOutputDirectory);

  //   m_sKitName is obsolete, use volume name

  RegisterWxString("kit", &m_sKitName);
  RegisterWxString("volume", &m_sVolumeName);
  RegisterWxString("ls", &m_sLsName);
  RegisterInt("minRFUsample", &m_nMinRFU_Sample);
  RegisterInt("minRFUinterlocus", &m_nMinRFU_Interlocus);
  RegisterInt("minRFUILS", &m_nMinRFU_ILS);
  RegisterInt("minRFUladder", &m_nMinRFU_Ladder);
  RegisterInt("minRFUladderInterlocus", &m_nMinLadderInterlocusRFU);
  RegisterInt("minRFUsampleDetection", &m_nSampleDetectionThreshold);
  RegisterWxString("AnalysisOverride", &m_sAnalysisOverride);
  RegisterIntVector("ChannelRFU", &m_anChannelRFU);
  RegisterIntVector("ChannelDetection", &m_anChannelDetection);
  RegisterBool("TimeStampSubDir", &m_bTimeStampSubDir);
  RegisterBool("DataAnalyzed", &m_bDataAnalyzed);

  //  CMF settings

  RegisterWxString("CMFsourceLab", &m_sCMFsourceLab);
  RegisterWxString("CMFdestLab", &m_sCMFdestLab);
  Register("CMFdefaultSample",&m_ioDefaultSample,(void *) &m_sCMFdefaultSample);
  Register("CMFbatchFormat",&m_ioBatchFormat,(void *) &m_sCMFbatchFormat);
  RegisterWxString("CMFuserID", &m_sCMFuserID);
  RegisterBool("CMFviewLocation", &m_bCMFviewLocation);
  RegisterBool("ShowFileLocationDir", &m_bShowFileLocationDir);

  //  PNG Export Settings

  RegisterDouble("PNGwidth", &m_dPNGwidth);
  RegisterDouble("PNGheight", &m_dPNGheight);
  RegisterInt("PNGunits", &m_nPNGunits);
  RegisterInt("PNGusage", &m_nPNGusage);
  RegisterBool("PNGviewLocation", &m_bPNGviewLocation);
  Register("ShowAlerts",&m_ioInt_1,(void *) &m_nShowAlerts);
  RegisterBoolTrue("ShowPreview", &m_bShowPreview);
  RegisterBool("HideGraphicToolbar", &m_bHideGraphicToolbar);
  RegisterBool("HideGraphicScrollbar", &m_bHideGraphicScrollbar);
  RegisterBool("HideTextToolbar", &m_bHideTextToolbar);
  RegisterBool("HideSampleToolbar", &m_bHideSampleToolbar);
  RegisterBoolTrue("StartupMRU", &m_bStartupMRU);
  RegisterBoolTrue("CheckBeforeExit", &m_bCheckBeforeExit);
  RegisterBoolTrue("WarnOnHistory", &m_bWarnOnHistory);
  RegisterDouble("ZoomLocusMargin", &m_dZoomLocusMargin);

  //  grid

  RegisterInt("TableDisplayPeak", &m_nTableDisplayPeak);
  RegisterInt("TableSortBy", &m_nTableSortBy);
  RegisterBoolTrue("TableControlsAtTop", &m_bTableControlsAtTop);
  RegisterInt("TableShowSampleDisplayNames", &m_nTableShowSampleDisplayNames);

  //  preview settings

  RegisterBoolTrue("PreviewDataAnalyzed", &m_bPreviewDataAnalyzed);
  RegisterBool("PreviewDataRaw", &m_bPreviewDataRaw);
  RegisterBool("PreviewDataLadder", &m_bPreviewDataLadder);
  RegisterBool("PreviewDataBaseline", &m_bPreviewDataBaseline);
  RegisterBool("PreviewShowILS", &m_bPreviewShowILS);
  RegisterBool("PreviewShowRFU", &m_bPreviewShowRFU);
  RegisterBool("PreviewShowLadderLabels", &m_bPreviewShowLadderLabels);
  RegisterBool("PreviewXBPS", &m_bPreviewXBPS);
  Register("PreviewArtifact",&m_ioIntViewPlotArtifact,(void *) &m_nPreviewShowArtifact);

  //  plot settings

  RegisterBoolTrue("PlotDataAnalyzed", &m_bPlotDataAnalyzed);
  RegisterBool("PlotDataRaw", &m_bPlotDataRaw);
  RegisterBool("PlotDataLadder", &m_bPlotDataLadder);
  RegisterBool("PlotDataBaseline", &m_bPlotDataBaseline);
  RegisterBool("PlotDataXBPS", &m_bPlotDataXBPS);
  RegisterBool("PlotShowILS", &m_bPlotShowILS);
  RegisterBool("PlotShowRFU", &m_bPlotShowRFU);
  RegisterBool("PlotShowLadderLabels", &m_bPlotShowLadderLabels);
  RegisterBoolTrue("PlotResizable", &m_bPlotResizable);
  Register("PlotMinHeight",&m_ioInt_1,(void *) &m_nPlotMinHeight);
  Register("PlotShowArtifact",&m_ioIntViewPlotArtifact,(void *) &m_nPlotShowArtifact);
  RegisterUintVector("PlotDisplayPeak", &m_anPlotDisplayPeak);
  Register("MaxLadderLabels",&m_ioInt_1,(void *) &m_nPlotMaxLadderLabels);

  //  plot print settings for analysis printout

  RegisterBoolTrue("PrintCurveAnalyzed", &m_bPrintCurveAnalyzed);
  RegisterBool("PrintCurveRaw", &m_bPrintCurveRaw);
  RegisterBool("PrintCurveLadder", &m_bPrintCurveLadder);
  RegisterBool("PrintCurveLadderLabels", &m_bPrintCurveLadderLabels);
  RegisterBool("PrintCurveBaseline", &m_bPrintCurveBaseline);
  RegisterBool("PrintCurveILSvertical", &m_bPrintCurveILSvertical);
  RegisterBool("PrintCurveMinRFU", &m_bPrintCurveMinRFU);
  RegisterBool("PrintXaxisILSBPS", &m_bPrintXaxisILSBPS);
  RegisterInt("PrintXscale", &m_nPrintXscale);
  RegisterInt("PrintXscaleMin", &m_nPrintXscaleMin);
  RegisterInt("PrintXscaleMax", &m_nPrintXscaleMax);
  RegisterInt("PrintYscale", &m_nPrintYscale);
  RegisterInt("PrintYscaleMin", &m_nPrintYscaleMin);
  RegisterInt("PrintYscaleMax", &m_nPrintYscaleMax);
  RegisterInt("PrintYcaleNegCtrl", &m_nPrintYcaleNegCtrl);
  RegisterUintVector("PrintLabelsPeak", &m_anPrintLabelsPeak);
  Register("PrintArtifact",&m_ioIntViewPlotArtifact,(void *) &m_nPrintArtifact);
  RegisterInt("PrintHeading", &m_bPrintHeading);
  RegisterWxString("PrintHeadingNotes", &m_sPrintHeadingNotes);
  RegisterInt("PrintChannelsSamples", &m_nPrintChannelsSamples);
  RegisterInt("PrintChannelsLadders", &m_nPrintChannelsLadders);
  RegisterInt("PrintChannelsNegCtrl", &m_nPrintChannelsNegCtrl);
  RegisterBool("PrintChannelsOmitILS", &m_bPrintChannelsOmitILS);
  RegisterBoolTrue("PrintSamplesLadders", &m_bPrintSamplesLadders);
  RegisterBoolTrue("PrintSamplesPosCtrl", &m_bPrintSamplesPosCtrl);
  RegisterBoolTrue("PrintSamplesNegCtrl", &m_bPrintSamplesNegCtrl);
  RegisterBool("PrintSamplesDisabled", &m_bPrintSamplesDisabled);
  RegisterInt("PrintColorRed", &m_nPrintColorRed);
  RegisterInt("PrintColorGreen", &m_nPrintColorGreen);
  RegisterInt("PrintColorBlue", &m_nPrintColorBlue);
  RegisterInt("PrintColorYellow", &m_nPrintColorYellow);
  RegisterInt("PrintColorOrange", &m_nPrintColorOrange);
  RegisterInt("PrintColorPurple", &m_nPrintColorPurple);
  RegisterInt("PrintColorGray", &m_nPrintColorGray);

  //  plot printout -- margins are in millimeters

  RegisterUint("PrintPlotMarginTop", &m_nPrintPlotMarginTop);
  RegisterUint("PrintPlotMarginBottom", &m_nPrintPlotMarginBottom);
  RegisterUint("PrintPlotMarginLeft", &m_nPrintPlotMarginLeft);
  RegisterUint("PrintPlotMarginRight", &m_nPrintPlotMarginRight);
  RegisterInt("PrintPlotPaperType", &m_nPrintPlotPaperType);
  RegisterInt("PrintPlotPaperWidth", &m_nPrintPlotPaperWidth);
  RegisterInt("PrintPlotPaperHeight", &m_nPrintPlotPaperHeight);
  RegisterBool("PrintPlotLandscape", &m_bPrintPlotLandscape);
  RegisterInt("PrintPreviewZoom", &m_nPrintPreviewZoom);

  //  XSLT saved parameter info

  RegisterWxString("LastXSLInputFileDir", &m_sLastXSLInputFileDir);
  RegisterBool("PrivacySeen", &m_bPrivacySeen);


  // end generated register

  Register("GridColors",&m_gridAttr);
}

CParmOsiris::~CParmOsiris()
{
  if(m_bAutoSave)
  {
    SaveIfModified();
  }
}

void CParmOsiris::SetDefaults()
{
  ConfigDir *pConfig = mainApp::GetConfig();
  const unsigned int RFU = 150;

  wxString sExe = pConfig->GetExePath();
  wxString sInputDirectory = sExe;
  sInputDirectory += "TestAnalysis";
  nwxFileUtil::EndWithSeparator(&sInputDirectory);
  sInputDirectory += "Cofiler";

  m_bModified = true;

  // begin generated defaults

  m_sLastExportXSLsearch.Empty();
  m_sLastExportDirectory.Empty();
  m_sInputDirectory = sInputDirectory;
  m_sOutputDirectory = pConfig->GetFilePath();

  //   m_sKitName is obsolete, use volume name

  m_sKitName = "Cofiler";
  m_sVolumeName.Empty();
  m_sLsName.Empty();
  m_nMinRFU_Sample = RFU;
  m_nMinRFU_Interlocus = RFU;
  m_nMinRFU_ILS = RFU;
  m_nMinRFU_Ladder = RFU;
  m_nMinLadderInterlocusRFU = -1;
  m_nSampleDetectionThreshold = -1;
  m_sAnalysisOverride.Empty();
  m_anChannelRFU.clear();
  m_anChannelRFU.push_back(-1);
  m_anChannelDetection.clear();
  m_anChannelDetection.push_back(-1);
  m_bTimeStampSubDir = false;
  m_bDataAnalyzed = false;

  //  CMF settings

  m_sCMFsourceLab.Empty();
  m_sCMFdestLab.Empty();
  m_sCMFdefaultSample.Empty();
  m_sCMFbatchFormat = DEFAULT_BATCH_FORMAT;
  m_sCMFuserID = wxGetUserId();
  m_bCMFviewLocation = false;
  m_bShowFileLocationDir = false;

  //  PNG Export Settings

  m_dPNGwidth = 1024.0;
  m_dPNGheight = 768.0;
  m_nPNGunits = 0;
  m_nPNGusage = 0;
  m_bPNGviewLocation = false;
  m_nShowAlerts = -1;
  m_bShowPreview = true;
  m_bHideGraphicToolbar = false;
  m_bHideGraphicScrollbar = false;
  m_bHideTextToolbar = false;
  m_bHideSampleToolbar = false;
  m_bStartupMRU = true;
  m_bCheckBeforeExit = true;
  m_bWarnOnHistory = true;
  m_dZoomLocusMargin = 0.0;

  //  grid

  m_nTableDisplayPeak = 0;
  m_nTableSortBy = 0;
  m_bTableControlsAtTop = true;
  m_nTableShowSampleDisplayNames = 0;

  //  preview settings

  m_bPreviewDataAnalyzed = true;
  m_bPreviewDataRaw = false;
  m_bPreviewDataLadder = false;
  m_bPreviewDataBaseline = false;
  m_bPreviewShowILS = false;
  m_bPreviewShowRFU = false;
  m_bPreviewShowLadderLabels = false;
  m_bPreviewXBPS = false;
  m_nPreviewShowArtifact = m_ioIntViewPlotArtifact.GetDefault();

  //  plot settings

  m_bPlotDataAnalyzed = true;
  m_bPlotDataRaw = false;
  m_bPlotDataLadder = false;
  m_bPlotDataBaseline = false;
  m_bPlotDataXBPS = false;
  m_bPlotShowILS = false;
  m_bPlotShowRFU = false;
  m_bPlotShowLadderLabels = false;
  m_bPlotResizable = true;
  m_nPlotMinHeight = -1;
  m_nPlotShowArtifact = m_ioIntViewPlotArtifact.GetDefault();
  m_anPlotDisplayPeak.clear();
  m_anPlotDisplayPeak.push_back(1);
  m_nPlotMaxLadderLabels = -1;

  //  plot print settings for analysis printout

  m_bPrintCurveAnalyzed = true;
  m_bPrintCurveRaw = false;
  m_bPrintCurveLadder = false;
  m_bPrintCurveLadderLabels = false;
  m_bPrintCurveBaseline = false;
  m_bPrintCurveILSvertical = false;
  m_bPrintCurveMinRFU = false;
  m_bPrintXaxisILSBPS = false;
  m_nPrintXscale = 0;
  m_nPrintXscaleMin = 0;
  m_nPrintXscaleMax = 20000;
  m_nPrintYscale = 0;
  m_nPrintYscaleMin = -200;
  m_nPrintYscaleMax = 6000;
  m_nPrintYcaleNegCtrl = 0;
  m_anPrintLabelsPeak.clear();
  m_anPrintLabelsPeak.push_back(1);
  m_nPrintArtifact = m_ioIntViewPlotArtifact.GetDefault();
  m_bPrintHeading = 0;
  m_sPrintHeadingNotes.Empty();
  m_nPrintChannelsSamples = 8;
  m_nPrintChannelsLadders = 8;
  m_nPrintChannelsNegCtrl = 8;
  m_bPrintChannelsOmitILS = false;
  m_bPrintSamplesLadders = true;
  m_bPrintSamplesPosCtrl = true;
  m_bPrintSamplesNegCtrl = true;
  m_bPrintSamplesDisabled = false;
  m_nPrintColorRed = 100;
  m_nPrintColorGreen = 100;
  m_nPrintColorBlue = 100;
  m_nPrintColorYellow = 100;
  m_nPrintColorOrange = 100;
  m_nPrintColorPurple = 100;
  m_nPrintColorGray = 100;

  //  plot printout -- margins are in millimeters

  m_nPrintPlotMarginTop = 25;
  m_nPrintPlotMarginBottom = 25;
  m_nPrintPlotMarginLeft = 25;
  m_nPrintPlotMarginRight = 25;
  m_nPrintPlotPaperType = -1;
  m_nPrintPlotPaperWidth = -1;
  m_nPrintPlotPaperHeight = -1;
  m_bPrintPlotLandscape = false;
  m_nPrintPreviewZoom = -1;

  //  XSLT saved parameter info

  m_sLastXSLInputFileDir.Empty();
  m_bPrivacySeen = false;


  // end generated defaults

  m_gridAttr.SetDefaults();
}
bool CParmOsiris::Save()
{
  bool bRtn = SaveFile(m_sFileName);
  if(!bRtn)
  {
    wxString sError =
      "An error occurred when writing OSIRS parameters file:\n";
    sError.Append(m_sFileName);
    wxASSERT_MSG(0,sError);
    mainApp::LogMessage(sError);
  }
  else
  {
    m_bModified = false;
  }
  return bRtn;
}

int CParmOsiris::GetPrintColorByName(const wxString &sName) const
{
  // return the printing color intensity (1-100) for printing
  // color specified by sName
  wxString s(sName);
  s.MakeUpper();
  int nRtn = 100; // if color not found, default is 100%
  
 #define _CHECK(name, fnc)  if(s == wxT(name)) nRtn = fnc()  
  _CHECK("RED", GetPrintColorRed);
  else _CHECK("GREEN", GetPrintColorGreen);
  else _CHECK("BLUE", GetPrintColorBlue);
  else _CHECK("YELLOW", GetPrintColorYellow);
  else _CHECK("ORANGE", GetPrintColorOrange);
  else _CHECK("PURPLE", GetPrintColorPurple);
#undef _CHECK

  return nRtn;
}

const wxString CParmOsiris::DEFAULT_BATCH_FORMAT("%Y%m%d_%H%M%S");
const wxString CParmOsiris::LABEL_VIEW_LOCATION("View File Location");
const wxString CParmOsiris::LABEL_ORIGIN("Origin ");
const wxString CParmOsiris::LABEL_INPUT("Input Directory ");
const wxString CParmOsiris::LABEL_OUTPUT("Output Directory ");
const wxString CParmOsiris::LABEL_KIT("Kit Name ");
const wxString CParmOsiris::LABEL_VOLUME("Operating Procedure Name ");
const wxString CParmOsiris::LABEL_LS("Internal Lane Standard ");
const wxString CParmOsiris::LABEL_RFU("Minimum RFU ");
const wxString CParmOsiris::LABEL_RFU_SAMPLE("Sample: ");
const wxString CParmOsiris::LABEL_RFU_ILS("ILS: ");
const wxString CParmOsiris::LABEL_RFU_LADDER("Ladder: ");
const wxString CParmOsiris::LABEL_RFU_INTERLOCUS("Interlocus: ");
const wxString CParmOsiris::LABEL_DATA("Data ");
const wxString CParmOsiris::LABEL_DATA_RAW("Raw  ");
const wxString CParmOsiris::LABEL_DATA_ANALYZED("Analyzed");
const wxString CParmOsiris::LABEL_ANALYSIS("Analysis: ");
const wxString CParmOsiris::LABEL_DETECTION("Detection: ");
const wxString CParmOsiris::LABEL_INTERLOCUS("Interlocus: ");
const wxString CParmOsiris::ERROR_MSG_INTERLOCUS("Minimum interlocus RFU must be\ngreater than or equal to the minimum RFU");


