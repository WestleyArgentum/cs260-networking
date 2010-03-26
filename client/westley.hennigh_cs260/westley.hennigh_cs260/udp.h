#ifndef UDP_H
#define UDP_H

class UDP // Universal Digipen Public drive
{
  public:
    UDP(char* file_): file(file_){};
    void write(void);
    void read(void);
  private:
    char* file;
};

#endif
