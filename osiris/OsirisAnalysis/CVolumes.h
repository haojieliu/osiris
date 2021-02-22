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
*  FileName: CVolumes.h
*  Author:   Douglas Hoffman
*
*/

#ifndef __C_VOLUMES_H__
#define __C_VOLUMES_H__

#define VOLUME_STRING "Operating Procedure"
#define Volume_string "operating procedure"
#define AVolume_string "An " Volume_string
#define aVolume_string "an " Volume_string

#include <wx/dir.h>
#include <wx/string.h>
#include <wx/datetime.h>
#include <wx/arrstr.h>
#include <set>
#include <map>
#include <vector>
#include "nwx/nsstd.h"
#include "nwx/nwxXmlPersist.h"
#include "nwx/nwxLock.h"
#include "CLabSettings.h"
#include "CXMLmessageBook.h"

//******************************************************  CVolume


class CVolume
{
   // wrapper around a volume and manages CLabSettings

public:
  enum
  {
    OSIRIS_KIT,
    USER_OP,
    USER_KIT
  };
  CVolume(const wxString &sPath, int nVolumeType = USER_OP);
  CVolume();
  CVolume(const CVolume &x);
  CVolume &operator = (const CVolume &x);
  virtual ~CVolume();
  CVolume *Clone() const
  {
    return new CVolume(*this);
  }

  const wxString &GetPath() const
  {
    return m_sPath;
  }
  wxString GetKitName();
  const wxString &GetVolumeName();
  CLabSettings *GetLabSettings()
  {
    return &m_lab;
  }
  const CLabSettings *GetLabSettings() const
  {
    return &m_lab;
  }
  const CXMLmessageBook *GetMessageBook()
  {
    return &m_book;
  }
  const wxString &GetDataFileType() const
  {
    return m_lab.GetLabSettingsInfo()->GetDataFileType();
  }
  wxString GetLabSettingsFileName() const;
  wxString GetAccessFileName() const;
  wxString GetMessageBookFileName() const;
  wxString GetStdSettingsFileName() const;
  bool InitNewVolume(const wxString &sName);
  void SetVolumeName(const wxString &sName);
  bool Load(const wxString &sPath, bool bSetReadOnly = false);
  bool Save();
  bool Save(const wxString &sPath);
  int GetVolumeType() const
  {
    return m_nVolumeType;
  }
  bool IsOK() const
  {
  // return false if an error occurred or no lab settings loaded
    return m_bOK;
  }
  bool IsReadOnly() const
  {
    return m_bReadOnly;
  }
  void SetReadOnly(bool bReadOnly = true)
  {
    m_bReadOnly = bReadOnly;
  }
  bool CreatedByNewerVersion();
  bool CheckReload(bool bForceReload = false);
  const wxString &GetLastError() const
  {
    return m_sLastError;
  }
  // Lock info

  bool IsNewVolume() const
  {
    return m_bNewVolume;
  }
#if 0
//  OS-679 - removed because access time is unreliable
  bool SetInUseOnTimer(int nms);
  bool SetInUse()
  {
    bool b = m_lockRead.Touch();
    return b;
  }
  bool AccessedSince(int nSeconds = 300);
#endif
  wxString GetLockUser();
  bool Lock();
  bool HasLock();
  bool ReleaseLock();
  bool IsLocked();
  bool RefreshLock() // 'touch' the lock file
  {
    bool bRtn = false;
    if(IsOK() && !IsReadOnly())
    {
      bRtn = m_lock.Touch();
    }
    return bRtn;
  }

  static wxString GetPathPrefix(const wxString &sDir);
  static bool HasAllFiles(const wxString &sDir, const wxChar *ppList[]);
private:
  void _InitError();
  wxString _GetFilePath(const wxChar *pFileName) const;
  nwxLock m_lock;
//  nwxLockRead m_lockRead;
  CLabSettings m_lab;
  CXMLmessageBook m_book;
  wxString m_sPath;
  wxString m_sLastError;
  int m_nCountDown;
  int m_nVolumeType;
  bool m_bReadOnly;
  bool m_bOK;
  bool m_bIgnoreReadLock;
  bool m_bNewVolume;
};

