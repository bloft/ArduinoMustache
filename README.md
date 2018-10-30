# ArduinoMustache
Mustache for Arduino and similar boards

This libread is still a work in progress

## Examble usage
```C++
Mustache::render("Hallo {{user}}", [](char c){
  std::cout << c;
});
```
