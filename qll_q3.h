/**
 * QLL Level loaders by Lautrivad
 *
 * Q3 Bsp loader
 * Based on "Unofficial Quake 3 Map Specs" by Kekoa Proudfoot
 * http://www.mralligator.com/q3/
 *
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

#ifndef QLL_LOADERS_Q3_H
#define QLL_LOADERS_Q3_H

#ifndef QLL_Q3_STRING
    #include <string>
    #define QLL_Q3_STRING std::string
    #define QLL_Q3_STRING_LENGTH(VALUE) VALUE.length()
    #define QLL_Q3_STRING_GET_CHAR(VALUE, POS) VALUE[POS]
    #define QLL_Q3_STRING_ADD_CHAR(VALUE, CHAR) VALUE.push_back(CHAR)
    #define QLL_Q3_STRING_FROM_VALUE(VALUE) std::to_string(VALUE)
#endif

#ifndef QLL_Q3_ARRAY
    #include <vector>
    #define QLL_Q3_ARRAY(T) std::vector<T>
    #define QLL_Q3_ARRAY_APPEND(ARRAY, ITEM) ARRAY.push_back(ITEM)
    #define QLL_Q3_ARRAY_ACCESS(ARRAY, INDEX) ARRAY[INDEX]
#endif

#ifndef QLL_Q3_PREVENT_ENTITY_PARSER
    #define QLL_Q3_USE_ENTITY_PARSER
#endif

#ifdef QLL_Q3_USE_ENTITY_PARSER
    #ifndef QLL_Q3_ASSOCIATIVE_ARRAY
        #include <map>
        #define QLL_Q3_ASSOCIATIVE_ARRAY(KEY_TYPE, VALUE_TYPE) std::map<KEY_TYPE, VALUE_TYPE>
        #define QLL_Q3_ASSOCIATIVE_ARRAY_SET(ARRAY, KEY, VALUE) ARRAY.emplace(KEY, VALUE)
        #define QLL_Q3_ASSOCIATIVE_ARRAY_GET(ARRAY, KEY) ARRAY[KEY]
    #endif

    #ifndef QLL_Q3_LOG_ERROR
        #define QLL_Q3_LOG_ERROR(ERROR) throw std::runtime_error(ERROR)
    #endif
#endif

namespace qll { namespace q3 {
    typedef unsigned char q3_ubyte;
    typedef int32_t q3_int;
    typedef float q3_float;

    /**
     * The textures lump stores information about surfaces and volumes.
     */
    struct Texture
    {
        QLL_Q3_STRING name;        // Texture name
        q3_int flags;              // Surface flags
        q3_int contents;           // Surface contents
    };

    /**
     * The planes lump stores a generic set of planes that are in turn referenced by nodes and brushsides.
     */
    struct Plane
    {
        q3_float normal[3];        // Plane normal
        q3_float distance;         // Distance from origin to plane along normal
    };

    /**
     * The nodes lump stores all of the nodes in the map's BSP tree.
     */
    struct Node
    {
        q3_int plane;              // Plane index
        q3_int front;              // The child index for the front node
        q3_int back;               // The child index for the back node
        q3_int mins[3];            // Integer bounding box min coord
        q3_int maxs[3];            // Integer bounding box max coord
    };

    /**
     * The leafs lump stores the leaves of the map's BSP tree.
     */
    struct Leaf
    {
        q3_int cluster;            // Visdata cluster index
        q3_int area;               // Areaportal area
        q3_int mins[3];            // Integer bounding box min coord
        q3_int maxs[3];            // Integer bounding box max coord
        q3_int leafface;           // First leafface for leaf
        q3_int n_leaffaces;        // Number of leaffaces for leaf
        q3_int leafbrush;          // First leafbrush for leaf
        q3_int n_leafbrushes;      // Number of leafbrushes for leaf
    };

    /**
     * The leaffaces lump stores lists of face indices, with one list per leaf.
     */
    typedef q3_int Leafface;

    /**
     * The leafbrushes lump stores lists of brush indices, with one list per leaf
     */
    typedef q3_int Leafbrush;

    /**
     * The models lump describes rigid groups of world geometry.
     */
    struct Model
    {
        q3_float mins[3];          // Bounding box min coord
        q3_float maxs[3];          // Bounding box max coord
        q3_int face;               // First face for model
        q3_int n_faces;            // Number of faces for model
        q3_int brush;              // First brush for model
        q3_int n_brushes;          // Number of brushes for model
    };

    /**
     * The brushes lump stores a set of brushes, which are in turn used for collision detection.
     */
    struct Brush
    {
        q3_int brushside;          // First brushside for brush
        q3_int n_brushsides;       // Number of brushsides for brush
        q3_int texture;            // Texture index
    };

    /**
     * The brushsides lump stores descriptions of brush bounding surfaces.
     */
    struct Brushside
    {
        q3_int plane;              // Plane index.
        q3_int texture;            // Texture index.
    };

    /**
     * The vertexes lump stores lists of vertices used to describe faces.
     */
    struct Vertex
    {
        q3_float position[3];      // Vertex position
        q3_float tex_coord[2][2];  // Vertex texture coordinates. 0 = Surface, 1 = Lightmap
        q3_float normal[3];        // Vertex normal
        q3_ubyte color[4];         // Vertex color (RGBA)
    };

    typedef q3_int Meshvert;

    /**
     * The effects lump stores references to volumetric shaders (typically fog) which affect the rendering of a particular group of faces.
     */
    struct Effect
    {
        QLL_Q3_STRING name;        // Effect shader
        q3_int brush;              // Brush that generated this effect
        q3_int unknown;            // Always 5, except in q3dm8, which has one effect with -1
    };

    /**
     * The faces lump stores information used to render the surfaces of the map.
     */
    struct Face
    {
        q3_int texture;            // Texture index
        q3_int effect;             // Index into Effects lump 12, or -1
        q3_int type;               // Face type. 1 = Polygon, 2 = Patch, 3 = Mesh, 4 = Billboard
        q3_int vertex;             // Index of first vertex
        q3_int n_vertices;         // Number of vertices
        q3_int meshvert;           // Index of first meshvert
        q3_int n_meshverts;        // Number of meshverts
        q3_int lm_index;           // Lightmap index
        q3_int lm_start[2];        // Corner of this face's lightmap image in lightmap
        q3_int lm_size[2];         // Size of this face's lightmap image in lightmap
        q3_float lm_origin[3];     // World space origin of lightmap
        q3_float lm_vecs[2][3];    // World space lightmap s and t unit vectors
        q3_float normal[3];        // Surface normal
        q3_int patch_size[2];      // Patch dimensions
    };

    /**
     * The lightmaps lump stores the light map textures used make surface lighting look more realistic.
     */
    struct Lightmap
    {
        q3_ubyte data[128][128][3]; // Lightmap color data. RGB
    };

    /**
     * The lightvols lump stores a uniform grid of lighting information used to illuminate non-map objects.
     */
    struct Lightvol
    {
        q3_ubyte ambient[3];       // Ambient color component. RGB
        q3_ubyte directional[3];   // Directional color component. RGB
        q3_ubyte dir[2];           // Direction to light. 0=phi, 1=theta
    };

    /**
     * The visdata lump stores bit vectors that provide cluster-to-cluster visibility information.
     * Cluster x is visible from cluster y if the (1 << y % 8) bit of vecs[x * sz_vecs + y / 8] is set
     */
    struct Visdata
    {
        q3_int n_vecs;             // The number of clusters
        q3_int sz_vecs;            // Size of each vector, in bytes
        q3_ubyte* vecs;            // Array of bytes holding the cluster vis
    };

    struct LevelData
    {
        QLL_Q3_STRING entities;
        QLL_Q3_ARRAY(Texture) textures;
        QLL_Q3_ARRAY(Plane) planes;
        QLL_Q3_ARRAY(Node) nodes;
        QLL_Q3_ARRAY(Leaf) leaves;
        QLL_Q3_ARRAY(Leafface) leaf_faces;
        QLL_Q3_ARRAY(Leafbrush) leaf_brushes;
        QLL_Q3_ARRAY(Model) models;
        QLL_Q3_ARRAY(Brush) brushes;
        QLL_Q3_ARRAY(Brushside) brush_sides;
        QLL_Q3_ARRAY(Vertex) vertices;
        QLL_Q3_ARRAY(Meshvert) mesh_vertices;
        QLL_Q3_ARRAY(Effect) effects;
        QLL_Q3_ARRAY(Face) faces;
        QLL_Q3_ARRAY(Lightmap) light_maps;
        QLL_Q3_ARRAY(Lightvol) light_vols;
        Visdata vis_data;
    };

    #ifdef QLL_Q3_USE_ENTITY_PARSER
    QLL_Q3_ARRAY(QLL_Q3_ASSOCIATIVE_ARRAY(QLL_Q3_STRING, QLL_Q3_STRING)) parse_entities(const QLL_Q3_STRING& lump_data);
    #endif

    class Q3Level
    {
        public:
            Q3Level(const QLL_Q3_STRING& filename);
            ~Q3Level();

            /**
             * Get raw level data
             */
            const LevelData& getData() const { return _data; }

            /**
             * Check if the file is a valid bsp map
             */
            static bool isValid(const QLL_Q3_STRING& filename);
        protected:
            LevelData _data;
    };
}}