class CLessVolumeName
{
  // similar to nwxStringLessNoCaseSort and
  // nwxStringLessNoCase, but puts strings beginning with
  // a left bracket ([) at the bottom
  //
  //  this creates the following scenario
  //
  //  'ABC' < 'abc' < 'ABCD'
  //  and 'ABC' != 'abc'
  //   '[ABC' > 'ABC'
  //
  //  This is useful for case sensitive sets with
  //  case insensitive sorting
public:
  CLessVolumeName() {;}
  bool operator()(const wxString &x1, const wxString &x2) const
  {
    int n = 0;
    const wxStringCharType *pc1 = x1.wx_str();
    const wxStringCharType *pc2 = x2.wx_str();
    if(*pc1 == *pc2)
    {}
    else if ((*pc1) == L'[')
    {
      // osiris kit - bottom
      n = 1;
    }
    else if ((*pc2) == L'[')
    {
      n = -1;
    }
    else if ((*pc1) == L'<')
    {
      // user defined kit - before osiris kits after user volumes
      n = 1;
    }
    else if ((*pc2) == L'<')
    {
      n = -1;
    }
    if(!n)
    {
      n = x1.CmpNoCase(x2);
      if(!n)
      {
        n = x1.Cmp(x2);
      }
    }
    return (n < 0);
  }
  bool operator()(const wxString *px1, const wxString *px2) const
  {
    return (*this)(*px1,*px2);
  }
};




typedef multimap<wxString, CVolume *,CLessVolumeName>
  MapVolume;

typedef set<wxString,CLessVolumeName>
  SetVolumeNames;

//******************************************************  CVolumes

class CVolumes : public wxDirTraverser
{
public:
  CVolumes();
  virtual ~CVolumes();
  bool CheckReload(bool bForceReload = false);
  void RefreshLocks();
  void RefreshLocksOnTimer(int nms);
  CVolume *Find(const wxString &sName);
  const CVolume *Find(const wxString &sName) const;
  size_t FindAll(vector<CVolume *> *pvVol, const wxString &sName);
  CVolume *Create(const wxString &sCopyFrom, const wxString &sName);
  bool Remove(CVolume *pVolume);
  bool Remove(const wxString &sName)
  {
    CVolume *p = Find(sName);
    bool bRtn = (p == NULL) ? false : Remove(p);
    return bRtn;
  }
  bool Rename(CVolume *pVolume, const wxString &sNewName);

  size_t GetNames(SetVolumeNames *pSet) const;

  bool Load()
  {
    return CheckReload(true);
  }
  virtual wxDirTraverseResult OnDir(const wxString& dirname);
  virtual wxDirTraverseResult OnFile(const wxString& filename);
  size_t GetCount() const
  {
    return m_mapVol.size();
  }
  wxString GetLastError() const
  {
    // not const wxString& because the string changes
    return m_sLastError;
  }
  const wxArrayString &GetKits() const;
  const wxArrayString &GetVolumeNames() const;
#ifdef __WXDEBUG__
  static void UnitTest();
#endif

private:
  static const wxChar *g_psNames[]; // file names w/in volume

  MapVolume m_mapVol;
  map<wxString,CVolume *> m_mapKitVolume;
  mutable wxArrayString m_asKits;
  mutable wxArrayString m_asVolumeNames;

  wxString m_sDirVolume; // path of site volumes or OPs
  wxString m_sDirUserKits;
  wxDateTime m_dtLastMod; // modification time of m_sDirVolume when loaded
  wxString m_sDirBase; // path of volumes included with Osiris
  wxString m_sLastError;
  int m_nCountDown;
  int m_nVolumeType; // true is loading base, false otherwise

  void _SetModified();
  bool _RemoveFiles(const wxString &sDir);
  bool _HasFiles(const wxString &dirname)
  {
    return CVolume::HasAllFiles(
      dirname, g_psNames);
  }
  void _Cleanup();
  void _LoadDir(const wxString &dirname);
  void _SetupKitVolumes();
  void _SetupPath();
  MapVolume::iterator _FindVolume(CVolume *pVolume);
  bool _BuildNewPath(const CVolume *pCopyFrom, wxString *psPath);

  void _SetErrorNameExists(const wxString &s);
  void _Load()
  {
    _Cleanup();
    m_nVolumeType = CVolume::OSIRIS_KIT;
    _LoadDir(m_sDirBase);
    m_nVolumeType = CVolume::USER_KIT;
    _LoadDir(m_sDirUserKits);
    m_nVolumeType = CVolume::USER_OP;
    if(m_sDirVolume.Len())
    {
      _LoadDir(m_sDirVolume);
    }
    _SetupKitVolumes();
  }
};


#endif
