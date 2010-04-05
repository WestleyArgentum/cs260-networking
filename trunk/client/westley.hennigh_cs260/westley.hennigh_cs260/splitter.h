#ifndef SPLITTER_H
#define SPLITTER_H

#include <vector>

// I Define the max buf0fer size equal to 4069 bytes.
#define MAX_SIZE 4096

enum
{E_BAD_SOURCE=-1, E_BAD_DESTINATION=-2, E_NO_MEMORY=-3, E_NO_ACTION=-4, E_SMALL_SIZE=-5};

class Data
{
  public:
    Data(std::string filename_, unsigned filesize): filename(filename_), size(filesize){};
    Data(std::vector<std::vector<char>> chunks_, size_t size_): chunks(chunks_), size(size_){};
    ~Data();
    int SplitFile(size_t size_);
    int JoinFiles(std::string filename_);
    std::vector<char> GetChunk(unsigned chunk);
    void SetChunk(std::vector<char>& data, unsigned chunk);
    void ResizeChunk(unsigned size_);
    unsigned GetSize(void);
		std::string filename;

  private:
    std::vector<std::vector<char>> chunks;
    size_t size;
};

#endif
