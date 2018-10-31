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

void Mustache::render(Reader &reader, JsonObject& json, char *startTag, char *endTag, std::function<void(char)> out) {
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

//            JsonVariant sectionValue = json.get<JsonVariant&>(buffer);
//
//            if(sectionValue.is<JsonArray>()) {
//              sectionValue.as<JsonArray&>();
//              for(int i = 0; i < ...size(); i++) {
//                reader.seek(bufferPos);
//                render(reader, json, startTag, endTag, out);
//              }
//            } else if(sectionValue.is<JsonObject>()) {
//              sectionValue.as<JsonObject&>();
//                render(reader, json, startTag, endTag, out);
//            } else if(sectionValue.is<bool>()) {
//              if(sectionValue.as<bool>()) {
//                render(reader, json, startTag, endTag, out);
//              } else {
//                render(reader, json, startTag, endTag, [](char c){});
//              }
//            } else {
//            }

            // Iterate {
            reader.seek(bufferPos);
            render(reader, json, startTag, endTag, [](char c){});
            // }

            break;
          case '>': // Partials
            break;
          case '=': // Set Delimiter
            break;
          case '/': // End of Section
            return;
          default:
            const char *value = json.get<const char*>(buffer);
            for(int i = 0; i <= strlen(value); i++) {
              out(value[i]);
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
