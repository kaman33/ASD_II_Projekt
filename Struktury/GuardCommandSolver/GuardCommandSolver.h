#ifndef GUARD_COMMAND_SOLVER_H
#define GUARD_COMMAND_SOLVER_H

#include "../SegmentTree.h"
#include "../Straznik.h"

#include <memory>
#include <vector>

struct GuardCommandResult {
    int index;
    int guardId;
    int loudness;
    bool found;

    GuardCommandResult();
    GuardCommandResult(int index, int guardId, int loudness);
};

class GuardCommandSolver {
private:
    std::vector<Straznik> guards;
    std::unique_ptr<SegmentTree> loudnessTree;

    std::vector<int> buildEncodedLoudnessValues() const;
    int encodeValue(size_t index, int loudness) const;
    int decodeIndex(int encodedValue) const;
    bool isValidRange(int left, int right) const;

public:
    explicit GuardCommandSolver(const std::vector<Straznik>& guards);

    GuardCommandResult findLoudestGuard(int left, int right) const;
    bool updateLoudness(int index, int newLoudness);

    int size() const;
    const std::vector<Straznik>& getGuards() const;
};

#endif