#endif

#ifdef QLL_Q3_IMPLEMENTATION

#ifndef QLL_Q3_FILE_TYPE
    #include <string>
    #define QLL_Q3_FILE_TYPE FILE*

    #define QLL_Q3_FILE_FOPEN(STR) fopen(STR.c_str(), "r+b")
    #define QLL_Q3_FILE_FCLOSE(HANDLE) fclose(HANDLE)

    #define QLL_Q3_FILE_FREAD(TARGET, COUNT, SIZE, HANDLE) fread(TARGET, SIZE, COUNT, HANDLE)

    #define QLL_Q3_FILE_FSEEK(HANDLE, OFFSET) fseek(HANDLE, OFFSET, SEEK_SET)
#endif

#ifndef QLL_Q3_CUSTOM_MEMALLOC
    #define QLL_Q3_MALLOC(SIZE) malloc(SIZE)
    #define QLL_Q3_FREE(BUFFER) free(BUFFER)
#endif

#define ENTITIES_LUMP         0x00
#define TEXTURES_LUMP         0x01
#define PLANES_LUMP           0x02
#define NODES_LUMP            0x03
#define LEAF_LUMP             0x04
#define LEAFFACES_LUMP        0x05
#define LEAFBRUSHES_LUMP      0x06
#define MODELS_LUMP           0x07
#define BRUSHES_LUMP          0x08
#define BRUSHSIDES_LUMP       0x09
#define VERTICES_LUMP         0x0A
#define MESHVERTS_LUMP        0x0B
#define EFFECTS_LUMP          0x0C
#define FACES_LUMP            0x0D
#define LIGHTMAPS_LUMP        0x0E
#define LIGHTVOLS_LUMP        0x0F
#define VISDATA_LUMP          0x10

