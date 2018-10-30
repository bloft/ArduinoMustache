#include <iostream>
#include <cstring>
#include "mustache.h"

int main () {
  const char *mTemplate = "Hallo {{value}} {{#test}}123{{/test}}!!!";

  StringReader reader(mTemplate);

  Mustache::render(mTemplate, [](char c){
    std::cout << c;
  });

  return 0;
}

