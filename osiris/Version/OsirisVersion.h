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
*  FileName: OsirisVersion.h
*  Author:   Douglas Hoffman
*
*/
#ifndef __OSIRIS_VERSION_H__
#define __OSIRIS_VERSION_H__

#ifdef WIN32
#define OSIRIS_OS "Windows"

#else

#ifdef Darwin
#define OSIRIS_OS "Macintosh"

#else

#ifdef Linux
#define OSIRIS_OS "Linux"

#endif
#endif
#endif

#define OSIRIS_VERS_BASE "2.16 Beta 13"
#define OSIRIS_VERS OSIRIS_VERS_BASE
#define OSIRIS_VERS_SEARCH_OS " for "
#define OSIRIS_VERSION OSIRIS_VERS OSIRIS_VERS_SEARCH_OS OSIRIS_OS
#define OSIRIS_FULLNAME "OSIRIS " OSIRIS_VERSION
#define OSIRIS_WINDOW_TITLE "OSIRIS " OSIRIS_VERS_BASE

#endif
