from itertools import chain
import cv2 as cv
import numpy as np

from stitching_detailed.feature_matcher import FeatureMatcher


class Subsetter:

    DEFAULT_CONFIDENCE_THRESHOLD = 1
    DEFAULT_MATCHES_GRAPH_DOT_FILE = None

    def __init__(self,
                 confidence_threshold=DEFAULT_CONFIDENCE_THRESHOLD,
                 matches_graph_dot_file=DEFAULT_MATCHES_GRAPH_DOT_FILE):
        self.confidence_threshold = confidence_threshold
        self.save_file = matches_graph_dot_file

    def subset(self,
               img_names,
               features,
               pairwise_matches):

        indices = self.get_indices_to_keep(features, pairwise_matches)
        indices_to_delete = self.get_indices_to_delete(len(features),
                                                       indices)

        img_names_subset = Subsetter.subset_list(img_names, indices)
        feature_subset = Subsetter.subset_list(features, indices)

        matches_matrix = FeatureMatcher.get_matches_matrix(pairwise_matches)
        matches_matrix_subset = Subsetter.subset_matrix(matches_matrix,
                                                        indices_to_delete)
        matches_subset = Subsetter.__matrix_rows_to_list(matches_matrix_subset)

        if self.save_file:
            with open(self.save_file, 'w') as filehandler:
                filehandler.write(self.get_matches_graph_dot_file(
                    img_names,
                    pairwise_matches
                    ))

        return img_names_subset, feature_subset, matches_subset

    def get_indices_to_keep(self, features, pairwise_matches):
        indices = cv.detail.leaveBiggestComponent(features,
                                                  pairwise_matches,
                                                  self.confidence_threshold)
        return [int(idx) for idx in list(indices[:, 0])]

    def get_indices_to_delete(self, list_lenght, indices_to_keep):
        return list(set(range(list_lenght)) - set(indices_to_keep))

    @staticmethod
    def subset_list(list_to_subset, indices):
        return [list_to_subset[i] for i in indices]

    @staticmethod
    def subset_matrix(matrix_to_subset, indices_to_delete):
        for idx, idx_to_delete in enumerate(indices_to_delete):
            matrix_to_subset = Subsetter.__delete_index_from_matrix(
                matrix_to_subset,
                idx_to_delete-idx  # matrix shape reduced by one at each step
                )

        return matrix_to_subset

    def get_matches_graph_dot_file(self, img_names, pairwise_matches):
        return cv.detail.matchesGraphAsString(
            img_names,
            pairwise_matches,
            self.confidence_threshold
            )

    def __delete_index_from_matrix(matrix, idx):
        mask = np.ones(matrix.shape[0], bool)
        mask[idx] = 0
        return matrix[mask, :][:, mask]

    def __matrix_rows_to_list(matrix):
        return list(chain.from_iterable(matrix.tolist()))
