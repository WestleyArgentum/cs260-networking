#ifndef SPLITTER_H
#define SPLITTER_H

#include <vector>

enum
{E_BAD_SOURCE=-1, E_BAD_DESTINATION=-2, E_NO_MEMORY=-3, E_NO_ACTION=-4, E_SMALL_SIZE=-5};

class Data
{
  public:
    Data(char*  filename_): filename(filename_){};
    Data(std::vector<char*> chunks_, size_t size_): chunks(chunks_), size(size_){};
    ~Data();
    int SplitFile(size_t size_);
    int JoinFiles(char* filename_);
    char* GetChunk(unsigned chunk);
    void SetChunk(char* data, unsigned chunk);
  private:
    char*  filename;
    std::vector<char*> chunks;
    size_t size;
};

#endif
