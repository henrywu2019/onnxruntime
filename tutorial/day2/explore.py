import onnx

# <class 'onnx.onnx_ml_pb2.ModelProto'>
model = onnx.load('../det_doc.onnx')
onnx.checker.check_model(model)

graph = model.graph


for node in graph.node:
    print("*"*80)
    print(f"name:{node.name},op:{node.op_type},i:{node.input},o:{node.output},d:{node.domain}")
    # Modify batchnorm attributes.
    if node.op_type == "BatchNormalization":
        pass
    if node.op_type == "Relu":
        pass
    if node.domain:
        print("FOUND")
        break


#print(graph)
