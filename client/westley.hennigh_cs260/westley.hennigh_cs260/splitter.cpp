#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "splitter.h"

Data::~Data()
{
  std::vector<char>* temp;
  for(unsigned i = 0; i < chunks.size(); ++i)
  {
    chunks[i].clear();
  }
  chunks.clear();
}
/******************************************************************************/
/*
\name SpliteFile
\fn int SplitFile(size_t size)
\brief Splits in inputed parent file into smaller child files.
\param size - The size of the outputed files (in bytes).
\return int - error or chuncks if successful.
*/
/******************************************************************************/
int Data::SplitFile(void)
{
  // The created files name buffer.
  char current_file_name[256];

  // Create a pointer to the parent file and opens it.
  FILE *fp_parent = fopen(filename.c_str(), "rb");
  // If the parent file wasnt open free your memory and return
  //    bad source file.
  if(!fp_parent)
    return E_BAD_SOURCE;

  // Buffer to move data from the parent to the different children.
  std::vector<char> vecbuf;
  vecbuf.reserve(MAX_SIZE);

  char buffer[MAX_SIZE];

  // While the parent file isnt at the end of the file keep in the to break
  //    up the parent file into child files.
  size_t temp;
  while(!feof(fp_parent))
  {
    // Do the first check before creating a file, if not preformed a file of
    //    0 bytes will be created on evenly split files.
    if(size < MAX_SIZE)
    {
      temp = fread(buffer, sizeof(char), sizeof(char) * size, fp_parent);
    }
    else
    {
      temp = fread(buffer, sizeof(char), sizeof(char) * MAX_SIZE, fp_parent);
    }

    // If nothing is loaded on the first fread break out of the loop.
    if(!temp)
      break;
    for(unsigned i = 0; i<temp; ++i)
      vecbuf.push_back(buffer[i]);

    chunks.push_back(vecbuf);

    // Until the child size is equal to the size the user requests or until it
    //    hits the end of the parent file, fill the buffer and transfer data.
    for(size_t child_size = temp; child_size < size && (!feof(fp_parent));)
    {
      vecbuf.clear();
      if(child_size + MAX_SIZE > size)
        temp = fread(buffer, sizeof(char), sizeof(char) * size - child_size, fp_parent);
      else
        temp = fread(buffer, sizeof(char), sizeof(char) * MAX_SIZE, fp_parent);

      // Writes the buffer into the child currently opened child file.
      for(unsigned i = 0; i<temp; ++i)
        vecbuf.push_back(buffer[i]);

      chunks.push_back(vecbuf);

      // Keeps track of the child size.
      child_size += temp;
    }
  }
  // Close the parent file, free the buffer, and return home.
  fclose(fp_parent);
  return 0;
}

/******************************************************************************/
/*
\name JoinFiles
\fn int JoinFiles(char** filenames, char* output)
\brief Joins all the inputed child files into one larger parent file.
\return int - Based on how the function ran, note 0 means it ran successfully.
*/
/******************************************************************************/
int Data::JoinFiles(void)
{
  // Create a pointer to the parent file and opens it.
  FILE *fp_parent = fopen(filename.c_str(), "wb");

  // If the parent file wasnt open free your memory and return
  //   bad source file.
  if(!fp_parent)
    return E_BAD_SOURCE;

  // Until I have seen every inputed file go through them all and take there data.
  for(unsigned i = 0; i < chunks.size(); ++i)
  {
    //char temp[MAX_SIZE];
    //for(unsigned j = 0; j < MAX_SIZE; ++j)
    //  temp[j] = chunks[i][j];
    fwrite(&chunks[i][0], sizeof(char), chunks[i].size(), fp_parent);
  }

  /*! Close the parent file, free the buffer, and return home. */
  fclose(fp_parent);
  return 0;
}
std::vector<char> Data::GetChunk(unsigned chunk)
{
  if(chunk >= chunks.size())
    return chunks[0];
  return chunks[chunk];
}
void Data::SetChunk(std::vector<char>& data, unsigned chunk)
{
  chunks.push_back(data);
}
unsigned Data::GetSize(void)
{
  return (ceil((float)size/(float)4096));
}
unsigned Data::GetChunkSize(void)
{
  return chunks.size();
}
void Data::ResizeChunk(unsigned size_)
{
  chunks.resize(size_);
}