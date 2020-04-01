#ifndef JO_STDCPP
#define JO_STDCPP

// Not in any way intended to be fastest implementation, just simple bare minimum we need to compile tinyexr

#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>

// 
// Simple C++std replacements...
//


template<typename T> struct jo_numeric_limits;

template<> struct jo_numeric_limits<int> {
    static int max() { return +2147483647; }
    static int min() { return -2147483648; }
};

#define jo_endl ("\n")
#define jo_string_npos (-1)

struct jo_string {
    char *str;
    
    jo_string() { str = strdup(""); }
    jo_string(const char *ss) { str = strdup(ss); }
    jo_string(char c) { str = strdup(" "); str[0] = c; }
    jo_string(const jo_string *other) { str = strdup(other->str); }
    jo_string(const jo_string &other) { str = strdup(other.str); }
    jo_string(const char *a, size_t size) {
        str = (char*)malloc(size+1);
        memcpy(str, a, size);
        str[size] = 0;
    }
    jo_string(const char *a, const char *b) {
        ptrdiff_t size = b - a;
        str = (char*)malloc(size+1);
        memcpy(str, a, size);
        str[size] = 0;
    }

    ~jo_string() {
        free(str);
        str = 0;
    }

    const char *c_str() const { return str; };
    int compare(const jo_string &other) { return strcmp(str, other.str); }
    bool empty() const { return str[0] == 0; }
    size_t size() const { return strlen(str); }
    size_t length() const { return strlen(str); }

    jo_string &operator=(const char *s) {
        char *tmp = strdup(s);
        free(str);
        str = tmp;
        return *this;
    }

    jo_string &operator=(const jo_string &s) {
        char *tmp = strdup(s.str);
        free(str);
        str = tmp;
        return *this;
    }

    jo_string &operator+=(const char *s) {
        char *new_str = (char*)realloc(str, strlen(str) + strlen(s) + 1);
        if(!new_str) {
            // malloc failed!
            return *this;
        }
        str = new_str;
        strcat(str, s);
        return *this;
    }
    jo_string &operator+=(const jo_string &s) { *this += s.c_str(); return *this; }

    size_t find_last_of(char c) {
        char *tmp = strrchr(str, c);
        if(!tmp) return jo_string_npos;
        return (size_t)(tmp - str);
    }

    jo_string &erase(size_t n) {
        str[n] = 0;
        return *this;
    }

    jo_string substr(size_t pos = 0, size_t len = jo_string_npos) {
        if(len == jo_string_npos) {
            len = size() - pos;
        }
        return jo_string(str + pos, len);
    }

    size_t find(char c, size_t pos = 0) const {
        const char *tmp = strchr(str+pos, c);
        if(!tmp) return jo_string_npos;
        return (size_t)(tmp - str);
    }
    size_t find(const char *s, size_t pos = 0) const {
        const char *tmp = strstr(str+pos, s);
        if(!tmp) return jo_string_npos;
        return (size_t)(tmp - str);
    }
    size_t find(const jo_string &s, size_t pos = 0) const { return find(s.c_str(), pos); }
};

