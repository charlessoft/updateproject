#pragma once

class CsysPath
{
public:
	CsysPath(void);
	//
	// Summary:
	//     Returns the directory information for the specified path string.
	//
	// Parameters:
	//   path:
	//     The path of a file or directory.
	//
	// Returns:
	//     A System.String containing directory information for path, or null if path
	//     denotes a root directory, is the empty string (""), or is null. Returns System.String.Empty
	//     if path does not contain directory information.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     The path parameter contains invalid characters, is empty, or contains only
	//     white spaces.
	//
	//   System.IO.PathTooLongException:
	//     The path parameter is longer than the system-defined maximum length.
	static string GetDirectoryName(string path);
	static wstring GetDirectoryName(wstring path);


	// Summary:
	//     Changes the extension of a path string.
	//
	// Parameters:
	//   path:
	//     The path information to modify. The path cannot contain any of the characters
	//     defined in System.IO.Path.GetInvalidPathChars().
	//
	//   extension:
	//     The new extension (with or without a leading period). Specify null to remove
	//     an existing extension from path.
	//
	// Returns:
	//     A string containing the modified path information.  On Windows-based desktop
	//     platforms, if path is null or an empty string (""), the path information
	//     is returned unmodified. If extension is null, the returned string contains
	//     the specified path with its extension removed. If path has no extension,
	//     and extension is not null, the returned path string contains extension appended
	//     to the end of path.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	static string ChangeExtension(string path, string extension);
	static wstring ChangeExtension(wstring path, wstring extension);
	//
	// Summary:
	//     Combines two path strings.
	//
	// Parameters:
	//   path1:
	//     The first path.
	//
	//   path2:
	//     The second path.
	//
	// Returns:
	//     A string containing the combined paths. If one of the specified paths is
	//     a zero-length string, this method returns the other path. If path2 contains
	//     an absolute path, this method returns path2.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path1 or path2 contain one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	//
	//   System.ArgumentNullException:
	//     path1 or path2 is null.
	static string Combine(string path1, string path2);
	static wstring Combine(wstring path1, wstring path2);
	
	//
	// Summary:
	//     Returns the extension of the specified path string.
	//
	// Parameters:
	//   path:
	//     The path string from which to get the extension.
	//
	// Returns:
	//     A System.String containing the extension of the specified path (including
	//     the "."), null, or System.String.Empty. If path is null, GetExtension returns
	//     null. If path does not have extension information, GetExtension returns Empty.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	static string GetExtension(string path);
	static wstring GetExtension(wstring path);
	//
	// Summary:
	//     Returns the file name and extension of the specified path string.
	//
	// Parameters:
	//   path:
	//     The path string from which to obtain the file name and extension.
	//
	// Returns:
	//     A System.String consisting of the characters after the last directory character
	//     in path. If the last character of path is a directory or volume separator
	//     character, this method returns System.String.Empty. If path is null, this
	//     method returns null.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	static string GetFileName(string path);
	static wstring GetFileName(wstring path);
	//
	// Summary:
	//     Returns the file name of the specified path string without the extension.
	//
	// Parameters:
	//   path:
	//     The path of the file.
	//
	// Returns:
	//     A System.String containing the string returned by System.IO.Path.GetFileName(System.String),
	//     minus the last period (.) and all characters following it.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	static string GetFileNameWithoutExtension(string path);
	static wstring GetFileNameWithoutExtension(wstring path);
	//
	// Summary:
	//     Returns the absolute path for the specified path string.
	//
	// Parameters:
	//   path:
	//     The file or directory for which to obtain absolute path information.
	//
	// Returns:
	//     A string containing the fully qualified location of path, such as "C:\MyFile.txt".
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path is a zero-length string, contains only white space, or contains one
	//     or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	//      -or- The system could not retrieve the absolute path.
	//
	//   System.Security.SecurityException:
	//     The caller does not have the required permissions.
	//
	//   System.ArgumentNullException:
	//     path is null.
	//
	//   System.NotSupportedException:
	//     path contains a colon (":") that is not part of a volume identifier (for
	//     example, "c:\").
	//
	//   System.IO.PathTooLongException:
	//     The specified path, file name, or both exceed the system-defined maximum
	//     length. For example, on Windows-based platforms, paths must be less than
	//     248 characters, and file names must be less than 260 characters.
	static string GetFullPath(string path);
	//
	// Summary:
	//     Gets an array containing the characters that are not allowed in file names.
	//
	// Returns:
	//     An array containing the characters that are not allowed in file names.  
	//////////static char[] GetInvalidFileNameChars();
	//
	// Summary:
	//     Gets an array containing the characters that are not allowed in path names.
	//
	// Returns:
	//     An array containing the characters that are not allowed in path names.  
	//////////static char[] GetInvalidPathChars();
	//
	// Summary:
	//     Gets the root directory information of the specified path.
	//
	// Parameters:
	//   path:
	//     The path from which to obtain root directory information.
	//
	// Returns:
	//     A string containing the root directory of path, such as "C:\", or null if
	//     path is null, or an empty string if path does not contain root directory
	//     information.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	//      -or- System.String.Empty was passed to path.
	static string GetPathRoot(string path);
	static wstring GetPathRoot(wstring path);
	//
	// Summary:
	//     Returns a random folder name or file name.
	//
	// Returns:
	//     A random folder name or file name.
	static string GetRandomFileNameA();
	static wstring GetRandomFileNameW();
	//
	// Summary:
	//     Creates a uniquely named, zero-byte temporary file on disk and returns the
	//     full path of that file.
	//
	// Returns:
	//     A System.String containing the full path of the temporary file.
	//
	// Exceptions:
	//   System.IO.IOException:
	//     An I/O error occurs, such as no unique temporary file name is available.
	//      - or - This method was unable to create a temporary file.
	static string GetTempFileNameA(char* headInfo);
	static wstring GetTempFileNameW(wchar_t* headInfo);
	//
	// Summary:
	//     Returns the path of the current system's temporary folder.
	//
	// Returns:
	//     A System.String containing the path information of a temporary directory.
	//
	// Exceptions:
	//   System.Security.SecurityException:
	//     The caller does not have the required permissions.
	static string GetTempPathA();
	static string GetTempPathW();
	//
	// Summary:
	//     Determines whether a path includes a file name extension.
	//
	// Parameters:
	//   path:
	//     The path to search for an extension.
	//
	// Returns:
	//     true if the characters that follow the last directory separator (\\ or /)
	//     or volume separator (:) in the path include a period (.) followed by one
	//     or more characters; otherwise, false.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	static BOOL HasExtension(string path);
	static BOOL HasExtension(wstring path);
	//
	// Summary:
	//     Gets a value indicating whether the specified path string contains absolute
	//     or relative path information.
	//
	// Parameters:
	//   path:
	//     The path to test.
	//
	// Returns:
	//     true if path contains an absolute path; otherwise, false.
	//
	// Exceptions:
	//   System.ArgumentException:
	//     path contains one or more of the invalid characters defined in System.IO.Path.GetInvalidPathChars().
	static bool IsPathRooted(string path);
public:
	~CsysPath(void);
};
