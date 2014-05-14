#pragma once

#if !defined(_RECURSEDIR_INCLUDED_)
#   define _RECURSEDIR_INCLUDED_

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#   ifndef lenof
#      define lenof(x)	  (sizeof(x) / sizeof((x)[0]))
#   endif

//
// in whatever context you use this class, you have to define the TRACE function
//
#if defined(_AFX)
#	define RDTRACE	TRACE
#elif defined(_ATL)
#	define RDTRACE	ATLTRACE
#else
#	define RDTRACE	_rdtrace
void _rdtrace(LPCTSTR fmt, ... );
#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#include <shlwapi.h>
#endif



#if defined(_AFX)
//
// we use CString and CArray
//
#else
	//
	// include the string class from the STL
	//
	#pragma warning(push, 2)
	#include <string>
	#include <vector>
	using std::string;
	using std::vector;
	#pragma warning(pop)
#endif

//
// error handling constants
//
#   define RDEH_CONTINUE		   0x0000
#   define RDEH_CONTINUENEXTFILE   0x0001
#   define RDEH_CONTINUENEXTDIR	   0x0002
#   define RDEH_STOP			   0x0004
#   define RDEH_ABORT			   0x0008
#   define RDEH_FAIL			   0x0010

//
// location constants
//
#   define RDLOC_RECURSEDIR		   2
#   define RDLOC_RECURSEFILE	   3
#   define RDLOC_RECURSEFILE_SKIP  4
#   define RDLOC_HANDLERAWFILE	   6
#   define RDLOC_HANDLEFILE		   7
#   define RDLOC_HANDLEERROR	   8

//////////////////////////////////////////////////////////////////////////
//
// Simple structure to hold a filename
//
typedef struct tagSimpleFile 
{
#if defined(_AFX)
	CString		name;
#else
	string		name;
#endif

	struct tagSimpleFile& operator=(const WIN32_FIND_DATA& r)
	{
		name = r.cFileName;
		return *this;
	}
} SIMPLE_FILE;

//////////////////////////////////////////////////////////////////////////
//
// template based directory recursion
//
template<class T>
class CRecurseDir
{
public:
	CRecurseDir(LPCTSTR pstrMask = NULL) :
		  m_dwLastError(ERROR_SUCCESS),
		  m_nFiles(0),
		  m_nDirs(0),
		  m_pstrMask(pstrMask)
	  {
		  m_hStopEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		  if(m_hStopEvent == NULL)
		  {
			  RDTRACE(_T("Could not create event."));
		  }
		  m_bOwnEvent = true;
		  
		  if(m_pstrMask == NULL)
			  m_pstrMask = _T("*");
	  }


	virtual	~CRecurseDir()
	{
		if(m_bOwnEvent)
			::CloseHandle(m_hStopEvent);

		m_hStopEvent = NULL;
	}


	// /////////////////////////////////////////////////////////////////////////////
	// called to handle a certain error, return one of the RDEH_
	virtual DWORD HandleError(DWORD dwLocation, DWORD dwErr, LPCTSTR pstrInfo = NULL)
	{
		TCHAR	szErr[4096];
		//////////////////
		
		::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szErr, 4096, NULL);
		
		Log(dwLocation, szErr, pstrInfo);
		
