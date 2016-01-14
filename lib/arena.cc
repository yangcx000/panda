#include "arena.h"

#include <cassert>
#include <cstdio>

namespace Ucloud {

static int kBlockSize = 4096;

Arena::Arena()
	: m_current(NULL),
	  m_remaining(0),
	  m_allocated(0) {
}

Arena::~Arena() {
	for (uint32_t i = 0; i < m_blocks.size(); ++i) {
		delete[] m_blocks[i];	
	}
}

char* Arena::Allocate(int bytes) {
	assert(bytes > 0);
	if (bytes < m_remaining) {
		char* result = m_current;
		m_current += bytes;
		m_remaining -= bytes;
		return result;
	}

	return AllocateFallback(bytes);
}

char* Arena::AllocateFallback(int bytes) {
	if (bytes > kBlockSize / 4) {
		return NewBlock(bytes);
	}

	m_current = NewBlock(kBlockSize);
	m_remaining = kBlockSize - bytes;
	return m_current;
}

char* Arena::NewBlock(int size) {
	char* area = new char[size];
	m_blocks.push_back(area);
	m_allocated++;
	return area;
}

}
