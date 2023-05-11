- Cut off for AMD_ARM precision comparison



```
./build.sh --config Debug --enable_pybind --build_wheel --parallel --build_shared_lib \
--cmake_extra_defines onnxruntime_DEBUG_NODE_INPUTS_OUTPUTS=1 onnxruntime_DEBUG_NODE_INPUTS_OUTPUTS_ENABLE_DUMP_TO_SQLDB=1 \
--skip_tests --skip_onnx_tests
```



```
orttraining/tools/scripts/sqldb_to_tensors.py
```


```
onnxruntime/core/framework/debug_node_inputs_outputs_utils.h
```


```c++
ORT_DEBUG_NODE_IO_DUMP_SHAPE_DATA
ORT_DEBUG_NODE_IO_DUMP_INPUT_DATA
ORT_DEBUG_NODE_IO_DUMP_OUTPUT_DATA
ORT_DEBUG_NODE_IO_DUMP_NODE_PLACEMENT

// Tensor shape and Node placement is printed by default unless it's turned OFF
// by setting the respective environment variables to 0
// set to non-zero to dump tensor shape data
constexpr const char* kDumpShapeData = "ORT_DEBUG_NODE_IO_DUMP_SHAPE_DATA";
// set to non-zero to dump node placement data
constexpr const char* kDumpNodePlacement = "ORT_DEBUG_NODE_IO_DUMP_NODE_PLACEMENT";
// set to non-zero to dump node input data
constexpr const char* kDumpInputData = "ORT_DEBUG_NODE_IO_DUMP_INPUT_DATA";
// set to non-zero to dump node output data
constexpr const char* kDumpOutputData = "ORT_DEBUG_NODE_IO_DUMP_OUTPUT_DATA";
// specify a node name filter to limit the nodes that are dumped
// see NodeDumpOptions::FilterOptions
constexpr const char* kNameFilter = "ORT_DEBUG_NODE_IO_NAME_FILTER";
// specify a node op type filter to limit the nodes that are dumped
// see NodeDumpOptions::FilterOptions
constexpr const char* kOpTypeFilter = "ORT_DEBUG_NODE_IO_OP_TYPE_FILTER";
// set to "stdout" or "files" or "sqlite" to select dump destination
constexpr const char* kDumpDataDestination = "ORT_DEBUG_NODE_IO_DUMP_DATA_DESTINATION";
// set to non-zero to append OpenMPI world rank to filename
constexpr const char* kAppendRankToFileName = "ORT_DEBUG_NODE_IO_APPEND_RANK_TO_FILE_NAME";
// specify the output directory for any data files produced
constexpr const char* kOutputDir = "ORT_DEBUG_NODE_IO_OUTPUT_DIR";
// specify the file prefix for sqlite3 db (process id will be appended)
constexpr const char* kSqliteDbPrefix = "ORT_DEBUG_NODE_IO_SQLITE_DB_PREFIX";
// set to non-zero to confirm that dumping data files for all nodes is acceptable
constexpr const char* kDumpingDataToFilesForAllNodesIsOk =
"ORT_DEBUG_NODE_IO_DUMPING_DATA_TO_FILES_FOR_ALL_NODES_IS_OK";

// Total number of elements which trigger snippet rather than full dump (default 200). Value 0 disables snippet.
constexpr const char* kSnippetThreshold = "ORT_DEBUG_NODE_IO_SNIPPET_THRESHOLD";
// Number of array items in snippet at beginning and end of each dimension (default 3)
constexpr const char* kSnippetEdgeItems = "ORT_DEBUG_NODE_IO_SNIPPET_EDGE_ITEMS";
```