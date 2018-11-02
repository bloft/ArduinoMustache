#pragma once
#include <functional>
#include <ArduinoJson.h>

class Reader {
  public:
    virtual char read() {};
    virtual bool seek(int pos) {};
    virtual bool isEnd() { return true; };
    virtual int getPos() {};
};

class StringReader : public Reader {
  protected:
    int pos = 0;
    const char *data;

  public:
    StringReader(const char *data) {
      this->data = data;
    }

    virtual char read() {
      return data[this->pos++];
    }

    virtual bool seek(int pos) {
      this->pos = pos;
    }

    virtual bool isEnd() {
      return this->pos >= strlen(this->data);
    }

    virtual int getPos() {
      return this->pos;
    }
};

class Mustache {
  public:
    static void render(const char *mustache, JsonObject& json, std::function<void(char)> out) {
      StringReader reader(mustache);
      render(reader, json, (char *)"{{", (char *)"}}", out);
    }

    static void render(Reader &reader, JsonObject& json, std::function<void(char)> out) {
      render(reader, json, (char *)"{{", (char *)"}}", out);
    }

    static void render(Reader &reader, JsonObject& json, char *startTag, char *endTag, std::function<void(char)> out);
};
