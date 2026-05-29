#include "GuardCommandSolver.h"

#include <limits>
#include <stdexcept>

GuardCommandResult::GuardCommandResult()
    : index(-1), guardId(-1), loudness(0), found(false) {}

GuardCommandResult::GuardCommandResult(int index, int guardId, int loudness)
    : index(index), guardId(guardId), loudness(loudness), found(true) {}

GuardCommandSolver::GuardCommandSolver(const std::vector<Straznik>& guards)
    : guards(guards), loudnessTree(nullptr) {
    if (!this->guards.empty()) {
        loudnessTree.reset(new SegmentTree(buildEncodedLoudnessValues()));
    }
}

std::vector<int> GuardCommandSolver::buildEncodedLoudnessValues() const {
    std::vector<int> values;
    values.reserve(guards.size());

    for (size_t i = 0; i < guards.size(); i++) {
        values.push_back(encodeValue(i, guards[i].getLoudness()));
    }

    return values;
}

int GuardCommandSolver::encodeValue(size_t index, int loudness) const {
    if (loudness < 0 || index >= guards.size()) {
        throw std::invalid_argument("Invalid guard loudness value");
    }

    const long long base = static_cast<long long>(guards.size()) + 1;
    const long long tieBreaker = static_cast<long long>(guards.size()) - static_cast<long long>(index);
    const long long encoded = static_cast<long long>(loudness) * base + tieBreaker;

    if (encoded > std::numeric_limits<int>::max()) {
        throw std::overflow_error("Guard loudness value is too large");
    }

    return static_cast<int>(encoded);
}

int GuardCommandSolver::decodeIndex(int encodedValue) const {
    if (guards.empty()) {
        return -1;
    }

    const int base = static_cast<int>(guards.size()) + 1;
    const int tieBreaker = encodedValue % base;
    return static_cast<int>(guards.size()) - tieBreaker;
}

bool GuardCommandSolver::isValidRange(int left, int right) const {
    return left >= 0 &&
           right >= left &&
           right < static_cast<int>(guards.size()) &&
           loudnessTree != nullptr;
}

GuardCommandResult GuardCommandSolver::findLoudestGuard(int left, int right) const {
    if (!isValidRange(left, right)) {
        return GuardCommandResult();
    }

    const int encodedResult = loudnessTree->query(left, right);
    const int index = decodeIndex(encodedResult);

    if (index < left || index > right) {
        return GuardCommandResult();
    }

    return GuardCommandResult(index, guards[index].getId(), guards[index].getLoudness());
}

bool GuardCommandSolver::updateLoudness(int index, int newLoudness) {
    if (index < 0 ||
        index >= static_cast<int>(guards.size()) ||
        newLoudness < 0 ||
        loudnessTree == nullptr) {
        return false;
    }

    guards[index].setLoudness(newLoudness);
    loudnessTree->update(index, encodeValue(static_cast<size_t>(index), newLoudness));

    return true;
}

int GuardCommandSolver::size() const {
    return static_cast<int>(guards.size());
}

const std::vector<Straznik>& GuardCommandSolver::getGuards() const {
    return guards;
}
