// timing.h
// Βοηθητικό εργαλείο για μέτρηση χρόνου εκτέλεσης σταδίων με chrono

#pragma once

#include <chrono>
#include <string>

// Απλός timer τύπου RAII για μέτρηση διάρκειας ενός scope
class ScopedTimer {
public:
    explicit ScopedTimer(double& output_ms)
        : output_ms_(output_ms),
          start_(std::chrono::high_resolution_clock::now()) {}

    ~ScopedTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        output_ms_ =
            std::chrono::duration<double, std::milli>(end - start_).count();
    }

private:
    double& output_ms_; // Μεταβλητή όπου αποθηκεύεται ο χρόνος (ms)
    std::chrono::high_resolution_clock::time_point start_; // Σημείο εκκίνησης
};