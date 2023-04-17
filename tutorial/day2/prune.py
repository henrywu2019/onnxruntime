import onnx
import numpy as np
import onnx_graphsurgeon as gs

# <class 'onnx.onnx_ml_pb2.ModelProto'>
graph = gs.import_onnx(onnx.load('../det_doc.onnx'))

r0=[i for i in graph.nodes if i.op=='Relu' and i.name=='Relu_0'][0]
graph.outputs = r0.outputs
r0.outputs[0].dtype=np.float32
graph.cleanup().toposort()
onnx.save(gs.export_onnx(graph), 'modified.onnx')


