#ifndef CORE_PUBLIC_H
#define CORE_PUBLIC_H

namespace PUBLIC {}
using namespace PUBLIC;

extern String read_memory(MemoryBuffer *buf, Allocator mem);
extern void sleep(i32 milliseconds);
extern i32 current_process_id();

#endif // CORE_PUBLIC_H
