#ifndef FILEREADER_H
#define FILEREADER_H

#include <string> 
#include <vector> 

namespace Graphics 
{
    class FileReader 
    {
        public: 
            static std::pair<std::string, std::string> getFileExtension(std::string path);
    
    	    // Get file name with extension 
    	    static std::string getFileName(std::string path);
     
    	    static std::pair<bool, std::string> getFile
    	    (
    	        std::string path, 
    	        std::string extension = ""
    	    );

    	    static std::vector<std::string> getFolders(std::string path);

    	    static std::vector<std::string> getFiles
    	    (
    	        std::string path, 
    	        std::vector<std::string> extensions = {}
    	    );
    
    	    static std::string getDirectory(std::string path, bool full = false);
    
    
    };
}

#endif
