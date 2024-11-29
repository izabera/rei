#include <algorithm>
#include <cxxabi.h>
#include <dlfcn.h>
#include <iostream>
#include <stdio.h>
#include <unordered_map>
#include <vector>

namespace {
struct calls {
    struct callinfo {
        size_t total{};
        std::unordered_map<void *, size_t> callers;
    };
    struct pair {
        void *func;
        size_t total;
        auto operator<(const auto &rhs) const { return total < rhs.total; }
    };

    struct compact_callinfo {
        void *func = nullptr;
        size_t total{};
        std::vector<pair> callers;

        compact_callinfo(void *f, const callinfo &info)
            : func(f) {
            total = info.total;
            for (const auto &[k, v] : info.callers)
                callers.push_back({k, v});
            std::sort(callers.begin(), callers.end());
        }
        auto operator<(const auto &rhs) const { return total < rhs.total; }
    };

    std::unordered_map<void *, callinfo> info;
    ~calls() {
        std::vector<compact_callinfo> sorted;
        for (const auto &[k, v] : info)
            sorted.push_back({k, v});
        std::sort(sorted.begin(), sorted.end());

        for (const auto &info : sorted) {
            Dl_info dyninfo;
            if (dladdr(info.func, &dyninfo) && dyninfo.dli_sname) {
                int status;
                char *name = abi::__cxa_demangle(dyninfo.dli_sname, nullptr, nullptr, &status);
                if (name) {
                    std::cout << info.total << '\t' << name << '\n';
                    free(name);
                }
                else
                    std::cout << info.total << '\t' << dyninfo.dli_sname << '\n';
            }
            else
                std::cout << info.total << '\t' << info.func << '\n';
        }
    }
} funcs;
} // namespace

extern "C" {
void __cyg_profile_func_enter(void *func, void *caller) {
    auto &info = funcs.info[func];
    info.total++;
    info.callers[caller]++;
}
}
