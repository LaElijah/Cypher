#include "FileReader.h"
#include <dirent.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>


std::pair<std::string, std::string> splitFilename(std::string& filename)
{
    size_t index = filename.find_last_of(".");
    
    if (index == std::string::npos)
    {
        return {filename, ""}; 
    }

    std::string name = filename.substr(0, index);
    std::string extension = filename.substr(index + 1);


    return {name, extension};
}



// Uses a string parameter to make a filereader object
// set to a given directory which can be relativley selected
Graphics::FileReader::FileReader(const char* dir)
{
    this->dirname = dir;
}

Graphics::FileReader::FileReader(std::string dir)
{
    this->dirname = dir.c_str();
}

std::string Graphics::FileReader::getDirname() 
{
    return (this->dirname);
}


void Graphics::FileReader::setDirname(std::string directory)
{
    this->dirname = directory;
}



DIR* Graphics::FileReader::getDirectory()
{
    DIR* dir = opendir((getDirname()).c_str());

    if (dir == NULL) 
    {
        throw std::runtime_error(std::string("ERROR: FAILED TO LOAD DIRECTORY ") + getDirname());
    }

    return dir; 
}



DIR* Graphics::FileReader::getDirectory(std::string dirname)
{
    DIR* dir = opendir(dirname.c_str());
    
    if (dir == NULL) 
    {
        throw std::runtime_error("ERROR: FAILED TO LOAD DIRECTORY " + dirname);
    }
   
    else 
    {
        return dir; 
    }
}


struct dirent* Graphics::FileReader::getEntry()
{
   try
   { 
        DIR* dir = getDirectory();
        struct dirent* entity = readdir(dir);
        return entity;
   }
   
   catch (std::runtime_error error)
   { 
       std::cout << "RUNTIME ERROR- " << error.what() << std::endl; 
       return NULL;
   }

}


struct dirent* Graphics::FileReader::getEntry(std::string dirname)
{
    try 
    {
        DIR* dir = getDirectory(dirname.c_str());
        struct dirent* entity = readdir(dir);
        return entity;
    }
   
    catch (std::runtime_error error)
    { 
        std::cout << "RUNTIME ERROR- " << error.what() << std::endl; 
	return NULL;
    }
}

std::vector<std::string> Graphics::FileReader::getFolders()
{ 
    std::vector<std::string> folders; 
   
    bool isEntries = false; 
    DIR* dir = getDirectory();
    struct dirent* entry = getEntry();
    entry = readdir(dir);

    while (entry != NULL)
    {    
	if (entry->d_type == DT_DIR)
        {
            std::cout << "FOLDER FOUND: " + std::string(entry->d_name) << std::endl;
	    isEntries = true;
	    folders.push_back(std::string(entry->d_name));	
            entry = readdir(dir);
	}
    }

    if (entry == NULL && isEntries)
    {
        std::cout << "FOLDERS PRINTED" << std::endl;
    }


    else if (entry == NULL && !isEntries)
    {
        std::cout << "NO FOLDERS FOUND" << std::endl; 
    }
 
    return folders;
}

std::vector<std::string> Graphics::FileReader::getFiles()
{
    std::vector<std::string> files; 
    
    bool isEntries = false; 
    DIR* dir = getDirectory();
    struct dirent* entry = getEntry();
    entry = readdir(dir);



    std::string entityName;


    while (entry != NULL)
    {  
	 
        entityName = entry->d_name;

	if (entityName != "." && entityName != "..")
        {
            std::cout << "FILE FOUND: " + std::string(entry->d_name) << " >IS REL: " << (entityName != "." && entityName != "..") << std::endl;

	    isEntries = true; 
	    files.push_back(entityName);
	}

        entry = readdir(dir);
    }


    if (entry == NULL && isEntries)
    {
        std::cout << "FILES PRINTED" << std::endl;
    }
    
    else if (entry == NULL && !isEntries)
    {
        std::cout << "NO FILES FOUND" << std::endl; 
    }

    return files;

   
}


// TODO add regex or maybe split at the last "." in the string 
std::vector<std::string> Graphics::FileReader::getFiles(std::string extension)
{
    std::vector<std::string> files; 
    
    bool isEntries = false; 
    DIR* dir = getDirectory();
    struct dirent* entry = getEntry();
    entry = readdir(dir);



    std::string entityName;


    while (entry != NULL)
    {  
	 
        entityName = entry->d_name;
        auto [name, fileExtension] = splitFilename(entityName);

	if (entityName != "." && entityName != ".." && fileExtension == extension)
        {
            std::cout << "FILE FOUND: " + std::string(entry->d_name) << " >IS REL: " << (entityName != "." && entityName != "..") << std::endl;
             
	    isEntries = true; 
	    files.push_back(entityName);
	}

        entry = readdir(dir);
    }



    if (entry == NULL && isEntries)
    {
        std::cout << "FILES PRINTED" << std::endl;
    }
    
    else if (entry == NULL && !isEntries)
    {
        std::cout << "NO FILES FOUND" << std::endl; 
    }

    return files;



}



