#ifndef PAGE_MANAGER_H
#define PAGE_MANAGER_H

#include <unordered_map>
#include <memory>
#include <array>
#include <stdlib.h>

using PageId = int;

class PageBuffer {
  public:
    uint8_t* page_buffer;
    PageBuffer(const int page_size) {
      page_buffer = (uint8_t*)calloc(page_size, sizeof(uint8_t));
    }

    ~PageBuffer() {
      std::free(page_buffer);
    }
};

class Page {
  const int page_size;
  const std::unique_ptr<PageBuffer> page_buffer;

  int cursor{0};
  
  public:
    Page(const int page_size) : page_size{page_size}, page_buffer{new PageBuffer(page_size)} {}

    // NOTE: flush must be called for writes to become visible and active
    //       otherwise writes mutate only internal memory page buffer
    void flush();

    void read(const unsigned int num_bytes, const uint8_t* data) const;
    void write(const unsigned int num_bytes, const uint8_t* data);
};

class PageManager {
  std::unordered_map<PageId, std::shared_ptr<Page>> map{};
  
  public:
    PageManager() {
      
    }

    std::shared_ptr<Page> load_page(const PageId page_id) const;

    // NOTE: does not flush page on unload
    void unload_page(const PageId page_id) const;
};

#endif
