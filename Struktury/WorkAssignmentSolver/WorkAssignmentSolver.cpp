#include "WorkAssignmentSolver.h"

#include "../Funkcje/distance.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace {
const int DISTANCE_SCALE = 1000;
}

WorkAssignment::WorkAssignment(int dwarfId, int mineId, const std::string& resourceType,
                               double distance, bool preferredResource)
    : dwarfId(dwarfId),
      mineId(mineId),
      resourceType(resourceType),
      distance(distance),
      preferredResource(preferredResource) {}

WorkAssignmentResult::WorkAssignmentResult()
    : assignedCount(0),
      preferredAssignedCount(0),
      totalDistance(0.0),
      optimizationCost(0) {}

bool WorkAssignmentResult::allDwarvesAssigned() const {
    return unassignedDwarfIds.empty();
}

WorkAssignmentSolver::WorkAssignmentSolver(const std::vector<Krasnal>& dwarves,
                                           const std::vector<Kopalnia>& mines)
    : dwarves(dwarves), mines(mines) {}

int WorkAssignmentSolver::sourceNode() const {
    return 0;
}

int WorkAssignmentSolver::sinkNode() const {
    return 1 + static_cast<int>(dwarves.size()) + static_cast<int>(mines.size());
}

int WorkAssignmentSolver::dwarfNode(size_t dwarfIndex) const {
    return 1 + static_cast<int>(dwarfIndex);
}

int WorkAssignmentSolver::mineNode(size_t mineIndex) const {
    return 1 + static_cast<int>(dwarves.size()) + static_cast<int>(mineIndex);
}

bool WorkAssignmentSolver::canWorkAtMine(const Krasnal& dwarf, const Kopalnia& mine) const {
    const std::vector<std::string>& skills = dwarf.getSkills();
    return std::find(skills.begin(), skills.end(), mine.getResourceType()) != skills.end();
}

bool WorkAssignmentSolver::isPreferredMine(const Krasnal& dwarf, const Kopalnia& mine) const {
    return dwarf.getPreferredResource() == mine.getResourceType();
}

double WorkAssignmentSolver::calculateTravelDistance(const Krasnal& dwarf, const Kopalnia& mine) const {
    return std::sqrt(distance(dwarf.getHome(), mine.getLocation()));
}

int WorkAssignmentSolver::calculateTravelCost(const Krasnal& dwarf, const Kopalnia& mine) const {
    return static_cast<int>(std::round(calculateTravelDistance(dwarf, mine) * DISTANCE_SCALE));
}

int WorkAssignmentSolver::calculatePreferencePenalty() const {
    int maxTravelCost = 0;

    for (const Krasnal& dwarf : dwarves) {
        for (const Kopalnia& mine : mines) {
            if (canWorkAtMine(dwarf, mine)) {
                maxTravelCost = std::max(maxTravelCost, calculateTravelCost(dwarf, mine));
            }
        }
    }

    if (maxTravelCost == 0) {
        return 1;
    }

    const long long penalty = static_cast<long long>(maxTravelCost) *
                              static_cast<long long>(dwarves.size()) + 1;

    if (penalty > std::numeric_limits<int>::max()) {
        return std::numeric_limits<int>::max();
    }

    return static_cast<int>(penalty);
}

void WorkAssignmentSolver::clearAssignments() {
    for (Krasnal& dwarf : dwarves) {
        dwarf.setAssignedToPreferredResource(false);
    }

    for (Kopalnia& mine : mines) {
        mine.setAssignedDwarves(std::vector<int>{});
    }
}

void WorkAssignmentSolver::buildGraph(Graph& graph, int preferencePenalty) const {
    for (size_t dwarfIndex = 0; dwarfIndex < dwarves.size(); dwarfIndex++) {
        graph.add_edge(sourceNode(), dwarfNode(dwarfIndex), 1, 0);
    }

    for (size_t dwarfIndex = 0; dwarfIndex < dwarves.size(); dwarfIndex++) {
        for (size_t mineIndex = 0; mineIndex < mines.size(); mineIndex++) {
            const Krasnal& dwarf = dwarves[dwarfIndex];
            const Kopalnia& mine = mines[mineIndex];

            if (!canWorkAtMine(dwarf, mine)) {
                continue;
            }

            long long cost = calculateTravelCost(dwarf, mine);
            if (!isPreferredMine(dwarf, mine)) {
                cost += preferencePenalty;
            }

            if (cost > std::numeric_limits<int>::max()) {
                cost = std::numeric_limits<int>::max();
            }

            graph.add_edge(dwarfNode(dwarfIndex), mineNode(mineIndex), 1, static_cast<int>(cost));
        }
    }

    for (size_t mineIndex = 0; mineIndex < mines.size(); mineIndex++) {
        graph.add_edge(mineNode(mineIndex), sinkNode(), mines[mineIndex].getCapacity(), 0);
    }
}

void WorkAssignmentSolver::extractAssignments(const Graph& graph, WorkAssignmentResult& result) {
    const int firstMineNode = mineNode(0);
    const int lastMineNode = sinkNode() - 1;

    std::vector<bool> assigned(dwarves.size(), false);

    for (size_t dwarfIndex = 0; dwarfIndex < dwarves.size(); dwarfIndex++) {
        const int currentDwarfNode = dwarfNode(dwarfIndex);

        for (const Edge& edge : graph.graph[currentDwarfNode]) {
            if (edge.flow <= 0 || edge.to < firstMineNode || edge.to > lastMineNode) {
                continue;
            }

            const size_t mineIndex = static_cast<size_t>(edge.to - firstMineNode);
            const Krasnal& dwarf = dwarves[dwarfIndex];
            const Kopalnia& mine = mines[mineIndex];
            const bool preferred = isPreferredMine(dwarf, mine);
            const double realDistance = calculateTravelDistance(dwarf, mine);

            result.assignments.emplace_back(dwarf.getId(), mine.getId(), mine.getResourceType(),
                                            realDistance, preferred);
            result.assignedCount++;
            result.totalDistance += realDistance;

            if (preferred) {
                result.preferredAssignedCount++;
            }

            dwarves[dwarfIndex].setAssignedToPreferredResource(preferred);
            mines[mineIndex].addDwarf(dwarf.getId());
            assigned[dwarfIndex] = true;
            break;
        }
    }

    for (size_t dwarfIndex = 0; dwarfIndex < dwarves.size(); dwarfIndex++) {
        if (!assigned[dwarfIndex]) {
            result.unassignedDwarfIds.push_back(dwarves[dwarfIndex].getId());
        }
    }
}

WorkAssignmentResult WorkAssignmentSolver::solve() {
    clearAssignments();

    WorkAssignmentResult result;
    if (dwarves.empty() || mines.empty()) {
        for (const Krasnal& dwarf : dwarves) {
            result.unassignedDwarfIds.push_back(dwarf.getId());
        }
        return result;
    }

    const int vertexCount = sinkNode() + 1;
    Graph graph(vertexCount);
    const int preferencePenalty = calculatePreferencePenalty();

    buildGraph(graph, preferencePenalty);

    const int maxFlow = static_cast<int>(dwarves.size());
    MinCostMaxFlowResult flowResult = min_cost_max_flow(graph, sourceNode(), sinkNode(), maxFlow);
    result.optimizationCost = flowResult.cost;

    extractAssignments(graph, result);

    return result;
}

const std::vector<Krasnal>& WorkAssignmentSolver::getDwarves() const {
    return dwarves;
}

const std::vector<Kopalnia>& WorkAssignmentSolver::getMines() const {
    return mines;
}
