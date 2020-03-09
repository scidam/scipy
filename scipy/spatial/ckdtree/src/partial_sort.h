#ifndef CKDTREE_PARTIAL_SORT
#define CKDTREE_PARTIAL_SORT

/* Adapted version of the code originally 
 * written by @jiefangxuanyan for scikit-learn.
 *
 */


#include <algorithm>

class IndexComparator {
    const double *data;
    ckdtree_intp_t split_dim, n_features;

public:
    IndexComparator(const double *data, ckdtree_intp_t split_dim, ckdtree_intp_t n_features):
        data(data), split_dim(split_dim), n_features(n_features) {}

    bool operator()(ckdtree_intp_t a, ckdtree_intp_t b) {
        return data[a * n_features + split_dim]
            < data[b * n_features + split_dim];
    }
};

int partition_node_indices(const double *data,
                           ckdtree_intp_t *node_indices,
                           ckdtree_intp_t split_dim,
                           ckdtree_intp_t split_index,
                           ckdtree_intp_t n_features,
                           ckdtree_intp_t n_points) {

    /* Partition points in the node into two equal-sized groups
     * Upon return, the values in node_indices will be rearranged such that
     * (assuming numpy-style indexing):
     *
     *   data[node_indices[0:split_index], split_dim]
     *     <= data[node_indices[split_index], split_dim]
     *
     * and
     *
     *   data[node_indices[split_index], split_dim]
     *     <= data[node_indices[split_index:n_points], split_dim]
     *
     * This is eassentially a wrapper around a standard C++ function 
     * ``std::nth_element``.
     * 
     *
     * Parameters
     * ----------
     * data : double pointer
     *    Pointer to a 2D array of the training data, of shape [N, n_features].
     *    N must be greater than any of the values in node_indices.
     * node_indices : int pointer
     *    Pointer to a 1D array of length n_points.  This lists the indices of
     *    each of the points within the current node.  This will be modified
     *    in-place.
     * split_dim : int
     *    the dimension on which to split.  This will usually be computed via
     *    the routine ``find_node_split_dim``
     * split_index : int
     *    the index within node_indices around which to split the points.
     *
     * Returns
     * -------
     * status : int
     *    integer exit status.  On return, the contents of node_indices are
     *    modified as noted above.
     */

    IndexComparator index_comparator(data, split_dim, n_features);
    std::nth_element(node_indices,
                     node_indices + split_index,
                     node_indices + n_points,
                     index_comparator);
    return 0;
}

#endif
