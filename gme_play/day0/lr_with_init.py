import numpy
import onnx
from onnx import numpy_helper, TensorProto
from onnx.helper import (
    make_model, make_node, set_model_props, make_tensor, make_graph,
    make_tensor_value_info)
from mlprodict.plotting.text_plot import onnx_simple_text_plot

# initializers
value = numpy.array([[0.5],
                     [-0.6]], dtype=numpy.float32)
A = numpy_helper.from_array(value, name='A')

value = numpy.array([[0.4]], dtype=numpy.float32)
C = numpy_helper.from_array(value, name='C')

# the part which does not change
X = make_tensor_value_info('X', TensorProto.FLOAT, [1,2])
Y = make_tensor_value_info('Y', TensorProto.FLOAT, [1,1])



node1 = make_node('MatMul', ['X', 'A'], ['AX'])
node2 = make_node('Add', ['AX', 'C'], ['Y'])
graph = make_graph([node1, node2], 'lr', [X], [Y], [A, C])
onnx_model = make_model(graph)

print(onnx.numpy_helper.to_array(onnx_model.graph.initializer[0]))
print(onnx.numpy_helper.to_array(onnx_model.graph.initializer[1]))

# y = 0.5 * x1 - 0.6 * x2 + 0.4
onnx.save(onnx_model, "linear_regression_model.onnx")

print('** intializer **')
for init in onnx_model.graph.initializer:
    print(init)