import onnxruntime as ort
import numpy as np
import time
from hiq.framework.onnxruntime import plot_ort_profile


print(ort.__file__)

model_file_path = 'det_doc.onnx'
so = ort.SessionOptions()
so.optimized_model_filepath = "/tmp/det_doc.ort"
sess = ort.InferenceSession(model_file_path, providers=['CPUExecutionProvider']) # 'CUDAExecutionProvider'

n = np.load('x.npy')
output = None
start = time.monotonic()



print("*"*80)
r = sess.run(output, {'x': n})
print("*"*80)



print(time.monotonic() - start)
print(r[0].shape)

pf = sess.end_profiling()
print(pf)
plot_ort_profile(pf)

