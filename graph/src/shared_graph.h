//
// Created by ivan on 21.04.17.
//

#ifndef GRAPH_SHARED_GRAPH_H
#define GRAPH_SHARED_GRAPH_H

#include <unordered_set>
#include <unordered_map>
#include <list>
#include "iterator.hpp"
#include <typeinfo>
#include <memory>
#include <iostream>

namespace au {

    struct sh_ptr_cont {
        struct hash {
            template<class T>
            size_t operator() (std::shared_ptr<T> const& p) const {
                return std::hash<T>()(*p);
            }
        };

        struct compare {
            template<class T>
            bool operator() (std::shared_ptr<T> const& lhs,
                             std::shared_ptr<T> const& rhs) const {
                return *lhs == *rhs;
            }
        };
    };

    template<class v_data, class e_data>
    struct shared_graph {
        using vertex_data = v_data;
        using edge_data = e_data;

        using vertex_shared = std::shared_ptr<vertex_data>;

        using vertex_set = std::unordered_set
                <
                        vertex_shared,
                        sh_ptr_cont::hash,
                        sh_ptr_cont::compare>;

        using vertex_map = std::unordered_map
                <
                        vertex_shared,
                        vertex_set,
                        sh_ptr_cont::hash,
                        sh_ptr_cont::compare>;

        using vertex_cont_const_iterator = typename vertex_set::const_iterator;
        using vertex_value_type = typename vertex_cont_const_iterator::value_type::element_type;

        using vertex_const_iterator = advanced_iter
                <
                        vertex_cont_const_iterator,
                        VertexPolicy<vertex_cont_const_iterator, vertex_value_type>,
                        VertexDereference<vertex_cont_const_iterator>,
                        DefaultPred<vertex_cont_const_iterator>>;

        using vertex_iterator = vertex_const_iterator;

        struct edge {

            edge() : from_(), to_(), data_() {}

            edge(vertex_shared const& from,
                 vertex_shared const& to,
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
            vertex_shared from_;
            vertex_shared to_;
            std::shared_ptr<vertex_set> vertices_;

            edge_data data_;
        };

        using edge_cont = std::unordered_map<vertex_shared,
                edge,
                sh_ptr_cont::hash,
                sh_ptr_cont::compare>;

        using edge_cover_cont = std::unordered_map<vertex_shared,
                edge_cont,
                sh_ptr_cont::hash,
                sh_ptr_cont::compare>;

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

        shared_graph() = default;

        shared_graph(shared_graph&& other) : shared_graph() {
            move_swap(std::move(other));
        }

        shared_graph(shared_graph const& other) : shared_graph() {
            for (vertex_shared shared : *(other.vertices_)) {
                add_vertex(*shared);
            }

            for (auto from_cont : other.edges_) {
                for (auto to_cont : from_cont.second) {

                    auto from_ = find_vertex(*to_cont.second.from());
                    auto to_ = find_vertex(*to_cont.second.to());

                    add_edge(from_, to_, to_cont.second.data());
                }
            }
        }

        void move_swap(shared_graph&& other) {
            vertices_ = std::move(other.vertices_);
            edges_ = std::move(other.edges_);
        }

        shared_graph& operator=(shared_graph other) {
            move_swap(std::move(other));
            return *this;
        }

        vertex_iterator add_vertex(vertex_data const& data) {
            vertex_cont_const_iterator iter;
            bool inserted = false;

            std::tie(iter, inserted) = vertices_->insert(vertex_data_to_shared(data));
            edges_[vertex_data_to_shared(data)];

            return vertex_iterator(iter);
        }

        void remove_vertex(vertex_iterator const& it) {
            if (it == vertex_end()) {
                return;
            }

            auto ver_shared = vertex_data_to_shared(*it);

            // delete incoming edges

            for (auto& from_iter : edges_) {
                from_iter.second.erase(ver_shared);
            }

            // delete outcoming edges
            edges_.erase(ver_shared);

            // delete vertex from vertex_set
            vertices_->erase(ver_shared);
        }

        vertex_iterator find_vertex(vertex_data const& data) {
            return vertex_iterator(vertices_->find(vertex_data_to_shared(data)));
        }

        vertex_const_iterator find_vertex(vertex_data const& data) const {
            return vertex_const_iterator(vertices_->find(vertex_data_to_shared(data)));
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

            vertex_shared from_sh = *vertices_->find(vertex_data_to_shared(*from));
            vertex_shared to_sh = *vertices_->find(vertex_data_to_shared(*to));

            return add_edge_to_cont(from_sh, to_sh, data);
        }

        void remove_edge(edge_iterator const& it) {
            if (it == edge_iterator() || it == edge_end(it.from())) {
                return;
            }

            vertex_shared from_sh = vertex_data_to_shared(*it.from());
            vertex_shared to_sh = vertex_data_to_shared(*it.to());

            remove_edge_shared(from_sh, to_sh);
        }

        edge_iterator find_edge(vertex_iterator const& from,
                                vertex_iterator const& to) {

            if (from == vertex_end() || to == vertex_end()) {
                return edge_iterator();
            }

            vertex_shared from_sh = vertex_data_to_shared(*from);
            vertex_shared to_sh = vertex_data_to_shared(*to);

            auto from_cont = edges_.find(from_sh);

            if (from_cont == edges_.end()) {
                return edge_iterator();
            }

            auto it = from_cont->second.find(to_sh);

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

            vertex_shared from_sh = vertex_data_to_shared(*from);
            vertex_shared to_sh = vertex_data_to_shared(*to);

            auto from_cont = edges_.find(from_sh);

            if (from_cont == edges_.end()) {
                return edge_const_iterator();
            }

            return edge_const_iterator(from_cont->second.find(to_sh));
        }

        edge_iterator edge_begin(vertex_iterator const& from) {
            if (from == vertex_end()) {
                return edge_iterator();
            }

            auto shared = vertex_data_to_shared(*from);

            return edge_iterator(edges_.find(shared)->second.begin());
        }

        edge_iterator edge_end(vertex_iterator const& from) {
            if (from == vertex_end()) {
                return edge_iterator();
            }

            auto shared = vertex_data_to_shared(*from);

            return edge_iterator(edges_.find(shared)->second.end());
        }

        edge_const_iterator edge_begin(vertex_const_iterator const& from) const {
            if (from == vertex_end()) {
                return edge_const_iterator();
            }

            auto shared = vertex_data_to_shared(*from);
            return edge_const_iterator(edges_.find(shared)->second.begin());
        }

        edge_const_iterator edge_end(vertex_const_iterator const& from) const {
            if (from == vertex_end()) {
                return edge_const_iterator();
            }

            auto shared = vertex_data_to_shared(*from);
            return edge_const_iterator(edges_.find(shared)->second.end());
        }

    private:

        void remove_edge_shared(vertex_shared const& from,
                                vertex_shared const& to) {

            auto from_cont = edges_.find(from);

            if (from_cont != edges_.end()) {
                edge_cont& to_cont = from_cont->second;

                to_cont.erase(to_cont.find(to));
            }
        }

        vertex_shared vertex_data_to_shared(vertex_data const& data) const {
            return std::make_shared<vertex_data>(data);
        }


        edge_iterator add_edge_to_cont(vertex_shared const& from,
                                       vertex_shared const& to,
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

#endif //GRAPH_SHARED_GRAPH_H
