#pragma once
#include "idaheaders.hpp"
//173 regs
#include <array>
constexpr unsigned NREGS = 173;

using csreglist_data_t = std::array<uint8_t, NREGS>;

class csreglist_t {


	//std::set<uint16_t> m_regs;

	csreglist_data_t m_data;

public:
	csreglist_t() : m_data({ 0 }) {}


	inline void add(unsigned index, unsigned size) {

		if (m_data[index] < size) {
			m_data[index] = size;
		}
	}

	inline unsigned at(unsigned index) {
		return m_data[index];
	}
	inline void clear() {
		memset(&m_data[0], 0, sizeof(m_data));
	}

	bool has(unsigned index) const {
		return m_data[index] != 0;
	}

	void print(bool use) {
		qstring regname_buf{};
		regname_buf.reserve(128);

		for (unsigned i = 0; i < NREGS; ++i) {
			auto value = m_data[i];

			if (value != 0) {
				get_reg_name(&regname_buf, i, value);

				if (use) {

					msg("Use : reg %s of size %d\n", &regname_buf[0], value);
				}
				else {
					msg("Def : reg %s of size %d\n", &regname_buf[0], value);
				}
			}
		}
	}

	auto begin() {
		return &m_data[0];
	}

	auto end() {
		return &m_data[NREGS];
	}


};
