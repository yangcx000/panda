#ifndef ARENA_H
#define ARENA_H

#include <vector>
#include <cstdint>

namespace Ucloud {

class Arena {
	public:
	Arena();
	~Arena();
	char* Allocate(int bytes);

	private:
	char* AllocateFallback(int bytes);
	char* NewBlock(int size);

	char* m_current;	
	uint64_t m_remaining;
	uint64_t m_allocated;
	std::vector<char*> m_blocks;
};

}
#endif
