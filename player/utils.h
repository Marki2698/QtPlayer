#ifndef UTILS_H
#define UTILS_H

#include<memory>

template<class T, class... Args>
std::unique_ptr<T> make_unique(Args&&... args) noexcept {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

#endif // UTILS_H
