#ifndef BUFFER_H
#define BUFFER_H
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <iostream>
#include <memory> 

#include <functional>
 








namespace Graphics
{

struct BufferRecord
{
    BufferRecord
    (
        int size = 0,
        std::function<void(BufferRecord)> callback = nullptr,
        int start = 0,
	std::string owner = ""
    )
    : size(size),
      cleanup(callback),
      start(start),
      owner(owner)
    {
        split = 0;
 
        if (size == 0)
            active = false;
    }
 
    int start;
    int size;
    int split;
    bool active = true;
    std::string owner;
 
    std::function<void(BufferRecord)> cleanup;
 
    ~BufferRecord()
    {
        if (cleanup && active)
        {
            active = false;
            cleanup(BufferRecord(size, nullptr, start));
        }
    }
 
    std::string toString()
    {
        return "START: " + std::to_string(start)
             + " | SIZE: "   + std::to_string(size)
             + " | SPLIT: "  + std::to_string(split)
             + " | ACTIVE: " + std::to_string(active);
    }
};
 
	    
 
class Buffer
{
    public:
        Buffer(int size, std::string label) : size(size), name(label)
        {
            tail = 0;
            this->cleanup = [this](auto deleted)
            {
                for (int i = records.size() - 1; i >= 0; i--)
                {
                    if (records[i].expired())
		    {
                        freeSpaces.push_back
			(
                            std::make_shared<BufferRecord>(deleted.size, nullptr, deleted.start, name)
			);

                        records.erase(records.begin() + i);
			break;
		    }
                }
            };
        }

	    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(Buffer&&) = delete;
 

	void clear()
	{

	    records.clear();
	    freeSpaces.clear();
	    tail = 0;
	}

        std::shared_ptr<BufferRecord> allocate(int byteSize)
        {
            if (byteSize > size)
                return nullptr;
 
            // Fresh buffer
            if (freeSpaces.empty() && records.empty())
            {
                auto record = std::make_shared<BufferRecord>(byteSize, cleanup, tail, name);
                records.push_back(record);   // store as weak_ptr
                tail += byteSize;
                return record;
            }
 
            // Try to fit into an existing free space
            for (int i = 0; i < freeSpaces.size(); i++)
            {
                if (freeSpaces[i]->size >= byteSize)
                {
                    auto record = std::make_shared<BufferRecord>(
                        byteSize, cleanup, freeSpaces[i]->start, name);
 
                    freeSpaces[i]->size  -= byteSize;
                    freeSpaces[i]->start += byteSize;
                    ++freeSpaces[i]->split;
 
                    if (freeSpaces[i]->size == 0)
                        freeSpaces.erase(freeSpaces.begin() + i);
 
                    records.push_back(record);   // store as weak_ptr
                    return record;
                }
            }
 
            // Try the unallocated tail
            if ((size - tail) >= byteSize)
            {
                auto record = std::make_shared<BufferRecord>(byteSize, cleanup, tail, name);
                records.push_back(record);   // store as weak_ptr
                tail += byteSize;
                return record;
            }
 
            return nullptr;
        }
 
        bool free(std::shared_ptr<BufferRecord> record)
        {
            if (!record)
                return false;
 
            for (int i = 0; i < (int)records.size(); i++)
            {
                if (records[i].lock() == record)
                {
                    freeSpaces.push_back(
                        std::make_shared<BufferRecord>(record->size, nullptr, record->start, name));
                    records.erase(records.begin() + i);
                    return true;
                }
            }
            return false;
        }
 
        std::vector<std::weak_ptr<BufferRecord>> getRecords()
        {
            return records;
        }
 
        std::vector<std::shared_ptr<BufferRecord>> getFreeSpaces()
        {
            return freeSpaces;
        }
 
        std::function<void(BufferRecord)> cleanup;

    private:
        std::vector<std::weak_ptr<BufferRecord>>   records;     // non-owning
        std::vector<std::shared_ptr<BufferRecord>> freeSpaces;  // buffer-owned
 
        int size;
        int tail;
	std::string name;
};


}

#endif
