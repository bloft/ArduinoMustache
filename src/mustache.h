#pragma once
#include <functional>

class Reader {
  public:
    virtual char read() {};
    virtual bool seek(int pos) {};
    virtual bool isEnd() { return true; };
    virtual int getPos() {};
};

class StringReader : public Reader {
  private:
    int pos = 0;
    const char *data;

  public:
    StringReader(const char *);
    virtual char read();
    virtual bool seek(int pos);
    virtual bool isEnd();
    virtual int getPos();
};

// ToDo: add FS reader
// ToDo: add progmem reader - https://www.arduino.cc/reference/en/language/variables/utilities/progmem/

class Mustache {
  public:
    static void render(const char *mustache, std::function<void(char)> out) {
      StringReader reader(mustache);
      render(reader, out, (char *)"{{", (char *)"}}");
    }

    static void render(Reader &reader, std::function<void(char)> out) {
      render(reader, out, (char *)"{{", (char *)"}}");
    }

    static void render(Reader &reader, std::function<void(char)> out, char *startTag, char *endTag);
};
