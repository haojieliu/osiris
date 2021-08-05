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

*  FileName: ConfigDir.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __CONFIG_DIR_H__
#define __CONFIG_DIR_H__

#include <wx/string.h>
#include <wx/filename.h>
#include "nwx/nwxFileUtil.h"
#include "nwx/stdb.h"
#include <set>
#include "nwx/stde.h"

class ConfigDir
{
public:
  ConfigDir(
    const char *psConfigSubdir = NULL,
    const char *psFileSubdir = NULL);
  virtual ~ConfigDir();
  void SetConfigSubdir(const char *psSubdir);
  void SetFileSubdir(const char *psSubdir);


  const wxString &GetExePath() const
  {
    return m_sExePath;
  }
  const wxString &GetExeConfigPath() const
  {
    return m_sExeConfigPath;
  }
  const wxString &GetExeVolumePath() const
  {
    return m_sExeVolumePath;
  }
  const wxString &GetConfigPath(const char *psSubdir)
  {
    if (psSubdir != NULL)
    {
      SetConfigSubdir(psSubdir);
    }
    return m_sConfigPath;
  }
  const wxString GetDebugConfigPath(bool bIfExists = true)
  {
    wxString s(GetConfigPath());
    s.Append(wxT("Debug"));
    if (bIfExists && !wxFileName::DirExists(s))
    {
      s.Clear();
    }
    else
    {
      nwxFileUtil::EndWithSeparator(&s);
    }
    return s;
  }
  const wxString &GetSitePath() const;
  bool IsSitePathWritable() const
  {
    bool bRtn = wxFileName::IsDirWritable(GetSitePath());
    return bRtn;
  }
  const wxString &GetConfigPath() const
  {
    return m_sConfigPath;
  }
  const wxString &GetFilePath(const char *psFileSubdir)
  {
    if (psFileSubdir != NULL)
    {
      SetFileSubdir(psFileSubdir);
    }
    return m_sFilePath;
  }
  const wxString &GetFilePath() const
  {
    return m_sFilePath;
  }
  bool IsOK() const
  {
    return !m_bError;
  }
  wxString GetLadderFileName() const
  {
    // Osiris v 1.x
    wxString sRtn = GetExeConfigPath();
    nwxFileUtil::EndWithSeparator(&sRtn);
    sRtn.Append("LadderData.xml");
    return sRtn;
  }
  wxString GetILSLadderFilePath() const
  {
    // Osiris v 2.1+
    wxString sRtn = GetExeConfigPath();
    sRtn.Append("LadderSpecifications");
    nwxFileUtil::EndWithSeparator(&sRtn);
    return sRtn;
  }
  wxString GetILSLadderFileName(const wxString &s) const
  {
    // Osiris v 2.x
    wxString sRtn = GetILSLadderFilePath();
    sRtn.Append(s);
    return sRtn;
  }
  wxString GetILSLadderFileName() const
  {
    // Osiris v 2.x
    return GetILSLadderFileName("ILSAndLadderInfo.xml");
  }
  wxString GetMessageBookFileName() const
  {
    wxString sRtn = GetILSLadderFilePath();
    sRtn.Append("MessageBook.xml");
    return sRtn;
  }
  wxString GetArtifactLabelsFileName() const
  {
    wxString sRtn;
    if (!_BuildFileName(GetILSLadderFilePath(), wxT("ArtifactLabels.xml"), &sRtn))
    {
      sRtn.Clear();
    }
    return sRtn;
  }
  wxString GetCMFListFileName() const
  {
    wxString sRtn = GetExeConfigPath();
    sRtn.Append(wxS("CmfList.xml"));
    return sRtn;
  }
  wxString GetUserVolumePath() const
  {
    wxString sRtn;
    _BuildFilePath(GetSitePath(), wxT("Volumes"), &sRtn);
    return sRtn;
  }

