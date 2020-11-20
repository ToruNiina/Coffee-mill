#ifndef COFFEE_MILL_COMMON_TOPOLOGY_HPP
#define COFFEE_MILL_COMMON_TOPOLOGY_HPP
#include <mill/common/Attribute.hpp>
#include <vector>
#include <stack>
#include <string>
#include <map>
#include <memory>

namespace mill
{

class Topology
{
  public:
    using real_type                = double;
    using attribute_type           = Attribute;
    using attribute_container_type = std::map<std::string, attribute_type>;

    // {name, {neighbors}}
    using node_type = std::map<std::string, std::vector<std::size_t>>;

  public:

    explicit Topology(std::size_t N): nodes_(N) {}
    Topology() = default;
    ~Topology() = default;
    Topology(Topology const&) = default;
    Topology(Topology &&)     = default;
    Topology& operator=(Topology const&) = default;
    Topology& operator=(Topology &&)     = default;

    void resize(const std::size_t N) {nodes_.resize(N);}
    void clear() {nodes_.clera();}
    std::size_t size() const noexcept {return nodes_.size();}

    std::vector<std::size_t> const&
    list_connected(const std::size_t i, const std::string& name) const noexcept
    {
        return nodes_.at(i).at(name);
    }

    void add_edge  (const std::size_t i, const std::size_t j, const std::string& kind)
    {
        {
            auto& neighbors = nodes_.at(i)[kind];
            if(std::find(neighbors.begin(), neighbors.end(), j) == neighbors.end())
            {
                neighbors.push_back(j);
            }
        }
        {
            auto& neighbors = nodes_.at(j)[kind];
            if(std::find(neighbors.begin(), neighbors.end(), i) == neighbors.end())
            {
                neighbors.push_back(i);
            }
        }
    }
    void erase_edge(const std::size_t i, const std::size_t j, const std::string& kind)
    {
        {
            auto& neighbors = nodes_.at(i)[kind];
            neighbors.erase(std::find(neighbors.begin(), neighbors.end(), j));
        }
        {
            auto& neighbors = nodes_.at(j)[kind];
            neighbors.erase(std::find(neighbors.begin(), neighbors.end(), i));
        }
    }

    bool has_edge  (const std::size_t i, const std::size_t j, const std::string& kind) const
    {
        const auto& neighbors = nodes_.at(i).at(kind);
        return std::find(neighbors.begin(), neighbors.end(), j) != neighbors.end();
    }
    std::vector<std::string> edges_between(const std::size_t i, const std::size_t j) const
    {
        std::vector<std::string> edges;
        for(const auto& [kind, neighbors] : nodes_.at(i))
        {
            if(std::find(neighbors.begin(), neighbors.end(), j) != neighbors.end())
            {
                edges.push_back(kind);
            }
        }
        return edges;
    }

  private:
    std::vector<node_type> nodes_;
};

} // mill
#endif // COFFEE_MILL_COMMON_SNAPSHOT_HPP
