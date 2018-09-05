//
// Created by ivan on 09.04.17.
//

#ifndef GRAPH_GRAPH_H
#define GRAPH_GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <list>
#include "iterator.hpp"
#include <typeinfo>
#include <memory>
#include <iostream>

namespace au {

    template<class v_data, class e_data>
    struct graph {
        using vertex_data = v_data;
        using edge_data = e_data;

        using vertex_set = std::unordered_set<vertex_data>;

        using vertex_map = std::unordered_map<vertex_data, vertex_set>;


        using vertex_cont_const_iterator = typename vertex_set::const_iterator;
        using vertex_value_type = typename vertex_cont_const_iterator::value_type;

        using vertex_const_iterator = advanced_iter
                <
                vertex_cont_const_iterator,
                VertexPolicy<vertex_cont_const_iterator, vertex_value_type>,
                Dereference<vertex_cont_const_iterator>,
                DefaultPred<vertex_cont_const_iterator>>;

        using vertex_iterator = vertex_const_iterator;

        struct edge {

            edge() : from_(), to_(), data_() {}

            edge(vertex_data const& from,
                 vertex_data const& to,
                 std::shared_ptr<vertex_set> vertices,
                 edge_data const& data)

                    : from_(from),
                      to_(to),
                      vertices_(vertices),
                      data_(data)
            {}

            vertex_const_iterator from() const{
                return vertex_const_iterator(vertices_->find(from_));
            }

            vertex_const_iterator to() const {
                return vertex_const_iterator(vertices_->find(to_));
            }

            edge_data& data() {
                return data_;
            }

            const edge_data& data() const {
                return data_;
            }

        private:
            vertex_data from_;
            vertex_data to_;
            std::shared_ptr<vertex_set> vertices_;

            edge_data data_;
        };

        using edge_cont = std::unordered_map<vertex_data, edge>;
        using edge_cover_cont = std::unordered_map<vertex_data, edge_cont>;

        using edge_cont_iterator = typename edge_cont::iterator;
        using edge_cont_const_iterator = typename edge_cont::const_iterator;

        using edge_iterator = advanced_iter
                <
                edge_cont_iterator,
                EdgePolicy<edge_cont_iterator, edge_data, vertex_iterator>,
                Dereference<edge_cont_iterator>,
                DefaultPred<edge_cont_iterator>>;

        using edge_const_iterator = advanced_iter
                <edge_cont_const_iterator,
                ConstEdgePolicy<edge_cont_const_iterator, edge_data, vertex_const_iterator>,
                Dereference<edge_cont_const_iterator>,
                DefaultPred<edge_cont_const_iterator>>;

        graph() = default;

        graph(graph&& other) : graph() {
            move_swap(std::move(other));
        }

        graph(graph const& other) : graph() {
            for (vertex_data vertex : *(other.vertices_)) {
                add_vertex(vertex);
            }

            edges_ = other.edges_;
        }

        void move_swap(graph&& other) {
            vertices_ = std::move(other.vertices_);
            edges_ = std::move(other.edges_);
        }

        graph& operator=(graph other) {
            move_swap(std::move(other));
            return *this;
        }

        vertex_iterator add_vertex(vertex_data const& data) {
            vertex_cont_const_iterator iter;
            bool inserted = false;

            std::tie(iter, inserted) = vertices_->insert(data);
            edges_[data];

            return vertex_iterator(iter);
        }

        void remove_vertex(vertex_iterator const& it) {
            if (it == vertex_end()) {
                return;
            }

            for (auto& from_iter : edges_) {
                from_iter.second.erase(*it);
            }

            edges_.erase(*it);

            vertices_->erase(*it);
        }

        vertex_iterator find_vertex(vertex_data const& data) {
            return vertex_iterator(vertices_->find(data));
        }

        vertex_const_iterator find_vertex(vertex_data const& data) const {
            return vertex_const_iterator(vertices_->find(data));
        }

        vertex_iterator vertex_begin() {
            return vertex_iterator(vertices_->begin());
        }

        vertex_const_iterator vertex_begin() const {
            return vertex_const_iterator(vertices_->begin());
        }

        vertex_iterator vertex_end() {
            return vertex_iterator(vertices_->end());
        }

        vertex_const_iterator vertex_end() const {
            return vertex_const_iterator(vertices_->end());
        }

        edge_iterator add_edge(vertex_iterator const& from,
                               vertex_iterator const& to,
                               edge_data const& data) {

            if (from == vertex_end() || to == vertex_end()) {
                return edge_iterator();
            }

            return add_edge_to_cont(*from, *to, data);
        }

        void remove_edge(edge_iterator const& it) {
            if (it == edge_iterator() || it == edge_end(it.from())) {
                return;
            }

            remove_edge_shared(*it.from(), *it.to());
        }

        edge_iterator find_edge(vertex_iterator const& from,
                                vertex_iterator const& to) {

            if (from == vertex_end() || to == vertex_end()) {
                return edge_iterator();
            }

            auto from_cont = edges_.find(*from);

            if (from_cont == edges_.end()) {
                return edge_iterator();
            }

            auto it = from_cont->second.find(*to);

            return edge_iterator(it);
        }

        void print_edges() {
            for (auto item : edges_) {
                std::cout << "first: " << *item.first << std::endl;
                for (auto item1 : item.second) {
                    std::cout << "     first: " << *item1.first << " " << item1.second.data() << std::endl;
                }
            }
        }

        edge_const_iterator find_edge(vertex_const_iterator const& from,
                                      vertex_const_iterator const& to) const {

            if (from == vertex_end() || to == vertex_end()) {
                return edge_const_iterator();
            }

            auto from_cont = edges_.find(*from);

            if (from_cont == edges_.end()) {
                return edge_const_iterator();
            }

            return edge_const_iterator(from_cont->second.find(*to));
        }

        edge_iterator edge_begin(vertex_iterator const& from) {
            if (from == vertex_end()) {
                return edge_iterator();
            }

            return edge_iterator(edges_.find(*from)->second.begin());
        }

        edge_iterator edge_end(vertex_iterator const& from) {
            if (from == vertex_end()) {
                return edge_iterator();
            }

            return edge_iterator(edges_.find(*from)->second.end());
        }

        edge_const_iterator edge_begin(vertex_const_iterator const& from) const {
            if (from == vertex_end()) {
                return edge_const_iterator();
            }

            return edge_const_iterator(edges_.find(*from)->second.begin());
        }

        edge_const_iterator edge_end(vertex_const_iterator const& from) const {
            if (from == vertex_end()) {
                return edge_const_iterator();
            }

            return edge_const_iterator(edges_.find(*from)->second.end());
        }

    private:

        void remove_edge_shared(vertex_data const& from,
                                vertex_data const& to) {

            auto from_cont = edges_.find(from);

            if (from_cont != edges_.end()) {
                edge_cont& to_cont = from_cont->second;

                to_cont.erase(to_cont.find(to));
            }
        }

        edge_iterator add_edge_to_cont(vertex_data const& from,
                                       vertex_data const& to,
                                       edge_data const& data) {

            auto from_cont = edges_.find(from);

            if (from_cont == edges_.end()) {
                edges_[from];
                from_cont = edges_.find(from);
            }

            edge_cont& to_cont = from_cont->second;

            to_cont[to] = edge(from, to, vertices_, data);

            return edge_iterator(to_cont.find(to));
        }

        std::shared_ptr<vertex_set> vertices_ = std::make_shared<vertex_set>();
        edge_cover_cont edges_;
    };

} // au


#endif //GRAPH_GRAPH_H