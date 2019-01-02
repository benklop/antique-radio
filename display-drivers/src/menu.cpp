//
//  menu.cpp
//  antique-radio
//
//  Created by <author> on 29/11/2018.
//
//

#include "menu.hpp"

menu::menu()  {
    menu_map = YAML::LoadFile(reader.GetInteger("menu", "map", "menu.yaml"));
}
menu::~menu() {}
