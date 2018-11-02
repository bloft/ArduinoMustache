#pragma once
#include "Mustache.h"
#include <Arduino.h>
#include "FS.h"

class PgmReader : public StringReader {
  public:
    PgmReader(const char * data) : StringReader(data) {}

    virtual char read() {
      return pgm_read_byte_near(data + pos++);
    }

    virtual bool isEnd() {
      return this->pos >= strlen_P(this->data);
    }
};

class FSReader : public Reader {
  private:
    File f;

  public:
    FSReader(const char *filename) {
      f = SPIFFS.open(filename, "r");
    }

    virtual char read() {
      return char(f.read());
    };

    virtual bool seek(int pos) {
      f.seek(pos, SeekSet);
    };

    virtual int getPos() {
      return f.position();
    };

    virtual bool isEnd() {
      return f.available();
    };

    void close() {
      f.close();
    };
};

class MustacheEsp : public Mustache {
  public:
    static void render_P(const char *mustache, JsonObject& json, std::function<void(char)> out) {
      PgmReader reader(mustache);
      render(reader, json, (char *)"{{", (char *)"}}", out);
    }

    static void render_F(const char *filename, JsonObject& json, std::function<void(char)> out) {
      FSReader reader(filename);
      render(reader, json, (char *)"{{", (char *)"}}", out);
      reader.close();
    }
};