namespace qll { namespace q3 {
    #define QUAKE3_BSP_MAGIC_LEN 4
    static const q3_ubyte __quake3_bsp_magic[] = "IBSP";
    static const q3_int __quake3_bsp_version = 0x2e; // 46

    static const q3_int __quake3_bsp_lumps_count = 17;

    bool Q3Level::isValid(const QLL_Q3_STRING& filename)
    {
        bool result = true;

        QLL_Q3_FILE_TYPE file_handle = QLL_Q3_FILE_FOPEN(filename);

        if (!file_handle)
            return false;

        if (result)
        {
            // Check the magic number
            q3_ubyte magic[QUAKE3_BSP_MAGIC_LEN];
            QLL_Q3_FILE_FREAD(magic, 1, QUAKE3_BSP_MAGIC_LEN, file_handle);

            for (q3_ubyte i = 0; i< QUAKE3_BSP_MAGIC_LEN; ++i)
            {
                if (magic[i] != __quake3_bsp_magic[i])
                {
                    result = false;
                    break;
                }
            }
        }

        if (result)
        {
            // Check if the bsp version is the Quake3 one
            q3_int version;
            QLL_Q3_FILE_FREAD(&version, 1, sizeof(q3_int), file_handle);

            if (version != __quake3_bsp_version)
                result = false;
        }

        if (file_handle)
            QLL_Q3_FILE_FCLOSE(file_handle);

        return result;
    }

