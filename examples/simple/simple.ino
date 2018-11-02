#include <Mustache.h>

void setup() {
  const char *mTemplate = "Hello {{user}}";

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["user"] = "World";

  Mustache::render(mTemplate, root, [](char c){
    std::cout << c; 
  });
  std::cout << "\n";
}

void loop() {
}
