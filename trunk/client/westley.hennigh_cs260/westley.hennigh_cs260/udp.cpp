#include "udp.h"
#include "stdio.h"
#include <cstring>

void UDP::write(void)
{
  char pub[] = "p:Test.txt";

  FILE* in = fopen(file, "rb");
  FILE* out = fopen(pub, "wb" );

  fseek(in, 0, SEEK_END);
  long Size = ftell(in);

  char* buffer = new char[Size];
  
  fseek(in, 0, SEEK_SET);

  if(in == NULL || out == NULL)
  {
      perror( "An error occured while opening files!!!" );
      return;
  }

  fread(buffer, Size, sizeof(char), in);
  fwrite(buffer, Size, sizeof(char), out);

  delete [] buffer;
  fclose(in);
  fclose(out);
}
void UDP::read(void)
{
  char pub[] = "p:Test.txt";

  FILE* in = fopen(pub, "rb");
  FILE* out = fopen(file, "wb" );

  fseek(in, 0, SEEK_END);
  long Size = ftell(in);

  char* buffer = new char[Size];

  fseek(in, 0, SEEK_SET);

  if(in == NULL || out == NULL)
  {
      perror( "An error occured while opening files!!!" );
      return;
  }

  size_t len = 0;
  while((len = fread(buffer, Size, sizeof(char), in)) > 0 )
      fwrite(buffer, Size, sizeof(char), out);

  remove(pub);

  delete [] buffer;
  fclose(in);
  fclose(out);
}
