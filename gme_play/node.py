import numpy
import onnx
from onnx import numpy_helper, TensorProto
from onnx.helper import (
    make_model, make_node, set_model_props, make_tensor,
    make_graph, make_tensor_value_info)
from mlprodict.plotting.text_plot import onnx_simple_text_plot


def shape2tuple(shape):
    return tuple(getattr(d, 'dim_value', 0) for d in shape.dim)


X = make_tensor_value_info('X', TensorProto.FLOAT, [None, None])
A = make_tensor_value_info('A', TensorProto.FLOAT, [None, None])
B = make_tensor_value_info('B', TensorProto.FLOAT, [None, None])
Y = make_tensor_value_info('Y', TensorProto.FLOAT, None)
node1 = make_node('MatMul', ['X', 'A'], ['XA'])
node2 = make_node('Add', ['XA', 'B'], ['Y'])
graph = make_graph([node1, node2], 'lr', [X, A, B], [Y])
onnx_model = make_model(graph)

# The serialization
#with open("linear_regression.onnx", "wb") as f:
#    f.write(onnx_model.SerializeToString())

onnx.save(onnx_model, "lr.onnx")

# display
print(onnx_simple_text_plot(onnx_model))