static inline jo_string operator+(const jo_string &lhs, const jo_string &rhs) { jo_string ret(lhs); ret += rhs; return ret; }
static inline jo_string operator+(const jo_string &lhs, const char *rhs) { jo_string ret(lhs); ret += rhs; return ret; }
static inline jo_string operator+(const char *lhs, const jo_string &rhs) { jo_string ret(lhs); ret += rhs; return ret; }
static inline jo_string operator+(const jo_string &lhs, char rhs) { jo_string ret(lhs); ret += rhs; return ret; }
static inline jo_string operator+(char lhs, const jo_string &rhs) { jo_string ret(lhs); ret += rhs; return ret; }
static inline bool operator==(const jo_string &lhs, const jo_string &rhs) { return !strcmp(lhs.c_str(), rhs.c_str()); }
static inline bool operator==(const char *lhs, const jo_string &rhs) { return !strcmp(lhs, rhs.c_str()); }
static inline bool operator==(const jo_string &lhs, const char *rhs) { return !strcmp(lhs.c_str(), rhs); }
static inline bool operator!=(const jo_string &lhs, const jo_string &rhs) { return !!strcmp(lhs.c_str(), rhs.c_str()); }
static inline bool operator!=(const char *lhs, const jo_string &rhs) { return !!strcmp(lhs, rhs.c_str()); }
static inline bool operator!=(const jo_string &lhs, const char *rhs) { return !!strcmp(lhs.c_str(), rhs); }
static inline bool operator<(const jo_string &lhs, const jo_string &rhs) { return strcmp(lhs.c_str(), rhs.c_str()) < 0; }
static inline bool operator<(const char *lhs, const jo_string &rhs) { return strcmp(lhs, rhs.c_str()) < 0; }
static inline bool operator<(const jo_string &lhs, const char *rhs) { return strcmp(lhs.c_str(), rhs) < 0; }
static inline bool operator<=(const jo_string &lhs, const jo_string &rhs) { return strcmp(lhs.c_str(), rhs.c_str()) <= 0; }
static inline bool operator<=(const char *lhs, const jo_string &rhs) { return strcmp(lhs, rhs.c_str()) <= 0; }
static inline bool operator<=(const jo_string &lhs, const char *rhs) { return strcmp(lhs.c_str(), rhs) <= 0; }
static inline bool operator>(const jo_string &lhs, const jo_string &rhs) { return strcmp(lhs.c_str(), rhs.c_str()) > 0; }
static inline bool operator>(const char *lhs, const jo_string &rhs) { return strcmp(lhs, rhs.c_str()) > 0; }
static inline bool operator>(const jo_string &lhs, const char *rhs) { return strcmp(lhs.c_str(), rhs) > 0; }
static inline bool operator>=(const jo_string &lhs, const jo_string &rhs) { return strcmp(lhs.c_str(), rhs.c_str()) >= 0; }
static inline bool operator>=(const char *lhs, const jo_string &rhs) { return strcmp(lhs, rhs.c_str()) >= 0; }
static inline bool operator>=(const jo_string &lhs, const char *rhs) { return strcmp(lhs.c_str(), rhs) >= 0; }

struct jo_stringstream {
    jo_string s;

    jo_string &str() { return s; }
    const jo_string &str() const { return s; }

    jo_stringstream &operator<<(int val) {
        char tmp[33];
        sprintf(tmp, "%i", val);
        s += tmp;
        return *this;
    }

    jo_stringstream &operator<<(const char *val) {
        s += val;
        return *this;
    }
};

template<typename T> static inline T jo_min(T a, T b) { return a < b ? a : b; }
template<typename T> static inline T jo_max(T a, T b) { return a > b ? a : b; }

#ifndef __PLACEMENT_NEW_INLINE
inline void *operator new(size_t s, void *p) { return p; }
#endif

template<typename T>
struct jo_vector {
    T *ptr;
    size_t ptr_size;
    size_t ptr_capacity;

    jo_vector() {
        ptr = 0;
        ptr_size = 0;
        ptr_capacity = 0;
    }

    jo_vector(size_t n) {
        ptr = 0;
        ptr_size = 0;
        ptr_capacity = 0;
        
        resize(n);
    }

    size_t size() const { return ptr_size; }

    T *data() { return ptr; }
    const T *data() const { return ptr; }

    T *begin() { return ptr; }
    const T *begin() const { return ptr; }

    T *end() { return ptr + ptr_size; }
    const T *end() const { return ptr + ptr_size; }

    T &at(size_t i) { return ptr[i]; }
    const T &at(size_t i) const { return ptr[i]; }

    T &operator[](size_t i) { return ptr[i]; }
    const T &operator[](size_t i) const { return ptr[i]; }

    void resize(size_t n) {
        if(n < ptr_size) {
            // call dtors on stuff your destructing before moving memory...
            for(size_t i = n; i < ptr_size; ++i) {
                ptr[i].~T();
            }
        }

        if(n > ptr_capacity) {
            T *newptr = (T*)malloc(n*sizeof(T));
            if(!newptr) {
                // malloc failed!
                return;
            }
            if(ptr) {
                memcpy(newptr, ptr, ptr_size*sizeof(T));
                free(ptr);
            }
            ptr = newptr;
            ptr_capacity = n;
        }

        if(n > ptr_size) {
            // in-place new on new data after moving memory to new location
            for(size_t i = ptr_size; i < n; ++i) {
                new(ptr+i) T(); // default c-tor
            }
        }

        ptr_size = n;
    }
    void clear() { resize(0); }

