#ifndef WORKASSIGNMENTSOLVER_H
#define WORKASSIGNMENTSOLVER_H

#include "../Kopalnia.h"
#include "../Krasnal.h"
#include "../MinCostMaxFlow.h"

#include <string>
#include <vector>

struct WorkAssignment {
    int dwarfId;
    int mineId;
    std::string resourceType;
    double distance;
    bool preferredResource;

    WorkAssignment(int dwarfId, int mineId, const std::string& resourceType,
                   double distance, bool preferredResource);
};

struct WorkAssignmentResult {
    std::vector<WorkAssignment> assignments;
    std::vector<int> unassignedDwarfIds;
    int assignedCount;
    int preferredAssignedCount;
    double totalDistance;
    long long optimizationCost;

    WorkAssignmentResult();
    bool allDwarvesAssigned() const;
};

class WorkAssignmentSolver {
private:
    std::vector<Krasnal> dwarves;
    std::vector<Kopalnia> mines;

    int sourceNode() const;
    int sinkNode() const;
    int dwarfNode(size_t dwarfIndex) const;
    int mineNode(size_t mineIndex) const;

    bool canWorkAtMine(const Krasnal& dwarf, const Kopalnia& mine) const;
    bool isPreferredMine(const Krasnal& dwarf, const Kopalnia& mine) const;
    double calculateTravelDistance(const Krasnal& dwarf, const Kopalnia& mine) const;
    int calculateTravelCost(const Krasnal& dwarf, const Kopalnia& mine) const;
    int calculatePreferencePenalty() const;

    void clearAssignments();
    void buildGraph(Graph& graph, int preferencePenalty) const;
    void extractAssignments(const Graph& graph, WorkAssignmentResult& result);

public:
    WorkAssignmentSolver(const std::vector<Krasnal>& dwarves,
                         const std::vector<Kopalnia>& mines);

    WorkAssignmentResult solve();

    const std::vector<Krasnal>& getDwarves() const;
    const std::vector<Kopalnia>& getMines() const;
};

#endif
