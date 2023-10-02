#include <tlsf/tlsf.h>

namespace alloc {

extern tlsf_t tlsf;

void init();

void *alloc(size_t size);
void *realloc(void *ptr, size_t size);
void *memalign(size_t align, size_t size);
void free(void *ptr);
size_t usableSize(void *ptr);

} // namespace alloc