		return RDEH_CONTINUENEXTFILE;
	}
	
	// called to log an event
	virtual void Log(DWORD dwLocation, LPCTSTR pstrFile, LPCTSTR pstrInfo = NULL)
	{
		if(pstrInfo)
			RDTRACE(_T("Location: %i, %s (%s)\r\n"), dwLocation, pstrFile, pstrInfo);
		else
			RDTRACE(_T("Location: %i, %s\r\n"), dwLocation, pstrFile);
	}
	
	// called to handle a certain file
	virtual	bool HandleRawFile(LPCTSTR pstrFile)
	{
		//
		// this is called before the file is processed and before HandleFile() is
		// called. if this returns false, the file is not processed at all
		
		Log(RDLOC_HANDLERAWFILE, pstrFile);
		
		//
		// if you add archive detection/extraction add extraction here and return
		// false after walking the tree
		//
		
		return true;
	}

	// called to handle a certain file 
	virtual void HandleFile(T* /*pFile*/)
	{
		// Log(RDLOC_HANDLEFILE, pFile->name.c_str());
	}
	
	// /////////////////////////////////////////////////////////////////////////////
	virtual	bool Run(LPCTSTR pstrDir)
	{
		return WalkTree(pstrDir);
	}
	

	// ///////////////////////////////////////////////////////////////////////////////
	// use to exclude directories from scanning
	virtual	bool CheckUseDir(LPCTSTR /*pstrPath*/, WIN32_FIND_DATA* /*pwfd*/)
	{
		return true;	// use it
	}
	
	// use to exclude files from scanning
	virtual	bool CheckUseFile(LPCTSTR /*pstrPath*/, WIN32_FIND_DATA* /*pwfd*/)
	{
		return true;	// use it
	}
	
	// called when recursion through a dir is completed, 
	// dir is not touched afterwards anymore
	virtual	void FinishedDir(LPCTSTR /*pstrDir*/)
	{
	}

	// returns the currently processed file
	T& GetCurrentFile(void)
	{
		return m_CurrentFile;
	}

	// returns the number of already processed directories (unfiltered)
	int GetDirCount(void) const
	{
		return m_nDirs;
	}

	// returns the number of already processed files (unfiltered)
	int GetFileCount(void) const
	{
		return m_nFiles;
	}

	// set the current file/dir counters to zero
	void ResetCounters(void)
	{
		m_nDirs = 0;
		m_nFiles = 0;
	}

	// set the stop event. if INVALID_HANDLE_VALUE is given, the object creates a temp event.
	void SetEvent(HANDLE hEvent = INVALID_HANDLE_VALUE)
	{
		if(m_bOwnEvent)
		{
			::CloseHandle(m_hStopEvent);
			m_bOwnEvent = FALSE;
		}
		
		if(hEvent == INVALID_HANDLE_VALUE)
		{
			hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			m_bOwnEvent = true;
		}
		
		m_hStopEvent = hEvent;
	}

