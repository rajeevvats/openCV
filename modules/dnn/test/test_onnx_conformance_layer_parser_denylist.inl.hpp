// The file is autogenerated
// Update note: execute <opencv_extra>/testdata/dnn/onnx/generate_conformance_list.py <gtest_xml_file>
"test_adagrad",  // Issues::Layer::Can't create layer "onnx_node_output_0!X1_new" of type "ai.onnx.preview.training.Adagrad" in function 'getLayerInstance'
"test_adagrad_multiple", // ---- same as above ---
"test_adam",  // Issues::Layer::Can't create layer "onnx_node_output_0!X1_new" of type "ai.onnx.preview.training.Adam" in function 'getLayerInstance'
"test_adam_multiple", // ---- same as above ---
"test_basic_convinteger", // Issues::Layer::Can't create layer "onnx_node_output_0!y" of type "ConvInteger" in function 'getLayerInstance'
"test_batchnorm_epsilon", // Issue:: Unkonwn error::Blob mean not found in const blobs in function 'getBlob'
"test_batchnorm_epsilon_training_mode",  // ---- same as above ---
"test_batchnorm_example",  // ---- same as above ---
"test_batchnorm_example_training_mode",  // ---- same as above ---
"test_bernoulli", // Issues::Layer::Can't create layer "onnx_node_output_0!y" of type "Bernoulli" in function 'getLayerInstance'
"test_bernoulli_double", // ---- same as above ---
"test_bernoulli_double_expanded",  // ---- same as above ---
"test_bernoulli_expanded",  // ---- same as above ---
"test_bernoulli_seed", // ---- same as above ---
"test_bernoulli_seed_expanded",  // ---- same as above ---
"test_bitshift_left_uint16", // Issue::Unsuppoted data type
"test_bitshift_left_uint32", // Issue::Unsuppoted data type
"test_bitshift_left_uint64", // Issue::Unsuppoted data type
"test_bitshift_left_uint8", // Issues::Layer::Can't create layer "onnx_node_output_0!z" of type "BitShift" in function 'getLayerInstance'
"test_bitshift_right_uint16", // Issue::Unsuppoted data type
"test_bitshift_right_uint32", // Issue::Unsuppoted data type
"test_bitshift_right_uint64", // Issue::Unsuppoted data type
"test_bitshift_right_uint8", // Issues::Layer::Can't create layer "onnx_node_output_0!z" of type "BitShift" in function 'getLayerInstance'
"test_cast_BFLOAT16_to_FLOAT",  // Issue::Unsuppoted data type
"test_cast_DOUBLE_to_FLOAT16", // Issue::Unsuppoted data type
"test_cast_FLOAT16_to_DOUBLE", // Issue::Unsuppoted data type
"test_cast_FLOAT16_to_FLOAT", // Issue::Unsuppoted data type
"test_cast_FLOAT_to_BFLOAT16", // Issue::Unsuppoted data type
"test_cast_FLOAT_to_DOUBLE", // Issue::Unsuppoted data type
"test_cast_FLOAT_to_FLOAT16", // Issue::Unsuppoted data type
"test_castlike_BFLOAT16_to_FLOAT",  // Issue::Unsuppoted data type
"test_castlike_BFLOAT16_to_FLOAT_expanded",  // Issue::Unsuppoted data type
"test_castlike_DOUBLE_to_FLOAT", // Issues::Layer::Can't create layer "onnx_node_output_0!output" of type "CastLike" in function 'getLayerInstance'
"test_castlike_DOUBLE_to_FLOAT16", // Issues::Layer::Can't create layer "onnx_node_output_0!output" of type "CastLike" in function 'getLayerInstance'
"test_castlike_DOUBLE_to_FLOAT16_expanded",  // Issues::Layer::mismatch in input and output shapes  inputs.size() == requiredOutputs in function 'getMemoryShapes'
"test_castlike_DOUBLE_to_FLOAT_expanded",  // Issues::Layer::mismatch in input and output shapes  inputs.size() == requiredOutputs in function 'getMemoryShapes'
"test_castlike_FLOAT16_to_DOUBLE",  // Issue::Unsuppoted data type
"test_castlike_FLOAT16_to_DOUBLE_expanded",  // Issue::Unsuppoted data type
"test_castlike_FLOAT16_to_FLOAT", // Issues::Layer::Can't create layer "onnx_node_output_0!output" of type "CastLike" in function 'getLayerInstance'
"test_castlike_FLOAT16_to_FLOAT_expanded", // Issues::Layer::mismatch in input and output shapes  inputs.size() == requiredOutputs in function 'getMemoryShapes'
"test_castlike_FLOAT_to_BFLOAT16", // Issue::Unsuppoted data type
"test_castlike_FLOAT_to_BFLOAT16_expanded", // Issue::Unsuppoted data type
"test_castlike_FLOAT_to_DOUBLE", // Issues::Layer::Can't create layer "onnx_node_output_0!output" of type "CastLike" in function 'getLayerInstance'
"test_castlike_FLOAT_to_DOUBLE_expanded", // Issue::Unsuppoted data type
"test_castlike_FLOAT_to_FLOAT16", // Issues::Layer::mismatch in input and output shapes  inputs.size() == requiredOutputs in function 'getMemoryShapes'
"test_castlike_FLOAT_to_FLOAT16_expanded", // Issues::Layer::mismatch in input and output shapes  inputs.size() == requiredOutputs in function 'getMemoryShapes'
"test_castlike_FLOAT_to_STRING",
"test_castlike_STRING_to_FLOAT", // Issues::Layer::Can't create layer "onnx_node_output_0!output" of type "CastLike" in function 'getLayerInstance'
"test_clip", // Issue:: Unkonwn error
"test_clip_default_inbounds", // ---- same as above ---
"test_clip_default_int8_inbounds",  // ---- same as above ---
"test_clip_default_int8_max",  // ---- same as above ---
"test_clip_default_int8_min",  // ---- same as above ---
"test_clip_default_max",  // ---- same as above ---
"test_clip_default_min",  // ---- same as above ---
"test_clip_example",  // ---- same as above ---
"test_clip_inbounds",  // ---- same as above ---
"test_clip_outbounds",  // ---- same as above ---
"test_clip_splitbounds",  // ---- same as above ---
"test_compress_0", // Issue::Can't create layer "onnx_node_output_0!output" of type "Compress" in function 'getLayerInstance'
"test_compress_1", // ---- same as above ---
"test_compress_default_axis", // ---- same as above ---
"test_compress_negative_axis", // ---- same as above ---
"test_constant",  // Issue::Wrong output
"test_constant_pad",  // Issue:: Unkonwn error
"test_constantofshape_float_ones", // Issue::Parser::Weights are required as inputs
"test_constantofshape_int_shape_zero", // Issue::Parser::Weights are required as inputs
"test_constantofshape_int_zeros", // Issue::Parser::Weights are required as inputs
"test_convinteger_with_padding", // Issues::Layer::Can't create layer "onnx_node_output_0!y" of type "ConvInteger" in function 'getLayerInstance'
"test_convinteger_without_padding", //Issues::Layer::Can't create layer "onnx_node_output_0!y" of type "ConvInteger" in function 'getLayerInstance'
"test_convtranspose", // Issue::Parser::Weights are required as inputs
"test_convtranspose_1d", // Issue::Parser::Weights are required as inputs
"test_convtranspose_3d", // Issue::Parser::Weights are required as inputs
"test_convtranspose_autopad_same", // Issue::Parser::Weights are required as inputs
"test_convtranspose_dilations", // Issue::Parser::Weights are required as inputs
"test_convtranspose_kernel_shape", // Issue::Parser::Weights are required as inputs
"test_convtranspose_output_shape", // Issue::Parser::Weights are required as inputs
"test_convtranspose_pad", // Issue::Parser::Weights are required as inputs
"test_convtranspose_pads", // Issue::Parser::Weights are required as inputs
"test_convtranspose_with_kernel", // Issue::Parser::Weights are required as inputs
"test_det_2d", // Issue:: Unkonwn error
"test_det_nd", // Issue:: Unkonwn error
"test_dropout_default_mask", // Issue::cvtest::norm::wrong data type
"test_dropout_default_mask_ratio", // ---- same as above ---
"test_dynamicquantizelinear", // Issue:: Unkonwn error
"test_dynamicquantizelinear_expanded", // ---- same as above ---
"test_dynamicquantizelinear_max_adjusted",  // ---- same as above ---
"test_dynamicquantizelinear_max_adjusted_expanded",  // ---- same as above ---
"test_dynamicquantizelinear_min_adjusted",  // ---- same as above ---
"test_dynamicquantizelinear_min_adjusted_expanded",  // ---- same as above ---
"test_edge_pad", // Issue::Parser::Weights are required as inputs
"test_einsum_inner_prod", // Issue::Output shape does not match with reference
"test_elu_default_expanded_ver18",
"test_elu_example_expanded_ver18",
"test_elu_expanded_ver18",
"test_expand_dim_changed", // Issue:: Unkonwn error
"test_expand_dim_unchanged", // Issue:: Unkonwn error
"test_eyelike_populate_off_main_diagonal", // Issues::Layer::Can't create layer::Can't create layer "onnx_node_output_0!y" of type "EyeLike" in function 'getLayerInstance'
"test_eyelike_with_dtype", // ---- same as above ---
"test_eyelike_without_dtype", // ---- same as above ---
"test_gelu_default_1_expanded", // parser: no corresponding layer for CastLike
"test_gelu_default_2_expanded", // parser: no corresponding layer for CastLike
"test_gelu_tanh_1_expanded", // parser: no corresponding layer for CastLike
"test_gelu_tanh_2_expanded", // parser: no corresponding layer for CastLike
"test_gemm_all_attributes",  // Issue::Wrong output
"test_gemm_alpha",  // Issue::Wrong output
"test_gemm_beta",  // Issue::Wrong output
"test_gemm_default_scalar_bias",  // Issue::Wrong output
"test_gemm_default_single_elem_vector_bias",  // Issue::Wrong output
"test_gemm_default_vector_bias",  // Issue::Wrong output
"test_gemm_default_zero_bias",  // Issue::Wrong output
"test_gemm_transposeA", // Issue::Wrong output
"test_gemm_transposeB",  // Issue::Wrong output
"test_gridsample", // Issues::Layer::Can't create layer "onnx_node_output_0!Y" of type "GridSample" in function 'getLayerInstance'
"test_gridsample_aligncorners_true", // ---- same as above ---
"test_gridsample_bicubic", // ---- same as above ---
"test_gridsample_bilinear", // ---- same as above ---
"test_gridsample_border_padding", // ---- same as above ---
"test_gridsample_nearest", // ---- same as above ---
"test_gridsample_reflection_padding", // ---- same as above ---
"test_gridsample_zeros_padding", // ---- same as above ---
"test_gru_batchwise", // Issues::Parser::node_proto.input_size() == 6 in function 'parseGRU'
"test_gru_defaults", // ---- same as above ---
"test_gru_seq_length", // ---- same as above ---
"test_gru_with_initial_bias", // ---- same as above ---
"test_identity_opt", //  23221 illegal hardware instruction
"test_identity_sequence",  // Issue:: Unkonwn error
"test_if", // Issue::'Graph' is not supported in function 'getLayerParams'
"test_if_opt", // Issue::Failed to allocate 17059022683624350 bytes in function 'OutOfMemoryError'
"test_if_seq", // Issue::typeProto.has_tensor_type() in function 'dumpValueInfoProto'
"test_isinf", // Issue::Can't create layer "onnx_node_output_0!y" of type "IsInf" in function 'getLayerInstance'
"test_isinf_negative", //-- same as above ---
"test_isinf_positive", //-- same as above ---
"test_isnan", // -- same as above ---
"test_loop11", // Issue::'Graph' is not supported in function 'getLayerParams'
"test_loop13_seq", // Issue::typeProto.has_tensor_type() in function 'populateNet'
"test_loop16_seq_none", // Issue::Failed to allocate 179812654996800 bytes in function 'OutOfMemoryError'
"test_matmulinteger", // Issues::Layer does not exist. Can't create layer "onnx_node_output_0!Y" of type "MatMulInteger" in function 'getLayerInstance'
"test_max_int16", // Issue:: Unsupported data type
"test_max_uint16", // Issue:: Unsupported data type
"test_max_uint32", // Issue:: Unsupported data type
"test_max_uint64", // Issue:: Unsupported data type
"test_mean_example", // Issues::Layer does not exist. Can't create layer "onnx_node_output_0!result" of type "Mean" in function 'getLayerInstance'
"test_mean_one_input",  // ---- same as above ---
"test_mean_two_inputs",  // ---- same as above ---
"test_min_int16",  // Issue:: Unsupported data type
"test_min_uint16", // Issue:: Unsupported data type
"test_min_uint32", // Issue:: Unkonwn error
"test_min_uint64", // Issue:: Unsupported data type
"test_mod_mixed_sign_int16", // Issue:: Unkonwn error
"test_mod_uint16", // Issue:: Unkonwn error
"test_mod_uint32", // ---- same as above ---
"test_mod_uint64", // ---- same as above ---
"test_momentum", // Issues::Layer does not exist. Can't create layer "onnx_node_output_0!X1_new" of type "ai.onnx.preview.training.Momentum" in function 'getLayerInstance'
"test_momentum_multiple", // ---- same as above ---
"test_mvn", // Issues::Wrong answer
"test_mvn_expanded", // Issues::Wrong answer
"test_nesterov_momentum", // Issues::Layer does not exist (NesterovsAcceleratedGradient) Can't create layer "onnx_node_output_0!X_new" of type "ai.onnx.preview.training.Momentum" in function 'getLayerInstance'
"test_nllloss_NC",  // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1_ii",  // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1_ii_expanded",  // Issue:: Unsupported data type
"test_nllloss_NCd1_mean_weight_negative_ii", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1_mean_weight_negative_ii_expanded", // Issue:: Unsupported data type
"test_nllloss_NCd1_weight", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1_weight_ii", // Issue:: Unsupported data type
"test_nllloss_NCd1_weight_ii_expanded", // Issue:: Unsupported data type
"test_nllloss_NCd1d2", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_no_weight_reduction_mean_ii", // Issue:: Unsupported data type
"test_nllloss_NCd1d2_no_weight_reduction_mean_ii_expanded", // Issue:: Unsupported data type
"test_nllloss_NCd1d2_reduction_mean", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_reduction_mean_expanded", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_reduction_sum", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_reduction_sum_expanded", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_with_weight", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_with_weight_reduction_mean",  // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_with_weight_reduction_sum", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_with_weight_reduction_sum_expanded", // Issue::Wrong output on CUDA
"test_nllloss_NCd1d2_with_weight_reduction_sum_ii", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2_with_weight_reduction_sum_ii_expanded",  // Issue:: Unsupported data type
"test_nllloss_NCd1d2d3_none_no_weight_negative_ii",  // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2d3_none_no_weight_negative_ii_expanded", // Issue:: Unsupported data type
"test_nllloss_NCd1d2d3_sum_weight_high_ii", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2d3_sum_weight_high_ii_expanded", // Issue:: Unsupported data type
"test_nllloss_NCd1d2d3d4d5_mean_weight", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nllloss_NCd1d2d3d4d5_mean_weight_expanded", // Issue::Wrong output
"test_nllloss_NCd1d2d3d4d5_none_no_weight", // Issue:: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_nonmaxsuppression_center_point_box_format", // Issue:: Layer does not exist (NonMaxSuppression)::Can't create layer "onnx_node_output_0!selected_indices" of type "NonMaxSuppression" in function 'getLayerInstance'
"test_nonmaxsuppression_flipped_coordinates", // ---- same as above ---
"test_nonmaxsuppression_identical_boxes", // ---- same as above ---
"test_nonmaxsuppression_limit_output_size", // ---- same as above ---
"test_nonmaxsuppression_single_box", // ---- same as above ---
"test_nonmaxsuppression_suppress_by_IOU", // ---- same as above ---
"test_nonmaxsuppression_suppress_by_IOU_and_scores", // ---- same as above ---
"test_nonmaxsuppression_two_batches", // ---- same as above ---
"test_nonmaxsuppression_two_classes", // ---- same as above ---
"test_nonzero_example", // Issue::Can't create layer "onnx_node_output_0!result" of type "NonZero" in function 'getLayerInstance'
"test_onehot_negative_indices", // Issue:: Layer does not exist (OneHot) :: Can't create layer "onnx_node_output_0!y" of type "OneHot" in function 'getLayerInstance'
"test_onehot_with_axis", // ---- same as above ---
"test_onehot_with_negative_axis", // ---- same as above ---
"test_onehot_without_axis", // ---- same as above ---
"test_optional_get_element", // Issue::out of memory :: Failed to allocate 1044051907127083008 bytes in function 'OutOfMemoryError'
"test_optional_get_element_sequence", // ---- same as above ---
"test_optional_has_element", // Issue::typeProto.has_tensor_type() in function 'populateNet'
"test_optional_has_element_empty", // ---- same as above ---
"test_pow_types_float", // Issue:: Unsupported data type
"test_pow_types_float32_int32", // ---- same as above ---
"test_pow_types_float32_int64", // ---- same as above ---
"test_pow_types_float32_uint32", // ---- same as above ---
"test_pow_types_float32_uint64", // ---- same as above ---
"test_pow_types_int", // Issue:: Unsupported data type
"test_pow_types_int32_float32", // ---- same as above ---
"test_pow_types_int32_int32", // ---- same as above ---
"test_pow_types_int64_float32", // ---- same as above ---
"test_prelu_broadcast", // Issue::Parser:Blob slope not found in const blobs in function 'getBlob' (weights are required as inputs)
"test_prelu_example", // ---- same as above ---
"test_qlinearconv", // Issue::Parser:  Blob x_scale not found in const blobs in function 'getBlob' (weights are required as inputs)
"test_qlinearmatmul_2D", // Issue:: Parser: Variable weights is not supported in function 'parseQMatMul'
"test_qlinearmatmul_3D", // ---- same as above ---
"test_range_float_type_positive_delta", // Issue:: Unsupported data type in function. Unsupported type in function 'parseCast'
"test_range_float_type_positive_delta_expanded", // ---- same as above ---
"test_range_int32_type_negative_delta", // Issue:: Unsupported data type: INT32 in function. Unsupported type in function 'parseCast'
"test_range_int32_type_negative_delta_expanded", // ---- same as above ---
"test_reduce_sum_default_axes_keepdims_example", // Issue:: Parser: Reduce layer doesn't support non contant axes: 'constBlobs.find(node_proto.input(1)) != constBlobs.end()' must be 'true' (layer does not support dynamic parameters)
"test_reduce_sum_default_axes_keepdims_random",  // ---- same as above ---
"test_reduce_sum_do_not_keepdims_example",  // ---- same as above ---
"test_reduce_sum_do_not_keepdims_random",  // ---- same as above ---
"test_reduce_sum_empty_axes_input_noop_example",  // ---- same as above ---
"test_reduce_sum_empty_axes_input_noop_random",  // ---- same as above ---
"test_reduce_sum_keepdims_example",  // ---- same as above ---
"test_reduce_sum_keepdims_random", // ---- same as above ---
"test_reduce_sum_negative_axes_keepdims_example",
"test_reduce_sum_negative_axes_keepdims_random", // ---- same as above ---
"test_reflect_pad", // Issue:: Parser:  Blob shape not found in const blobs in function 'getBlob' (weights are required as inputs)
"test_reshape_allowzero_reordered",
"test_reshape_extended_dims", // ---- same as above ---
"test_reshape_negative_dim", // ---- same as above ---
"test_reshape_negative_extended_dims", // ---- same as above ---
"test_reshape_one_dim", // ---- same as above ---
"test_reshape_reduced_dims", // ---- same as above ---
"test_reshape_reordered_all_dims", // ---- same as above ---
"test_reshape_reordered_last_dims", // ---- same as above ---
"test_reshape_zero_and_negative_dim", // ---- same as above ---
"test_reshape_zero_dim", // ---- same as above ---
"test_resize_downsample_scales_cubic",  // Issue:: Parser: layer_id.find(node_proto.input(i)) == layer_id.end() in function 'parseResize'
"test_resize_downsample_scales_cubic_A_n0p5_exclude_outside", // ---- same as above ---
"test_resize_downsample_scales_cubic_align_corners",  // ---- same as above ---
"test_resize_downsample_scales_linear", // ---- same as above ---
"test_resize_downsample_scales_linear_align_corners", // ---- same as above ---
"test_resize_downsample_scales_nearest", // ---- same as above ---
"test_resize_downsample_sizes_cubic", // ---- same as above ---
"test_resize_downsample_sizes_linear_pytorch_half_pixel", // ---- same as above ---
"test_resize_downsample_sizes_nearest", // ---- same as above ---
"test_resize_downsample_sizes_nearest_tf_half_pixel_for_nn", // ---- same as above ---
"test_resize_tf_crop_and_resize", // ---- same as above ---
"test_resize_upsample_scales_cubic",  // Issue:: Parser: layer_id.find(node_proto.input(i)) == layer_id.end() in function 'parseResize'
"test_resize_upsample_scales_cubic_A_n0p5_exclude_outside", // ---- same as above ---
"test_resize_upsample_scales_cubic_align_corners", // ---- same as above ---
"test_resize_upsample_scales_cubic_asymmetric", // ---- same as above ---
"test_resize_upsample_scales_linear", // ---- same as above ---
"test_resize_upsample_scales_linear_align_corners", // ---- same as above ---
"test_resize_upsample_scales_nearest", // ---- same as above ---
"test_resize_upsample_sizes_cubic", // ---- same as above ---
"test_resize_upsample_sizes_nearest", // ---- same as above ---
"test_resize_upsample_sizes_nearest_ceil_half_pixel", // ---- same as above ---
"test_resize_upsample_sizes_nearest_floor_align_corners", // ---- same as above ---
"test_resize_upsample_sizes_nearest_round_prefer_ceil_asymmetric", // ---- same as above ---
"test_reversesequence_batch",  // Issue:: Parser: Can't create layer "onnx_node_output_0!y" of type "ReverseSequence" in function 'getLayerInstance'
"test_reversesequence_time", // ---- same as above ---
"test_rnn_seq_length", // Issue:: Parser:  Can't create layer "onnx_node_output_1!Y_h" of type "RNN" in function 'getLayerInstance'
"test_roialign_aligned_false", // Issue:: Parser: Layer does not exist (RoiAlign)
"test_roialign_aligned_true", // ---- same as above ---
"test_scan9_sum",  // Issue:: Parser: 'Graph' is not supported in function 'getLayerParams'
"test_scan_sum", // ---- same as above ---
"test_sce_NCd1_mean_weight_negative_ii", // Issue:: Parser: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_sce_NCd1_mean_weight_negative_ii_expanded", // ---- same as above ---
"test_sce_NCd1_mean_weight_negative_ii_log_prob", // ---- same as above ---
"test_sce_NCd1_mean_weight_negative_ii_log_prob_expanded", // ---- same as above ---
"test_sce_NCd1d2d3_none_no_weight_negative_ii", // ---- same as above ---
"test_sce_NCd1d2d3_none_no_weight_negative_ii_expanded", // ---- same as above ---
"test_sce_NCd1d2d3_none_no_weight_negative_ii_log_prob", // ---- same as above ---
"test_sce_NCd1d2d3_none_no_weight_negative_ii_log_prob_expanded", // ---- same as above ---
"test_sce_NCd1d2d3_sum_weight_high_ii", // ---- same as above ---
"test_sce_NCd1d2d3_sum_weight_high_ii_expanded", // ---- same as above ---
"test_sce_NCd1d2d3_sum_weight_high_ii_log_prob", // ---- same as above ---
"test_sce_NCd1d2d3_sum_weight_high_ii_log_prob_expanded", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_mean_weight", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_mean_weight_expanded", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_mean_weight_log_prob", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_mean_weight_log_prob_expanded", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_none_no_weight", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_none_no_weight_expanded", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_none_no_weight_log_prob", // ---- same as above ---
"test_sce_NCd1d2d3d4d5_none_no_weight_log_prob_expanded", // ---- same as above ---
"test_sce_mean", // Issue:: Parser: Layer does not exist (NegativeLogLikelihoodLoss, SoftmaxCrossEntropyLoss)
"test_sce_mean_3d", // ---- same as above ---
"test_sce_mean_3d_expanded", // ---- same as above ---
"test_sce_mean_3d_log_prob", // ---- same as above ---
"test_sce_mean_3d_log_prob_expanded", // ---- same as above ---
"test_sce_mean_expanded", // ---- same as above ---
"test_sce_mean_log_prob", // ---- same as above ---
"test_sce_mean_log_prob_expanded", // ---- same as above ---
"test_sce_mean_no_weight_ii", // ---- same as above ---
"test_sce_mean_no_weight_ii_3d", // ---- same as above ---
"test_sce_mean_no_weight_ii_3d_expanded", // ---- same as above ---
"test_sce_mean_no_weight_ii_3d_log_prob", // ---- same as above ---
"test_sce_mean_no_weight_ii_3d_log_prob_expanded", // ---- same as above ---
"test_sce_mean_no_weight_ii_4d", // ---- same as above ---
"test_sce_mean_no_weight_ii_4d_expanded", // ---- same as above ---
"test_sce_mean_no_weight_ii_4d_log_prob", // ---- same as above ---
"test_sce_mean_no_weight_ii_4d_log_prob_expanded", // ---- same as above ---
"test_sce_mean_no_weight_ii_expanded", // ---- same as above ---
"test_sce_mean_no_weight_ii_log_prob", // ---- same as above ---
"test_sce_mean_no_weight_ii_log_prob_expanded", // ---- same as above ---
"test_sce_mean_weight", // ---- same as above ---
"test_sce_mean_weight_expanded", // ---- same as above ---
"test_sce_mean_weight_ii", // ---- same as above ---
"test_sce_mean_weight_ii_3d", // ---- same as above ---
"test_sce_mean_weight_ii_3d_expanded", // ---- same as above ---
"test_sce_mean_weight_ii_3d_log_prob", // ---- same as above ---
"test_sce_mean_weight_ii_3d_log_prob_expanded", // ---- same as above ---
"test_sce_mean_weight_ii_4d", // ---- same as above ---
"test_sce_mean_weight_ii_4d_expanded", // ---- same as above ---
"test_sce_mean_weight_ii_4d_log_prob", // ---- same as above ---
"test_sce_mean_weight_ii_4d_log_prob_expanded", // ---- same as above ---
"test_sce_mean_weight_ii_expanded", // ---- same as above ---
"test_sce_mean_weight_ii_log_prob", // ---- same as above ---
"test_sce_mean_weight_ii_log_prob_expanded", // ---- same as above ---
"test_sce_mean_weight_log_prob", // ---- same as above ---
"test_sce_mean_weight_log_prob_expanded", // ---- same as above ---
"test_sce_none",  // ---- same as above ---
"test_sce_none_expanded", // ---- same as above ---
"test_sce_none_log_prob", // ---- same as above ---
"test_sce_none_log_prob_expanded", // ---- same as above ---
"test_sce_none_weights", // ---- same as above ---
"test_sce_none_weights_expanded", // ---- same as above ---
"test_sce_none_weights_log_prob", // ---- same as above ---
"test_sce_none_weights_log_prob_expanded", // ---- same as above ---
"test_sce_sum", // ---- same as above ---
"test_sce_sum_expanded", // ---- same as above ---
"test_sce_sum_log_prob", // ---- same as above ---
"test_sce_sum_log_prob_expanded", // ---- same as above ---
"test_selu_default_expanded_ver18",
"test_selu_example_expanded_ver18",
"test_selu_expanded_ver18",
"test_sequence_insert_at_back", // Issue:: Parser:  typeProto.has_tensor_type() in function 'populateNet'
"test_sequence_insert_at_front", // ---- same as above ---
"test_shape", // Issue:: Parser: DNN/ONNX: can't find layer for output name: 'y'. Does model imported properly?
"test_shape_clip_end", // ---- same as above ---
"test_shape_clip_start", // ---- same as above ---
"test_shape_end_1", // ---- same as above ---
"test_shape_end_negative_1", // ---- same as above ---
"test_shape_example", // ---- same as above ---
"test_shape_start_1", // ---- same as above ---
"test_shape_start_1_end_2", // ---- same as above ---
"test_shape_start_1_end_negative_1", // ---- same as above ---
"test_shape_start_negative_1", // ---- same as above ---
"test_simple_rnn_batchwise", // Issue:: Parser: Can't create layer "onnx_node_output_1!Y_h" of type "RNN" in function 'getLayerInstance'
"test_simple_rnn_defaults", // ---- same as above ---
"test_simple_rnn_with_initial_bias", // ---- same as above ---
"test_size", // Issue:: Parser:  Can't create layer "onnx_node_output_0!y" of type "Size" in function 'getLayerInstance'
"test_size_example", // ---- same as above ---
"test_slice", // Issue:: Parser: constBlobs.find(node_proto.input(i)) != constBlobs.end() in function 'parseSlice'
"test_slice_default_axes",  // ---- same as above ---
"test_slice_default_steps",  // ---- same as above ---
"test_slice_end_out_of_bounds",  // ---- same as above ---
"test_slice_neg",  // ---- same as above ---
"test_slice_neg_steps",  // ---- same as above ---
"test_slice_negative_axes",  // ---- same as above ---
"test_slice_start_out_of_bounds",  // ---- same as above ---
"test_split_variable_parts_1d",  // ---- same as above ---
"test_split_variable_parts_2d",  // ---- same as above ---
"test_split_variable_parts_default_axis",  // ---- same as above ---
"test_split_zero_size_splits",  // ---- same as above ---
"test_squeeze", // Issue:: Parser: ONNX/Squeeze: doesn't support non-constant 'axes' input in function 'parseSqueeze' (layer parameters are dynamic)
"test_squeeze_negative_axes", // ---- same as above ---
"test_strnormalizer_export_monday_casesensintive_lower", // 'Strings' (1) are not supported in function 'getLayerParams'
"test_strnormalizer_export_monday_casesensintive_nochangecase", // ---- same as above ---
"test_strnormalizer_export_monday_casesensintive_upper", // ---- same as above ---
"test_strnormalizer_export_monday_empty_output", // ---- same as above ---
"test_strnormalizer_export_monday_insensintive_upper_twodim", // ---- same as above ---
"test_strnormalizer_nostopwords_nochangecase", // Issue:: Parser: Can't create layer "onnx_node_output_0!y" of type "StringNormalizer" in function 'getLayerInstance'
"test_tfidfvectorizer_tf_batch_onlybigrams_skip0", // Issue:: Parser: Can't create layer "onnx_node_output_0!Y" of type "TfIdfVectorizer" in function 'getLayerInstance'
"test_tfidfvectorizer_tf_batch_onlybigrams_skip5",  // ---- same as above ---
"test_tfidfvectorizer_tf_batch_uniandbigrams_skip5",  // ---- same as above ---
"test_tfidfvectorizer_tf_only_bigrams_skip0",  // ---- same as above ---
"test_tfidfvectorizer_tf_onlybigrams_levelempty",  // ---- same as above ---
"test_tfidfvectorizer_tf_onlybigrams_skip5",  // ---- same as above ---
"test_tfidfvectorizer_tf_uniandbigrams_skip5", // Issue:: Parser: Can't create layer "onnx_node_output_0!Y" of type "TfIdfVectorizer" in function 'getLayerInstance'
"test_tile", // Issue:: Parser: ONNX/Tile: repeats being non-constant is not supported. in function 'parseTile' (layer parameters are dynamic)
"test_tile_precomputed", //  // ---- same as above ---
"test_top_k", // Issue:: K being input is not compatible with the current engine
"test_top_k_negative_axis",  // ---- same as above ---
"test_top_k_smallest",  // ---- same as above ---
"test_training_dropout", // Issue::cvtest::norm::wrong data type
"test_training_dropout_default",  // ---- same as above ---
"test_training_dropout_default_mask",  // ---- same as above ---
"test_training_dropout_mask",  // ---- same as above ---
"test_training_dropout_zero_ratio",  // ---- same as above ---
"test_training_dropout_zero_ratio_mask",  // ---- same as above ---
"test_tril", // Issue:: Parser: Can't create layer "onnx_node_output_0!y" of type "Trilu" in function 'getLayerInstance'
"test_tril_neg",  // ---- same as above ---
"test_tril_one_row_neg",  // ---- same as above ---
"test_tril_out_neg",  // ---- same as above ---
"test_tril_out_pos",  // ---- same as above ---
"test_tril_pos",  // ---- same as above ---
"test_tril_square",  // ---- same as above ---
"test_tril_square_neg",  // ---- same as above ---
"test_tril_zero",  // ---- same as above ---
"test_triu",  // ---- same as above ---
"test_triu_neg",  // ---- same as above ---
"test_triu_one_row",  // ---- same as above ---
"test_triu_out_neg_out",  // ---- same as above ---
"test_triu_out_pos",  // ---- same as above ---
"test_triu_pos",  // ---- same as above ---
"test_triu_square",  // ---- same as above ---
"test_triu_square_neg",  // ---- same as above ---
"test_triu_zero",  // ---- same as above ---
"test_unique_not_sorted_without_axis", // Issue:: Parser: Can't create layer "onnx_node_output_0!Y" of type "Unique" in function 'getLayerInstance'
"test_unique_sorted_with_axis",  // ---- same as above ---
"test_unique_sorted_with_axis_3d",  // ---- same as above ---
"test_unique_sorted_with_negative_axis",  // ---- same as above ---
"test_unique_sorted_without_axis",  // ---- same as above ---
"test_unsqueeze_axis_0", // Issue::(most probaly weights are required as inputs: Blob axes not found in const blobs in function 'getBlob'
"test_unsqueeze_axis_1",  // ---- same as above ---
"test_unsqueeze_axis_2",  // ---- same as above ---
"test_unsqueeze_negative_axes",  // ---- same as above ---
"test_unsqueeze_three_axes",  // ---- same as above ---
"test_unsqueeze_two_axes",   // ---- same as above ---)
"test_unsqueeze_unsorted_axes",  // ---- same as above ---)