  wxString GetSiteConfigPath() const
  {
    wxString sRtn;
    _BuildFilePath(GetSitePath(), wxT("Config"), &sRtn);
    return sRtn;
  }
  bool InOsirisConfigPath(const wxString &sPath) const
  {
    // OS-1568 check if a file is in the OSIRIS config path
    return nwxFileUtil::FileInDirectory(sPath, GetExeConfigPath());
  }
  bool InSiteConfigPath(const wxString &sPath) const
  {
    // OS-1568 check if a file is in the Site config path
    return nwxFileUtil::FileInDirectory(sPath, GetSiteConfigPath());
  }

  wxString GetArtifactLabelsUserFileName() const
  {
    wxString sRtn;
    _BuildFileName(GetSitePath(), wxT("UserArtifactLabels.xml"), &sRtn);
    return sRtn;
  }
  wxString GetSiteKitPath() const
  {
    wxString sRtn;
    _BuildFilePath(GetSiteConfigPath(), wxT("Volumes"), &sRtn);
    return sRtn;
  }
  wxString GetSiteToolsPath() const
  {
    wxString sRtn;
    _BuildFilePath(GetSitePath(), wxT("ConfigurationTools"), &sRtn);
    return sRtn;
  }
  wxString GetExeToolsPath() const
  {
    wxString sRtn;
    _BuildFilePath(GetExeConfigPath(), wxT("UserTools"), &sRtn);
    return sRtn;
  }
  wxString GetSiteILSLadderFilePath() const
  {
    wxString sRtn;
    _BuildFilePath(GetSiteConfigPath(), wxT("LadderSpecifications"), &sRtn);
    return sRtn;
  }
  wxString GetSitePositiveControlsFileName() const
  {
    wxString sRtn;
    _BuildFileName(
      GetSiteILSLadderFilePath(), wxT("StandardPositiveControls.xml"), &sRtn);
    return sRtn;
  }
  wxString GetPositiveControlsFileName() const
  {
    wxString sRtn;
    _BuildFileName(
      GetILSLadderFilePath(), wxT("StandardPositiveControls.xml"), &sRtn);
    return sRtn;
  }
  size_t GetSiteILSFileNames(std::set<wxString> *) const;

#ifdef __WXDEBUG__
  void Log();
#endif

private:

  wxString m_sExePath;
  // full path of argv[0] w/o program name
  wxString m_sExeConfigPath;
  // m_sExePath + "Config"
  wxString m_sExeVolumePath;
  // m_sExeConfigPath + "Volumes"
  wxString m_sConfigPath;
  // full path for config files m_sTopConfigPath + m_sSubdir
  wxString m_sFilePath;
  // full path for data files m_sTopFilePath + m_sFileSubdir
  wxString m_sTopConfigPath;
  // top level directory for config files (${HOME}/Application Data/.osiris)
  wxString m_sTopFilePath;
  // top level directory for data files (${HOME}/My Documents/Osiris)
  wxString m_sSubdir;
  // Subdir for config files (.osiris)
  wxString m_sFileSubdir;
  // Subdir for data files (Osiris)
  // Subdir for user config files, operating procedures (volumes), etc.
  bool m_bError;
  // true is an error occurred when creating a directory

  static bool _BuildFileName(const wxString &sPath, const wxChar *psFileName, wxString *psRtn)
  {
    // load the file name in psRtn whether or not it exists
    wxString sRtn = sPath;
    nwxFileUtil::EndWithSeparator(&sRtn);
    sRtn.Append(psFileName);
    *psRtn = sRtn;
    bool bRtn = wxFileName::IsFileReadable(sRtn);
    return bRtn;
  }
  static bool _BuildFilePath(const wxString &sPath, const wxChar *psFileName, wxString *psRtn)
  {
    bool bRtn = _BuildFileName(sPath, psFileName, psRtn);
    nwxFileUtil::EndWithSeparator(psRtn);
    return bRtn;
  }
};

#endif
