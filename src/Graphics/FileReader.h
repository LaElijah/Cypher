#ifndef FILE_READER_H
#define FILE_READER_H

#include <string>
#include <dirent.h>
#include <vector>

namespace Graphics 
{

    /**
     * This class handles file I/O and provides file 
     * filtering functionality.
     *
     * Planned Features:
     * - Regex file searching
     *
     * Known Bugs:
     * - Folder to file differentation doesent work for all functions 
     *   must remove for more accurate testing seperately
     */
    class FileReader 
    {
    
        public:
	    FileReader(const char* directory);
	    FileReader(std::string directory);

	    std::string getDirname();
	    void setDirname(std::string directory);	
	    
	    std::vector<std::string> getFolders();
	    std::vector<std::string> getFiles();
	    std::vector<std::string> getFiles(std::string extension);

	    std::string getFile();
	    std::string getFile(std::string extension);

	    static std::pair<std::string, std::string> splitFileExtension(std::string filename);
	    static std::string getNameFromDirectory(std::string& directory);

	private: 
	    std::string dirname;

	    DIR* getDirectory();
	    DIR* getDirectory(std::string dirname);

            struct dirent* getEntry();
            struct dirent* getEntry(std::string dirname);	 
    };
}




#endif