protected:
	bool WalkTree(LPCTSTR pstrDir)
	{
		WIN32_FIND_DATA wfd;
		HANDLE			hFind;
		TCHAR			szPath[MAX_PATH];
		bool			bRecurse = true;
		_tcsncpy(szPath, pstrDir, MAX_PATH);
		//////////////////////////////////

		::PathAppend(szPath, _T("*"));

		m_nDirs++;	// count this directory

		Log(RDLOC_RECURSEDIR, pstrDir);

		// Iterate through dirs

		hFind = ::FindFirstFile(szPath, &wfd);
		if(hFind != INVALID_HANDLE_VALUE)
		{
			do
			{

				// FIRST check if its a dir

				if(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{

					// if not a DOT, it will not even do the string compare

					if(!((wfd.cFileName[0] == '.') && ((wfd.cFileName[1] == '.') || (wfd.cFileName[1] == '\0'))))
					{
						if(bRecurse)
						{
							// Recurse Dirs

							if(CheckUseDir(pstrDir, &wfd))
							{
								TCHAR szNextPath[MAX_PATH];
								::PathCombine(szNextPath, pstrDir, wfd.cFileName);

								WalkTree(szNextPath);
							}
							else
							{
								Log(RDLOC_RECURSEDIR, pstrDir, _T("skipping directory"));
							}
						}
					}
				}
			} while((::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0) && (::FindNextFile(hFind, &wfd)));
			::FindClose(hFind);
		}
		else
		{
			m_dwLastError = ::GetLastError();
			switch(HandleError(RDLOC_RECURSEDIR, m_dwLastError, pstrDir))
			{
				case RDEH_CONTINUE:
				case RDEH_CONTINUENEXTFILE:
					break;

				case RDEH_CONTINUENEXTDIR:
					return true;

				default:
					return false;
			}
		}

		// Iterate through files

		if(::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0)
		{
			hFind = ::FindFirstFile(szPath, &wfd);
			if(hFind != INVALID_HANDLE_VALUE)
			{
				do
				{

					if((wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
					{
						m_nFiles++;

						if(CheckUseFile(pstrDir, &wfd))
						{
							TCHAR szNextFile[MAX_PATH];
							_tcsncpy(szNextFile, pstrDir, MAX_PATH);
							::PathAppend(szNextFile, wfd.cFileName);

							// handle the filename, e.g. extract archives

							if(HandleRawFile(szNextFile))
							{
								m_CurrentFile = wfd;
								HandleFile(&m_CurrentFile);
							}
							else
							{
								Log(RDLOC_RECURSEFILE_SKIP, szNextFile);
							}
						}
					}
				} while((::WaitForSingleObject(m_hStopEvent, 0) != WAIT_OBJECT_0) && (::FindNextFile(hFind, &wfd)));
				::FindClose(hFind);
			}
			else
			{
				m_dwLastError = ::GetLastError();
				switch(HandleError(RDLOC_RECURSEFILE, m_dwLastError, pstrDir))
				{
					case RDEH_CONTINUE:
					case RDEH_CONTINUENEXTFILE:
					case RDEH_CONTINUENEXTDIR:
						return true;

					default:
						return false;
				}
			}

			FinishedDir(pstrDir);
		}
		else
		{
			Log(RDLOC_RECURSEFILE, _T("aborted"));
		}

		return true;
	}

	// set the event to stop enumeration
	void	CancelRun(void)
	{
		::SetEvent(m_hStopEvent);
	}

private:
	DWORD	m_dwLastError;

	T		m_CurrentFile;

	int		m_nFiles;			// number of processed files
	int		m_nDirs;			// number of processed dirs

	HANDLE	m_hStopEvent;		// event for stopping enumeration
	bool	m_bOwnEvent;		// true if the object owns the event, thus deletes it in the dtor
	LPCTSTR m_pstrMask;
};

//////////////////////////////////////////////////////////////////////////
//
// sample implementation: recursively delete a directory and all files within
//
class CCleanDir : public CRecurseDir<struct tagSimpleFile>
{
public:
	bool	Run(LPCTSTR pstrDir)
	{
		RDTRACE(_T("Nuking directory %s"), pstrDir);
		return CRecurseDir<struct tagSimpleFile>::Run(pstrDir);
	}
	
	DWORD HandleError(DWORD /*dwLocation*/, DWORD /*dwErr*/, LPCTSTR /*pstrInfo*/)
	{
		return RDEH_CONTINUE;
	}
	
	bool HandleRawFile(LPCTSTR pstrFile)
	{
		if(::SetFileAttributes(pstrFile, FILE_ATTRIBUTE_NORMAL))
		{
			if(!::DeleteFile(pstrFile))
			{
				RDTRACE(_T("could not delete %s"), pstrFile);
			}
		}
		else
		{
			RDTRACE(_T("could not set file attributes on %s"), pstrFile);
		}
		
		return false; // dont use the file anymore
	}
	
	void FinishedDir(LPCTSTR pstrDir)
	{
		if(::SetFileAttributes(pstrDir, NULL))
		{
			if(!::RemoveDirectory(pstrDir))
			{
				RDTRACE(_T("could not remove directory %s"), pstrDir);
			}		
		}
		else
		{
			RDTRACE(_T("could not set file attributes on %s"), pstrDir);
		}
		
	}
};

class CDirectoryContent : public CRecurseDir<WIN32_FIND_DATA>
{
	// define a appropriate array type
#if defined(_AFX)
	typedef CArray<WIN32_FIND_DATA,WIN32_FIND_DATA>	arraytype;
#else
	typedef std::vector<WIN32_FIND_DATA>	arraytype;
#endif

public:
	DWORD HandleError(DWORD /*dwLocation*/, DWORD /*dwErr*/, LPCTSTR /*pstrInfo*/)
	{
		return RDEH_CONTINUE;	// ignore any error
	}

	virtual bool CheckUseDir(LPCTSTR, WIN32_FIND_DATA*)
	{
		return false;	// we do not recurse
	}

	void HandleFile(WIN32_FIND_DATA* pFile)
	{
#if defined(_AFX)
		m_list.Add(*pFile);
#else
		m_list.push_back(*pFile);
#endif
	}

	arraytype& List()
	{
		return m_list;
	}

protected:
	arraytype m_list;
};

#endif // !defined(_RECURSEDIR_INCLUDED_)
