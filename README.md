# QLL : Level loaders

## What is it?
QLL is a collection of libs aimed to game developers / enthusiasts who want to use existing game files 

* Game developers who don't want the hassle of creating a new file format for their game levels
* People who want to bring existing game levels to new purposes (by creating viewers for example)

We are mainly targeting game levels which formats are well documented and open-source (to prevent us / library users of legal issues with file format)

## A word about Licence
Those loaders are published under the WTFPLv2

If you don't understand, think its kind alike public domain / CC0 (basically that means that you can do anything you want)

## Loaders
### QLL_Q3: Quake3 bsp loader  

This loader let you open Quake3 maps (or any game that uses IBSP version 0x02 format)
All lumps can be retrived 

Example usage:
```cpp
#define QLL_Q3_IMPLEMENTATION // Do this define only in one CPP file
#include "qll_q3.h"

int main(int argc, char** argv)
{
    // In this example, we consider that a map called "my_map.bsp" is in the application folder
    qll::q3::Q3Level level = new qll::q3::Q3Level("my_map.bsp");
    
    const qll::q3::LevelData level_data = level.getData();
    
    // Example: we fetch entities key/values
    // Each map contained in the vector represents one entity with its key/values (all as string)
    std::vector<std::map<std::string, std::string>> entities = qll::q3::parse_entities(level_data.entities);
    
    // Example game code to load entities
    MyGame::EntityList::loadFromQ3(entities, level.models);
    
    // Another example: Extract all level textures names
    for (unsigned int i = 0; i < level_data.textures.size(); ++i)
	{
	    const qll::q3::Texture& texture_raw = level_data.textures[i];
	    MyGame::SomeTextureLoader::loadTexture(texture_raw.name);
    }
    
    // You can fetch many other data (like Models, Brushes, Faces and so on), just take a look at qll::q3::LevelData definition
    // But the data types are very similar to the ones used in http://www.mralligator.com/q3/

    return 0;
}
```
You can also extend the file to use your own containers / file classes, just take a look at the first `#defines`

## TODO
* More game loaders

## Disclaimer
You will solely be responsible if you use this library to load copyrighted content (ie. official game assets)

## Links

* Q3 loader is based on "Unofficial Quake 3 Map Specs" by Kekoa Proudfoot at http://www.mralligator.com/q3/

Feel free to ask in issues if you have any problems / suggestions
