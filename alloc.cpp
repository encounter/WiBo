#include "alloc.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <sys/mman.h>

namespace alloc {

tlsf_t tlsf = nullptr;
static void *HeapStart = reinterpret_cast<void *>(0x40000000); // 1 GiB
static void *HeapEnd = reinterpret_cast<void *>(0x80000000);   // 2 GiB
static const size_t PoolSize = 0x1000000;					   // 16 MiB
static void *NextPool = HeapStart;

void init() {
	void *pool = mmap(NextPool, PoolSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (pool == MAP_FAILED) {
		perror("allocInit");
		exit(1);
	} else if (pool >= HeapEnd) {
		fprintf(stderr, "allocInit: pool %p >= HeapEnd %p\n", pool, HeapEnd);
		exit(1);
	}
	NextPool = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(pool) + PoolSize);
	fprintf(stderr, "allocInit: pool %p\n", pool);
	tlsf = tlsf_create_with_pool(pool, PoolSize);
}

static void addPool(size_t size) {
	fprintf(stderr, "addPool(%zu)\n", size);
	void *pool = mmap(NextPool, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (pool == MAP_FAILED) {
		perror("addPool");
		exit(1);
	} else if (pool >= HeapEnd) {
		fprintf(stderr, "addPool: pool %p >= HeapEnd %p\n", pool, HeapEnd);
		exit(1);
	}
	NextPool = reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(pool) + PoolSize);
	tlsf_add_pool(tlsf, pool, size);
}

__attribute__((const)) static inline uint32_t p2(uint32_t x) { return 1 << (32 - __builtin_clz(x - 1)); }

void *alloc(size_t size) {
	void *ptr = tlsf_malloc(tlsf, size);
	if (ptr == nullptr) {
		addPool(std::max(PoolSize, p2(size)));
		ptr = tlsf_malloc(tlsf, size);
	}
	return ptr;
}

void *realloc(void *ptr, size_t size) {
	void *newPtr = tlsf_realloc(tlsf, ptr, size);
	if (newPtr == nullptr) {
		addPool(std::max(PoolSize, p2(size)));
		newPtr = tlsf_realloc(tlsf, ptr, size);
	}
	return newPtr;
}

void *memalign(size_t align, size_t size) {
	void *ptr = tlsf_memalign(tlsf, align, size);
	if (ptr == nullptr) {
		addPool(std::max(PoolSize, p2(size)));
		ptr = tlsf_memalign(tlsf, align, size);
	}
	return ptr;
}

void free(void *ptr) { tlsf_free(tlsf, ptr); }

size_t usableSize(void *ptr) { return tlsf_block_size(ptr); }

}; // namespace alloc