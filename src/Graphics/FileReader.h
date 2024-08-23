#include <string>
#include <dirent.h>
#include <vector>

namespace Graphics {

    class FileReader {
    
        public:
	FileReader(const char* directory);
	FileReader(std::string directory);

	std::string getDirname();
	void setDirname(std::string directory);	
	
	std::vector<std::string> getFolders();
	std::vector<std::string> getFiles();
	std::vector<std::string> getFiles(std::string extension);

	std::vector<std::string> getFile();
	std::string getFile(std::string extension);
	static std::pair<std::string, std::string> splitFilename(std::string& filename);

	private: 
	std::string dirname;
	DIR* getDirectory();
	DIR* getDirectory(std::string dirname);
        struct dirent* getEntry();
        struct dirent* getEntry(std::string dirname);	 
    };
}
