#include "Mustache.h"
#include <cstring>
#include <stdio.h>

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
            {
              bool invert = buffer[0] == '^';
            for(int i = 1; i <= bufferPos; i++) {
              buffer[i-1] = buffer[i];
            }
            bufferPos = reader.getPos(); // Store old pos to be able to return before every recursive call
            if(!json.containsKey(buffer)) {
              if(invert) {
                render(reader, json, startTag, endTag, out);
              } else {
                render(reader, json, startTag, endTag, [](char c){});
              }
            } else if(json[buffer].is<JsonArray>()) {
              if(invert && json[buffer].size() == 0) {
                render(reader, json, startTag, endTag, out);
              } else if(!invert && json[buffer].size() > 0) {
                for(unsigned int i = 0; i < json[buffer].size(); i++) {
                  reader.seek(bufferPos);
                  render(reader, json[buffer][i].as<JsonObject&>(), startTag, endTag, out);
                }
              } else {
                render(reader, json[buffer], startTag, endTag, [](char c){});
              }
            } else if(json[buffer].is<JsonObject>()) {
              if(invert) {
                render(reader, json[buffer], startTag, endTag, [](char c){});
              } else {
                render(reader, json[buffer], startTag, endTag, out);
              }
            } else if(json[buffer].is<bool>()) {
              if(invert ^ json[buffer].as<bool>()) {
                render(reader, json, startTag, endTag, out);
              } else {
                render(reader, json, startTag, endTag, [](char c){});
              }
            } else {
                // Error invalid data
                render(reader, json, startTag, endTag, [](char c){});
            }
            }
            break;
          case '>': // Partials
            break;
          case '=': // Set Delimiter
            break;
          case '/': // End of Section
            return;
          default:
            {
              if(json[buffer].is<const char*>()) {
                strcpy(buffer, json[buffer].as<const char*>());
              } else if(json[buffer].is<int>()) {
                sprintf(buffer, "%d", json[buffer].as<int>());
              } else if(json[buffer].is<float>()) {
                sprintf(buffer, "%f", json[buffer].as<float>());
              } else if(json[buffer].is<bool>()) {
                sprintf(buffer, "%s", json[buffer].as<bool>() ? "true" : "false");
              }
              for(int i = 0; i <= strlen(buffer); i++) {
                out(buffer[i]);
              }
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
