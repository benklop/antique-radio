//
//  menu.hpp
//  antique-radio
//
//  Created by <author> on 29/11/2018.
//
//

#ifndef menu_hpp
#define menu_hpp

#include <stdio.h>
#include "yaml-cpp/yaml.h"

class menu {
 private:
    YAML::Node menu_map;
 public:
    menu();
    ~menu();
 protected:
};


#endif /* menu_hpp */
