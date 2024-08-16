#include <cstring>
#include <iostream>
#include <fstream>

uint8_t read_be_uint8_t(uint8_t* mem) {
    return static_cast<uint8_t>(mem[0]);
}

uint16_t read_be_uint16_t(uint8_t* mem) {
    return (static_cast<uint8_t>(mem[1]) | (static_cast<uint8_t>(mem[0]) << 8));
}

uint32_t read_be_uint32_t(uint8_t* mem) {
    return (static_cast<uint8_t>(mem[3]) | (static_cast<uint8_t>(mem[2]) << 8) | 
           (static_cast<uint8_t>(mem[1]) << 16) | (static_cast<uint8_t>(mem[0]) << 24));
}

// NOTE: https://www.sqlite.org/fileformat.html#the_database_header
typedef struct {
    // NOTE: "SQLite format 3\000"
    char header_str[16];
    // NOTE: db page size in bytes, between 512 and 32,768,
    //       value 1 means page size 65,536
    uint16_t page_size;
    // NOTE: 1 for legacy 2 for WAL
    uint8_t file_fmt_write;
    // NOTE: 1 for legacy 2 for WAL
    uint8_t file_fmt_read;
    // NOTE: bytes of unused space at end of page
    uint8_t rsv;
    // NOTE: max payload frac, must be 64
    uint8_t max_payload_frac;
    // NOTE: min payload frac, must be 32
    uint8_t min_payload_frac;
    // NOTE: leaf payload frac, must be 32
    uint8_t leaf_payload_frac;
    // NOTE: file change counter
    uint32_t file_change_cnt;
    // NOTE: size of the db in pages
    uint32_t db_page_size;
    // NOTE: page number of first freelist trunk page
    uint32_t first_freelist_trunk_page;
    // NOTE: total number of freelist pages
    uint32_t num_freelist_pages;
    // NOTE: schema cookie
    uint32_t schema_cookie;
    // NOTE: schema format number, either 1, 2, 3, or 4
    uint32_t schema_format_number;
    // NOTE: default page cache size
    uint32_t default_page_cache_size;
    // NOTE: page number largest root b-tree page when in auto-vacuum
    //       or incremental-vacuum modes, or zero otherwise
    uint32_t largest_btree_page_number;
    // NOTE: db text encoding, 1 = UTF-8, 2 = UTF-16Ie, 3 = UTF-16be
    uint32_t text_encoding;
    // NOTE: user version
    uint32_t user_version;
    // NOTE: true is non-zero for inc vacuum mode false otherwise
    uint32_t inc_vacuum_mode;
    // NOTE: application ID
    uint32_t app_id;
    // NOTE: reserved must be zero
    uint8_t rsv_zero[20];
    // NOTE: version-valid-for number
    uint32_t version_valid_for;
    // NOTE: SQLITE version number
    uint32_t sqllite_version_number;
} DbHeader;

DbHeader read_db_header(std::ifstream& db_file) {
    DbHeader db_header = {};

    uint8_t uint8_t_buffer[1];
    uint8_t uint16_t_buffer[2];
    uint8_t uint32_t_buffer[4];
    
    db_file.seekg(0);

    db_file.read(db_header.header_str, 16);

    db_file.read(reinterpret_cast<char*>(uint16_t_buffer), 2);
    db_header.page_size = read_be_uint16_t(uint16_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    db_header.file_fmt_write = read_be_uint8_t(uint8_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    db_header.file_fmt_read = read_be_uint8_t(uint8_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    db_header.rsv = read_be_uint8_t(uint8_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    db_header.max_payload_frac = read_be_uint8_t(uint8_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    db_header.min_payload_frac = read_be_uint8_t(uint8_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    db_header.leaf_payload_frac = read_be_uint8_t(uint8_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.file_change_cnt = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.db_page_size = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.first_freelist_trunk_page = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.num_freelist_pages = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.schema_cookie = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.schema_format_number= read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.default_page_cache_size = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.largest_btree_page_number = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.text_encoding = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.user_version = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.inc_vacuum_mode = read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.app_id= read_be_uint32_t(uint32_t_buffer);

    db_file.read(reinterpret_cast<char*>(db_header.rsv_zero), 20);
    
    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.version_valid_for = read_be_uint32_t(uint32_t_buffer);
    
    db_file.read(reinterpret_cast<char*>(uint32_t_buffer), 4);
    db_header.sqllite_version_number = read_be_uint32_t(uint32_t_buffer);

    return db_header;
}

enum class PageType {
    INTER_INDEX = 0x02,
    INTER_TABLE = 0x05,
    LEAF_INDEX = 0x0a,
    LEAF_TABLE = 0x0d,  
};

typedef struct {
    PageType page_type;
    // NOTE: first freeblock on page or zero if none
    uint16_t first_freeblock;
    // NOTE: number of cells on page
    uint16_t num_cells;
    // NOTE: start of cell content area, 0 is 65,536
    uint16_t start_cell_content_area;
    // NOTE: fragmented free bytes within cell content area
    uint8_t frag_free_bytes;
} BTreeHeader;

typedef struct {
    BTreeHeader btree_header;
} BTreeLeafHeader;

typedef struct {
    BTreeHeader btree_header;

    // NOTE: page number right-most pointer
    uint32_t rm_pointer;
} BTreeInteriorHeader;

PageType get_page_type(std::ifstream& if_stream, int page_size, int page) {
    int page_index = page_size * page;
    if_stream.seekg(0);

    if(!page) {
        std::cout << "first page, skipping 100 bytes..." << std::endl;
        if_stream.seekg(100);
    }

    uint8_t uint8_t_buffer[1] = {0};
    if_stream.read(reinterpret_cast<char*>(uint8_t_buffer), 1);
    PageType pg_type = static_cast<PageType>(*uint8_t_buffer);

    return pg_type;
}

BTreeHeader get_page_header(std::ifstream& if_stream, int page_size, int page) {
    int page_index = page_size * page;
    if_stream.seekg(0);

    if(!page) {
        std::cout << "first page, skipping 100 bytes..." << std::endl;
        if_stream.seekg(100);
    }

    BTreeHeader btree_header;
    
}

BTreeInteriorHeader get_interior_page_header(std::ifstream& if_stream, int page_size, int page) {
    BTreeInteriorHeader b_tree_interior_header;
    BTreeHeader b_tree_header = get_page_header(if_stream, page_size, page);

    b_tree_interior_header.btree_header = b_tree_header;

    // NOTE: extra four bytes for this type of page    


    return b_tree_interior_header;
}

int get_num_tables() {
    
}

BTreeLeafHeader get_leaf_page_header(std::ifstream& if_stream, int page_size, int page) {
    BTreeLeafHeader b_tree_leaf_header;
    BTreeHeader b_tree_header = get_page_header(if_stream, page_size, page);

    b_tree_leaf_header.btree_header = b_tree_header;

    return b_tree_leaf_header;
}

int main(int argc, char* argv[]) {
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string database_file_path = argv[1];
    std::string command = argv[2];

    if (command == ".dbinfo") {
        std::ifstream db_file(database_file_path, std::ios::binary);
        if (!db_file) {
            std::cerr << "Failed to open the database file" << std::endl;
            return 1;
        }

        DbHeader db_header = read_db_header(db_file);
        std::cout << "database page size: " << db_header.page_size << std::endl;

        int num_tables = get_num_tables();
    }

    return 0;
}
