//
// Created by ivan on 11.04.17.
//

#ifndef GRAPH_FILTERED_GRAPH_H
#define GRAPH_FILTERED_GRAPH_H

#include "graph.h"

namespace au {

template<class graph, class v_filter, class e_filter>
struct filtered_graph {

    using vertex_data = typename graph::vertex_data;
    using edge_data = typename graph::edge_data;
    using const_graph_vertex_iterator = typename graph::vertex_const_iterator;
    using const_graph_edge_iterator = typename graph::edge_const_iterator;
    using vertex_value_type = typename const_graph_vertex_iterator::value_type;


    using vertex_const_iterator = advanced_iter
            <
            const_graph_vertex_iterator,
            VertexPolicy<const_graph_vertex_iterator, vertex_value_type>,
            Dereference<const_graph_vertex_iterator>,
            DefaultPred<const_graph_vertex_iterator>>;

    using edge_const_iterator = advanced_iter
            <
            const_graph_edge_iterator,
            FilteredEdgePolicy<const_graph_edge_iterator, const_graph_vertex_iterator>,
            Dereference<const_graph_edge_iterator>,
            DefaultPred<const_graph_edge_iterator>>;


    struct vertex_predicate {
        vertex_predicate(std::function<bool(vertex_data)> const& pred,
                         const_graph_vertex_iterator const& iter)
                : pred_(pred), end_(iter)
        {}

        bool operator()(const_graph_vertex_iterator const& it) const {
            return (it == end_) || pred_(*it);
        }

        std::function<bool(vertex_data)> pred_;
        const_graph_vertex_iterator end_;
    };

    struct edge_predicate {
        edge_predicate(std::function<bool(vertex_data)> const& vertex_pred,
                       std::function<bool(edge_data)> const& edge_pred,
                       const_graph_edge_iterator const& iter)

                : v_pred_(vertex_pred), e_pred_(edge_pred), end_(iter)
        {}

        bool operator()(const_graph_edge_iterator const& it) const {
            return (it == end_) || (e_pred_(*it) && v_pred_(*it.from()) && v_pred_(*it.to()));
        }

        std::function<bool(vertex_data)> v_pred_;
        std::function<bool(edge_data)> e_pred_;
        const_graph_edge_iterator end_;
    };

    filtered_graph(graph const& gh, v_filter const& v_fil, e_filter const& e_fil)
            : graph_(gh), v_filter_(v_fil), e_filter_(e_fil) {}


    vertex_const_iterator find_vertex(typename graph::vertex_data const& data) const {
        if (v_filter_(data)) {
            return vertex_const_iterator(graph_.find_vertex(data), vertex_predicate(v_filter_, graph_.vertex_end()));
        } else {
            return vertex_const_iterator(graph_.vertex_end(), vertex_predicate(v_filter_, graph_.vertex_end()));
        }
    }

    edge_const_iterator find_edge(vertex_const_iterator const& from, vertex_const_iterator const& to) const {
        if (from == vertex_end() || to == vertex_end()) {
            return edge_const_iterator();
        }

        auto from_vertex = *from;
        auto to_vertex = *to;
        auto from_iter = graph_.find_vertex(from_vertex);
        auto to_iter = graph_.find_vertex(to_vertex);

        auto it = graph_.find_edge(from_iter, to_iter);

        if (it == graph_.edge_end(from_iter)) {
            return edge_const_iterator();
        }

        if (e_filter_(*it) && v_filter_(*it.from() && v_filter_(*it.to()))) {
            return edge_const_iterator(it, edge_predicate(v_filter_, e_filter_, graph_.edge_end(from_iter)));
        }
        return edge_const_iterator(graph_.edge_end(from_iter), edge_predicate(v_filter_, e_filter_, graph_.edge_end(from_iter)));
    }

    vertex_const_iterator vertex_begin() const {
        if (graph_.vertex_begin() != graph_.vertex_end()) {

            vertex_const_iterator v_it(graph_.vertex_begin(), vertex_predicate(v_filter_, graph_.vertex_end()));

            if (!v_filter_(*v_it)) {
                ++v_it;
            }
            return v_it;
        }
        return vertex_const_iterator(graph_.vertex_end(), vertex_predicate(v_filter_, graph_.vertex_end()));
    }

    vertex_const_iterator vertex_end() const {
        return vertex_const_iterator(graph_.vertex_end(), vertex_predicate(v_filter_, graph_.vertex_end()));
    }

    edge_const_iterator edge_begin(vertex_const_iterator const& from) const {

        if (from == vertex_end()) {
            return edge_const_iterator();
        }

        auto const from_data = *from;

        typename graph::vertex_const_iterator const from_ = graph_.find_vertex(from_data);

        if (graph_.edge_begin(from_) != graph_.edge_end(from_)) {
            edge_const_iterator e_it(graph_.edge_begin(from_), edge_predicate(v_filter_, e_filter_, graph_.edge_end(from_)));

            if (!e_filter_(*e_it) || !v_filter_(*e_it.from()) || !v_filter_(*e_it.to())) {
                ++e_it;
            }

            return e_it;
        }
        return edge_const_iterator(graph_.edge_end(from_), edge_predicate(v_filter_, e_filter_, graph_.edge_end(from_)));
    }

    edge_const_iterator edge_end(vertex_const_iterator const& from) const {
        if (from == vertex_const_iterator(graph_.vertex_end())) {
            return edge_const_iterator();
        }

        auto const from_data = *from;
        typename graph::vertex_const_iterator const from_ = graph_.find_vertex(from_data);

        return edge_const_iterator(graph_.edge_end(from_), edge_predicate(v_filter_, e_filter_, graph_.edge_end(from_)));
    }

private:
    graph const& graph_;
    v_filter v_filter_;
    e_filter e_filter_;
};

} // au

#endif //GRAPH_FILTERED_GRAPH_H