    struct __lump_header
    {
        q3_int offset;
        q3_int length;
    };

    template <typename T> static void __read_lump
    (
        QLL_Q3_FILE_TYPE file_handle, QLL_Q3_ARRAY(T)& result, const __lump_header& header
    );

    // Special case for texture lump
    template <> void __read_lump(QLL_Q3_FILE_TYPE file_handle, QLL_Q3_ARRAY(Texture)& result, const __lump_header& header);

    // Special case for effect lump
    template <> void __read_lump(QLL_Q3_FILE_TYPE file_handle, QLL_Q3_ARRAY(Effect)& result, const __lump_header& header);

    // Entities / Visdata have their own special cases
    static void __read_entities_lump(QLL_Q3_FILE_TYPE file_handle, QLL_Q3_STRING& result, const __lump_header& header);
    static void __read_visdata_lump(QLL_Q3_FILE_TYPE file_handle, Visdata& result, const __lump_header& header);

    static void __read_string(QLL_Q3_FILE_TYPE file_handle, QLL_Q3_STRING& result, const q3_int& length);

    Q3Level::Q3Level(const QLL_Q3_STRING& filename)
    {
        if (!isValid(filename))
            return;

        QLL_Q3_FILE_TYPE file_handle = QLL_Q3_FILE_FOPEN(filename);

        // Read headers (Ignore magic + version)
        QLL_Q3_FILE_FSEEK(file_handle, QUAKE3_BSP_MAGIC_LEN + sizeof(q3_int));

        QLL_Q3_ARRAY(__lump_header) headers;

        for (q3_int i = 0; i<__quake3_bsp_lumps_count; ++i)
        {
            __lump_header item;
            QLL_Q3_FILE_FREAD(&item, 1, sizeof(__lump_header), file_handle);
            QLL_Q3_ARRAY_APPEND(headers, item);
        }

        // Read all lumpes
        __read_entities_lump(file_handle, _data.entities, headers[ENTITIES_LUMP]);
        __read_lump<Texture>(file_handle, _data.textures, headers[TEXTURES_LUMP]);
        __read_lump<Plane>(file_handle, _data.planes, headers[PLANES_LUMP]);
        __read_lump<Node>(file_handle, _data.nodes, headers[NODES_LUMP]);
        __read_lump<Leaf>(file_handle, _data.leaves, headers[LEAF_LUMP]);
        __read_lump<Leafface>(file_handle, _data.leaf_faces, headers[LEAFFACES_LUMP]);
        __read_lump<Leafbrush>(file_handle, _data.leaf_brushes, headers[LEAFBRUSHES_LUMP]);
        __read_lump<Model>(file_handle, _data.models, headers[MODELS_LUMP]);
        __read_lump<Brush>(file_handle, _data.brushes, headers[BRUSHES_LUMP]);
        __read_lump<Brushside>(file_handle, _data.brush_sides, headers[BRUSHSIDES_LUMP]);
        __read_lump<Vertex>(file_handle, _data.vertices, headers[VERTICES_LUMP]);
        __read_lump<Meshvert>(file_handle, _data.mesh_vertices, headers[MESHVERTS_LUMP]);
        __read_lump<Effect>(file_handle, _data.effects, headers[EFFECTS_LUMP]);
        __read_lump<Face>(file_handle, _data.faces, headers[FACES_LUMP]);
        __read_lump<Lightmap>(file_handle, _data.light_maps, headers[LIGHTMAPS_LUMP]);
        __read_lump<Lightvol>(file_handle, _data.light_vols, headers[LIGHTVOLS_LUMP]);
        __read_visdata_lump(file_handle, _data.vis_data, headers[VISDATA_LUMP]);

        QLL_Q3_FILE_FCLOSE(file_handle);
    }

