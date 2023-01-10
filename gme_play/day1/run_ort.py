import onnxruntime
import numpy as np

print(onnxruntime.__version__)
session = onnxruntime.InferenceSession('lr_init.onnx')
print("start to run")
outputs = session.run(None, {'X': np.array([[1.0,2.0]], dtype="float32")})
"""
#
y = 0.5 * x1 - 0.6 * x2 + 0.4

                 [0.5 ]
[y] = [x1  x2] x |    | + [0.4]
                 [-0.6]
0.5 * 1 - 0.6 * 2 + 0.4 = -0.3
"""
print(outputs)

