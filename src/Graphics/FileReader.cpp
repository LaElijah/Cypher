#include "FileReader.h"
#include <dirent.h>
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>

std::string Graphics::FileReader::getNameFromDirectory(std::string &directory)
{
    return directory.substr(directory.find_last_of("/\\") + 1);
}

std::pair<std::string, std::string> Graphics::FileReader::splitFileExtension(std::string filename)
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
Graphics::FileReader::FileReader(const char *dir)
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

// TODO: Rename to getDirectoryObject
DIR* Graphics::FileReader::getDirectory()
{
    DIR *dir = opendir((getDirname()).c_str());

    // If no directory at this location was found 
    if (dir == NULL)
    {
        throw std::runtime_error(std::string("ERROR: FAILED TO LOAD DIRECTORY ") + getDirname());
    }

    return dir;
}

// TODO: Rename to findDirectoryObject
DIR *Graphics::FileReader::getDirectory(std::string dirname)
{
    DIR *dir = opendir(dirname.c_str());

    if (dir == NULL)
    {
        throw std::runtime_error("ERROR: FAILED TO LOAD DIRECTORY " + dirname);
    }

    else
    {
        return dir;
    }
}

struct dirent *Graphics::FileReader::getEntry()
{
    try
    {
        DIR *dir = getDirectory();
        struct dirent* entity = readdir(dir);
        return entity;
    }

    catch (std::runtime_error error)
    {
        std::cout << "RUNTIME ERROR- " << error.what() << std::endl;
        return NULL;
    }
}

struct dirent *Graphics::FileReader::getEntry(std::string dirname)
{
    try
    {
        DIR *dir = getDirectory(dirname.c_str());
        struct dirent *entity = readdir(dir);
        return entity;
    }

    catch (std::runtime_error error)
    {
        std::cout << "RUNTIME ERROR- " << error.what() << std::endl;
        return NULL;
    }
}
#include <sys/stat.h>

std::vector<std::string> Graphics::FileReader::getFolders()
{
    std::vector<std::string> folders;

    bool isEntries = false;
    DIR *dir = getDirectory();
    struct dirent *entry = getEntry();
    struct stat entry_stat;
    entry = readdir(dir);

    while (entry != NULL)
    {
        if (std::string(entry->d_name) == "." || std::string(entry->d_name) == "..") {
            continue;
        }

        std::string fullPath = getDirname() + "/" + entry->d_name;
        if (stat(fullPath.c_str(), &entry_stat) != 0)
        {
            //std::cout << "FOLDER FOUND: " + std::string(entry->d_name) << std::endl;
            isEntries = true;
            folders.push_back(std::string(entry->d_name));
            entry = readdir(dir);
        }
    }

    if (entry == NULL && isEntries)
    {
        //std::cout << "FOLDERS PRINTED" << std::endl;
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
    DIR *dir = getDirectory();
    struct dirent *entry = getEntry();
    entry = readdir(dir);

    std::string entityName;

    while (entry != NULL)
    {
        entityName = entry->d_name;

        if (entityName != "." && entityName != "..")
        {
            //std::cout << "FILE FOUND: " + std::string(entry->d_name) << " >IS REL: " << (entityName != "." && entityName != "..") << std::endl;

            isEntries = true;
            files.push_back(entityName);
        }

        entry = readdir(dir);
    }

    if (entry == NULL && isEntries)
    {
        //std::cout << "FILES PRINTED" << std::endl;
    }

    else if (entry == NULL && !isEntries)
    {
        std::cout << "NO FILES FOUND" << std::endl;
    }

    return files;
}

// TODO add regex or maybe split at the last "." in the string
std::string Graphics::FileReader::getFile()
{
    std::vector<std::string> file;

    bool isEntries = false;
    DIR *dir = getDirectory();
    struct dirent *entry = getEntry();
    entry = readdir(dir);

    std::string entityName;

    if (entry != NULL)
    {

        entityName = entry->d_name;

        if (entityName != "." && entityName != "..")
        {
            //std::cout << "FILE FOUND: " + std::string(entry->d_name) << " >IS REL: " << (entityName != "." && entityName != "..") << std::endl;

            isEntries = true;
            file.push_back(entityName);
        }

        entry = readdir(dir);
    }

    if (entry == NULL && isEntries)
    {
        //std::cout << "FILE PRINTED" << std::endl;
    }

    else if (entry == NULL && !isEntries)
    {
        //std::cout << "NO FILE FOUND" << std::endl;
    }

    return file.front();
}

std::string Graphics::FileReader::getFile(std::string extension)
{
    std::vector<std::string> file;

    bool isEntries = false;
    DIR *dir = getDirectory();
    struct dirent *entry = getEntry();
    entry = readdir(dir);

    std::string entityName;

    while (entry != NULL)
    {

        entityName = entry->d_name;

        if (entityName != "." && entityName != "..")
        {
            std::pair<std::string, std::string> fileInfo = FileReader::splitFileExtension(entityName);

            std::string fileExtension = fileInfo.second;
            std::string name = fileInfo.first;

            if (fileExtension == extension)
            {

                //std::cout << "FILE FOUND: " + std::string(entry->d_name) << " >IS REL: " << (entityName != "." && entityName != "..") << std::endl;

                isEntries = true;
                file.push_back(entityName);
            }
        }

        entry = readdir(dir);
    }

    if (isEntries)
    {
        //std::cout << "FILE PRINTED" << std::endl;
    }

    else if (!isEntries)
    {
        std::cout << "NO FILE FOUND" << std::endl;
    }

    return file.front();
}

// TODO add regex or maybe split at the last "." in the string
std::vector<std::string> Graphics::FileReader::getFiles(std::string extension)
{
    std::vector<std::string> files;

    bool isEntries = false;
    DIR *dir = getDirectory();
    struct dirent *entry = getEntry();
    entry = readdir(dir);

    std::string entityName;

    while (entry != NULL)
    {

        entityName = entry->d_name;
        std::pair<std::string, std::string> fileInfo = FileReader::splitFileExtension(entityName);

        std::string fileExtension = fileInfo.second;
        std::string name = fileInfo.first;

        if (entityName != "." && entityName != ".." && fileExtension == extension)
        {
            //std::cout << "FILE FOUND: " + std::string(entry->d_name) << " >IS REL: " << (entityName != "." && entityName != "..") << std::endl;

            isEntries = true;
            files.push_back(entityName);
        }

        entry = readdir(dir);
    }

    if (entry == NULL && isEntries)
    {
        //std::cout << "FILES PRINTED" << std::endl;
    }

    else if (entry == NULL && !isEntries)
    {
        std::cout << "NO FILES FOUND" << std::endl;
    }

    return files;
}
