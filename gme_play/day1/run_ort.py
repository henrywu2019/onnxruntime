import onnxruntime
import numpy as np

print(onnxruntime.__version__)
session = onnxruntime.InferenceSession('lr_init.onnx')
outputs = session.run(None, {'input': np.array([1,2])})

print(outputs)

