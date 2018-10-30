#include "mustache.h"
#include <cstring>

StringReader::StringReader(const char *data) {
  this->data = data;
}

char StringReader::read() {
  return data[this->pos++];
}

bool StringReader::seek(int pos) {
  this->pos = pos;
}

bool StringReader::isEnd() {
  return this->pos >= strlen(this->data);
}

int StringReader::getPos() {
  return this->pos;
}

void Mustache::render(Reader &reader, std::function<void(char)> out, char *startTag, char *endTag) {
  int bufferPos = 0;
  char buffer[10];
  while(!reader.isEnd()) {
    buffer[bufferPos] = reader.read();

    if(buffer[bufferPos] == startTag[bufferPos]) {
      bufferPos++;
      if(strlen(startTag) <= bufferPos) {
        bufferPos = 0;
        // Found start Tag
        bool endFound = false;
        while(!reader.isEnd() && !endFound) {
          buffer[bufferPos++] = reader.read();
          buffer[bufferPos] = '\0';
          endFound = bufferPos >= strlen(endTag) && strstr(buffer, endTag) != NULL;
        }
        bufferPos -= strlen(endTag);
        buffer[bufferPos] = '\0';
        
        switch(buffer[0]) {
          case '#': // Sections
          case '^': // Inverted Sections
            for(int i = 1; i <= bufferPos; i++) {
              buffer[i-1] = buffer[i];
            }
            bufferPos = reader.getPos(); // Store old pos to be able to return before every recursive call

            // Iterate {
            reader.seek(bufferPos);
            render(reader, [](char c){}, startTag, endTag);
            // }

            break;
          case '>': // Partials
            break;
          case '=': // Set Delimiter
            break;
          case '/': // End of Section
            return;
          default:
            for(int i = 0; i <= bufferPos; i++) {
              out(buffer[i]);
            }
        }
        bufferPos = 0;
      }
    } else {
      for(int i = 0; i <= bufferPos; i++) {
        out(buffer[i]);
      }
    }
  }
}
