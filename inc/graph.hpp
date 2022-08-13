#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <map>
#include <compare>
#include <queue>
#include <atomic>
#include <mutex>
#include <cassert>
#include <functional>

// translation model from which protocols are derived (e.g. COBS, CLAF, etc)

/*
The vertex class, a generic data structure for points on a graph, needs the following:
std::unordered_set<std::string> - vert.p_trips(), to get parent vert trips
std::string - vert.trip(), to get the vert trip
std::size_t - std::hash(vert)
bool - (==)(vert A, vert B)
*/

template<class vertex>
struct linked {
    std::string trip;
    vertex ref;
    std::unordered_set<linked<vertex>*> parents;
    std::unordered_set<linked<vertex>*> children;
};

template<class vertex>
class graph_model {
    public:
        graph_model();

        linked<vertex> get_root();

        bool check_rooted();

        std::map<std::string, linked<vertex>> get_graph();

        void set_push_callback(
            std::function<void(
              std::map<
                std::string, 
                linked<vertex>>, 
              std::unordered_set<std::string>
            )> callback);

        void queue_unit(vertex to_queue);

        void queue_batch(std::unordered_set<vertex> to_queue);

        void queue_batch(std::vector<vertex> to_queue);
    protected:
        std::map<std::string, linked<vertex>> graph;

        linked<vertex>* graph_root;

        bool rooted;

        std::queue<std::unordered_set<vertex>> awaiting_push_batches;

        std::atomic<bool> push_proc_active = false;

        std::mutex push_proc_mtx;

        void link(std::string target);

        void batch_push(std::unordered_set<vertex> to_push, std::unordered_set<std::string> flags = std::unordered_set<std::string>());

        void push_proc();

        std::unordered_set<vertex> get_connected(std::unordered_set<vertex> to_check);

        virtual void graph_configure(vertex root) = 0;

        virtual std::unordered_set<vertex> get_valid(std::unordered_set<vertex> to_check) = 0;

        virtual void push_response(std::unordered_set<std::string> new_trips, std::unordered_set<std::string> flags = std::unordered_set<std::string>()) = 0;
};