    void insert(const T *where, const T *what, size_t how_many) {
        if(how_many == 0) {
            return;
        }

        size_t n = ptr_size + how_many;
        ptrdiff_t where_at = where - ptr;

        // resize if necessary
        if(n > ptr_capacity) {
            size_t new_capacity = n + n/2; // grow by 50%
            T *newptr = (T*)malloc(new_capacity*sizeof(T));
            if(!newptr) {
                // malloc failed!
                return;
            }
            if(ptr) {
                memcpy(newptr, ptr, ptr_size*sizeof(T));
                free(ptr);
            }
            ptr = newptr;
            ptr_capacity = new_capacity;
        }

        // simple case... add to end of array.
        if(where == ptr+ptr_size || where == 0) {
            for(size_t i = ptr_size; i < n; ++i) {
                new(ptr+i) T(what[i - ptr_size]);
            }
            ptr_size = n;
            return;
        }

        // insert begin/middle means we need to move the data past where to the right, and insert how_many there...
        memmove(ptr + where_at, ptr + where_at + how_many, sizeof(T)*(ptr_size - where_at));
        for(size_t i = where_at; i < where_at + how_many; ++i) {
            new(ptr+i) T(what[i - where_at]);
        }
        ptr_size = n;
    }

    void insert(const T *where, const T *what_begin, const T *what_end) {
        insert(where, what_begin, (size_t)(what_end - what_begin));
    }

    void push_back(const T& val) { insert(end(), &val, 1); }
    T pop_back() { T ret = ptr[ptr_size-1]; resize(ptr_size-1); return ret; }
    T &back() { return ptr[ptr_size-1]; }
    const T &back() const { return ptr[ptr_size-1]; }

    T &front() { return ptr[0]; }
    const T &front() const { return ptr[back0]; }
};

template<typename T, typename TT>
void jo_sift_down(T *begin, T *end, size_t root, TT cmp) {
    ptrdiff_t n = end - begin;
    size_t parent = 0;
    size_t child = 2 * parent + 1;
    while (child < n) {
        if (cmp(begin[child], begin[child + 1]) && child + 1 < n) {
            child++;
        }
        if (!cmp(begin[child], begin[parent])) {
            T tmp = begin[child];
            begin[child] = begin[parent];
            begin[parent] = tmp;

            parent = child;
            child = 2 * parent + 1;
        } else {
            break;
        }
    }
}

template<typename T, typename TT>
void jo_sift_up(T *begin, T *end, size_t child, TT cmp) {
   	size_t parent = (child - 1) >> 1;
	while (child > 0) {
		if(!cmp(begin[child], begin[parent])) {
			T tmp = begin[child];
			begin[child] = begin[parent];
			begin[parent] = tmp;

			child = parent;
			parent = (child - 1) >> 1;
		} else {
			break;
		}
	}
}

template<typename T, typename TT>
void jo_make_heap(T *begin, T *end, TT cmp) {
    if(begin >= end) {
        return;
    }
    ptrdiff_t n = end - begin;
    size_t root = (n - 2) >> 1;
    for (; root >= 0; root--) {
        jo_sift_down(begin, end, root, cmp);
    }
}

template<typename T, typename TT>
void jo_pop_heap(T *begin, T *end, TT cmp) {
    if(begin >= end) {
        return;
    }
    T tmp = begin[0];
    begin[0] = end[-1];
    end[-1] = tmp;
    jo_sift_down(begin, end, 0, cmp);
}

template<typename T, typename TT>
void jo_push_heap(T *begin, T *end, TT cmp) {
    ptrdiff_t n = end - begin;
    if(n <= 1) {
        return; // nothing to do...
    }
    jo_sift_up(begin, end, n - 1, cmp);
}

template<typename T, typename TT>
void jo_sort_heap(T *begin, T *end, TT cmp) {
    jo_make_heap(begin, end, cmp);
    ptrdiff_t n = end - begin;
    for (ptrdiff_t i = n; i > 0; --i) {
        begin[0] = begin[i-1];
        jo_sift_down(begin, begin + i - 1, 0, cmp);
    }
}

template<typename T>
const T *jo_find(const T *begin, const T *end, const T &needle) {
    for(const T *ptr = begin; ptr != end; ++ptr) {
        if(*ptr == needle) return ptr;
    }
    return end;
}

template<typename T>
T *jo_find(T *begin, T *end, const T &needle) {
    for(T *ptr = begin; ptr != end; ++ptr) {
        if(*ptr == needle) return ptr;
    }
    return end;
}

#endif // JO_STDCPP