    Q3Level::~Q3Level()
    {
        if (_data.vis_data.vecs)
            QLL_Q3_FREE(_data.vis_data.vecs);
    }

    // Tools functions

    static void __read_string(
        QLL_Q3_FILE_TYPE file_handle,
        QLL_Q3_STRING& result,
        const q3_int& length
    )
    {
        char* raw_data = (char*)QLL_Q3_MALLOC(length);

        QLL_Q3_FILE_FREAD(raw_data, 1, length, file_handle);
        result = QLL_Q3_STRING(raw_data);
        QLL_Q3_FREE(raw_data);
    }

    template <typename T> static void __read_lump
    (
        QLL_Q3_FILE_TYPE file_handle,
        QLL_Q3_ARRAY(T)& result,
        const __lump_header& header
    )
    {
        int item_count = header.length / sizeof(T);

        // Go to the start of the chunk.
        QLL_Q3_FILE_FSEEK(file_handle, header.offset);

        for (int i = 0; i < item_count; ++i)
        {
            T item;
            QLL_Q3_FILE_FREAD(&item, 1, sizeof(T), file_handle);
            QLL_Q3_ARRAY_APPEND(result, item);
        }
    }

    template <> void __read_lump
    (
        QLL_Q3_FILE_TYPE file_handle,
        QLL_Q3_ARRAY(Texture)& result,
        const __lump_header& header
    )
    {
        const int lump_size = 2 * sizeof(q3_int) + 64;
        int item_count = header.length / lump_size;

        // Go to the start of the chunk.
        QLL_Q3_FILE_FSEEK(file_handle, header.offset);

        for (int i = 0; i < item_count; ++i)
        {
            Texture item;

            __read_string(file_handle, item.name, 64);
            QLL_Q3_FILE_FREAD(&item.flags, 1, sizeof(q3_int), file_handle);
            QLL_Q3_FILE_FREAD(&item.contents, 1, sizeof(q3_int), file_handle);

            QLL_Q3_ARRAY_APPEND(result, item);
        }
    }

    template <> void __read_lump
    (
        QLL_Q3_FILE_TYPE file_handle,
        QLL_Q3_ARRAY(Effect)& result,
        const __lump_header& header
    )
    {
        const int lump_size = 2 * sizeof(q3_int) + 64;
        int item_count = header.length / lump_size;

        // Go to the start of the chunk.
        QLL_Q3_FILE_FSEEK(file_handle, header.offset);

        for (int i = 0; i < item_count; ++i)
        {
            Effect item;

            __read_string(file_handle, item.name, 64);
            QLL_Q3_FILE_FREAD(&item.brush, 1, sizeof(q3_int), file_handle);
            QLL_Q3_FILE_FREAD(&item.unknown, 1, sizeof(q3_int), file_handle);

            QLL_Q3_ARRAY_APPEND(result, item);
        }
    }

