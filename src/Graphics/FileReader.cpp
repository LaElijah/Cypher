#include <iostream> 
#include <filesystem>
#include <algorithm>
#include "FileReader.h"


std::string Graphics::FileReader::getFileName(std::string path)
{
    const std::filesystem::path Path{path};
    return Path.filename().string();
}

std::string Graphics::FileReader::getDirectory(std::string path, bool full)
{
    const std::filesystem::path Path{path};
    if (full) 
        return Path.parent_path().string();

    return Path.parent_path().filename().string();
}


std::pair<std::string, std::string> Graphics::FileReader::getFileExtension(std::string path)
{
    const std::filesystem::path Path{path};

    return {Path.stem().string(), Path.extension().string()};
}

std::pair<bool, std::string> Graphics::FileReader::getFile
(
    std::string path, 
    std::string extension
)
{
 
    for (const auto& file : std::filesystem::directory_iterator(path))
    {
       if 
       (
           std::filesystem::is_regular_file
	       (std::filesystem::status(file))
       )
           if 
	   (
	       extension == getFileExtension
	       (
	           file.path().string()
	       ).second
	   )
	       return {true, file.path().string()}; 
    }	    

    return {false, ""};
}



std::vector<std::string> Graphics::FileReader::getFiles
(
    std::string path, 
    std::vector<std::string> extensions
)
{
    std::vector<std::string> files;

    for (const auto& file : std::filesystem::directory_iterator(path))
    {
       if 
       (
           std::filesystem::is_regular_file
	       (std::filesystem::status(path))
       )
           if 
	   (
	       extensions.size() != 0 && 
	       std::find
	           (
		       extensions.begin(), 
		       extensions.end(), 
	               getFileExtension(path).second
		   ) 
		   != extensions.end()
	   ) 
	       files.push_back(file.path().string()); 
    }	    

    return files;
}



std::vector<std::string> Graphics::FileReader::getFolders(std::string path)
{
    std::vector<std::string> files;

    for (const auto& file : std::filesystem::directory_iterator(path))
    {
       if (std::filesystem::is_directory(std::filesystem::status(path)))
           files.push_back(file.path().string()); 
    }	    

    return files;
}



