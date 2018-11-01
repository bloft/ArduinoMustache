# ArduinoMustache
Mustache for Arduino and similar boards

This libread is still a work in progress

## Examble usage
```C++

StaticJsonBuffer<1000> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
root["user"] = "World";

Mustache::render("Hello {{user}}", [](char c){
  std::cout << c;
});
```
