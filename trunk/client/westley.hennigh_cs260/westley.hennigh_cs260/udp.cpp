#include "udp.h"
#include "stdio.h"
#include <cstring>

void UDP::write(void)
{
  char pub[] = "p:";
  strcat(pub, file);

  FILE* in = fopen(file, "rb");
  FILE* out = fopen(pub, "wb" );
  fseek(in, 0, SEEK_END);

  char* buffer = new char[ftell(in)];
  
  fseek(in, 0, SEEK_SET);

  if(in == NULL || out == NULL)
  {
      perror( "An error occured while opening files!!!" );
      return;
  }

  size_t len = 0;
  while((len = fread(buffer, BUFSIZ, sizeof(char), in)) > 0 )
      fwrite(buffer, BUFSIZ, sizeof(char), out);

  delete [] buffer;
  fclose(in);
  fclose(out);
}
void UDP::read(void)
{
  char pub[] = "p:";
  strcat(pub, file);

  FILE* in = fopen(pub, "rb");
  FILE* out = fopen(file, "wb" );
  fseek(in, 0, SEEK_END);

  char* buffer = new char[ftell(in)];

  fseek(in, 0, SEEK_SET);

  if(in == NULL || out == NULL)
  {
      perror( "An error occured while opening files!!!" );
      return;
  }

  size_t len = 0;
  while((len = fread(buffer, BUFSIZ, sizeof(char), in)) > 0 )
      fwrite(buffer, BUFSIZ, sizeof(char), out);

  delete [] buffer;
  fclose(in);
  fclose(out);
}
