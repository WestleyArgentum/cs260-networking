#include <stdio.h>
#include <stdlib.h>
#include "splitter.h"

// I Define the max buffer size equal to 4069 bytes.
#define MAX_SIZE 4096

Data::~Data()
{
  std::vector<char>* temp;
  unsigned chunk_count = chunks.size();
  while(chunk_count)
  {
    temp = &chunks[chunk_count++];
    chunks.pop_back();
    delete temp;
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
int Data::SplitFile(size_t size_)
{
  size = size_;
  // The created files name buffer.
  char current_file_name[256];

  // Create a pointer to the parent file and opens it.
  FILE *fp_parent = fopen(filename, "rb");
  // If the parent file wasnt open free your memory and return
  //    bad source file.
  if(!fp_parent)
    return E_BAD_SOURCE;

  // Buffer to move data from the parent to the different children.
  std::vector<char>* buffer;
  buffer->reserve(size);
  // If the buffer wasnt created return memory wasnt found.
  //if(buffer == NULL)
  //  return E_NO_MEMORY;

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

    chunks.push_back(*buffer);

    // Until the child size is equal to the size the user requests or until it
    //    hits the end of the parent file, fill the buffer and transfer data.
    for(size_t child_size = temp; child_size < size && (!feof(fp_parent));)
    {
      if(child_size + MAX_SIZE > size)
        temp = fread(buffer, sizeof(char), sizeof(char) * size - child_size, fp_parent);
      else
        temp = fread(buffer, sizeof(char), sizeof(char) * MAX_SIZE, fp_parent);

      // Writes the buffer into the child currently opened child file.
      chunks.push_back(*buffer);

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
int Data::JoinFiles(char* filename_)
{
  filename = filename_;

  // Create a pointer to the parent file and opens it.
  FILE *fp_parent = fopen(filename, "wb");

  // If the parent file wasnt open free your memory and return
  //   bad source file.
  if(!fp_parent)
    return E_BAD_SOURCE;

  // Until I have seen every inputed file go through them all and take there data.
  for(unsigned i = 0; i < chunks.size(); ++i)
  {
    char* temp = new char(chunks[i].size());
    for(unsigned j = 0; j < chunks[i].size(); ++j)
      temp[j] = chunks[i][j];
    fwrite(temp, sizeof(char), size, fp_parent);
    delete [] temp;
  }

  /*! Close the parent file, free the buffer, and return home. */
  fclose(fp_parent);
  return 0;
}
std::vector<char> Data::GetChunk(unsigned chunk)
{
  return chunks[chunk];
}
void Data::SetChunk(std::vector<char>& data, unsigned chunk)
{
  if(chunks.size() < chunk)
    chunks.resize(chunk);  // <-- once we reach the initial size we will make a new copy of the whole file every time we get a new packet
  chunks[chunk] = data;
}