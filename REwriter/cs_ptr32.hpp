#pragma once
#ifndef _MSC_VER
template<typename T>
struct ptr32_t {
private:
	unsigned _ptr;
	static constexpr bool is_voidy = std::is_void_v<T> || std::is_void_v<std::remove_cv_t<T>>;

	inline T* get() {
		return reinterpret_cast<T*>(static_cast<uintptr_t>(_ptr));
	}
	inline const T* get() const {
		return reinterpret_cast<const T*>(static_cast<uintptr_t>(_ptr));
	}
public:

	inline ptr32_t() : _ptr(0u) {}

	inline ptr32_t(T* p) : _ptr(reinterpret_cast<unsigned>(p)) {}

	inline ptr32_t<T>& operator =(ptr32_t<T> other) {
		_ptr = other._ptr;
		return *this;
	}

	template<typename = std::enable_if_t<!is_voidy>>
	inline T * operator ->() {
		return get();
	}

	template<typename TRes = std::enable_if_t<!is_voidy, T>>
	inline TRes& operator *() {
		return *get();
	}
	template<typename TRes = std::enable_if_t<!is_voidy, T>>
	inline TRes& operator [](unsigned idx) {
		return get()[idx];
	}

	inline ptr32_t<T> operator + (unsigned n) {
		return get() + n;
	}
	inline ptr32_t<T>& operator += (unsigned n) {
		*this = get() + n;
		return *this;
	}


	inline ptr32_t<T> operator - (unsigned n) {
		return get() - n;
	}

	inline int operator - (const ptr32_t<T> other) const {
		return (int)_ptr - (int)other._ptr;
	}
	template<typename = std::enable_if_t<!is_voidy>>
	inline  T * operator ->() const {
		return const_cast<T*>(get());
	}
	template<typename TRes = std::enable_if_t<!is_voidy, T>>
	inline const TRes& operator *() const {
		return *get();
	}

	inline explicit operator bool() {
		return _ptr;
	}

	inline explicit operator void* () {
		return (void*)get();
	}
	inline bool operator == (const ptr32_t<T> other) const {
		return other._ptr == _ptr;

	}

	inline bool operator > (const ptr32_t<T> other) const {
		return _ptr > other._ptr;
	}

	inline bool operator >= (const ptr32_t<T> other) const {
		return _ptr >= other._ptr;
	}


	inline bool operator < (const ptr32_t<T> other) const {
		return _ptr < other._ptr;
	}


	inline bool operator <= (const ptr32_t<T> other) const {
		return _ptr > other._ptr;
	}
	inline bool operator != (const ptr32_t<T> other) const {
		return other._ptr == _ptr;
	}
	template<typename TRes = std::enable_if_t<!is_voidy, T>>
	inline const TRes& operator [](unsigned idx) const {
		return get()[idx];
	}
};

#else
template<typename T>
using ptr32_t = T * __ptr32;
#endif