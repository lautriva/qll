#include <iostream>

#define QLL_Q3_IMPLEMENTATION // Do this define only in one CPP file
#include "../qll_q3.h"

int main(int argc, char** argv)
{
    unsigned int count;
    
    // In this example, we consider that a map called "my_map.bsp" is in the application folder
    qll::q3::Q3Level level = qll::q3::Q3Level("data/test.bsp");
    
    const qll::q3::LevelData level_data = level.getData();
    
    // Example: we fetch entities key/values
    // Each map contained in the vector represents one entity with its key/values (all as string)
    std::vector<std::map<std::string, std::string>> entities = qll::q3::parse_entities(level_data.entities);
    
    count = entities.size();
    std::cout << "There are " << count << " entities:" << std::endl;
    
    for (unsigned int i = 0; i < count; ++i)
    {
        std::cout << "- " << (entities[i].count("classname") ? entities[i]["classname"] : "?") << " (" << i << ")" << std::endl;
        
        for (auto it : entities[i])
        {
            // Do not show again entity class name
            if (it.first != "classname")
                std::cout << "  - " << it.first << " => '"  << it.second << "'" << std::endl;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    
    // Extract all level textures names
    count = level_data.textures.size();
    std::cout << "There are " << count << " textures:" << std::endl;
    
    for (unsigned int i = 0; i < count; ++i)
    {
        const qll::q3::Texture& texture_raw = level_data.textures[i];
        std::cout << "- " << texture_raw.name << std::endl;
    }
    
    std::cout << std::endl;

    // For those next, only print the count
    std::cout << "There are " << level_data.models.size() << " models" << std::endl;
    std::cout << "There are " << level_data.faces.size() << " faces" << std::endl;
    std::cout << "There are " << level_data.brushes.size() << " brushes" << std::endl;

    return 0;
}