    static void __read_visdata_lump
    (
        QLL_Q3_FILE_TYPE file_handle,
        Visdata& result,
        const __lump_header& header
    )
    {
        QLL_Q3_FILE_FSEEK(file_handle, header.offset);

        QLL_Q3_FILE_FREAD(&result.n_vecs, 1, sizeof(q3_int), file_handle);
        QLL_Q3_FILE_FREAD(&result.sz_vecs, 1, sizeof(q3_int), file_handle);

        int visdata_length = result.n_vecs * result.sz_vecs;

        result.vecs = (q3_ubyte*)QLL_Q3_MALLOC(visdata_length);

        QLL_Q3_FILE_FREAD(result.vecs, 1, visdata_length, file_handle);
    }

    static void __read_entities_lump
    (
        QLL_Q3_FILE_TYPE file_handle,
        QLL_Q3_STRING& result,
        const __lump_header& header
    )
    {
        QLL_Q3_FILE_FSEEK(file_handle, header.offset);

        char* entity_raw = (char*)QLL_Q3_MALLOC(header.length);

        QLL_Q3_FILE_FREAD(entity_raw, header.length, 1, file_handle);

        result = QLL_Q3_STRING(entity_raw);

        QLL_Q3_FREE(entity_raw);
    }


    #define QLL_Q3_ENTITY_TYPE QLL_Q3_ASSOCIATIVE_ARRAY(QLL_Q3_STRING, QLL_Q3_STRING)
    #define QLL_Q3_ENTITIES_RESULT_TYPE QLL_Q3_ARRAY(QLL_Q3_ENTITY_TYPE)
    QLL_Q3_ENTITIES_RESULT_TYPE parse_entities(const QLL_Q3_STRING& entities_lump)
    {
        QLL_Q3_ENTITIES_RESULT_TYPE result;
        QLL_Q3_ENTITY_TYPE current_entity;

        unsigned int i = 0;

        bool inKey = false;
        bool inValue = false;
        bool inGroup = false;
        bool keyFilled = false;

        QLL_Q3_STRING key, value;

        while (i < QLL_Q3_STRING_LENGTH(entities_lump))
        {
            const char current_char = QLL_Q3_STRING_GET_CHAR(entities_lump, i);

            if (current_char == '{' && !inKey && !inValue)
            {
                if (!inGroup)
                    inGroup = true;
                else
                    QLL_Q3_LOG_ERROR("Unexpected '{' at position " + QLL_Q3_STRING_FROM_VALUE(i));
            }
            else if (current_char == '}' && !inKey && !inValue)
            {
                if (inGroup)
                {
                    QLL_Q3_ARRAY_APPEND(result, current_entity);
                    current_entity = {};

                    inGroup = false;
                }
                else
                    QLL_Q3_LOG_ERROR("Unexpected '}' at position " + QLL_Q3_STRING_FROM_VALUE(i));
            }
            else if (current_char == '"')
            {
                if (inGroup)
                {
                    if (!inKey && !inValue)
                    {
                        // This is the begining of a string (key or value)

                        if (!keyFilled)
                            inKey = true;
                        else
                            inValue = true;
                    }
                    else
                    {
                        // End of a string (key or value)

                        if (inKey)
                        {
                            keyFilled = true;
                            inKey = false;
                        }
                        else if (inValue)
                        {
                            QLL_Q3_ASSOCIATIVE_ARRAY_SET(current_entity, key, value);
                            key = value = "";

                            // Reset the parser states
                            inValue = false;
                            keyFilled = false;
                        }
                    }
                }
                else
                    QLL_Q3_LOG_ERROR("Unexpected '\"' at position " + QLL_Q3_STRING_FROM_VALUE(i));
            }
            else
            {
                // Adding a character

                if (inKey)
                    QLL_Q3_STRING_ADD_CHAR(key, current_char);

                if (inValue)
                    QLL_Q3_STRING_ADD_CHAR(value, current_char);
            }

            i++;
        }

        if (inGroup)
            QLL_Q3_LOG_ERROR("Unexpected end!");

        return result;
    }
}}
#endif
