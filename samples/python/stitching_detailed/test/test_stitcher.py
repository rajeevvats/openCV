import unittest
import os
import sys

import numpy as np
import cv2 as cv

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__),
                                                '..', '..')))

from stitching_detailed.stitcher import Stitcher
# %%


class TestStitcher(unittest.TestCase):

    def test_stitcher_aquaduct(self):
        result = Stitcher.stitch(["s1.jpg", "s2.jpg"], n_features=250)
        cv.imwrite("result.jpg", result)

        max_image_shape_derivation = 3
        np.testing.assert_allclose(result.shape[:2],
                                   (700, 1811),
                                   atol=max_image_shape_derivation)

    @unittest.skip("skip boat test (high resuolution ran >30s)")
    def test_stitcher_boat1(self):
        settings = {"warp": "fisheye",
                    "wave_correct": "no",
                    "seam": "dp_colorgrad",
                    "expos_comp": "no",
                    "conf_thresh": 0.3}

        result = Stitcher.stitch(["boat5.jpg", "boat2.jpg",
                                  "boat3.jpg", "boat4.jpg",
                                  "boat1.jpg", "boat6.jpg"], **settings)

        cv.imwrite("boat_fisheye.jpg", result)

        max_image_shape_derivation = 600
        np.testing.assert_allclose(result.shape[:2],
                                   (14488,  7556),
                                   atol=max_image_shape_derivation)

    @unittest.skip("skip boat test (high resuolution ran >30s)")
    def test_stitcher_boat2(self):
        settings = {"warp": "compressedPlaneA2B1",
                    "seam": "dp_colorgrad",
                    "expos_comp": "channel_blocks",
                    "conf_thresh": 0.3}

        result = Stitcher.stitch(["boat5.jpg", "boat2.jpg",
                                  "boat3.jpg", "boat4.jpg",
                                  "boat1.jpg", "boat6.jpg"], **settings)

        cv.imwrite("boat_plane.jpg", result)

        max_image_shape_derivation = 600
        np.testing.assert_allclose(result.shape[:2],
                                   (7400, 12340),
                                   atol=max_image_shape_derivation)

    def test_stitcher_boat_aquaduct_subset(self):
        settings = {"compose_megapix": 1}

        result = Stitcher.stitch(["boat5.jpg",
                                  "s1.jpg", "s2.jpg",
                                  "boat2.jpg",
                                  "boat3.jpg", "boat4.jpg",
                                  "boat1.jpg", "boat6.jpg"], **settings)

        cv.imwrite("subset_low_res.jpg", result)

        max_image_shape_derivation = 100
        np.testing.assert_allclose(result.shape[:2],
                                   (839, 3384),
                                   atol=max_image_shape_derivation)

    def test_stitcher_budapest(self):
        settings = {"matcher": "affine",
                    "estimator": "affine",
                    "ba": "affine",
                    "warp": "affine",
                    "wave_correct": "no",
                    "conf_thresh": 0.3}

        result = Stitcher.stitch(["budapest1.jpg", "budapest2.jpg",
                                  "budapest3.jpg", "budapest4.jpg",
                                  "budapest5.jpg", "budapest6.jpg"],
                                 **settings)

        cv.imwrite("budapest.jpg", result)

        max_image_shape_derivation = 30
        np.testing.assert_allclose(result.shape[:2],
                                   (1155, 2310),
                                   atol=max_image_shape_derivation)


def starttest():
    unittest.main()


if __name__ == "__main__":
    starttest()
